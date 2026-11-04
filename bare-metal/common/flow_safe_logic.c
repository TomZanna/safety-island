/*
 * flow_safe_logic.c
 *
 * See flow_safe_logic.h.  Pure function: no time, no I/O, no randomness.  The
 * M7 and M33 instances therefore produce the same outputs for the same inputs.
 */

#include "flow_safe_logic.h"
#include "flow_mem.h"

void flow_safe_logic_init (flow_safe_logic_t * logic)
{
   if (logic != NULL)
   {
      flow_memset (logic, 0, (flow_u32)sizeof (*logic));
   }
}

void flow_safe_logic_step (
   const flow_safe_inputs_t * inputs,
   flow_safe_logic_t * logic,
   flow_safe_outputs_t * outputs)
{
   bool estop_a;
   bool estop_b;
   bool door_a;
   bool door_b;
   bool reset;
   bool discrepancy;
   bool safe_conditions;

   estop_a = (inputs->ch_a & FLOW_SAFE_IN_ESTOP_OK) != 0u;
   estop_b = (inputs->ch_b & FLOW_SAFE_IN_ESTOP_OK) != 0u;
   door_a = (inputs->ch_a & FLOW_SAFE_IN_DOOR_CLOSED) != 0u;
   door_b = (inputs->ch_b & FLOW_SAFE_IN_DOOR_CLOSED) != 0u;
   reset = (inputs->ch_a & FLOW_SAFE_IN_RESET) != 0u;
   discrepancy = (estop_a != estop_b) || (door_a != door_b);

   logic->cycles++;

   /* 1oo2 discrepancy monitoring. */
   if (discrepancy)
   {
      logic->discrepancy_count++;
      if (logic->discrepancy_count >= FLOW_DISCREPANCY_CYCLES)
      {
         logic->latched_fault = 1u;
      }
   }
   else
   {
      logic->discrepancy_count = 0u;
   }

   safe_conditions = estop_a && estop_b && door_a && door_b &&
      (logic->latched_fault == 0u);

   /* Manual reset on a rising edge, accepted only when the safety conditions
    * are satisfied.  It also clears a latched fault. */
   if (reset && (logic->reset_previous == 0u))
   {
      if (estop_a && estop_b && door_a && door_b)
      {
         logic->latched_fault = 0u;
         logic->discrepancy_count = 0u;
         logic->enabled = 1u;
      }
   }
   logic->reset_previous = reset ? 1u : 0u;

   /* Any loss of safety conditions drops the output immediately. */
   if (!safe_conditions)
   {
      logic->enabled = 0u;
   }

   outputs->enable = (logic->enabled != 0u) ?
      FLOW_SAFE_OUTPUT_ENABLED : 0x0000u;
   outputs->status = 0u;

   if (logic->enabled != 0u)
   {
      outputs->status |= FLOW_SAFE_STATUS_ENABLED;
   }
   if (logic->latched_fault != 0u)
   {
      outputs->status |= FLOW_SAFE_STATUS_LATCHED_FAULT;
   }
   if (discrepancy)
   {
      outputs->status |= FLOW_SAFE_STATUS_DISCREPANCY;
   }
   if (!estop_a || !estop_b)
   {
      outputs->status |= FLOW_SAFE_STATUS_ESTOP_OPEN;
   }
   if (!door_a || !door_b)
   {
      outputs->status |= FLOW_SAFE_STATUS_DOOR_OPEN;
   }
   if (estop_a && door_a)
   {
      outputs->status |= FLOW_SAFE_STATUS_CH_A_OK;
   }
   if (estop_b && door_b)
   {
      outputs->status |= FLOW_SAFE_STATUS_CH_B_OK;
   }
}
