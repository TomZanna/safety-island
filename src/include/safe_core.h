/*
 * safe_core.h
 *
 * One instance of this module is executed by the M7 thread and one by the M33
 * thread. It implements the whole safe cycle of the sequence diagram:
 *
 *   1. deterministic safe tick + local latch of the raw RX frames
 *   2. FSoE stack synchronisation (fsoemaster_sync_with_slave) with the
 *      egress PDU cross-check and the SHMEM 2-phase commit
 *   3. cross-check of the decoded SafeInputs and of the connection status
 *   4. safe logic execution and cross-check of the computed SafeOutputs
 */

#ifndef SAFE_CORE_H
#define SAFE_CORE_H

#include "flow_config.h"
#include "flow_rng.h"
#include "flow_setup.h"
#include "fsoe_app_glue.h"
#include "fsoe_slot.h"
#include "mu_channel.h"
#include "safe_logic.h"
#include "safe_tick.h"
#include "shmem_mailbox.h"

#include <fsoemaster.h>

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct flow_core_stats
{
   uint32_t cycles;
   uint32_t frames_sent;
   uint32_t frames_received;
   uint32_t pdu_matches;
   uint32_t pdu_mismatches;
   uint32_t egress_timeouts;
   uint32_t commit_failures;
   uint32_t inputs_ok;
   uint32_t inputs_mismatches;
   uint32_t logic_ok;
   uint32_t logic_mismatches;
   uint32_t rx_latch_failures;
   uint32_t session_negotiations;
   uint32_t session_failures;
   uint32_t reset_events;
   uint32_t reset_requests;
   uint32_t state_changes;
   uint32_t tick_timeouts;
   uint32_t enable_set;
   uint32_t enable_clear;
   uint32_t mu_anomalies;
   uint32_t data_cycles;
} flow_core_stats_t;

/** \brief Everything the core needs from its environment (no globals) */
typedef struct flow_core_env
{
   mu_node_t id;
   const char * tag;
   flow_shmem_t * shm;
   mu_bus_t * mu;
   flow_safe_tick_t * tick;
   flow_fault_cfg_t * fault;
   const flow_cli_config_t * cfg;
   flow_rng_t * rng;
   atomic_bool * stop;
   atomic_uint * session_id;       /**< negotiated Session ID, for reports */
   atomic_uint * session_requests;
} flow_core_env_t;

typedef struct flow_safe_core
{
   flow_core_env_t env;

   fsoemaster_t master[FLOW_NUM_CONNECTIONS];
   flow_app_ctx_t ctx[FLOW_NUM_CONNECTIONS];

   /* Local RAM copy of the raw RX frames (diagram step 1) */
   flow_fsoe_slot_t latched[FLOW_NUM_CONNECTIONS];
   uint32_t latched_published[FLOW_NUM_CONNECTIONS];
   uint32_t provided_published[FLOW_NUM_CONNECTIONS];

   flow_safe_inputs_t safe_inputs[FLOW_NUM_CONNECTIONS];
   flow_safe_inputs_t raw_inputs[FLOW_NUM_CONNECTIONS];
   flow_safe_outputs_t safe_outputs[FLOW_NUM_CONNECTIONS];
   flow_safe_logic_t logic[FLOW_NUM_CONNECTIONS];
   fsoemaster_syncstatus_t sync[FLOW_NUM_CONNECTIONS];
   bool send_enabled[FLOW_NUM_CONNECTIONS];

   uint32_t cycle;
   uint32_t anomalies;   /**< Divergence counter, kept equal on both cores  */
   uint32_t data_state_cycles;

   flow_core_stats_t stats;
} flow_safe_core_t;

void flow_safe_core_init (
   flow_safe_core_t * core,
   const flow_core_env_t * env);

/** \brief Run the safe cycle loop until the runtime stop flag is set */
void flow_safe_core_run (flow_safe_core_t * core);

/** \brief pthread entry point, argument is a flow_safe_core_t * */
void * flow_safe_core_thread (void * argument);

#endif /* SAFE_CORE_H */
