/*
 * flow_platform_host.c
 *
 * POSIX implementation of the platform hooks for the host validation test.
 * It emulates the external safe tick, the hardware time source, the TRNG and
 * the inter-core messaging unit.  This file is never part of the target
 * safety build.
 */

#include "flow_platform_host.h"
#include "flow_config.h"
#include "flow_mu.h"

#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct flow_host_platform
{
   pthread_mutex_t lock;
   pthread_cond_t tick_cv;
   pthread_cond_t mu_cv;

   pthread_mutex_t log_lock;

   flow_u32 tick_generation;
   bool tick_stop;
   bool tick_started;
   pthread_t tick_thread;

   flow_mu_message_t mu[FLOW_CORE_COUNT][FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX];
   bool mu_valid[FLOW_CORE_COUNT][FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX];

   flow_u32 seed;
   bool verbose;
   flow_u32 start_us;
} flow_host_platform_t;

static flow_host_platform_t host;
static pthread_once_t host_once = PTHREAD_ONCE_INIT;
static flow_u32 configured_seed = 0x1A2B3C4Du;
static bool configured_verbose = false;

static _Thread_local flow_core_id_t thread_core = FLOW_CORE_M7;
static _Thread_local flow_u32 thread_rng;
static _Thread_local bool thread_rng_ready;

static flow_u32 host_now_us (void)
{
   struct timespec now;

   (void)clock_gettime (CLOCK_MONOTONIC, &now);

   return (flow_u32)(((unsigned long long)now.tv_sec * 1000000ull) +
      ((unsigned long long)now.tv_nsec / 1000ull));
}

static void host_deadline_after_us (
   struct timespec * deadline,
   flow_u32 duration_us)
{
   unsigned long long now_us;
   unsigned long long target_us;

   (void)clock_gettime (CLOCK_MONOTONIC, deadline);

   now_us = ((unsigned long long)deadline->tv_sec * 1000000ull) +
      ((unsigned long long)deadline->tv_nsec / 1000ull);
   target_us = now_us + duration_us;

   deadline->tv_sec = (time_t)(target_us / 1000000ull);
   deadline->tv_nsec = (long)((target_us % 1000000ull) * 1000ull);
}

static void * host_tick_thread (void * argument)
{
   struct timespec next;
   const unsigned long long period_us =
      (unsigned long long)FLOW_SAFE_CYCLE_MS * 1000ull;

   (void)argument;

   (void)clock_gettime (CLOCK_MONOTONIC, &next);

   for (;;)
   {
      unsigned long long next_us =
         ((unsigned long long)next.tv_sec * 1000000ull) +
         ((unsigned long long)next.tv_nsec / 1000ull) + period_us;

      next.tv_sec = (time_t)(next_us / 1000000ull);
      next.tv_nsec = (long)((next_us % 1000000ull) * 1000ull);

      while (clock_nanosleep (CLOCK_MONOTONIC, TIMER_ABSTIME, &next, NULL) ==
             EINTR)
      {
      }

      pthread_mutex_lock (&host.lock);
      if (host.tick_stop)
      {
         pthread_mutex_unlock (&host.lock);
         break;
      }
      host.tick_generation++;
      pthread_cond_broadcast (&host.tick_cv);
      pthread_mutex_unlock (&host.lock);
   }

   return NULL;
}

static void host_init_once (void)
{
   pthread_condattr_t attr;
   int result;

   memset (&host, 0, sizeof (host));
   host.seed = configured_seed;
   host.verbose = configured_verbose;

   pthread_mutex_init (&host.lock, NULL);
   pthread_mutex_init (&host.log_lock, NULL);
   pthread_condattr_init (&attr);
   pthread_condattr_setclock (&attr, CLOCK_MONOTONIC);
   pthread_cond_init (&host.tick_cv, &attr);
   pthread_cond_init (&host.mu_cv, &attr);
   pthread_condattr_destroy (&attr);

   host.start_us = host_now_us ();

   result = pthread_create (&host.tick_thread, NULL, host_tick_thread, NULL);
   if (result != 0)
   {
      fprintf (stderr, "host platform: cannot create tick thread\n");
      abort ();
   }
   host.tick_started = true;
}

void flow_platform_host_configure (flow_u32 seed, bool verbose)
{
   configured_seed = (seed == 0u) ? 0x1A2B3C4Du : seed;
   configured_verbose = verbose;
}

void flow_platform_set_core (flow_core_id_t core)
{
   if (core >= FLOW_CORE_COUNT)
   {
      abort ();
   }

   thread_core = core;
   thread_rng_ready = false;
}

void flow_platform_init (void)
{
   (void)pthread_once (&host_once, host_init_once);

   if (!thread_rng_ready)
   {
      thread_rng = host.seed + 0x1000u * ((flow_u32)thread_core + 1u);
      if (thread_rng == 0u)
      {
         thread_rng = 0xDEADBEEFu;
      }
      thread_rng_ready = true;
   }
}

bool flow_platform_tick_wait (void)
{
   flow_u32 seen;
   bool running = true;

   pthread_mutex_lock (&host.lock);
   seen = host.tick_generation;

   while ((host.tick_generation == seen) && !host.tick_stop)
   {
      pthread_cond_wait (&host.tick_cv, &host.lock);
   }

   if (host.tick_stop)
   {
      running = false;
   }

   pthread_mutex_unlock (&host.lock);

   return running;
}

flow_u32 flow_platform_time_us (void)
{
   return host_now_us ();
}

flow_u16 flow_platform_random_u16 (void)
{
   flow_u32 value;

   if (!thread_rng_ready)
   {
      flow_platform_init ();
   }

   value = thread_rng;
   value ^= value << 13;
   value ^= value >> 17;
   value ^= value << 5;
   thread_rng = (value == 0u) ? 0xDEADBEEFu : value;

   return (flow_u16)(thread_rng >> 16);
}

void flow_platform_fatal (void)
{
   fprintf (stderr, "flow: fatal safety platform error\n");
   abort ();
}

void flow_platform_log (const char * tag, const char * format, ...)
{
   va_list arguments;
   flow_u32 elapsed;

   if (!host.verbose)
   {
      return;
   }

   elapsed = host_now_us () - host.start_us;

   pthread_mutex_lock (&host.log_lock);
   printf ("[%3u.%03u] %s | ", (unsigned)(elapsed / 1000000u),
      (unsigned)((elapsed / 1000u) % 1000u), tag);
   va_start (arguments, format);
   vprintf (format, arguments);
   va_end (arguments);
   printf ("\n");
   fflush (stdout);
   pthread_mutex_unlock (&host.log_lock);
}


bool flow_platform_mu_post (
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   const void * message,
   flow_u32 size)
{
   if ((self >= FLOW_CORE_COUNT) || (channel >= FLOW_MU_CHAN_COUNT) ||
       (tag >= FLOW_MU_TAG_MAX) || (message == NULL) ||
       (size > sizeof (flow_mu_message_t)))
   {
      return false;
   }

   pthread_mutex_lock (&host.lock);
   memset (&host.mu[self][channel][tag], 0,
      sizeof (host.mu[self][channel][tag]));
   memcpy (&host.mu[self][channel][tag], message, size);
   host.mu_valid[self][channel][tag] = true;
   pthread_cond_broadcast (&host.mu_cv);
   pthread_mutex_unlock (&host.lock);

   return true;
}

bool flow_platform_mu_wait (
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   flow_u32 cycle,
   void * message,
   flow_u32 max_size,
   flow_u32 timeout_us)
{
   flow_core_id_t peer;
   struct timespec deadline;
   bool found = false;

   if ((self >= FLOW_CORE_COUNT) || (channel >= FLOW_MU_CHAN_COUNT) ||
       (tag >= FLOW_MU_TAG_MAX) || (message == NULL) ||
       (max_size < sizeof (flow_mu_message_t)))
   {
      return false;
   }

   peer = (self == FLOW_CORE_M7) ? FLOW_CORE_M33 : FLOW_CORE_M7;
   host_deadline_after_us (&deadline, timeout_us);

   pthread_mutex_lock (&host.lock);

   for (;;)
   {
      const flow_mu_message_t * candidate = &host.mu[peer][channel][tag];

      if (host.mu_valid[peer][channel][tag] &&
          (candidate->cycle == cycle) && (candidate->tag == tag))
      {
         memcpy (message, candidate, sizeof (*candidate));
         found = true;
         break;
      }

      if (pthread_cond_timedwait (&host.mu_cv, &host.lock, &deadline) != 0)
      {
         break;
      }
   }

   pthread_mutex_unlock (&host.lock);

   return found;
}

void flow_platform_watchdog_kick (void)
{
   /* Host test: the external watchdog is not emulated. */
}

void flow_platform_shutdown (void)
{
   bool join_tick = false;

   pthread_mutex_lock (&host.lock);
   if (host.tick_started)
   {
      host.tick_stop = true;
      pthread_cond_broadcast (&host.tick_cv);
      join_tick = true;
      host.tick_started = false;
   }
   pthread_mutex_unlock (&host.lock);

   if (join_tick)
   {
      (void)pthread_join (host.tick_thread, NULL);
   }
}

void flow_platform_tick_isr (void)
{
   /* Host test: the tick thread calls the equivalent logic directly. */
}

