/*
 * safe_core.c
 *
 * See safe_core.h. This file implements, step by step, the safe cycle of
 * sequence diag.md. The very same code runs in the M7 thread and in the M33
 * thread: the two instances only differ by their identity (mu_node_t), their
 * RNG and their FSoE master instance.
 */

#include "safe_core.h"
#include "flow_digest.h"
#include "flow_log.h"
#include "flow_time.h"

#include <string.h>

/* ------------------------------------------------------------- MU payloads */

/* MU_CHAN_SESSION_ID */
typedef struct core_session_msg
{
   uint16_t k;
   uint16_t rnd;
} core_session_msg_t;

/* MU_CHAN_RX_SNAPSHOT: which SHMEM bank M7 latched */
typedef struct core_rx_snapshot_msg
{
   uint16_t k;
   uint16_t bank;
   uint32_t published;
   uint32_t digest;
   uint32_t len;
   uint32_t anomalies;
} core_rx_snapshot_msg_t;

/* MU_CHAN_EGRESS_PDU: digest of the PDU generated for connection k */
typedef struct core_egress_msg
{
   uint16_t k;
   uint16_t len;
   uint32_t tx_digest;    /**< digest of the generated PDU                    */
   uint32_t tx_bank;      /**< SHMEM bank staged by M7 (used by M33 to commit) */
   uint32_t rx_published; /**< publication counter of the latched RX frame    */
   uint32_t rx_digest;    /**< digest of the latched RX frame                 */
   uint32_t anomalies;
} core_egress_msg_t;

/* MU_CHAN_SAFE_INPUTS: decoded safe inputs + connection status */
typedef struct core_inputs_msg
{
   uint16_t k;
   uint16_t flags; /**< bit0 process data received, bit1 status OK            */
   uint8_t inputs[FLOW_SAFE_DATA_SIZE];
   uint8_t state;  /**< fsoemaster_state_t                                    */
   uint8_t sent_frame_type;
   uint8_t recv_frame_type;
   uint8_t reserved;
   uint32_t anomalies;
} core_inputs_msg_t;

#define CORE_INPUTS_FLAG_PROCESS_DATA 0x0001u
#define CORE_INPUTS_FLAG_STATUS_OK 0x0002u
#define CORE_INPUTS_FLAG_SEND_ENABLED 0x0004u

/* MU_CHAN_SAFE_LOGIC: computed SafeOutputs + internal logic state */
typedef struct core_logic_msg
{
   uint16_t k;
   uint16_t logic_state; /**< bit0 enabled, bit1 latched fault, bit2 discrep. */
   uint8_t outputs[sizeof (flow_safe_outputs_t)];
   uint32_t digest;
   uint32_t anomalies;
} core_logic_msg_t;

/* ---------------------------------------------------------------- helpers */

static uint32_t core_logic_digest (
   const flow_safe_logic_t * logic,
   const flow_safe_outputs_t * outputs)
{
   uint32_t state[3];
   uint32_t digest;

   state[0] = logic->enabled;
   state[1] = logic->latched_fault;
   state[2] = logic->discrepancy_count;

   digest = flow_digest (outputs, sizeof (*outputs));
   digest = flow_digest_update (digest, state, sizeof (state));

   return digest;
}

static uint16_t core_logic_state_bits (const flow_safe_logic_t * logic)
{
   uint16_t bits = 0u;

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

static bool core_stopping (const flow_safe_core_t * core)
{
   return atomic_load_explicit (core->env.stop, memory_order_relaxed);
}

static void core_note_anomaly (
   flow_safe_core_t * core,
   const char * what)
{
   if (core_stopping (core))
   {
      /* Shutdown in progress: the peer thread is already gone, this is not a
       * divergence of the 1oo2 pair. */
      LOG_TRACE (core->env.tag, "shutdown during cycle %u (%s)",
         core->cycle, what);
      return;
   }

   core->anomalies++;
   core->stats.mu_anomalies++;

   LOG_ERROR (core->env.tag,
      "ANOMALY (cycle %u): %s -> fail-safe + connection reset",
      core->cycle,
      what);
}

/* Keep the divergence counter aligned on both cores: as soon as one core
 * notices a problem, the peer adopts the same counter value. */
static void core_merge_anomalies (flow_safe_core_t * core, uint32_t peer)
{
   if (peer > core->anomalies)
   {
      core->anomalies = peer;
   }
}

static void core_request_reset (flow_safe_core_t * core, uint32_t k)
{
   if (core_stopping (core))
   {
      return;
   }

   if (fsoemaster_set_reset_request_flag (&core->master[k]) ==
       FSOEMASTER_STATUS_OK)
   {
      core->stats.reset_requests++;
      LOG_WARN (core->env.tag,
         "k=%u: fsoemaster_set_reset_request_flag()", k);
   }
}

static void core_set_sending_enabled (
   flow_safe_core_t * core,
   uint32_t k,
   bool enable)
{
   fsoemaster_status_t status;

   if (enable == core->send_enabled[k])
   {
      return;
   }

   if (enable)
   {
      status = fsoemaster_set_process_data_sending_enable_flag (
         &core->master[k]);
   }
   else
   {
      status = fsoemaster_clear_process_data_sending_enable_flag (
         &core->master[k]);
   }

   if (status != FSOEMASTER_STATUS_OK)
   {
      return;
   }

   core->send_enabled[k] = enable;

   if (enable)
   {
      core->stats.enable_set++;
      LOG_INFO (core->env.tag,
         "k=%u: process data sending ENABLED (SafeOutputs are sent)", k);
   }
   else
   {
      core->stats.enable_clear++;
      LOG_WARN (core->env.tag,
         "k=%u: process data sending DISABLED (FailSafeData is sent)", k);
   }
}


/* ------------------------------------------------- Session ID negotiation */

/*
 * 1oo2 Session ID negotiation (see pseudo_code.md, "Session ID"):
 * both cores generate their own random number, exchange it over the messaging
 * unit and the Session ID actually used is the sum of the two.
 *
 * The function is called by the stack from inside fsoemaster_sync_with_slave()
 * when the master transitions from Reset to Session, i.e. exactly once per
 * connection establishment attempt. Both cores are in lockstep, so both reach
 * this rendezvous in the same safe cycle.
 */
static uint16_t core_session_id (flow_app_ctx_t * ctx)
{
   flow_safe_core_t * core = ctx->owner;
   core_session_msg_t message;
   const core_session_msg_t * peer_message;
   mu_message_t peer;
   uint32_t random_value;
   uint16_t session_id;

   if (core_stopping (core))
   {
      return (uint16_t)(ctx->session_id & 0xFFFFu);
   }

   memset (&message, 0, sizeof (message));

   random_value = flow_rng_next_u16 (core->env.rng);

   message.k = (uint16_t)ctx->k;
   message.rnd = (uint16_t)random_value;

   core->stats.session_negotiations++;
   atomic_fetch_add_explicit (
      core->env.session_requests, 1u, memory_order_relaxed);

   if (!mu_exchange (
          core->env.mu,
          core->env.id,
          MU_CHAN_SESSION_ID,
          core->cycle,
          ctx->k,
          &message,
          (uint32_t)sizeof (message),
          &peer,
          FLOW_MU_TIMEOUT_US))
   {
      core->stats.session_failures++;
      core_note_anomaly (core, "Session ID negotiation failed (peer timeout)");
      core_request_reset (core, ctx->k);
      return (uint16_t)(ctx->session_id & 0xFFFFu);
   }

   if (peer.len != sizeof (core_session_msg_t))
   {
      core->stats.session_failures++;
      core_note_anomaly (core, "Session ID message of unexpected size");
      core_request_reset (core, ctx->k);
      return (uint16_t)(ctx->session_id & 0xFFFFu);
   }

   peer_message = (const core_session_msg_t *)peer.payload;
   session_id = (uint16_t)((uint32_t)(uint16_t)random_value +
                           (uint32_t)peer_message->rnd);

   LOG_INFO (core->env.tag,
      "k=%u SESSION ID negotiation: rnd(self)=0x%04x + rnd(peer)=0x%04x "
      "= 0x%04x",
      ctx->k,
      (unsigned)random_value,
      (unsigned)peer_message->rnd,
      (unsigned)session_id);

   /* Fault injection: the corruption is applied to the *final* Session ID of
    * M33, i.e. the value its stack will actually use (memory corruption of
    * the negotiated value). A corrupted random number would not be a valid
    * test: because the Session ID is the sum of the two randoms, both cores
    * would still compute the same sum. */
   if ((core->env.id == MU_NODE_M33) &&
       flow_fault_hits (core->env.fault, FLOW_FAULT_SESSION, core->cycle))
   {
      session_id ^= 0x0100u;
      flow_fault_note (core->env.fault);
      LOG_ERROR (core->env.tag,
         "FAULT INJECTION (session) cycle %u: M33 will use Session ID 0x%04x "
         "instead of 0x%04x",
         core->cycle,
         (unsigned)session_id,
         (unsigned)(session_id ^ 0x0100u));
   }

   ctx->session_rnd = random_value;
   ctx->session_id = (uint32_t)session_id;

   atomic_store_explicit (
      core->env.session_id, (uint32_t)session_id, memory_order_relaxed);

   return session_id;
}


/* --------------------------------------------- Step 2b: egress PDU check */

/*
 * Called from inside fsoemaster_sync_with_slave() whenever the stack wants to
 * put a frame on the black channel (fsoeapp_send).
 *
 *   1. both cores publish the digest of the generated PDU (and the digest of
 *      the RX frame they latched) on the messaging unit
 *   2. if the digests are identical M7 stages the PDU into the inactive SHMEM
 *      bank and M33 validates + commits it (2-phase commit)
 *   3. otherwise the connection is reset on both cores
 */
static void core_on_egress (
   flow_app_ctx_t * ctx,
   const flow_fsoe_slot_t * frame)
{
   flow_safe_core_t * core = ctx->owner;
   const uint32_t k = ctx->k;
   core_egress_msg_t message;
   const core_egress_msg_t * peer_message;
   mu_message_t peer;
   bool match;

   if (core_stopping (core))
   {
      return;
   }

   memset (&message, 0, sizeof (message));

   message.k = (uint16_t)k;
   message.len = (uint16_t)frame->len;
   message.tx_digest = frame->digest;
   message.tx_bank = 0u;
   message.rx_published = core->latched_published[k];
   message.rx_digest = core->latched[k].digest;
   message.anomalies = core->anomalies;

   core->stats.frames_sent++;

   if ((core->env.id == MU_NODE_M33) &&
       flow_fault_hits (core->env.fault, FLOW_FAULT_PDU, core->cycle))
   {
      message.tx_digest ^= 0x0000A5A5u;
      flow_fault_note (core->env.fault);
      LOG_ERROR (core->env.tag,
         "FAULT INJECTION (pdu) cycle %u: corrupted PDU digest for k=%u",
         core->cycle,
         k);
   }

   /* Phase 1: M7 prepares the bank, but does not publish it. */
   if (core->env.id == MU_NODE_M7)
   {
      if (!flow_shmem_tx_stage (
             core->env.shm, k, frame, &message.tx_bank, NULL))
      {
         core->stats.commit_failures++;
         core_note_anomaly (core, "cannot stage the TX frame in SHMEM");
         core_request_reset (core, k);
         return;
      }
   }

   if (!mu_exchange (
          core->env.mu,
          core->env.id,
          MU_CHAN_EGRESS_PDU,
          core->cycle,
          k,
          &message,
          (uint32_t)sizeof (message),
          &peer,
          FLOW_MU_TIMEOUT_US))
   {
      core->stats.egress_timeouts++;
      core_note_anomaly (core, "egress PDU cross-check timed out");
      core_request_reset (core, k);
      return;
   }

   if (peer.len != sizeof (core_egress_msg_t))
   {
      core->stats.egress_timeouts++;
      core_note_anomaly (core, "egress message of unexpected size");
      core_request_reset (core, k);
      return;
   }

   peer_message = (const core_egress_msg_t *)peer.payload;

   match = (peer_message->tx_digest == message.tx_digest) &&
      (peer_message->len == message.len) &&
      (peer_message->rx_digest == message.rx_digest) &&
      (peer_message->rx_published == message.rx_published) &&
      (peer_message->anomalies == message.anomalies);

   core_merge_anomalies (core, peer_message->anomalies);

   if (!match)
   {
      core->stats.pdu_mismatches++;
      LOG_ERROR (core->env.tag,
         "EGRESS CROSS-CHECK FAILED k=%u: digest own|peer 0x%08x|0x%08x "
         "len %u|%u rx 0x%08x|0x%08x anomalies %u|%u",
         k,
         message.tx_digest,
         peer_message->tx_digest,
         (unsigned)message.len,
         (unsigned)peer_message->len,
         message.rx_digest,
         peer_message->rx_digest,
         (unsigned)message.anomalies,
         (unsigned)peer_message->anomalies);
      core_note_anomaly (core, "the two cores generated different FSoE PDUs");
      core_request_reset (core, k);
      return;
   }

   core->stats.pdu_matches++;

   LOG_DETAIL (core->env.tag,
      "k=%u egress PDU OK: %s len=%u digest=0x%08x (tx bank %u)",
      k,
      flow_fsoe_command_name (flow_fsoe_command (frame)),
      (unsigned)frame->len,
      message.tx_digest,
      (unsigned)((core->env.id == MU_NODE_M7) ?
         message.tx_bank : peer_message->tx_bank));

   /* Phase 2: M33 validates the bank staged by M7 and publishes it. */
   if (core->env.id == MU_NODE_M33)
   {
      uint32_t published = 0u;

      if (!flow_shmem_tx_commit (
             core->env.shm,
             k,
             peer_message->tx_bank,
             peer_message->tx_digest,
             &published))
      {
         core->stats.commit_failures++;
         core_note_anomaly (core, "TX bank commit failed");
         core_request_reset (core, k);
         return;
      }

      LOG_TRACE (core->env.tag,
         "k=%u: TX bank %u validated and committed (published=%u)",
         k,
         (unsigned)peer_message->tx_bank,
         (unsigned)published);
   }
}

/* --------------------------------------- Step 1: latch of the raw RX frames */

static void core_latch_rx (flow_safe_core_t * core)
{
   uint32_t k;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      flow_fsoe_slot_t * slot = &core->latched[k];

      if (core_stopping (core))
      {
         break;
      }

      if (core->env.id == MU_NODE_M7)
      {
         core_rx_snapshot_msg_t message;

         memset (&message, 0, sizeof (message));

         if (!flow_shmem_rx_latch (
                core->env.shm, k, slot, &core->latched_published[k]))
         {
            core->stats.rx_latch_failures++;
            slot->len = 0u;
            slot->digest = 0u;
            core->latched_published[k] = 0u;
         }

         /* Tell M33 exactly which bank has been latched, so that both cores
          * work on byte identical raw frames. */
         message.k = (uint16_t)k;
         message.bank = flow_shmem_rx_active (core->env.shm, k);
         message.published = core->latched_published[k];
         message.digest = slot->digest;
         message.len = slot->len;
         message.anomalies = core->anomalies;

         (void)mu_publish (
            core->env.mu,
            core->env.id,
            MU_CHAN_RX_SNAPSHOT,
            core->cycle,
            k,
            &message,
            (uint32_t)sizeof (message));
      }
      else
      {
         mu_message_t peer;
         const core_rx_snapshot_msg_t * message;
         bool latched = false;

         if (mu_wait (
                core->env.mu,
                core->env.id,
                MU_CHAN_RX_SNAPSHOT,
                core->cycle,
                k,
                &peer,
                FLOW_MU_TIMEOUT_US) &&
             (peer.len == sizeof (core_rx_snapshot_msg_t)))
         {
            message = (const core_rx_snapshot_msg_t *)peer.payload;

            core_merge_anomalies (core, message->anomalies);

            if (flow_shmem_rx_read_bank (
                   core->env.shm, k, message->bank, slot) &&
                (slot->digest == message->digest) &&
                (slot->len == message->len))
            {
               core->latched_published[k] = message->published;
               latched = true;
            }
         }

         if (!latched)
         {
            /* Fallback: latch whatever is currently published. */
            core->stats.rx_latch_failures++;
            if (!flow_shmem_rx_latch (
                   core->env.shm, k, slot, &core->latched_published[k]))
            {
               slot->len = 0u;
               slot->digest = 0u;
               core->latched_published[k] = 0u;
            }
         }
      }

      if ((slot->len > 0u) &&
          (core->provided_published[k] != core->latched_published[k]))
      {
         flow_app_ctx_provide_rx (
            &core->ctx[k], slot, core->latched_published[k]);
         core->provided_published[k] = core->latched_published[k];

         LOG_TRACE (core->env.tag,
            "k=%u latched RX %s len=%u fc=%u digest=0x%08x",
            k,
            flow_fsoe_command_name (flow_fsoe_command (slot)),
            (unsigned)slot->len,
            (unsigned)core->latched_published[k],
            slot->digest);
      }
   }
}

/* ------------------------------------ Step 2: FSoE stack synchronisation */

static void core_sync (flow_safe_core_t * core)
{
   uint32_t k;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      fsoemaster_state_t previous_state = core->sync[k].current_state;
      fsoemaster_status_t status;

      memset (&core->raw_inputs[k], 0, sizeof (core->raw_inputs[k]));
      memset (&core->sync[k], 0, sizeof (core->sync[k]));

      status = fsoemaster_sync_with_slave (
         &core->master[k],
         &core->safe_outputs[k],
         &core->raw_inputs[k],
         &core->sync[k]);

      if (status != FSOEMASTER_STATUS_OK)
      {
         LOG_ERROR (core->env.tag,
            "k=%u: fsoemaster_sync_with_slave() failed", k);
         continue;
      }

      if (core->sync[k].is_process_data_received)
      {
         core->stats.frames_received++;
      }

      if (core->sync[k].current_state != previous_state)
      {
         core->stats.state_changes++;
         LOG_INFO (core->env.tag,
            "k=%u FSoE state: %s -> %s",
            k,
            fsoemaster_state_description (previous_state),
            fsoemaster_state_description (core->sync[k].current_state));
      }

      if (core->sync[k].reset_event != FSOEMASTER_RESETEVENT_NONE)
      {
         core->stats.reset_events++;
         /* The stack falls back to FailSafeData on a reset: the application
          * has to re-enable process data explicitly. */
         core->send_enabled[k] = false;
         LOG_WARN (core->env.tag,
            "k=%u connection reset by %s, cause: %s",
            k,
            (core->sync[k].reset_event == FSOEMASTER_RESETEVENT_BY_MASTER) ?
               "master" : "slave",
            fsoemaster_reset_reason_description (core->sync[k].reset_reason));
      }
   }
}


/* ------------------------ Step 3: cross-check of the decoded SafeInputs */

static void core_crosscheck_inputs (flow_safe_core_t * core)
{
   uint32_t k;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      core_inputs_msg_t message;
      const core_inputs_msg_t * peer_message;
      mu_message_t peer;
      bool match;

      if (core_stopping (core))
      {
         break;
      }

      memset (&message, 0, sizeof (message));

      message.k = (uint16_t)k;
      message.state = (uint8_t)core->sync[k].current_state;
      message.sent_frame_type = (uint8_t)core->sync[k].sent_frame_type;
      message.recv_frame_type = (uint8_t)core->sync[k].received_frame_type;
      message.anomalies = core->anomalies;
      memcpy (message.inputs, &core->raw_inputs[k], FLOW_SAFE_DATA_SIZE);

      if (core->sync[k].is_process_data_received)
      {
         message.flags |= CORE_INPUTS_FLAG_PROCESS_DATA;
      }
      if (core->sync[k].reset_event == FSOEMASTER_RESETEVENT_NONE)
      {
         message.flags |= CORE_INPUTS_FLAG_STATUS_OK;
      }
      if (core->send_enabled[k])
      {
         message.flags |= CORE_INPUTS_FLAG_SEND_ENABLED;
      }

      if ((core->env.id == MU_NODE_M33) &&
          flow_fault_hits (core->env.fault, FLOW_FAULT_INPUTS, core->cycle))
      {
         message.inputs[0] ^= 0x01u;
         flow_fault_note (core->env.fault);
         LOG_ERROR (core->env.tag,
            "FAULT INJECTION (inputs) cycle %u: corrupted decoded SafeInputs "
            "for k=%u",
            core->cycle,
            k);
      }

      if (!mu_exchange (
             core->env.mu,
             core->env.id,
             MU_CHAN_SAFE_INPUTS,
             core->cycle,
             k,
             &message,
             (uint32_t)sizeof (message),
             &peer,
             FLOW_MU_TIMEOUT_US) ||
          (peer.len != sizeof (core_inputs_msg_t)))
      {
         core->stats.inputs_mismatches++;
         memset (&core->safe_inputs[k], 0, sizeof (core->safe_inputs[k]));
         core_set_sending_enabled (core, k, false);
         core_note_anomaly (core, "SafeInputs cross-check timed out");
         continue;
      }

      peer_message = (const core_inputs_msg_t *)peer.payload;
      core_merge_anomalies (core, peer_message->anomalies);

      match = (memcmp (
                  message.inputs,
                  peer_message->inputs,
                  FLOW_SAFE_DATA_SIZE) == 0) &&
         (message.flags == peer_message->flags) &&
         (message.state == peer_message->state) &&
         (message.anomalies == peer_message->anomalies);

      if (!match)
      {
         core->stats.inputs_mismatches++;
         LOG_ERROR (core->env.tag,
            "INPUTS CROSS-CHECK FAILED k=%u: in 0x%02x%02x|0x%02x%02x "
            "flags 0x%04x|0x%04x state %s|%s anomalies %u|%u",
            k,
            message.inputs[1], message.inputs[0],
            peer_message->inputs[1], peer_message->inputs[0],
            (unsigned)message.flags,
            (unsigned)peer_message->flags,
            fsoemaster_state_description ((fsoemaster_state_t)message.state),
            fsoemaster_state_description ((fsoemaster_state_t)peer_message->state),
            (unsigned)message.anomalies,
            (unsigned)peer_message->anomalies);

         memset (&core->safe_inputs[k], 0, sizeof (core->safe_inputs[k]));
         core_set_sending_enabled (core, k, false);
         core_note_anomaly (core, "divergent SafeInputs");
         continue;
      }

      if ((message.flags & CORE_INPUTS_FLAG_PROCESS_DATA) == 0u)
      {
         /* No valid process data yet (startup, fail-safe data): SafeInputs
          * are forced to zero. Once the connection is in Data state this is
          * also the trigger to stop sending process data. */
         memset (&core->safe_inputs[k], 0, sizeof (core->safe_inputs[k]));
         if (core->sync[k].current_state == FSOEMASTER_STATE_DATA)
         {
            core_set_sending_enabled (core, k, false);
         }
      }
      else
      {
         core->safe_inputs[k] = core->raw_inputs[k];
         core->stats.inputs_ok++;
      }
   }
}


/* ------------------- Step 4: safe logic + cross-check of SafeOutputs */

static void core_run_logic (flow_safe_core_t * core)
{
   uint32_t k;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      flow_safe_outputs_t next_outputs;
      core_logic_msg_t message;
      const core_logic_msg_t * peer_message;
      mu_message_t peer;
      uint32_t digest;
      bool match = false;

      if (core_stopping (core))
      {
         break;
      }

      memset (&next_outputs, 0, sizeof (next_outputs));
      memset (&message, 0, sizeof (message));

      flow_safe_logic_step (
         &core->safe_inputs[k], &core->logic[k], &next_outputs);
      digest = core_logic_digest (&core->logic[k], &next_outputs);

      message.k = (uint16_t)k;
      message.logic_state = core_logic_state_bits (&core->logic[k]);
      memcpy (message.outputs, &next_outputs, sizeof (next_outputs));
      message.digest = digest;
      message.anomalies = core->anomalies;

      if ((core->env.id == MU_NODE_M33) &&
          flow_fault_hits (core->env.fault, FLOW_FAULT_LOGIC, core->cycle))
      {
         message.digest ^= 0x5A5A0000u;
         flow_fault_note (core->env.fault);
         LOG_ERROR (core->env.tag,
            "FAULT INJECTION (logic) cycle %u: corrupted SafeOutputs digest "
            "for k=%u",
            core->cycle,
            k);
      }

      if (mu_exchange (
             core->env.mu,
             core->env.id,
             MU_CHAN_SAFE_LOGIC,
             core->cycle,
             k,
             &message,
             (uint32_t)sizeof (message),
             &peer,
             FLOW_MU_TIMEOUT_US) &&
          (peer.len == sizeof (core_logic_msg_t)))
      {
         peer_message = (const core_logic_msg_t *)peer.payload;
         core_merge_anomalies (core, peer_message->anomalies);

         match = (peer_message->digest == message.digest) &&
            (peer_message->logic_state == message.logic_state) &&
            (memcmp (
                peer_message->outputs,
                message.outputs,
                sizeof (next_outputs)) == 0) &&
            (peer_message->anomalies == message.anomalies);
      }

      if (!match)
      {
         core->stats.logic_mismatches++;
         memset (&core->safe_outputs[k], 0, sizeof (core->safe_outputs[k]));
         core_set_sending_enabled (core, k, false);
         core_note_anomaly (core, "divergent SafeOutputs (safe logic)");
         core_request_reset (core, k);
         continue;
      }

      core->safe_outputs[k] = next_outputs;
      core->stats.logic_ok++;
      core_set_sending_enabled (core, k, true);

      LOG_DETAIL (core->env.tag,
         "k=%u logic OK: in=0x%02x%02x out=0x%04x/0x%04x digest=0x%08x",
         k,
         ((const uint8_t *)&core->safe_inputs[k])[1],
         ((const uint8_t *)&core->safe_inputs[k])[0],
         (unsigned)next_outputs.enable,
         (unsigned)next_outputs.status,
         digest);
   }
}

/* ------------------------------------------------------------ cycle driver */

static void core_publish_telemetry (flow_safe_core_t * core)
{
   const uint32_t slot = (core->env.id == MU_NODE_M7) ? 0u : 1u;
   uint32_t state = 0u;
   uint32_t outputs = 0u;

   if (FLOW_NUM_CONNECTIONS > 0u)
   {
      state = (uint32_t)core->sync[0].current_state;
      outputs = (uint32_t)core->safe_outputs[0].enable |
         ((uint32_t)core->safe_outputs[0].status << 16);
   }

   atomic_store_explicit (
      &core->env.shm->core_state[slot], state, memory_order_relaxed);
   atomic_store_explicit (
      &core->env.shm->core_outputs[slot], outputs, memory_order_relaxed);
}


static void core_cycle (flow_safe_core_t * core)
{
   core->cycle++;
   core->stats.cycles++;

   /* 1. Local latch of the raw frames received from the SHMEM mailbox. */
   flow_shmem_cores_enter_cycle (core->env.shm, core->cycle);
   core_latch_rx (core);

   /* 2. FSoE stack synchronisation (inversion of control). The egress PDU
    *    cross-check and the SHMEM 2-phase commit happen inside this call. */
   core_sync (core);
   flow_shmem_cores_exit_cycle (core->env.shm);

   /* 3. Cross-check of the decoded SafeInputs and of the status. */
   core_crosscheck_inputs (core);

   /* 4. Safe logic and cross-check of the computed SafeOutputs. */
   core_run_logic (core);

   core_publish_telemetry (core);

   if (FLOW_NUM_CONNECTIONS > 0u)
   {
      if ((core->sync[0].current_state == FSOEMASTER_STATE_DATA) &&
          core->sync[0].is_process_data_received)
      {
         core->data_state_cycles++;
         core->stats.data_cycles++;
      }

      if (flow_log_is_enabled (FLOW_LOG_LEVEL_DETAIL))
      {
         char description[64];

         LOG_DETAIL (core->env.tag,
            "cyc %-4u %-10s in=0x%02x%02x out=%s anomalies=%u",
            core->cycle,
            fsoemaster_state_description (core->sync[0].current_state),
            ((const uint8_t *)&core->safe_inputs[0])[1],
            ((const uint8_t *)&core->safe_inputs[0])[0],
            flow_safe_logic_describe (
               &core->safe_outputs[0], description, sizeof (description)),
            core->anomalies);
      }
   }
}

/* ------------------------------------------------------------------- API */

void flow_safe_core_init (flow_safe_core_t * core, const flow_core_env_t * env)
{
   const uint16_t app_parameters[2] =
   {
      (uint16_t)FLOW_APP_PARAMETER_1,
      (uint16_t)FLOW_APP_PARAMETER_2,
   };
   uint32_t k;

   memset (core, 0, sizeof (*core));
   core->env = *env;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      const fsoemaster_cfg_t cfg =
      {
         .slave_address = (uint16_t)(FLOW_SLAVE_ADDRESS + k),
         .connection_id = (uint16_t)(FLOW_CONNECTION_ID + k),
         .watchdog_timeout_ms = (uint16_t)FLOW_WATCHDOG_MS,
         .application_parameters = app_parameters,
         .application_parameters_size = FLOW_APP_PARAMETERS_SIZE,
         .outputs_size = FLOW_SAFE_DATA_SIZE,
         .inputs_size = FLOW_SAFE_DATA_SIZE,
      };
      const flow_ctx_kind_t kind =
         (env->id == MU_NODE_M7) ? FLOW_CTX_M7 : FLOW_CTX_M33;

      flow_safe_logic_init (&core->logic[k]);
      memset (&core->safe_inputs[k], 0, sizeof (core->safe_inputs[k]));
      memset (&core->raw_inputs[k], 0, sizeof (core->raw_inputs[k]));
      memset (&core->safe_outputs[k], 0, sizeof (core->safe_outputs[k]));
      memset (&core->sync[k], 0, sizeof (core->sync[k]));
      flow_fsoe_slot_clear (&core->latched[k]);
      core->latched_published[k] = 0u;
      core->provided_published[k] = 0u;
      core->send_enabled[k] = false;

      flow_app_ctx_init (
         &core->ctx[k],
         kind,
         k,
         &core->master[k],
         core,
         core_on_egress,
         core_session_id);

      if (fsoemaster_init (&core->master[k], &cfg, &core->ctx[k]) !=
          FSOEMASTER_STATUS_OK)
      {
         LOG_ERROR (env->tag, "k=%u: fsoemaster_init() failed", k);
      }
   }

   LOG_INFO (env->tag,
      "safe core ready (%u connection(s), watchdog %u ms, %u data bytes)",
      (unsigned)FLOW_NUM_CONNECTIONS,
      (unsigned)FLOW_WATCHDOG_MS,
      (unsigned)FLOW_SAFE_DATA_SIZE);
}

void flow_safe_core_run (flow_safe_core_t * core)
{
   LOG_INFO (core->env.tag, "thread started");

   while (!atomic_load_explicit (core->env.stop, memory_order_relaxed))
   {
      flow_tick_result_t tick =
         flow_safe_tick_wait (core->env.tick, core->env.id);

      if (tick == FLOW_TICK_STOPPED)
      {
         break;
      }
      if (tick == FLOW_TICK_TIMEOUT)
      {
         core->stats.tick_timeouts++;
      }

      core_cycle (core);
   }

   LOG_INFO (core->env.tag, "thread stopped after %u cycles", core->cycle);
}

void * flow_safe_core_thread (void * argument)
{
   flow_safe_core_t * core = (flow_safe_core_t *)argument;

   flow_safe_core_run (core);

   return NULL;
}

