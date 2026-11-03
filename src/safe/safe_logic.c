/*
 * safe_logic.c
 *
 * See safe_logic.h
 */

#include "safe_logic.h"

#include <stdio.h>
#include <string.h>

void flow_safe_logic_init (flow_safe_logic_t * logic)
{
   memset (logic, 0, sizeof (*logic));
   logic->enabled = 0u;
   logic->latched_fault = 0u;
   logic->discrepancy_count = 0u;
   logic->reset_previous = 0u;
   logic->cycles = 0u;
}

void flow_safe_logic_step (
   const flow_safe_inputs_t * inputs,
   flow_safe_logic_t * logic,
   flow_safe_outputs_t * outputs)
{
   bool estop_a = (inputs->ch_a & FLOW_SAFE_IN_ESTOP_OK) != 0u;
   bool estop_b = (inputs->ch_b & FLOW_SAFE_IN_ESTOP_OK) != 0u;
   bool door_a = (inputs->ch_a & FLOW_SAFE_IN_DOOR_CLOSED) != 0u;
   bool door_b = (inputs->ch_b & FLOW_SAFE_IN_DOOR_CLOSED) != 0u;
   bool reset = (inputs->ch_a & FLOW_SAFE_IN_RESET) != 0u;
   bool discrepancy = (estop_a != estop_b) || (door_a != door_b);
   bool safe_conditions;

   logic->cycles++;

   /* Discrepancy monitoring between the two input channels. A discrepancy
    * that persists for FLOW_DISCREPANCY_CYCLES latches a fault. */
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

   /* Manual reset on a rising edge, accepted only if the safety conditions
    * are satisfied (also clears a latched fault). */
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

   /* Any loss of safety conditions drops the output immediately (fail-safe) */
   if (!safe_conditions)
   {
      logic->enabled = 0u;
   }

   outputs->enable = (logic->enabled != 0u) ? FLOW_SAFE_OUTPUT_ENABLED : 0x0000u;
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

const char * flow_safe_logic_describe (
   const flow_safe_outputs_t * outputs,
   char * buffer,
   unsigned size)
{
   const char * state;

   if ((outputs->enable == 0u) && (outputs->status == 0u))
   {
      state = "FAIL-SAFE";
   }
   else if (outputs->enable == FLOW_SAFE_OUTPUT_ENABLED)
   {
      state = "ENABLED";
   }
   else
   {
      state = "DISABLED";
   }

   snprintf (buffer, size, "%s (w0=0x%04x w1=0x%04x)", state,
      (unsigned)outputs->enable, (unsigned)outputs->status);

   return buffer;
}
