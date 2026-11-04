/*
 * flow_safe.h
 *
 * One safe core (M7 or M33).  The very same source runs in both cores; the
 * build selects FLOW_CORE_ID and the core gets its own FSoE master instance,
 * its own MU sequence state and its own SHMEM TX channels.
 */

#ifndef FLOW_SAFE_H
#define FLOW_SAFE_H

#include "flow_config.h"
#include "flow_mu.h"
#include "flow_safe_logic.h"
#include "flow_shm.h"
#include "flow_types.h"

#include <fsoemaster.h>

#define FLOW_FSOE_REF_MAGIC 0x46534F45u /* "FSOE" */
#define FLOW_FSOE_KIND_MASTER 0u
#define FLOW_FSOE_KIND_SLAVE 1u

/* Application reference passed to fsoemaster_init().  On the target there is
 * only one kind (master); the host test uses the same layout to dispatch
 * between the two masters and the emulated slave. */
typedef struct flow_fsoe_app_ref
{
   flow_u32 magic;
   flow_u32 kind;
   flow_u32 k;
   void * owner;
} flow_fsoe_app_ref_t;

/* Local copy of a raw RX/TX frame (diagram step 1). */
typedef struct flow_fsoe_slot
{
   flow_u8 pdu[FLOW_PDU_MAX];
   flow_u32 len;
   flow_u32 fc;
} flow_fsoe_slot_t;

/* Fault injection, only compiled in when FLOW_DEBUG is enabled. */
typedef enum flow_test_fault
{
   FLOW_TEST_FAULT_NONE = 0,
   FLOW_TEST_FAULT_PDU,
   FLOW_TEST_FAULT_INPUTS,
   FLOW_TEST_FAULT_LOGIC,
   FLOW_TEST_FAULT_SESSION,
   FLOW_TEST_FAULT_STALL
} flow_test_fault_t;

typedef struct flow_safe
{
   flow_core_id_t id;
   flow_shm_t * shm;

   flow_mu_state_t mu;

   fsoemaster_t master[FLOW_NUM_CONNECTIONS];
   flow_fsoe_app_ref_t app_ref[FLOW_NUM_CONNECTIONS];

   /* Local RAM copy of the raw RX frames. */
   flow_fsoe_slot_t rx_slot[FLOW_NUM_CONNECTIONS];
   bool rx_ready[FLOW_NUM_CONNECTIONS];
   flow_u32 rx_count[FLOW_NUM_CONNECTIONS];
   flow_u32 tx_count[FLOW_NUM_CONNECTIONS];

   flow_safe_inputs_t raw_inputs[FLOW_NUM_CONNECTIONS];
   flow_safe_inputs_t safe_inputs[FLOW_NUM_CONNECTIONS];
   flow_safe_outputs_t safe_outputs[FLOW_NUM_CONNECTIONS];
   flow_safe_logic_t logic[FLOW_NUM_CONNECTIONS];
   fsoemaster_syncstatus_t sync[FLOW_NUM_CONNECTIONS];
   bool send_enabled[FLOW_NUM_CONNECTIONS];

   flow_u32 cycle;
   flow_u32 anomalies;
   bool cycle_fault;
   flow_u32 session_id;

   flow_test_fault_t fault;

   struct
   {
      flow_u32 cycles;
      flow_u32 data_cycles;
      flow_u32 tx_published;
      flow_u32 pdu_mismatches;
      flow_u32 inputs_mismatches;
      flow_u32 logic_mismatches;
      flow_u32 reset_requests;
      flow_u32 rx_latch_failures;
      flow_u32 session_requests;
   } stats;
} flow_safe_t;

void flow_safe_init (
   flow_safe_t * safe,
   flow_core_id_t id,
   flow_shm_t * shm);

/* Execute one complete safe cycle (steps 1..4 of the sequence diagram). */
void flow_safe_step (flow_safe_t * safe);

/* Callbacks used by the FSoE application glue. */
void flow_safe_on_egress (
   flow_safe_t * safe,
   flow_u32 k,
   const void * buffer,
   size_t size);

size_t flow_safe_on_recv (
   flow_safe_t * safe,
   flow_u32 k,
   void * buffer,
   size_t size);

flow_u16 flow_safe_on_session (flow_safe_t * safe, flow_u32 k);

flow_u8 flow_safe_validate_parameters (
   flow_u16 timeout_ms,
   const void * parameters,
   size_t size);

#endif /* FLOW_SAFE_H */
