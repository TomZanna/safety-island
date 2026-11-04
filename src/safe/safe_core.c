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

/* MU_CHAN_EGRESS_PDU: cross-check of the PDU generated for connection k */
typedef struct core_egress_msg
{
   uint16_t k;
   uint16_t len;
   uint32_t tx_digest; /**< digest of the frame as stored in SHMEM            */
   uint32_t rx_digest; /**< digest of the latched RX frame                    */
   uint32_t anomalies;
} core_egress_msg_t;

/* MU_CHAN_SAFE_INPUTS: decoded safe inputs + status + SHMEM counters */
typedef struct core_inputs_msg
{
   uint16_t k;
   uint16_t flags; /**< bit0 process data received, bit1 status OK            */
   uint8_t inputs[FLOW_SAFE_DATA_SIZE];
   uint8_t state;  /**< fsoemaster_state_t                                    */
   uint8_t sent_frame_type;
   uint8_t recv_frame_type;
   uint8_t reserved;
   uint32_t tx_index;  /**< frames published to the A55 by this core           */
   uint32_t rx_index;  /**< frames consumed from the A55 by this core          */
   uint32_t rx_digest; /**< digest of the RX frame this core is working on     */
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
   core->cycle_fault = true;

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
 *   1. both cores exchange on the messaging unit the digest of the frame their
 *      stack generated: the two stacks must produce the same PDU
 *   2. only if that holds, each core writes **its own half** of the frame in
 *      its own SHMEM bank: the CRC core contributes the CRC words, the data
 *      core the command byte, the safety data and the Connection ID
 *   3. the core reads its half back from SHMEM and compares it byte by byte
 *      with what it generated (a corruption of the copy is caught here)
 *   4. the frame is published (own counter incremented)
 *
 * The A55 assembles the final frame by interleaving the two halves. Nobody
 * coordinates anybody: the counters of the two cores stay aligned because a
 * frame is published only after the cross-check succeeded.
 */
static void core_on_egress (
   flow_app_ctx_t * ctx,
   const flow_fsoe_slot_t * frame)
{
   flow_safe_core_t * core = ctx->owner;
   const uint32_t k = ctx->k;
   flow_shm_chan_t * chan = &core->env.shm->tx[k][core->env.id];
   const bool crc_part = (core->env.id == MU_CRC_NODE);
   const uint32_t index = core->tx_count[k] + 1u;
   flow_fsoe_slot_t part;
   flow_fsoe_slot_t stored;
   core_egress_msg_t message;
   const core_egress_msg_t * peer_message;
   mu_message_t peer;
   bool match;

   if (core_stopping (core))
   {
      return;
   }

   memset (&message, 0, sizeof (message));

   core->stats.frames_sent++;

   /* Phase 1: both stacks must have generated the same PDU. */
   message.k = (uint16_t)k;
   message.len = (uint16_t)frame->len;
   message.tx_digest = frame->digest;
   message.rx_digest = core->latched[k].digest;
   message.anomalies = core->anomalies;

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

   /* Fault injection: one single core does not publish its frame even if it is
    * identical to the one of the peer (lost publication -> counter drift). */
   if ((core->env.id == MU_NODE_M33) &&
       flow_fault_hits (core->env.fault, FLOW_FAULT_STALL, core->cycle))
   {
      flow_fault_note (core->env.fault);
      LOG_ERROR (core->env.tag,
         "FAULT INJECTION (stall) cycle %u: %s does not publish, its "
         "tx_index stays at %u",
         core->cycle,
         (core->env.id == MU_NODE_M7) ? "M7" : "M33",
         core->tx_count[k]);
      return;
   }

   /* Phase 2: write only my half of the frame (frame number tx_count + 1). */
   part = *frame;
   flow_fsoe_pdu_split_copy (part.pdu, frame->pdu, frame->len, crc_part);
   part.digest = flow_digest (part.pdu, frame->len);

   if (!flow_shm_stage (chan, index, &part, NULL))
   {
      core_note_anomaly (core, "cannot write the TX frame into SHMEM");
      core_request_reset (core, k);
      return;
   }

   /* Phase 3: the bytes this core contributed must be intact in SHMEM. */
   if (!flow_shm_read_index (chan, index, &stored) ||
       !flow_fsoe_pdu_part_matches (
          stored.pdu, frame->pdu, frame->len, crc_part))
   {
      core->stats.tx_verify_failures++;
      core_note_anomaly (core, "TX half corrupted in SHMEM (read-back)");
      core_request_reset (core, k);
      return;
   }

   /* Fault injection: the data core re-writes its copy with one corrupted
    * safety data byte *after* the read-back check. The assembled frame will
    * carry good CRCs (from the other core) and bad data, so only the FSoE
    * slave can detect it - which is exactly what this test demonstrates. */
   if ((core->env.id == MU_DATA_NODE) &&
       (frame->len > FLOW_FSOE_PDU_FIRST_DATA_BYTE) &&
       flow_fault_hits (core->env.fault, FLOW_FAULT_FRAME, core->cycle))
   {
      flow_fault_note (core->env.fault);
      LOG_ERROR (core->env.tag,
         "FAULT INJECTION (frame) cycle %u: corrupting data byte %u of the "
         "SHMEM copy after the local check (the CRCs stay valid)",
         core->cycle,
         (unsigned)FLOW_FSOE_PDU_FIRST_DATA_BYTE);
      part.pdu[FLOW_FSOE_PDU_FIRST_DATA_BYTE] ^= 0xFFu;
      part.digest = flow_digest (part.pdu, frame->len);
      (void)flow_shm_stage (chan, index, &part, NULL);
   }

   /* Phase 4: publish (frame tx_count + 1 is now visible to the A55). */
   core->tx_count[k]++;
   flow_shm_commit (chan, core->tx_count[k]);
   core->stats.tx_published++;

   LOG_DETAIL (core->env.tag,
      "k=%u egress PDU OK: %s len=%u digest=0x%08x -> published frame %u "
      "(%s half)",
      k,
      flow_fsoe_command_name (flow_fsoe_command (frame)),
      (unsigned)frame->len,
      frame->digest,
      (unsigned)core->tx_count[k],
      crc_part ? "CRC" : "data");
}

/* --------------------------------------- Step 1: latch of the raw RX frames */

static void core_latch_rx (flow_safe_core_t * core)
{
   uint32_t k;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      flow_shm_chan_t * chan = &core->env.shm->rx[k];
      flow_fsoe_slot_t * slot = &core->latched[k];
      uint32_t published;

      if (core_stopping (core))
      {
         break;
      }

      /* Independent readers: every core uses its own counter, nothing is
       * announced by the peer. If the two cores end up working on different
       * frames, the cross-check in step 3 detects it (rx_index/rx_digest)
       * and brings the pair fail-safe. */
      published = flow_shm_count (chan);

      if ((published == 0u) || (published == core->rx_count[k]))
      {
         continue; /* no new frame from the black channel */
      }

      if (!flow_shm_read_index (chan, published, slot))
      {
         core->stats.rx_latch_failures++;
         continue;
      }

      /* Never trust a digest computed by somebody else (here the A55 would
       * have computed it): this core recomputes it on the bytes it actually
       * latched, so that the rx_digest exchanged with the peer really is a
       * comparison of the two frames. */
      slot->digest = flow_digest (slot->pdu, slot->len);

      core->rx_count[k] = published;

      flow_app_ctx_provide_rx (&core->ctx[k], slot, published);

      LOG_TRACE (core->env.tag,
         "k=%u latched RX %s len=%u frame=%u digest=0x%08x",
         k,
         flow_fsoe_command_name (flow_fsoe_command (slot)),
         (unsigned)slot->len,
         (unsigned)published,
         slot->digest);
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

#define CORE_MIN(a, b) (((a) < (b)) ? (a) : (b))

/*
 * Re-align the SHMEM counters of this core with the peer.
 *
 * The two cores take this decision independently but on the same pair of
 * values exchanged over the messaging unit, and MIN() is commutative: both
 * cores converge to the same value in the same cycle. No core is a master and
 * nothing is announced.
 *
 * Converging to MIN() (and not to MAX()) keeps the invariant used by the A55:
 * the frame selected by MIN(count_m7, count_m33) is always a frame that both
 * cores actually wrote.
 */
static void core_realign_counters (
   flow_safe_core_t * core,
   uint32_t k,
   const core_inputs_msg_t * peer)
{
   const uint32_t tx_common = CORE_MIN (core->tx_count[k], peer->tx_index);
   const uint32_t rx_common = CORE_MIN (core->rx_count[k], peer->rx_index);

   if (tx_common < core->tx_count[k])
   {
      flow_shm_commit (&core->env.shm->tx[k][core->env.id], tx_common);
      core->tx_count[k] = tx_common;
   }

   core->rx_count[k] = rx_common;
}

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
      message.tx_index = core->tx_count[k];
      message.rx_index = core->rx_count[k];
      message.rx_digest = core->latched[k].digest;
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

      /* ---- SHMEM counters and RX frame identity -------------------------
       * A divergence of the counters (or two cores working on different RX
       * frames) is a fault: it brings the pair fail-safe and it is re-aligned
       * to MIN(own, peer) on both cores in the same cycle. */
      if ((message.tx_index != peer_message->tx_index) ||
          (message.rx_index != peer_message->rx_index) ||
          (message.rx_digest != peer_message->rx_digest))
      {
         if (message.tx_index != peer_message->tx_index)
         {
            core->stats.tx_index_faults++;
         }
         if ((message.rx_index != peer_message->rx_index) ||
             (message.rx_digest != peer_message->rx_digest))
         {
            core->stats.rx_index_faults++;
         }

         LOG_ERROR (core->env.tag,
            "SHMEM COUNTER MISMATCH k=%u: tx_index own|peer %u|%u, "
            "rx_index own|peer %u|%u, rx_digest own|peer 0x%08x|0x%08x "
            "-> fail-safe, re-aligning to MIN()",
            k,
            (unsigned)message.tx_index,
            (unsigned)peer_message->tx_index,
            (unsigned)message.rx_index,
            (unsigned)peer_message->rx_index,
            message.rx_digest,
            peer_message->rx_digest);

         memset (&core->safe_inputs[k], 0, sizeof (core->safe_inputs[k]));
         core_set_sending_enabled (core, k, false);
         core_note_anomaly (core, "SHMEM counter divergence");
         core_request_reset (core, k);
         core_realign_counters (core, k, peer_message);
         continue;
      }

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

      core->stats.logic_ok++;

      if (core->cycle_fault)
      {
         /* A cross-check failed somewhere in this cycle: the freshly computed
          * outputs are not sent (the stack will send FailSafeData). */
         memset (&core->safe_outputs[k], 0, sizeof (core->safe_outputs[k]));
         core_set_sending_enabled (core, k, false);
      }
      else
      {
         core->safe_outputs[k] = next_outputs;
         core_set_sending_enabled (core, k, true);
      }

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
   core->cycle_fault = false;

   /* 1. Local latch of the raw frames received from the SHMEM mailbox.
    *    Every core uses its own RX counter, nothing is announced. */
   core_latch_rx (core);

   /* 2. FSoE stack synchronisation (inversion of control). The egress PDU
    *    cross-check and the publication of the frame happen inside this call. */
   core_sync (core);

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
      core->rx_count[k] = 0u;
      core->tx_count[k] = 0u;
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

