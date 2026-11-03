/*
 * runtime.c
 *
 * Construction of the whole prototype: allocates the emulated hardware
 * objects (SHMEM mailbox, messaging unit, safe tick) and the three software
 * components, then starts and stops the threads.
 */

#include "flow_runtime.h"
#include "flow_log.h"
#include "flow_time.h"

#include <fsoe_app_glue.h>

#include <stdio.h>
#include <string.h>

bool flow_runtime_init (flow_runtime_t * runtime, const flow_cli_config_t * cfg)
{
   flow_a55_env_t a55_env;
   uint32_t node;

   memset (runtime, 0, sizeof (*runtime));
   runtime->cfg = *cfg;

   atomic_init (&runtime->stop, false);
   atomic_init (&runtime->slave_session_id, 0u);
   for (node = 0; node < (uint32_t)MU_NODE_COUNT; node++)
   {
      atomic_init (&runtime->session_id[node], 0u);
      atomic_init (&runtime->session_requests[node], 0u);
      flow_rng_seed (&runtime->rng[node], cfg->seed + 0x1000u * (node + 1u));
   }
   flow_rng_seed (&runtime->rng_slave, cfg->seed ^ 0x5A5A5A5Au);

   flow_app_glue_init (cfg->seed);

   flow_fault_init (
      &runtime->fault, cfg->fault, cfg->fault_first_cycle, cfg->fault_count);

   mu_bus_init (&runtime->mu);
   mu_bus_set_abort (&runtime->mu, &runtime->stop);
   flow_safe_tick_init (
      &runtime->tick, (uint64_t)cfg->safe_cycle_ms * 1000ull);

   runtime->shm = flow_shmem_create ();
   if (runtime->shm == NULL)
   {
      LOG_ERROR (FLOW_TAG_MAIN, "cannot allocate the SHMEM mailbox");
      return false;
   }

   flow_m7_core_setup (runtime);
   flow_m33_core_setup (runtime);

   memset (&a55_env, 0, sizeof (a55_env));
   a55_env.shm = runtime->shm;
   a55_env.fault = &runtime->fault;
   a55_env.cfg = &runtime->cfg;
   a55_env.rng = &runtime->rng_slave;
   a55_env.stop = &runtime->stop;
   a55_env.slave_session_id = &runtime->slave_session_id;

   flow_a55_core_init (&runtime->a55, &a55_env);

   if (cfg->fault != FLOW_FAULT_NONE)
   {
      LOG_INFO (FLOW_TAG_MAIN,
         "fault injection armed: %s at safe cycle %u for %u cycle(s)",
         flow_fault_name (cfg->fault),
         cfg->fault_first_cycle,
         cfg->fault_count);
   }

   return true;
}

bool flow_runtime_start (flow_runtime_t * runtime)
{
   int result;

   /* Re-arm the tick so that the first safe cycle starts one period from now,
    * after every thread has been created. */
   flow_safe_tick_rearm (&runtime->tick);

   /* The black channel and the second core start first, then M7 (the master
    * of the tick) closes the loop. */
   result = pthread_create (
      &runtime->thread[2], NULL, flow_a55_core_thread, &runtime->a55);
   if (result != 0)
   {
      LOG_ERROR (FLOW_TAG_MAIN, "cannot create the A55 thread (%d)", result);
      return false;
   }
   runtime->thread_started[2] = true;

   result = pthread_create (
      &runtime->thread[MU_NODE_M33], NULL, flow_m33_thread, runtime);
   if (result != 0)
   {
      LOG_ERROR (FLOW_TAG_MAIN, "cannot create the M33 thread (%d)", result);
      return false;
   }
   runtime->thread_started[MU_NODE_M33] = true;

   result = pthread_create (
      &runtime->thread[MU_NODE_M7], NULL, flow_m7_thread, runtime);
   if (result != 0)
   {
      LOG_ERROR (FLOW_TAG_MAIN, "cannot create the M7 thread (%d)", result);
      return false;
   }
   runtime->thread_started[MU_NODE_M7] = true;

   return true;
}

void flow_runtime_request_stop (flow_runtime_t * runtime)
{
   atomic_store_explicit (&runtime->stop, true, memory_order_seq_cst);
   flow_safe_tick_stop (&runtime->tick);
}

void flow_runtime_join (flow_runtime_t * runtime)
{
   uint32_t i;

   for (i = 0; i < FLOW_ARRAY_SIZE (runtime->thread); i++)
   {
      if (runtime->thread_started[i])
      {
         (void)pthread_join (runtime->thread[i], NULL);
         runtime->thread_started[i] = false;
      }
   }
}

void flow_runtime_cleanup (flow_runtime_t * runtime)
{
   flow_shmem_destroy (runtime->shm);
   runtime->shm = NULL;
   mu_bus_close (&runtime->mu);
   pthread_mutex_destroy (&runtime->tick.lock);
   pthread_cond_destroy (&runtime->tick.cv);
}

/* ----------------------------------------------------------------- report */

static void report_check (bool condition, const char * description, bool * ok)
{
   printf ("  [%s] %s\n", condition ? "PASS" : "FAIL", description);

   if (!condition)
   {
      *ok = false;
   }
}

static void report_core (
   const char * name,
   const flow_safe_core_t * core)
{
   const flow_core_stats_t * s = &core->stats;

   printf ("%s  cycles=%u  frames_tx=%u  frames_rx=%u  data_cycles=%u\n",
      name, s->cycles, s->frames_sent, s->frames_received, s->data_cycles);
   printf ("      pdu_ok=%u pdu_ko=%u egress_timeouts=%u commit_failures=%u\n",
      s->pdu_matches, s->pdu_mismatches, s->egress_timeouts,
      s->commit_failures);
   printf ("      inputs_ok=%u inputs_ko=%u logic_ok=%u logic_ko=%u\n",
      s->inputs_ok, s->inputs_mismatches, s->logic_ok, s->logic_mismatches);
   printf ("      session_neg=%u session_fail=%u resets=%u reset_req=%u\n",
      s->session_negotiations, s->session_failures, s->reset_events,
      s->reset_requests);
   printf ("      state_changes=%u enable_set=%u enable_clear=%u anomalies=%u\n",
      s->state_changes, s->enable_set, s->enable_clear, s->mu_anomalies);
}

bool flow_runtime_report (const flow_runtime_t * runtime)
{
   const flow_core_stats_t * m7 = &runtime->core[MU_NODE_M7].stats;
   const flow_core_stats_t * m33 = &runtime->core[MU_NODE_M33].stats;
   const flow_a55_stats_t * a55 = &runtime->a55.stats;
   mu_stats_t mu;
   bool ok = true;
   bool no_unexpected_faults;
   uint32_t session_m7;
   uint32_t session_m33;
   bool session_ok;
   bool data_ok;
   bool fault_ok;
   uint32_t total_mismatches;

   mu_bus_stats (&runtime->mu, &mu);

   session_m7 = atomic_load (&runtime->session_id[MU_NODE_M7]);
   session_m33 = atomic_load (&runtime->session_id[MU_NODE_M33]);

   printf ("\n");
   printf ("=============================================================\n");
   printf (" imx943-flow - 1oo2 dual core FSoE prototype - final report\n");
   printf ("=============================================================\n");
   printf (" safe cycle         : %u ms   (M7 is the tick master)\n",
      runtime->cfg.safe_cycle_ms);
   printf (" bus cycle (A55)    : %u ms\n", runtime->cfg.a55_cycle_ms);
   printf (" connections        : %u   slave address 0x%04x   connection id %u\n",
      (unsigned)FLOW_NUM_CONNECTIONS, (unsigned)FLOW_SLAVE_ADDRESS,
      (unsigned)FLOW_CONNECTION_ID);
   printf (" watchdog           : %u ms\n", (unsigned)FLOW_WATCHDOG_MS);
   printf (" fault injection    : %s (cycle %u, %u cycle(s), fired %u time(s))\n",
      flow_fault_name (runtime->cfg.fault),
      runtime->cfg.fault_first_cycle,
      runtime->cfg.fault_count,
      atomic_load (&runtime->fault.fired));
   printf ("-------------------------------------------------------------\n");
   report_core ("M7 ", &runtime->core[MU_NODE_M7]);
   report_core ("M33", &runtime->core[MU_NODE_M33]);
   printf ("A55  bus_cycles=%u  shmem_tx_read=%u  shmem_rx_published=%u  "
      "dropped=%u  deferred=%u\n",
      a55->bus_cycles, a55->tx_frames_read, a55->slave_frames_published,
      a55->tx_frames_dropped, a55->publish_deferred);
   printf ("     master_frames_delivered=%u  slave_frames=%u/%u  "
      "process_data_cycles=%u\n",
      a55->master_frames_delivered, a55->slave_frames_in,
      a55->slave_frames_out, a55->process_data_cycles);
   printf ("MU   messages=%u delivered=%u timeouts=%u digest_errors=%u "
      "routing_errors=%u\n",
      mu.sent, mu.delivered, mu.timeouts, mu.digest_errors, mu.routing_errors);
   printf ("SHM  rx_publishes=%u tx_stages=%u tx_commits=%u "
      "last_outputs=0x%04x/0x%04x actuator=%s\n",
      atomic_load (&runtime->shm->rx_publishes),
      atomic_load (&runtime->shm->tx_stages),
      atomic_load (&runtime->shm->tx_commits),
      atomic_load (&runtime->shm->a55.safe_outputs_word0),
      atomic_load (&runtime->shm->a55.safe_outputs_word1),
      atomic_load (&runtime->shm->a55.actuator_enabled) ? "ENABLED" : "SAFE STOP");
   printf ("-------------------------------------------------------------\n");
   printf (" Session ID M7  : 0x%04x\n", (unsigned)session_m7);
   printf (" Session ID M33 : 0x%04x\n", (unsigned)session_m33);
   printf ("-------------------------------------------------------------\n");

   printf ("checks:\n");

   if (runtime->cfg.fault == FLOW_FAULT_SESSION)
   {
      /* The injected fault deliberately makes M33 use a different Session ID:
       * what must be verified is that the divergence is detected. */
      report_check ((m7->pdu_mismatches > 0u) && (m7->reset_requests > 0u),
         "injected Session ID divergence detected by the PDU cross-check",
         &ok);
   }
   else
   {
      session_ok = (atomic_load (&runtime->session_requests[MU_NODE_M7]) > 0u) &&
         (session_m7 == session_m33);

      report_check (session_ok,
         "both safety cores negotiated the same Session ID (sum of randoms)",
         &ok);
   }

   data_ok = (m7->data_cycles > 0u) && (m33->data_cycles > 0u);

   fault_ok = (runtime->cfg.fault == FLOW_FAULT_NONE) ||
      (atomic_load (&runtime->fault.fired) > 0u);

   total_mismatches = m7->pdu_mismatches + m33->pdu_mismatches +
      m7->inputs_mismatches + m33->inputs_mismatches +
      m7->logic_mismatches + m33->logic_mismatches;

   no_unexpected_faults = (runtime->cfg.fault != FLOW_FAULT_NONE) ||
      ((total_mismatches == 0u) && (mu.timeouts == 0u) &&
       (mu.digest_errors == 0u));

   printf ("-------------------------------------------------------------\n");
   report_check (data_ok,
      "both FSoE connections reached the DATA state", &ok);
   report_check (no_unexpected_faults,
      "no cross-check mismatch / MU timeout without an injected fault", &ok);
   report_check (fault_ok,
      "the injected fault actually fired", &ok);
   report_check (m7->pdu_matches > 0u,
      "the egress PDU cross-check ran (frames committed through SHMEM)", &ok);

   printf ("-------------------------------------------------------------\n");
   printf (" RESULT: %s\n", ok ? "PASS" : "FAIL");
   printf ("=============================================================\n\n");

   return ok;
}

