/*
 * flow_setup.h
 *
 * Runtime configuration coming from the command line.
 */

#ifndef FLOW_SETUP_H
#define FLOW_SETUP_H

#include "flow_config.h"
#include "fault_inject.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct flow_cli_config
{
   uint32_t duration_ms;      /**< 0 = run until SIGINT                    */
   uint32_t seed;
   uint32_t safe_cycle_ms;
   uint32_t a55_cycle_ms;
   uint32_t connections;      /**< must match FLOW_NUM_CONNECTIONS        */
   bool verbose;              /**< one log line per safe cycle            */
   bool trace;                /**< one log line per FSoE frame / MU msg   */
   flow_fault_type_t fault;
   uint32_t fault_first_cycle;
   uint32_t fault_count;
} flow_cli_config_t;

void flow_cli_config_defaults (flow_cli_config_t * cfg);

#endif /* FLOW_SETUP_H */
