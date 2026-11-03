/*
 * m7_core.c
 *
 * "Safety Core 1" of the i.MX943: instantiates the safe core logic with the M7
 * identity (its own FSoE master instance, its own MU node, its own RNG).
 *
 * It is the master of the safe tick: the 10 ms rigid cycle is generated here
 * with clock_nanosleep() on an absolute CLOCK_MONOTONIC deadline, exactly like
 * the hardware safe timer on the target.
 */

#include "flow_runtime.h"
#include "flow_log.h"

#include <string.h>

void flow_m7_core_setup (flow_runtime_t * runtime)
{
   flow_core_env_t env;

   memset (&env, 0, sizeof (env));

   env.id = MU_NODE_M7;
   env.tag = FLOW_TAG_M7;
   env.shm = runtime->shm;
   env.mu = &runtime->mu;
   env.tick = &runtime->tick;
   env.fault = &runtime->fault;
   env.cfg = &runtime->cfg;
   env.rng = &runtime->rng[MU_NODE_M7];
   env.stop = &runtime->stop;
   env.session_id = &runtime->session_id[MU_NODE_M7];
   env.session_requests = &runtime->session_requests[MU_NODE_M7];

   flow_safe_core_init (&runtime->core[MU_NODE_M7], &env);

   LOG_INFO (FLOW_TAG_M7, "M7 safety core instantiated (tick master)");
}

void * flow_m7_thread (void * argument)
{
   flow_runtime_t * runtime = (flow_runtime_t *)argument;

   flow_safe_core_run (&runtime->core[MU_NODE_M7]);

   return NULL;
}
