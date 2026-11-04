/*
 * host_main.c
 *
 * Host validation of the bare metal architecture:
 *   - two safe cores (M7 and M33) executing the same flow_safe.c
 *   - the portable A55 mailbox merge
 *   - an emulated FSoE slave and field process image
 *   - an external tick thread
 *
 * This file is test-only and is not part of the target build.
 */

#include "flow_a55.h"
#include "flow_platform_host.h"
#include "flow_safe.h"
#include "flow_shm.h"
#include "host_field.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static atomic_bool stop_flag;
static flow_shm_t * shared_shm;
static flow_safe_t safe_core[FLOW_CORE_COUNT];
static flow_test_fault_t fault_type = FLOW_TEST_FAULT_NONE;
static flow_u32 duration_ms = 2000u;
static flow_u32 seed = 0x1A2B3C4Du;
static bool verbose = false;
static const flow_core_id_t core_id[FLOW_CORE_COUNT] =
{
   FLOW_CORE_M7, FLOW_CORE_M33
};

static void * safe_thread (void * argument)
{
   const flow_core_id_t id = *(const flow_core_id_t *)argument;

   flow_platform_set_core (id);
   flow_platform_init ();
   flow_safe_init (&safe_core[id], id, shared_shm);
   safe_core[id].fault = fault_type;

   while (!atomic_load (&stop_flag))
   {
      if (!flow_platform_tick_wait ())
      {
         break;
      }
      flow_safe_step (&safe_core[id]);
   }

   return NULL;
}

static void host_a55_cycle (
   flow_a55_t * a55,
   host_slave_t * slave,
   flow_u32 bus_cycle)
{
   flow_a55_frame_t frame;
   flow_u8 tx_pdu[FLOW_PDU_MAX];
   flow_u32 tx_len = 0u;

   if (flow_a55_get_tx (a55, 0u, &frame) == FLOW_A55_OK)
   {
      host_slave_provide_rx (slave, frame.pdu, frame.len);
   }

   host_slave_step (slave, bus_cycle);

   if (host_slave_take_tx (slave, tx_pdu, &tx_len))
   {
      (void)flow_a55_put_rx (a55, 0u, tx_pdu, tx_len);
   }
}

static void usage (const char * program)
{
   printf ("Usage: %s [options]\n", program);
   printf ("  --duration MS   test duration (default 2000)\n");
   printf ("  --seed N        RNG seed\n");
   printf ("  --fault NAME    none|pdu|inputs|logic|session|stall\n");
   printf ("  --verbose       print cycle logs\n");
}

static bool parse_fault (const char * name, flow_test_fault_t * fault)
{
   if (strcmp (name, "none") == 0)
   {
      *fault = FLOW_TEST_FAULT_NONE;
   }
   else if (strcmp (name, "pdu") == 0)
   {
      *fault = FLOW_TEST_FAULT_PDU;
   }
   else if (strcmp (name, "inputs") == 0)
   {
      *fault = FLOW_TEST_FAULT_INPUTS;
   }
   else if (strcmp (name, "logic") == 0)
   {
      *fault = FLOW_TEST_FAULT_LOGIC;
   }
   else if (strcmp (name, "session") == 0)
   {
      *fault = FLOW_TEST_FAULT_SESSION;
   }
   else if (strcmp (name, "stall") == 0)
   {
      *fault = FLOW_TEST_FAULT_STALL;
   }
   else
   {
      return false;
   }

   return true;
}

static unsigned long long now_us64 (void)
{
   struct timespec now;

   (void)clock_gettime (CLOCK_MONOTONIC, &now);

   return (unsigned long long)now.tv_sec * 1000000ull +
      (unsigned long long)now.tv_nsec / 1000ull;
}

int main (int argc, char ** argv)
{
   flow_a55_t a55;
   host_slave_t slave;
   pthread_t threads[FLOW_CORE_COUNT];
   unsigned long long start_us;
   unsigned long long end_us;
   flow_u32 bus_cycle = 0u;
   bool data_ok;
   bool session_ok;
   bool fault_ok;
   bool enabled_ok;
   bool ok;
   int i;

   for (i = 1; i < argc; i++)
   {
      if ((strcmp (argv[i], "--verbose") == 0) ||
          (strcmp (argv[i], "-v") == 0))
      {
         verbose = true;
      }
      else if ((strcmp (argv[i], "--duration") == 0) && ((i + 1) < argc))
      {
         duration_ms = (flow_u32)strtoul (argv[++i], NULL, 0);
      }
      else if ((strcmp (argv[i], "--seed") == 0) && ((i + 1) < argc))
      {
         seed = (flow_u32)strtoul (argv[++i], NULL, 0);
      }
      else if ((strcmp (argv[i], "--fault") == 0) && ((i + 1) < argc))
      {
         if (!parse_fault (argv[++i], &fault_type))
         {
            fprintf (stderr, "invalid fault: %s\n", argv[i]);
            return 2;
         }
      }
      else if ((strcmp (argv[i], "--help") == 0) ||
               (strcmp (argv[i], "-h") == 0))
      {
         usage (argv[0]);
         return 0;
      }
      else
      {
         fprintf (stderr, "unknown option: %s\n", argv[i]);
         usage (argv[0]);
         return 2;
      }
   }

   if (duration_ms == 0u)
   {
      duration_ms = 2000u;
   }

   flow_platform_host_configure (seed, verbose);
   flow_platform_set_core (FLOW_CORE_M7);
   flow_platform_init ();

   shared_shm = (flow_shm_t *)calloc (1u, sizeof (*shared_shm));
   if (shared_shm == NULL)
   {
      fprintf (stderr, "cannot allocate the shared mailbox\n");
      return 1;
   }

   flow_shm_init (shared_shm, FLOW_CORE_M7);
   if (!flow_shm_validate (shared_shm))
   {
      fprintf (stderr, "shared mailbox validation failed\n");
      free (shared_shm);
      return 1;
   }

   flow_a55_init (&a55, shared_shm);
   host_slave_init (&slave, (flow_u16)FLOW_SLAVE_ADDRESS);

   atomic_init (&stop_flag, false);

   for (i = 0; i < (int)FLOW_CORE_COUNT; i++)
   {
      if (pthread_create (&threads[i], NULL, safe_thread,
             (void *)&core_id[i]) != 0)
      {
         fprintf (stderr, "cannot create safe core thread %d\n", i);
         return 1;
      }
   }

   start_us = now_us64 ();
   end_us = start_us + (unsigned long long)duration_ms * 1000ull;

   while (!atomic_load (&stop_flag) && (now_us64 () < end_us))
   {
      const struct timespec period =
      {
         .tv_sec = 0,
         .tv_nsec = (long)FLOW_A55_CYCLE_MS * 1000000L,
      };

      (void)nanosleep (&period, NULL);
      host_a55_cycle (&a55, &slave, bus_cycle);
      bus_cycle++;
   }

   atomic_store (&stop_flag, true);
   flow_platform_shutdown ();

   for (i = 0; i < (int)FLOW_CORE_COUNT; i++)
   {
      (void)pthread_join (threads[i], NULL);
   }

   data_ok = (safe_core[FLOW_CORE_M7].stats.data_cycles > 0u) &&
      (safe_core[FLOW_CORE_M33].stats.data_cycles > 0u);
   session_ok = (safe_core[FLOW_CORE_M7].session_id != 0u) &&
      (safe_core[FLOW_CORE_M7].session_id ==
       safe_core[FLOW_CORE_M33].session_id);
   enabled_ok = slave.enabled_seen;

   if (fault_type == FLOW_TEST_FAULT_NONE)
   {
      fault_ok = (safe_core[FLOW_CORE_M7].anomalies == 0u) &&
         (safe_core[FLOW_CORE_M33].anomalies == 0u);
   }
   else
   {
      fault_ok = (safe_core[FLOW_CORE_M7].anomalies > 0u) ||
         (safe_core[FLOW_CORE_M33].anomalies > 0u);
   }

   printf ("\nhost validation\n");
   printf ("  bus cycles        : %u\n", (unsigned)bus_cycle);
   printf ("  safe cycles M7    : %u (data %u)\n",
      (unsigned)safe_core[FLOW_CORE_M7].stats.cycles,
      (unsigned)safe_core[FLOW_CORE_M7].stats.data_cycles);
   printf ("  safe cycles M33   : %u (data %u)\n",
      (unsigned)safe_core[FLOW_CORE_M33].stats.cycles,
      (unsigned)safe_core[FLOW_CORE_M33].stats.data_cycles);
   printf ("  session id        : 0x%04x / 0x%04x\n",
      (unsigned)safe_core[FLOW_CORE_M7].session_id,
      (unsigned)safe_core[FLOW_CORE_M33].session_id);
   printf ("  anomalies         : M7=%u M33=%u\n",
      (unsigned)safe_core[FLOW_CORE_M7].anomalies,
      (unsigned)safe_core[FLOW_CORE_M33].anomalies);
   printf ("  pdu mismatches    : M7=%u M33=%u\n",
      (unsigned)safe_core[FLOW_CORE_M7].stats.pdu_mismatches,
      (unsigned)safe_core[FLOW_CORE_M33].stats.pdu_mismatches);
   printf ("  inputs mismatches : M7=%u M33=%u\n",
      (unsigned)safe_core[FLOW_CORE_M7].stats.inputs_mismatches,
      (unsigned)safe_core[FLOW_CORE_M33].stats.inputs_mismatches);
   printf ("  logic mismatches  : M7=%u M33=%u\n",
      (unsigned)safe_core[FLOW_CORE_M7].stats.logic_mismatches,
      (unsigned)safe_core[FLOW_CORE_M33].stats.logic_mismatches);
   printf ("  actuator enabled  : %s\n", enabled_ok ? "yes" : "no");
   printf ("  A55 invalid frames: %u\n", (unsigned)a55.invalid_frames);

   ok = data_ok && session_ok && fault_ok && enabled_ok;

   printf ("  checks            : data=%s session=%s fault=%s actuator=%s\n",
      data_ok ? "ok" : "FAIL",
      session_ok ? "ok" : "FAIL",
      fault_ok ? "ok" : "FAIL",
      enabled_ok ? "ok" : "FAIL");
   printf ("RESULT: %s\n", ok ? "PASS" : "FAIL");

   free (shared_shm);

   return ok ? 0 : 1;
}

