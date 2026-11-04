/*
 * flow_safe.c
 *
 * One instance of the safe core.  The implementation follows the four steps
 * of the sequence diagram:
 *
 *   1. external safe tick + local latch of the raw RX frames
 *   2. FSoE stack synchronisation, egress PDU cross-check and SHMEM publish
 *   3. cross-check of the decoded SafeInputs and connection status
 *   4. safe logic execution and cross-check of the computed SafeOutputs
 *
 * There is no POSIX code and no dynamic allocation here.  The same source is
 * compiled for M7 and M33 with FLOW_CORE_ID; all mutable state lives in
 * flow_safe_t.
 */

#include "flow_safe.h"
#include "flow_debug.h"
#include "flow_mem.h"
#include "flow_platform.h"
#include "flow_shm.h"

#include <fsoeapp.h>
#include <fsoemaster.h>

FLOW_STATIC_ASSERT (
   FLOW_PDU_MAX == FSOEMASTER_FRAME_SIZE (FLOW_SAFE_DATA_SIZE),
   "FLOW_PDU_MAX does not match the FSoE configuration");

/* ------------------------------------------------------------- MU payloads */

typedef struct FLOW_PACKED safe_session_message
{
   flow_u16 k;
   flow_u16 random;
} safe_session_message_t;

typedef struct FLOW_PACKED safe_egress_message
{
   flow_u16 k;
   flow_u16 len;
   flow_u32 rx_fc;
   flow_u32 anomalies;
   flow_u8 pdu[FLOW_PDU_MAX];
} safe_egress_message_t;

typedef struct FLOW_PACKED safe_inputs_message
{
   flow_u16 k;
   flow_u16 flags;
   flow_u8 inputs[FLOW_SAFE_DATA_SIZE];
   flow_u8 state;
   flow_u8 sent_frame_type;
   flow_u8 recv_frame_type;
   flow_u8 reserved;
   flow_u32 tx_index;
   flow_u32 rx_index;
   flow_u32 rx_fc;
   flow_u32 anomalies;
} safe_inputs_message_t;

#define SAFE_INPUTS_PROCESS_DATA 0x0001u
#define SAFE_INPUTS_STATUS_OK 0x0002u
#define SAFE_INPUTS_SENDING 0x0004u

typedef struct FLOW_PACKED safe_logic_message
{
   flow_u16 k;
   flow_u16 logic_state;
   flow_u8 outputs[FLOW_SAFE_DATA_SIZE];
   flow_u32 anomalies;
} safe_logic_message_t;

FLOW_STATIC_ASSERT (sizeof (safe_egress_message_t) <= FLOW_MU_PAYLOAD_MAX,
   "egress cross-check message does not fit the MU payload");
FLOW_STATIC_ASSERT (sizeof (safe_inputs_message_t) <= FLOW_MU_PAYLOAD_MAX,
   "inputs cross-check message does not fit the MU payload");
FLOW_STATIC_ASSERT (sizeof (safe_logic_message_t) <= FLOW_MU_PAYLOAD_MAX,
   "logic cross-check message does not fit the MU payload");

/* ---------------------------------------------------------------- helpers */

static const flow_u16 safe_app_parameters[2] =
{
   (flow_u16)FLOW_APP_PARAMETER_1,
   (flow_u16)FLOW_APP_PARAMETER_2,
};

#if FLOW_DEBUG
static const char * safe_tag (const flow_safe_t * safe)
{
   return (safe->id == FLOW_CORE_M7) ? "M7  " : "M33 ";
}
#endif

static flow_u32 safe_min (flow_u32 a, flow_u32 b)
{
   return (a < b) ? a : b;
}

static void safe_note_anomaly (flow_safe_t * safe, const char * what)
{
   safe->anomalies++;
   safe->cycle_fault = true;
   FLOW_LOG (safe_tag (safe), "cycle %u: ANOMALY %s -> fail-safe",
      safe->cycle, what);
}

static void safe_merge_anomalies (flow_safe_t * safe, flow_u32 peer)
{
   if (peer > safe->anomalies)
   {
      safe->anomalies = peer;
   }
}

static void safe_request_reset (flow_safe_t * safe, flow_u32 k)
{
   if (fsoemaster_set_reset_request_flag (&safe->master[k]) ==
       FSOEMASTER_STATUS_OK)
   {
      safe->stats.reset_requests++;
      FLOW_LOG (safe_tag (safe), "k=%u reset requested", k);
   }
}

static void safe_set_sending (flow_safe_t * safe, flow_u32 k, bool enable)
{
   fsoemaster_status_t status;

   if (enable == safe->send_enabled[k])
   {
      return;
   }

   if (enable)
   {
      status = fsoemaster_set_process_data_sending_enable_flag (
         &safe->master[k]);
   }
   else
   {
      status = fsoemaster_clear_process_data_sending_enable_flag (
         &safe->master[k]);
   }

   if (status == FSOEMASTER_STATUS_OK)
   {
      safe->send_enabled[k] = enable;
   }
}

static flow_u16 safe_logic_state_bits (const flow_safe_logic_t * logic)
{
   flow_u16 bits = 0u;

   if (logic->enabled != 0u)
   {
      bits |= 0x0001u;
   }
   if (logic->latched_fault != 0u)
   {
      bits |= 0x0002u;
   }
   if (logic->discrepancy_count != 0u)
   {
      bits |= 0x0004u;
   }

   return bits;
}

#if FLOW_DEBUG
#define SAFE_FAULT_CYCLE 30u

static bool safe_fault_active (
   const flow_safe_t * safe,
   flow_test_fault_t type)
{
   return (safe->fault == type) &&
      (safe->cycle >= SAFE_FAULT_CYCLE) &&
      (safe->cycle < (SAFE_FAULT_CYCLE + 1u));
}
#else
#define safe_fault_active(safe, type) (0)
#endif

/* ------------------------------------------------------------ step 1: latch */

static void safe_latch_rx (flow_safe_t * safe)
{
   flow_u32 k;

   for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++)
   {
      const flow_shm_channel_t * channel = &safe->shm->rx[k];
      flow_shm_slot_t slot;
      flow_u32 published;

      published = flow_shm_count (channel);

      if ((published == 0u) || (published == safe->rx_count[k]))
      {
         continue; /* no new frame from the black channel */
      }

      if (!flow_shm_read_index (channel, published, &slot))
      {
         safe->stats.rx_latch_failures++;
         FLOW_LOG (safe_tag (safe), "k=%u RX slot %u invalid",
            k, published);
         continue;
      }

      safe->rx_slot[k].len = slot.len;
      safe->rx_slot[k].fc = slot.fc;
      flow_memcpy (safe->rx_slot[k].pdu, slot.pdu, slot.len);
      safe->rx_count[k] = published;
      safe->rx_ready[k] = true;

      FLOW_LOG (safe_tag (safe), "k=%u latched RX frame %u len=%u fc=%u",
         k, published, slot.len, slot.fc);
   }
}

/* --------------------------------------------------- step 2: FSoE sync */

static void safe_sync (flow_safe_t * safe)
{
   flow_u32 k;

   for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++)
   {
      fsoemaster_state_t previous_state = safe->sync[k].current_state;
      fsoemaster_status_t status;

      flow_memset (&safe->raw_inputs[k], 0, sizeof (safe->raw_inputs[k]));
      flow_memset (&safe->sync[k], 0, sizeof (safe->sync[k]));

      status = fsoemaster_sync_with_slave (
         &safe->master[k],
         &safe->safe_outputs[k],
         &safe->raw_inputs[k],
         &safe->sync[k]);

      if (status != FSOEMASTER_STATUS_OK)
      {
         FLOW_LOG (safe_tag (safe), "k=%u fsoemaster_sync failed", k);
         continue;
      }

      if (safe->sync[k].current_state != previous_state)
      {
         FLOW_LOG (safe_tag (safe), "k=%u state %s -> %s", k,
            fsoemaster_state_description (previous_state),
            fsoemaster_state_description (safe->sync[k].current_state));
      }

      if (safe->sync[k].reset_event != FSOEMASTER_RESETEVENT_NONE)
      {
         safe->send_enabled[k] = false;
         FLOW_LOG (safe_tag (safe), "k=%u reset by %s: %s", k,
            (safe->sync[k].reset_event == FSOEMASTER_RESETEVENT_BY_MASTER) ?
               "master" : "slave",
            fsoemaster_reset_reason_description (safe->sync[k].reset_reason));
      }
   }
}


/* ------------------------ step 3: cross-check of decoded SafeInputs */

static void safe_realign_counters (
   flow_safe_t * safe,
   flow_u32 k,
   const safe_inputs_message_t * peer)
{
   flow_u32 tx_common = safe_min (safe->tx_count[k], peer->tx_index);
   flow_u32 rx_common = safe_min (safe->rx_count[k], peer->rx_index);

   if (tx_common != safe->tx_count[k])
   {
      flow_shm_commit (&safe->shm->tx[k][safe->id], tx_common);
      safe->tx_count[k] = tx_common;
   }

   if (rx_common != safe->rx_count[k])
   {
      safe->rx_count[k] = rx_common;
   }
}

static void safe_crosscheck_inputs (flow_safe_t * safe)
{
   flow_u32 k;

   for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++)
   {
      safe_inputs_message_t message;
      const safe_inputs_message_t * peer_message;
      flow_mu_message_t peer;
      bool match;

      flow_memset (&message, 0, sizeof (message));

      message.k = (flow_u16)k;
      message.state = (flow_u8)safe->sync[k].current_state;
      message.sent_frame_type = (flow_u8)safe->sync[k].sent_frame_type;
      message.recv_frame_type = (flow_u8)safe->sync[k].received_frame_type;
      message.tx_index = safe->tx_count[k];
      message.rx_index = safe->rx_count[k];
      message.rx_fc = safe->rx_slot[k].fc;
      message.anomalies = safe->anomalies;
      flow_memcpy (message.inputs, &safe->raw_inputs[k],
         FLOW_SAFE_DATA_SIZE);

      if (safe->sync[k].is_process_data_received)
      {
         message.flags |= SAFE_INPUTS_PROCESS_DATA;
      }
      if (safe->sync[k].reset_event == FSOEMASTER_RESETEVENT_NONE)
      {
         message.flags |= SAFE_INPUTS_STATUS_OK;
      }
      if (safe->send_enabled[k])
      {
         message.flags |= SAFE_INPUTS_SENDING;
      }

#if FLOW_DEBUG
      if ((safe->id == FLOW_CORE_M33) &&
          safe_fault_active (safe, FLOW_TEST_FAULT_INPUTS))
      {
         message.inputs[0] ^= 0x01u;
         FLOW_LOG (safe_tag (safe), "fault injection: inputs k=%u", k);
      }
#endif

      if (!flow_mu_exchange (
             &safe->mu,
             FLOW_MU_CHAN_INPUTS,
             (flow_u8)k,
             safe->cycle,
             &message,
             (flow_u16)sizeof (message),
             &peer,
             FLOW_MU_TIMEOUT_US) ||
          (peer.length != sizeof (message)))
      {
         safe->stats.inputs_mismatches++;
         flow_memset (&safe->safe_inputs[k], 0,
            sizeof (safe->safe_inputs[k]));
         safe_set_sending (safe, k, false);
         safe_note_anomaly (safe, "SafeInputs cross-check timeout");
         safe_request_reset (safe, k);
         continue;
      }

      peer_message = (const safe_inputs_message_t *)peer.payload;
      safe_merge_anomalies (safe, peer_message->anomalies);

      if ((message.tx_index != peer_message->tx_index) ||
          (message.rx_index != peer_message->rx_index) ||
          (message.rx_fc != peer_message->rx_fc))
      {
         safe->stats.inputs_mismatches++;
         flow_memset (&safe->safe_inputs[k], 0,
            sizeof (safe->safe_inputs[k]));
         safe_set_sending (safe, k, false);
         safe_note_anomaly (safe, "SHMEM counters/RX frame divergence");
         safe_request_reset (safe, k);
         safe_realign_counters (safe, k, peer_message);
         continue;
      }

      match = (flow_memcmp (message.inputs, peer_message->inputs,
                  FLOW_SAFE_DATA_SIZE) == 0) &&
         (message.flags == peer_message->flags) &&
         (message.state == peer_message->state) &&
         (message.anomalies == peer_message->anomalies);

      if (!match)
      {
         safe->stats.inputs_mismatches++;
         flow_memset (&safe->safe_inputs[k], 0,
            sizeof (safe->safe_inputs[k]));
         safe_set_sending (safe, k, false);
         safe_note_anomaly (safe, "divergent SafeInputs");
         continue;
      }

      if ((message.flags & SAFE_INPUTS_PROCESS_DATA) == 0u)
      {
         flow_memset (&safe->safe_inputs[k], 0,
            sizeof (safe->safe_inputs[k]));
         if (safe->sync[k].current_state == FSOEMASTER_STATE_DATA)
         {
            safe_set_sending (safe, k, false);
         }
      }
      else
      {
         safe->safe_inputs[k] = safe->raw_inputs[k];
      }
   }
}


/* ------------------- step 4: safe logic + SafeOutputs cross-check */

static void safe_run_logic (flow_safe_t * safe)
{
   flow_u32 k;

   for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++)
   {
      flow_safe_outputs_t next_outputs;
      safe_logic_message_t message;
      const safe_logic_message_t * peer_message;
      flow_mu_message_t peer;
      bool match;

      flow_memset (&next_outputs, 0, sizeof (next_outputs));
      flow_memset (&message, 0, sizeof (message));

      flow_safe_logic_step (
         &safe->safe_inputs[k], &safe->logic[k], &next_outputs);

      message.k = (flow_u16)k;
      message.logic_state = safe_logic_state_bits (&safe->logic[k]);
      flow_memcpy (message.outputs, &next_outputs, FLOW_SAFE_DATA_SIZE);
      message.anomalies = safe->anomalies;

#if FLOW_DEBUG
      if ((safe->id == FLOW_CORE_M33) &&
          safe_fault_active (safe, FLOW_TEST_FAULT_LOGIC))
      {
         message.outputs[0] ^= 0x01u;
         FLOW_LOG (safe_tag (safe), "fault injection: logic k=%u", k);
      }
#endif

      if (!flow_mu_exchange (
             &safe->mu,
             FLOW_MU_CHAN_LOGIC,
             (flow_u8)k,
             safe->cycle,
             &message,
             (flow_u16)sizeof (message),
             &peer,
             FLOW_MU_TIMEOUT_US) ||
          (peer.length != sizeof (message)))
      {
         safe->stats.logic_mismatches++;
         flow_memset (&safe->safe_outputs[k], 0,
            sizeof (safe->safe_outputs[k]));
         safe_set_sending (safe, k, false);
         safe_note_anomaly (safe, "SafeOutputs cross-check timeout");
         safe_request_reset (safe, k);
         continue;
      }

      peer_message = (const safe_logic_message_t *)peer.payload;
      safe_merge_anomalies (safe, peer_message->anomalies);

      match = (peer_message->logic_state == message.logic_state) &&
         (flow_memcmp (
             peer_message->outputs,
             message.outputs,
             FLOW_SAFE_DATA_SIZE) == 0) &&
         (peer_message->anomalies == message.anomalies);

      if (!match)
      {
         safe->stats.logic_mismatches++;
         flow_memset (&safe->safe_outputs[k], 0,
            sizeof (safe->safe_outputs[k]));
         safe_set_sending (safe, k, false);
         safe_note_anomaly (safe, "divergent SafeOutputs");
         safe_request_reset (safe, k);
         continue;
      }

      if (safe->cycle_fault)
      {
         flow_memset (&safe->safe_outputs[k], 0,
            sizeof (safe->safe_outputs[k]));
         safe_set_sending (safe, k, false);
      }
      else
      {
         safe->safe_outputs[k] = next_outputs;
         safe_set_sending (safe, k, true);
      }
   }
}

/* ------------------------------------------------------------ cycle driver */

void flow_safe_step (flow_safe_t * safe)
{
   safe->cycle++;
   safe->stats.cycles++;
   safe->cycle_fault = false;

   /* 1. latch the raw frames received through the SHMEM mailbox */
   safe_latch_rx (safe);

   /* 2. FSoE stack synchronisation; the egress cross-check and publication
    *    happen inside the fsoeapp_send() callback */
   safe_sync (safe);

   /* 3. cross-check of the decoded SafeInputs and connection status */
   safe_crosscheck_inputs (safe);

   /* 4. safe logic and cross-check of the computed SafeOutputs */
   safe_run_logic (safe);

   flow_shm_update_heartbeat (safe->shm, safe->id, safe->cycle);

   if (FLOW_NUM_CONNECTIONS > 0u)
   {
      if ((safe->sync[0].current_state == FSOEMASTER_STATE_DATA) &&
          safe->sync[0].is_process_data_received)
      {
         safe->stats.data_cycles++;
      }
   }

   FLOW_LOG (safe_tag (safe), "cycle %u done, anomalies=%u", safe->cycle,
      safe->anomalies);
}


/* ------------------------------------------------------------------- init */

void flow_safe_init (flow_safe_t * safe, flow_core_id_t id, flow_shm_t * shm)
{
   flow_u32 k;

   flow_memset (safe, 0, (flow_u32)sizeof (*safe));

   safe->id = id;
   safe->shm = shm;
   safe->fault = FLOW_TEST_FAULT_NONE;
   flow_mu_init (&safe->mu, id);

   for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++)
   {
      const fsoemaster_cfg_t cfg =
      {
         .slave_address = (uint16_t)(FLOW_SLAVE_ADDRESS + k),
         .connection_id = (uint16_t)(FLOW_CONNECTION_ID + k),
         .watchdog_timeout_ms = (uint16_t)FLOW_WATCHDOG_MS,
         .application_parameters = safe_app_parameters,
         .application_parameters_size = FLOW_APP_PARAMETERS_SIZE,
         .outputs_size = FLOW_SAFE_DATA_SIZE,
         .inputs_size = FLOW_SAFE_DATA_SIZE,
      };

      safe->app_ref[k].magic = FLOW_FSOE_REF_MAGIC;
      safe->app_ref[k].kind = FLOW_FSOE_KIND_MASTER;
      safe->app_ref[k].k = k;
      safe->app_ref[k].owner = safe;

      flow_safe_logic_init (&safe->logic[k]);

      if (fsoemaster_init (
             &safe->master[k], &cfg, &safe->app_ref[k]) !=
          FSOEMASTER_STATUS_OK)
      {
         FLOW_LOG (safe_tag (safe), "k=%u fsoemaster_init failed", k);
         flow_platform_fatal ();
      }
   }

   FLOW_LOG (safe_tag (safe),
      "safe core ready (%u connection(s), watchdog %u ms)",
      (unsigned)FLOW_NUM_CONNECTIONS, (unsigned)FLOW_WATCHDOG_MS);
}

/* ------------------------------------------------- Session ID negotiation */

flow_u16 flow_safe_on_session (flow_safe_t * safe, flow_u32 k)
{
   safe_session_message_t message;
   const safe_session_message_t * peer_message;
   flow_mu_message_t peer;
   flow_u16 random;
   flow_u16 session;

   flow_memset (&message, 0, sizeof (message));

   random = flow_platform_random_u16 ();
   message.k = (flow_u16)k;
   message.random = random;

#if FLOW_DEBUG
   if ((safe->id == FLOW_CORE_M33) &&
       (safe->fault == FLOW_TEST_FAULT_SESSION) &&
       (safe->session_id == 0u))
   {
      message.random ^= 0x0100u;
      FLOW_LOG (safe_tag (safe), "fault injection: session id");
   }
#endif

   safe->stats.session_requests++;

   if (!flow_mu_exchange (
          &safe->mu,
          FLOW_MU_CHAN_SESSION,
          0u,
          safe->cycle,
          &message,
          (flow_u16)sizeof (message),
          &peer,
          FLOW_MU_TIMEOUT_US) ||
       (peer.length != sizeof (message)))
   {
      safe_note_anomaly (safe, "Session ID negotiation failed");
      safe_request_reset (safe, 0u);
      return 0u;
   }

   peer_message = (const safe_session_message_t *)peer.payload;
   session = (flow_u16)(random + peer_message->random);
   safe->session_id = session;

   FLOW_LOG (safe_tag (safe),
      "SESSION ID: rnd(self)=0x%04x + rnd(peer)=0x%04x = 0x%04x",
      (unsigned)random, (unsigned)peer_message->random,
      (unsigned)session);

   return session;
}


/* --------------------------------------------- step 2b: egress PDU check */

void flow_safe_on_egress (
   flow_safe_t * safe,
   flow_u32 k,
   const void * buffer,
   size_t size)
{
   const flow_u8 * frame = (const flow_u8 *)buffer;
   safe_egress_message_t message;
   const safe_egress_message_t * peer_message;
   flow_mu_message_t peer;
   flow_u8 part[FLOW_PDU_MAX];
   flow_shm_slot_t stored;
   flow_u32 len = (flow_u32)size;
   flow_u32 index;
   const bool crc_part = (safe->id == FLOW_SHM_CRC_NODE);

   if ((k >= FLOW_NUM_CONNECTIONS) || (frame == NULL) ||
       (len == 0u) || (len > FLOW_PDU_MAX))
   {
      safe_note_anomaly (safe, "invalid egress frame");
      return;
   }

   flow_memset (&message, 0, sizeof (message));
   message.k = (flow_u16)k;
   message.len = (flow_u16)len;
   message.rx_fc = safe->rx_slot[k].fc;
   message.anomalies = safe->anomalies;
   flow_memcpy (message.pdu, frame, len);

#if FLOW_DEBUG
   if ((safe->id == FLOW_CORE_M33) &&
       safe_fault_active (safe, FLOW_TEST_FAULT_PDU) && (len > 1u))
   {
      message.pdu[1] ^= 0xFFu;
      FLOW_LOG (safe_tag (safe), "fault injection: egress PDU k=%u", k);
   }
#endif

   if (!flow_mu_exchange (
          &safe->mu,
          FLOW_MU_CHAN_EGRESS,
          (flow_u8)k,
          safe->cycle,
          &message,
          (flow_u16)sizeof (message),
          &peer,
          FLOW_MU_TIMEOUT_US) ||
       (peer.length != sizeof (message)))
   {
      safe->stats.pdu_mismatches++;
      safe_note_anomaly (safe, "egress PDU cross-check timeout");
      safe_request_reset (safe, k);
      return;
   }

   peer_message = (const safe_egress_message_t *)peer.payload;
   safe_merge_anomalies (safe, peer_message->anomalies);

   if ((peer_message->len != message.len) ||
       (peer_message->rx_fc != message.rx_fc) ||
       (peer_message->anomalies != message.anomalies) ||
       (flow_memcmp (peer_message->pdu, message.pdu, len) != 0))
   {
      safe->stats.pdu_mismatches++;
      safe_note_anomaly (safe, "the two cores generated different PDUs");
      safe_request_reset (safe, k);
      return;
   }

   /* Fault injection: one core does not publish even if its PDU is valid.
    * The counters then diverge and the next inputs cross-check detects it. */
   if ((safe->id == FLOW_CORE_M33) &&
       safe_fault_active (safe, FLOW_TEST_FAULT_STALL))
   {
      FLOW_LOG (safe_tag (safe), "fault injection: stall k=%u", k);
      return;
   }

   flow_shm_split (part, frame, len, crc_part);

   index = safe->tx_count[k] + 1u;

   if (!flow_shm_stage (
          &safe->shm->tx[k][safe->id], index, len, safe->cycle, 0u, part))
   {
      safe_note_anomaly (safe, "cannot write the TX half into SHMEM");
      safe_request_reset (safe, k);
      return;
   }

   if (!flow_shm_read_index (
          &safe->shm->tx[k][safe->id], index, &stored) ||
       !flow_shm_part_matches (stored.pdu, frame, len, crc_part))
   {
      safe_note_anomaly (safe, "TX half corrupted in SHMEM (read-back)");
      safe_request_reset (safe, k);
      return;
   }

   safe->tx_count[k]++;
   flow_shm_commit (&safe->shm->tx[k][safe->id], safe->tx_count[k]);
   safe->stats.tx_published++;

   FLOW_LOG (safe_tag (safe), "k=%u published frame %u len=%u (%s half)",
      k, safe->tx_count[k], len, crc_part ? "CRC" : "data");
}

size_t flow_safe_on_recv (
   flow_safe_t * safe,
   flow_u32 k,
   void * buffer,
   size_t size)
{
   if ((k >= FLOW_NUM_CONNECTIONS) || (buffer == NULL))
   {
      return 0u;
   }

   if (!safe->rx_ready[k])
   {
      return 0u;
   }

   safe->rx_ready[k] = false;

   if (safe->rx_slot[k].len != (flow_u32)size)
   {
      safe->stats.rx_latch_failures++;
      FLOW_LOG (safe_tag (safe), "k=%u RX frame has unexpected length %u",
         k, safe->rx_slot[k].len);
      return 0u;
   }

   flow_memcpy (buffer, safe->rx_slot[k].pdu, (flow_u32)size);

   return size;
}

flow_u8 flow_safe_validate_parameters (
   flow_u16 timeout_ms,
   const void * parameters,
   size_t size)
{
   const flow_u8 * bytes = (const flow_u8 *)parameters;
   flow_u16 param1;
   flow_u16 param2;

   (void)timeout_ms;

   if ((bytes == NULL) || (size != FLOW_APP_PARAMETERS_SIZE))
   {
      return FSOEAPP_STATUS_BAD_APP_PARAMETER;
   }

   param1 = (flow_u16)(((flow_u16)bytes[1] << 8u) | bytes[0]);
   param2 = (flow_u16)(((flow_u16)bytes[3] << 8u) | bytes[2]);

   if ((param1 != FLOW_APP_PARAMETER_1) || (param2 != FLOW_APP_PARAMETER_2))
   {
      return FSOEAPP_STATUS_BAD_APP_PARAMETER;
   }

   return FSOEAPP_STATUS_OK;
}

