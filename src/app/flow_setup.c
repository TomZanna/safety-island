/*
 * flow_setup.c
 *
 * Default runtime configuration.
 */

#include "flow_setup.h"

#include <string.h>

void flow_cli_config_defaults (flow_cli_config_t * cfg)
{
   memset (cfg, 0, sizeof (*cfg));

   cfg->duration_ms = FLOW_DEFAULT_DURATION_MS;
   cfg->seed = FLOW_DEFAULT_SEED;
   cfg->safe_cycle_ms = FLOW_SAFE_CYCLE_MS;
   cfg->a55_cycle_ms = FLOW_A55_CYCLE_MS;
   cfg->connections = FLOW_NUM_CONNECTIONS;
   cfg->verbose = false;
   cfg->trace = false;
   cfg->fault = FLOW_FAULT_NONE;
   cfg->fault_first_cycle = 30u;
   cfg->fault_count = 1u;
}
