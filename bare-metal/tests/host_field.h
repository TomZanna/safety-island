/*
 * host_field.h
 *
 * Test-only emulation of the FSoE slave and of the field process image.
 * This is not part of the target build.
 */

#ifndef HOST_FIELD_H
#define HOST_FIELD_H

#include "flow_config.h"
#include "flow_safe.h"
#include "flow_safe_logic.h"
#include "flow_types.h"

#include <fsoeslave.h>

typedef struct host_slave
{
   flow_fsoe_app_ref_t ref;
   fsoeslave_t slave;
   fsoeslave_syncstatus_t sync;

   flow_safe_inputs_t field_inputs;
   flow_safe_outputs_t field_outputs;

   flow_u8 rx_pdu[FLOW_PDU_MAX];
   flow_u32 rx_len;
   bool rx_ready;

   flow_u8 tx_pdu[FLOW_PDU_MAX];
   flow_u32 tx_len;
   bool tx_ready;

   flow_u16 session_counter;
   bool outputs_valid;
   bool enabled_seen;
} host_slave_t;

void host_slave_init (host_slave_t * host, flow_u16 slave_address);

void host_slave_provide_rx (
   host_slave_t * host,
   const flow_u8 * pdu,
   flow_u32 len);

void host_slave_step (host_slave_t * host, flow_u32 bus_cycle);

bool host_slave_take_tx (
   host_slave_t * host,
   flow_u8 * pdu,
   flow_u32 * len);

#endif /* HOST_FIELD_H */
