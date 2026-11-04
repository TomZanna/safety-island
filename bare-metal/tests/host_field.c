/*
 * host_field.c
 *
 * Test-only FSoE slave and field stimulus.
 */

#include "host_field.h"
#include "flow_mem.h"

#include <stdio.h>

void host_slave_init (host_slave_t * host, flow_u16 slave_address)
{
   const fsoeslave_cfg_t cfg =
   {
      .slave_address = slave_address,
      .application_parameters_size = FLOW_APP_PARAMETERS_SIZE,
      .inputs_size = FLOW_SAFE_DATA_SIZE,
      .outputs_size = FLOW_SAFE_DATA_SIZE,
   };

   flow_memset (host, 0, (flow_u32)sizeof (*host));

   host->ref.magic = FLOW_FSOE_REF_MAGIC;
   host->ref.kind = FLOW_FSOE_KIND_SLAVE;
   host->ref.k = 0u;
   host->ref.owner = host;

   if (fsoeslave_init (&host->slave, &cfg, &host->ref) != FSOESLAVE_STATUS_OK)
   {
      fprintf (stderr, "host: fsoeslave_init failed\n");
   }
}

void host_slave_provide_rx (
   host_slave_t * host,
   const flow_u8 * pdu,
   flow_u32 len)
{
   if ((pdu == NULL) || (len > FLOW_PDU_MAX))
   {
      return;
   }

   flow_memcpy (host->rx_pdu, pdu, len);
   host->rx_len = len;
   host->rx_ready = true;
}

bool host_slave_take_tx (
   host_slave_t * host,
   flow_u8 * pdu,
   flow_u32 * len)
{
   if (!host->tx_ready)
   {
      return false;
   }

   flow_memcpy (pdu, host->tx_pdu, host->tx_len);
   *len = host->tx_len;
   host->tx_ready = false;

   return true;
}

static void host_field_scenario (host_slave_t * host, flow_u32 bus_cycle)
{
   const flow_u32 ms = bus_cycle * FLOW_A55_CYCLE_MS;
   flow_u16 channel_a = 0u;
   flow_u16 channel_b = 0u;

   /* 0..120 ms E-stop pressed, 200..240 ms reset, 700..1100 ms door open,
    * 1150..1190 ms reset again. */
   if (ms >= 120u)
   {
      channel_a |= FLOW_SAFE_IN_ESTOP_OK;
      channel_b |= FLOW_SAFE_IN_ESTOP_OK;
   }
   if (!((ms >= 700u) && (ms < 1100u)))
   {
      channel_a |= FLOW_SAFE_IN_DOOR_CLOSED;
      channel_b |= FLOW_SAFE_IN_DOOR_CLOSED;
   }
   if (((ms >= 200u) && (ms < 240u)) || ((ms >= 1150u) && (ms < 1190u)))
   {
      channel_a |= FLOW_SAFE_IN_RESET;
   }

   host->field_inputs.ch_a = channel_a;
   host->field_inputs.ch_b = channel_b;
}

void host_slave_step (host_slave_t * host, flow_u32 bus_cycle)
{
   fsoeslave_status_t status;

   host_field_scenario (host, bus_cycle);

   status = fsoeslave_sync_with_master (
      &host->slave,
      &host->field_inputs,
      &host->field_outputs,
      &host->sync);

   if (status != FSOESLAVE_STATUS_OK)
   {
      fprintf (stderr, "host: fsoeslave_sync_with_master failed\n");
      return;
   }

   if (host->sync.current_state == FSOESLAVE_STATE_PARAMETER)
   {
      (void)fsoeslave_set_process_data_sending_enable_flag (&host->slave);
   }

   if (host->sync.is_process_data_received)
   {
      host->outputs_valid = true;
      if (host->field_outputs.enable == FLOW_SAFE_OUTPUT_ENABLED)
      {
         host->enabled_seen = true;
      }
   }
}
