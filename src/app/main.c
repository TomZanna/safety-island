/*
 * main.c
 *
 * Entry point of the imx943-flow prototype: command line parsing, start/stop
 * of the three threads and final report.
 */

#include "flow_runtime.h"
#include "flow_log.h"
#include "flow_time.h"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static volatile sig_atomic_t signal_received = 0;

static void main_on_signal (int signal_number)
{
   (void)signal_number;
   signal_received = 1;
}

static void main_usage (const char * program)
{
   printf ("Usage: %s [options]\n\n", program);
   printf ("Emulates on a Linux PC the 1oo2 dual core safety architecture of\n");
   printf ("the i.MX943: M7 + M33 FSoE masters, cross-checked through an\n");
   printf ("emulated messaging unit, exchanging frames with an A55 black\n");
   printf ("channel through a SHMEM mailbox.\n\n");
   printf ("Options:\n");
   printf ("  --duration MS       runtime in ms, 0 = until SIGINT (default %u)\n",
      (unsigned)FLOW_DEFAULT_DURATION_MS);
   printf ("  --cycle-ms MS       safe cycle of M7/M33 (default %u)\n",
      (unsigned)FLOW_SAFE_CYCLE_MS);
   printf ("  --a55-ms MS         black channel bus cycle (default %u)\n",
      (unsigned)FLOW_A55_CYCLE_MS);
   printf ("  --seed N            RNG seed, makes the run reproducible\n");
   printf ("  --fault NAME        none|session|pdu|inputs|logic|busdrop|stall|frame\n");
   printf ("  --fault-cycle N     safe cycle (bus cycle for busdrop) of the fault\n");
   printf ("  --fault-count N     how many cycles the fault lasts\n");
   printf ("  --connections N     number of FSoE connections (compile time, =%u)\n",
      (unsigned)FLOW_NUM_CONNECTIONS);
   printf ("  -v, --verbose       one line per safe cycle / bus cycle\n");
   printf ("  -t, --trace         one line per FSoE frame and per MU message\n");
   printf ("  -q, --quiet         errors only\n");
   printf ("  -h, --help          this help\n");
}

static bool main_parse_u32 (const char * text, uint32_t * value)
{
   char * end = NULL;
   unsigned long parsed;

   parsed = strtoul (text, &end, 0);

   if ((end == text) || (*end != '\0') || (parsed > 0xFFFFFFFFul))
   {
      return false;
   }

   *value = (uint32_t)parsed;

   return true;
}

int main (int argc, char ** argv)
{
   flow_cli_config_t cfg;
   flow_runtime_t runtime;
   flow_log_level_t level = FLOW_LOG_LEVEL_INFO;
   uint64_t duration_us;
   bool ok;
   int i;

   flow_cli_config_defaults (&cfg);

   for (i = 1; i < argc; i++)
   {
      const char * option = argv[i];
      const char * value = ((i + 1) < argc) ? argv[i + 1] : NULL;

      if ((strcmp (option, "-h") == 0) || (strcmp (option, "--help") == 0))
      {
         main_usage (argv[0]);
         return 0;
      }
      else if ((strcmp (option, "-v") == 0) ||
               (strcmp (option, "--verbose") == 0))
      {
         cfg.verbose = true;
         level = FLOW_LOG_LEVEL_DETAIL;
      }
      else if ((strcmp (option, "-t") == 0) ||
               (strcmp (option, "--trace") == 0))
      {
         cfg.verbose = true;
         cfg.trace = true;
         level = FLOW_LOG_LEVEL_TRACE;
      }
      else if ((strcmp (option, "-q") == 0) ||
               (strcmp (option, "--quiet") == 0))
      {
         level = FLOW_LOG_LEVEL_ERROR;
      }
      else if (strcmp (option, "--duration") == 0)
      {
         if (!main_parse_u32 (value, &cfg.duration_ms))
         {
            fprintf (stderr, "Invalid duration: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--cycle-ms") == 0)
      {
         if (!main_parse_u32 (value, &cfg.safe_cycle_ms) ||
             (cfg.safe_cycle_ms == 0u))
         {
            fprintf (stderr, "Invalid safe cycle: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--a55-ms") == 0)
      {
         if (!main_parse_u32 (value, &cfg.a55_cycle_ms) ||
             (cfg.a55_cycle_ms == 0u))
         {
            fprintf (stderr, "Invalid bus cycle: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--seed") == 0)
      {
         if (!main_parse_u32 (value, &cfg.seed))
         {
            fprintf (stderr, "Invalid seed: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--fault") == 0)
      {
         if (!flow_fault_parse (value, &cfg.fault))
         {
            fprintf (stderr, "Invalid fault name: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--fault-cycle") == 0)
      {
         if (!main_parse_u32 (value, &cfg.fault_first_cycle))
         {
            fprintf (stderr, "Invalid fault cycle: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--fault-count") == 0)
      {
         if (!main_parse_u32 (value, &cfg.fault_count) ||
             (cfg.fault_count == 0u))
         {
            fprintf (stderr, "Invalid fault count: %s\n", option);
            return 2;
         }
         i++;
      }
      else if (strcmp (option, "--connections") == 0)
      {
         if (!main_parse_u32 (value, &cfg.connections))
         {
            fprintf (stderr, "Invalid connections: %s\n", option);
            return 2;
         }
         i++;
      }
      else
      {
         fprintf (stderr, "Unknown or incomplete option: %s\n\n", option);
         main_usage (argv[0]);
         return 2;
      }
   }

   flow_log_set_level (level);

   if (cfg.connections != FLOW_NUM_CONNECTIONS)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "this binary was built for %u connection(s), %u requested: rebuild "
         "with make CONNECTIONS=%u",
         (unsigned)FLOW_NUM_CONNECTIONS,
         cfg.connections,
         cfg.connections);
      return 2;
   }

   printf ("\n");
   printf ("imx943-flow - 1oo2 dual core FSoE prototype (PC emulation)\n");
   printf ("-------------------------------------------------------------\n");
   LOG_INFO (FLOW_TAG_MAIN,
      "M7 : FSoE master #1, safe cycle %u ms, tick master", cfg.safe_cycle_ms);
   LOG_INFO (FLOW_TAG_MAIN,
      "M33: FSoE master #2, same safe tick, independent stack instance");
   LOG_INFO (FLOW_TAG_MAIN,
      "A55: black channel, bus cycle %u ms, emulated FSoE slave",
      cfg.a55_cycle_ms);

   if (!flow_runtime_init (&runtime, &cfg))
   {
      return 1;
   }

   (void)signal (SIGINT, main_on_signal);
   (void)signal (SIGTERM, main_on_signal);

   if (!flow_runtime_start (&runtime))
   {
      flow_runtime_request_stop (&runtime);
      flow_runtime_join (&runtime);
      flow_runtime_cleanup (&runtime);
      return 1;
   }

   duration_us = (uint64_t)cfg.duration_ms * 1000ull;

   LOG_INFO (FLOW_TAG_MAIN, "running%s",
      (cfg.duration_ms == 0u) ? " (Ctrl-C to stop)" : "");

   while (!signal_received)
   {
      if ((duration_us > 0u) && (flow_elapsed_us () >= duration_us))
      {
         break;
      }
      flow_sleep_us (2000u);
   }

   LOG_INFO (FLOW_TAG_MAIN, "stopping the threads");
   flow_runtime_request_stop (&runtime);
   flow_runtime_join (&runtime);

   ok = flow_runtime_report (&runtime);

   flow_runtime_cleanup (&runtime);

   return ok ? 0 : 1;
}

