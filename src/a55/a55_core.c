/*
 * a55_core.c
 *
 * See a55_core.h. The A55 thread runs the black channel: it is asynchronous
 * with respect to the safe cycle (default 2 ms) and it performs, at every bus
 * cycle, the fieldbus exchange with the emulated FSoE slave.
 */

#include "a55_core.h"
#include "flow_log.h"
#include "flow_time.h"

#include <string.h>

/* --------------------------------------------------------------- callbacks */

/*
 * Called by the stack (inside fsoeslave_sync_with_master -> fsoeapp_send) when
 * the slave wants to reply to the master. The frame is already stored in
 * ctx->tx; the bus loop picks it up and publishes it into the SHMEM.
 */
static void a55_on_egress (
   flow_app_ctx_t * ctx,
   const flow_fsoe_slot_t * frame)
{
   flow_a55_core_t * a55 = ctx->owner;

   a55->stats.slave_frames_in++;

   LOG_TRACE (FLOW_TAG_A55,
      "k=%u slave generated %s len=%u digest=0x%08x",
      ctx->k,
      flow_fsoe_command_name (flow_fsoe_command (frame)),
      (unsigned)frame->len,
      frame->digest);
}

/* ------------------------------------------------------------- field model */

void flow_a55_field_scenario (
   flow_a55_core_t * a55,
   uint32_t k,
   uint32_t bus_cycle,
   flow_safe_inputs_t * inputs)
{
   const uint32_t tick_ms = (a55->env.cfg->a55_cycle_ms == 0u) ?
      2u : a55->env.cfg->a55_cycle_ms;
   const uint32_t ms = bus_cycle * tick_ms;

   uint16_t channel_a = 0u;
   uint16_t channel_b = 0u;
   bool estop_released;
   bool door_closed;
   bool reset_pressed;

   (void)k;

   /* Timeline of the simulated machine:
    *   0   .. 120 ms  E-stop pressed (both channels), safety function off
    *   200 .. 240 ms  reset push button pressed -> safety function ENABLED
    *   700 .. 1100 ms guard door open -> safe stop
    *   1150..1190 ms  reset push button pressed -> safety function ENABLED
    */
   estop_released = (ms >= 120u);
   door_closed = !((ms >= 700u) && (ms < 1100u));
   reset_pressed = ((ms >= 200u) && (ms < 240u)) ||
      ((ms >= 1150u) && (ms < 1190u));

   if (estop_released)
   {
      channel_a |= FLOW_SAFE_IN_ESTOP_OK;
      channel_b |= FLOW_SAFE_IN_ESTOP_OK;
   }
   if (door_closed)
   {
      channel_a |= FLOW_SAFE_IN_DOOR_CLOSED;
      channel_b |= FLOW_SAFE_IN_DOOR_CLOSED;
   }
   if (reset_pressed)
   {
      channel_a |= FLOW_SAFE_IN_RESET;
   }

   inputs->ch_a = channel_a;
   inputs->ch_b = channel_b;
}

/* ------------------------------------------------------------- bus exchange */

/* Read a new frame committed by the safety cores (SHMEM TX channel) */
static void a55_read_shmem (flow_a55_core_t * a55, uint32_t k)
{
   flow_fsoe_slot_t slot;
   uint32_t published = 0u;

   if (!flow_shmem_tx_read (a55->env.shm, k, &slot, &published))
   {
      return;
   }

   if (published == a55->last_tx_published[k])
   {
      return; /* no new frame since the previous bus cycle */
   }

   a55->last_tx_published[k] = published;
   a55->stats.tx_frames_read++;
   atomic_fetch_add_explicit (
      &a55->env.shm->a55.tx_frames_read, 1u, memory_order_relaxed);

   LOG_TRACE (FLOW_TAG_A55,
      "k=%u SHMEM TX read: %s len=%u fc=%u digest=0x%08x",
      k,
      flow_fsoe_command_name (flow_fsoe_command (&slot)),
      (unsigned)slot.len,
      (unsigned)published,
      slot.digest);

   if (flow_fault_hits (a55->env.fault, FLOW_FAULT_BUSDROP, a55->bus_cycle))
   {
      flow_fault_note (a55->env.fault);
      a55->stats.tx_frames_dropped++;
      atomic_fetch_add_explicit (
         &a55->env.shm->a55.tx_frames_dropped, 1u, memory_order_relaxed);
      LOG_ERROR (FLOW_TAG_A55,
         "FAULT INJECTION (busdrop) bus cycle %u: frame fc=%u dropped, the "
         "safe cores will not receive an answer",
         a55->bus_cycle,
         (unsigned)published);
      return;
   }

   a55->pending_master[k] = slot;
   a55->pending_master_ready[k] = true;
   a55->stats.master_frames_delivered++;
}

/* Run the emulated slave stack for one bus cycle */
static void a55_sync_slave (flow_a55_core_t * a55, uint32_t k)
{
   fsoeslave_status_t status;
   fsoeslave_state_t state;

   if (a55->pending_master_ready[k])
   {
      flow_app_ctx_provide_rx (
         &a55->ctx[k], &a55->pending_master[k], a55->last_tx_published[k]);
      a55->pending_master_ready[k] = false;
   }

   flow_a55_field_scenario (a55, k, a55->bus_cycle, &a55->field_inputs[k]);

   status = fsoeslave_sync_with_master (
      &a55->slave[k],
      &a55->field_inputs[k],
      &a55->field_outputs[k],
      &a55->sync[k]);

   if (status != FSOESLAVE_STATUS_OK)
   {
      LOG_ERROR (FLOW_TAG_A55, "k=%u: fsoeslave_sync_with_master() failed", k);
      return;
   }

   state = a55->sync[k].current_state;

   if (state != a55->last_state[k])
   {
      a55->stats.state_changes++;
      LOG_INFO (FLOW_TAG_A55,
         "k=%u FSoE slave state: %s -> %s",
         k,
         fsoeslave_state_description (a55->last_state[k]),
         fsoeslave_state_description (state));
      a55->last_state[k] = state;
   }

   if (a55->sync[k].reset_event != FSOESLAVE_RESETEVENT_NONE)
   {
      LOG_WARN (FLOW_TAG_A55,
         "k=%u slave connection reset (%s): %s",
         k,
         (a55->sync[k].reset_event == FSOESLAVE_RESETEVENT_BY_MASTER) ?
            "by master" : "by slave",
         fsoeslave_reset_reason_description (a55->sync[k].reset_reason));
   }

   if (state == FSOESLAVE_STATE_PARAMETER)
   {
      (void)fsoeslave_set_process_data_sending_enable_flag (&a55->slave[k]);
   }

   if (a55->sync[k].is_process_data_received)
   {
      a55->stats.process_data_cycles++;
      a55->field_outputs_valid[k] = true;

      /* Publish what the A55 has decoded into the SHMEM telemetry area */
      atomic_store_explicit (
         &a55->env.shm->a55.safe_outputs_word0,
         (uint32_t)a55->field_outputs[k].enable,
         memory_order_relaxed);
      atomic_store_explicit (
         &a55->env.shm->a55.safe_outputs_word1,
         (uint32_t)a55->field_outputs[k].status,
         memory_order_relaxed);
      atomic_store_explicit (
         &a55->env.shm->a55.actuator_enabled,
         (a55->field_outputs[k].enable == FLOW_SAFE_OUTPUT_ENABLED) ? 1u : 0u,
         memory_order_relaxed);

      /* Print only when the actuator state changes, not every bus cycle */
      if ((a55->field_outputs[k].enable != a55->reported_outputs[k].enable) ||
          (a55->field_outputs[k].status != a55->reported_outputs[k].status))
      {
         char description[64];

         a55->reported_outputs[k] = a55->field_outputs[k];
         a55->stats.actuator_enable_changes++;

         LOG_INFO (FLOW_TAG_A55,
            "k=%u SafeOutputs received over FSoE and read from SHMEM: %s",
            k,
            flow_safe_logic_describe (
               &a55->field_outputs[k], description, sizeof (description)));
      }
   }

   if (flow_app_ctx_take_tx (&a55->ctx[k], &a55->pending_slave[k]))
   {
      a55->pending_slave_ready[k] = true;
      a55->stats.slave_frames_out++;
   }
}


/* Publish the slave reply into the SHMEM RX channel (2-phase commit) */
static void a55_publish_slave_frame (flow_a55_core_t * a55, uint32_t k)
{
   if (!a55->pending_slave_ready[k])
   {
      return;
   }

   /* Never publish while a safety core is inside its cycle: the cores must
    * always latch the very same snapshot. If we cannot publish now the frame
    * stays pending and is retried at the next bus cycle. */
   if (!flow_shmem_cores_idle (a55->env.shm))
   {
      a55->stats.publish_deferred++;
      atomic_fetch_add_explicit (
         &a55->env.shm->rx_publish_deferred, 1u, memory_order_relaxed);
      return;
   }

   a55->rx_frames[k]++;
   a55->pending_slave[k].fc = a55->rx_frames[k];

   if (!flow_shmem_rx_publish (a55->env.shm, k, &a55->pending_slave[k]))
   {
      a55->stats.publish_deferred++;
      return;
   }

   a55->pending_slave_ready[k] = false;
   a55->stats.slave_frames_published++;

   atomic_store_explicit (
      &a55->env.shm->a55.slave_state[k],
      (uint32_t)a55->sync[k].current_state,
      memory_order_relaxed);
   atomic_fetch_add_explicit (
      &a55->env.shm->a55.slave_frames_out[k], 1u, memory_order_relaxed);

   LOG_TRACE (FLOW_TAG_A55,
      "k=%u SHMEM RX published: %s len=%u fc=%u digest=0x%08x",
      k,
      flow_fsoe_command_name (flow_fsoe_command (&a55->pending_slave[k])),
      (unsigned)a55->pending_slave[k].len,
      (unsigned)a55->rx_frames[k],
      a55->pending_slave[k].digest);
}

/* One bus cycle of the black channel */
static void a55_bus_cycle (flow_a55_core_t * a55)
{
   uint32_t k;

   a55->bus_cycle++;
   a55->stats.bus_cycles++;

   atomic_store_explicit (
      &a55->env.shm->a55.bus_cycles, a55->bus_cycle, memory_order_relaxed);

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      a55_read_shmem (a55, k);
      a55_sync_slave (a55, k);
      a55_publish_slave_frame (a55, k);
   }

   if (flow_log_is_enabled (FLOW_LOG_LEVEL_DETAIL) && (FLOW_NUM_CONNECTIONS > 0u))
   {
      char description[64];

      LOG_DETAIL (FLOW_TAG_A55,
         "bus %-4u slave=%-10s field=0x%02x/0x%02x actuator=%s shm_tx_fc=%u",
         a55->bus_cycle,
         fsoeslave_state_description (a55->sync[0].current_state),
         ((const uint8_t *)&a55->field_inputs[0])[1],
         ((const uint8_t *)&a55->field_inputs[0])[0],
         flow_safe_logic_describe (&a55->field_outputs[0], description,
            sizeof (description)),
         (unsigned)a55->last_tx_published[0]);
   }
}

/* ------------------------------------------------------------------- API */

void flow_a55_core_init (flow_a55_core_t * a55, const flow_a55_env_t * env)
{
   uint32_t k;

   memset (a55, 0, sizeof (*a55));
   a55->env = *env;

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      const fsoeslave_cfg_t cfg =
      {
         .slave_address = (uint16_t)(FLOW_SLAVE_ADDRESS + k),
         .application_parameters_size = FLOW_APP_PARAMETERS_SIZE,
         .inputs_size = FLOW_SAFE_DATA_SIZE,
         .outputs_size = FLOW_SAFE_DATA_SIZE,
      };

      memset (&a55->field_inputs[k], 0, sizeof (a55->field_inputs[k]));
      memset (&a55->field_outputs[k], 0, sizeof (a55->field_outputs[k]));
      memset (&a55->reported_outputs[k], 0, sizeof (a55->reported_outputs[k]));
      memset (&a55->sync[k], 0, sizeof (a55->sync[k]));
      flow_fsoe_slot_clear (&a55->pending_master[k]);
      flow_fsoe_slot_clear (&a55->pending_slave[k]);
      a55->pending_master_ready[k] = false;
      a55->pending_slave_ready[k] = false;
      a55->last_tx_published[k] = 0u;
      a55->rx_frames[k] = 0u;
      a55->field_outputs_valid[k] = false;
      a55->last_state[k] = FSOESLAVE_STATE_RESET;

      flow_app_ctx_init (
         &a55->ctx[k],
         FLOW_CTX_SLAVE,
         k,
         &a55->slave[k],
         a55,
         a55_on_egress,
         NULL);

      if (fsoeslave_init (&a55->slave[k], &cfg, &a55->ctx[k]) !=
          FSOESLAVE_STATUS_OK)
      {
         LOG_ERROR (FLOW_TAG_A55, "k=%u: fsoeslave_init() failed", k);
      }

      LOG_INFO (FLOW_TAG_A55,
         "k=%u emulated FSoE slave ready (address 0x%04x, %u data bytes)",
         k,
         (unsigned)(FLOW_SLAVE_ADDRESS + k),
         (unsigned)FLOW_SAFE_DATA_SIZE);
   }
}

void flow_a55_core_run (flow_a55_core_t * a55)
{
   const uint32_t cycle_ms = (a55->env.cfg->a55_cycle_ms == 0u) ?
      2u : a55->env.cfg->a55_cycle_ms;
   const uint64_t cycle_us = (uint64_t)cycle_ms * 1000ull;
   uint64_t deadline = flow_deadline_after_us (cycle_us);

   LOG_INFO (FLOW_TAG_A55, "thread started (%u ms bus cycle)", cycle_ms);

   while (!atomic_load_explicit (a55->env.stop, memory_order_relaxed))
   {
      if (flow_sleep_until_us (deadline) != 0)
      {
         break;
      }

      deadline += cycle_us;
      a55_bus_cycle (a55);
   }

   LOG_INFO (FLOW_TAG_A55,
      "thread stopped after %u bus cycles", a55->bus_cycle);
}

void * flow_a55_core_thread (void * argument)
{
   flow_a55_core_t * a55 = (flow_a55_core_t *)argument;

   flow_a55_core_run (a55);

   return NULL;
}

