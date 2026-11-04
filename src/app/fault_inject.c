/*
 * fault_inject.c
 *
 * See fault_inject.h
 */

#include "fault_inject.h"

#include <string.h>

const char * flow_fault_name (flow_fault_type_t type)
{
   switch (type)
   {
   case FLOW_FAULT_NONE:     return "none";
   case FLOW_FAULT_SESSION:  return "session";
   case FLOW_FAULT_PDU:      return "pdu";
   case FLOW_FAULT_INPUTS:   return "inputs";
   case FLOW_FAULT_LOGIC:    return "logic";
   case FLOW_FAULT_BUSDROP:  return "busdrop";
   case FLOW_FAULT_STALL:    return "stall";
   case FLOW_FAULT_FRAME:    return "frame";
   default:                  return "?";
   }
}

bool flow_fault_parse (const char * name, flow_fault_type_t * type)
{
   flow_fault_type_t candidate;

   if ((name == NULL) || (type == NULL))
   {
      return false;
   }

   for (candidate = FLOW_FAULT_NONE; candidate <= FLOW_FAULT_FRAME; candidate++)
   {
      if (strcmp (name, flow_fault_name (candidate)) == 0)
      {
         *type = candidate;
         return true;
      }
   }

   return false;
}

bool flow_fault_expected (const flow_fault_cfg_t * cfg, flow_fault_type_t type)
{
   return (cfg != NULL) && (cfg->type == type);
}

void flow_fault_init (
   flow_fault_cfg_t * cfg,
   flow_fault_type_t type,
   uint32_t first_cycle,
   uint32_t count)
{
   memset (cfg, 0, sizeof (*cfg));
   cfg->type = type;
   cfg->first_cycle = first_cycle;
   cfg->count = (count == 0u) ? 1u : count;
   atomic_init (&cfg->fired, 0u);
}

bool flow_fault_hits (
   const flow_fault_cfg_t * cfg,
   flow_fault_type_t type,
   uint32_t cycle)
{
   if ((cfg == NULL) || (cfg->type != type) || (type == FLOW_FAULT_NONE))
   {
      return false;
   }

   return (cycle >= cfg->first_cycle) &&
          (cycle < (cfg->first_cycle + cfg->count));
}

void flow_fault_note (flow_fault_cfg_t * cfg)
{
   if (cfg != NULL)
   {
      atomic_fetch_add_explicit (&cfg->fired, 1u, memory_order_relaxed);
   }
}
