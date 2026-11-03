/*
 * flow_runtime.h
 *
 * Glue between main() and the three threads: owns the shared objects (SHMEM
 * mailbox, messaging unit, safe tick, fault configuration, RNGs) and the
 * static description of the two safety cores and of the black channel.
 */

#ifndef FLOW_RUNTIME_H
#define FLOW_RUNTIME_H

#include "a55_core.h"
#include "flow_config.h"
#include "flow_rng.h"
#include "flow_setup.h"
#include "mu_channel.h"
#include "safe_core.h"
#include "safe_tick.h"
#include "shmem_mailbox.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct flow_runtime
{
   flow_cli_config_t cfg;

   /* Shared objects emulating the target hardware */
   flow_shmem_t * shm;
   mu_bus_t mu;
   flow_safe_tick_t tick;
   flow_fault_cfg_t fault;

   /* Random number generators (one per core, one for the slave) */
   flow_rng_t rng[MU_NODE_COUNT];
   flow_rng_t rng_slave;

   atomic_bool stop;
   atomic_uint session_id[MU_NODE_COUNT];
   atomic_uint session_requests[MU_NODE_COUNT];
   atomic_uint slave_session_id;

   flow_safe_core_t core[MU_NODE_COUNT];
   flow_a55_core_t a55;

   pthread_t thread[MU_NODE_COUNT + 1];
   bool thread_started[MU_NODE_COUNT + 1];
} flow_runtime_t;

/** \brief Allocate and connect all the components (no threads yet) */
bool flow_runtime_init (flow_runtime_t * runtime, const flow_cli_config_t * cfg);

/* Safety cores (cores/m7_core.c and cores/m33_core.c) */
void flow_m7_core_setup (flow_runtime_t * runtime);
void * flow_m7_thread (void * argument);
void flow_m33_core_setup (flow_runtime_t * runtime);
void * flow_m33_thread (void * argument);

/** \brief Create and start the three threads */
bool flow_runtime_start (flow_runtime_t * runtime);

/** \brief Ask all threads to stop (safe tick stop + stop flag) */
void flow_runtime_request_stop (flow_runtime_t * runtime);

/** \brief Join the threads */
void flow_runtime_join (flow_runtime_t * runtime);

/** \brief Release all the resources */
void flow_runtime_cleanup (flow_runtime_t * runtime);

/** \brief Print the final report and return true when the run is a success */
bool flow_runtime_report (const flow_runtime_t * runtime);

#endif /* FLOW_RUNTIME_H */
