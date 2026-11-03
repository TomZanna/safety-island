/*
 * safe_tick.c
 *
 * See safe_tick.h
 */

#include "safe_tick.h"
#include "flow_config.h"
#include "flow_log.h"
#include "flow_time.h"

#include <errno.h>

void flow_safe_tick_init (flow_safe_tick_t * tick, uint64_t period_us)
{
   pthread_condattr_t attr;

   pthread_condattr_init (&attr);
   pthread_condattr_setclock (&attr, CLOCK_MONOTONIC);

   pthread_mutex_init (&tick->lock, NULL);
   pthread_cond_init (&tick->cv, &attr);

   pthread_condattr_destroy (&attr);

   tick->period_us = period_us;
   tick->deadline_us = flow_deadline_after_us (period_us);
   tick->cycles = 0u;
   tick->release_count = 0u;
   tick->jitter_us = 0u;
   tick->parked[MU_NODE_M7] = false;
   tick->parked[MU_NODE_M33] = false;
   tick->abort = false;
}

static flow_tick_result_t tick_wait_release (
   flow_safe_tick_t * tick,
   uint64_t deadline_us)
{
   struct timespec until;
   flow_tick_result_t result = FLOW_TICK_OK;
   uint64_t now;

   /* Hardware timer tick: the master of the tick sleeps on an absolute
    * CLOCK_MONOTONIC deadline, so there is no drift accumulation. */
   (void)flow_sleep_until_us (deadline_us);

   now = flow_now_us ();

   pthread_mutex_lock (&tick->lock);

   tick->jitter_us = (now > deadline_us) ? (now - deadline_us) : 0u;

   /* Wait (bounded) until the second core has parked on the barrier. */
   flow_timespec_after_us (&until, FLOW_TICK_PARK_TIMEOUT_US);
   while (!tick->parked[MU_NODE_M33] && !tick->abort)
   {
      if (pthread_cond_timedwait (&tick->cv, &tick->lock, &until) != 0)
      {
         result = FLOW_TICK_TIMEOUT;
         break;
      }
   }

   if (tick->abort)
   {
      result = FLOW_TICK_STOPPED;
   }

   /* Release the cycle. */
   tick->parked[MU_NODE_M7] = false;
   tick->parked[MU_NODE_M33] = false;
   tick->deadline_us = deadline_us + tick->period_us;
   tick->cycles++;
   tick->release_count++;
   pthread_cond_broadcast (&tick->cv);

   pthread_mutex_unlock (&tick->lock);

   if (result == FLOW_TICK_TIMEOUT)
   {
      LOG_WARN (FLOW_TAG_SAFE,
         "M7 released safe cycle %llu without M33 parked (peer late by >%u us)",
         (unsigned long long)tick->cycles,
         (unsigned)FLOW_TICK_PARK_TIMEOUT_US);
   }

   return result;
}

static flow_tick_result_t tick_wait_release_by_peer (
   flow_safe_tick_t * tick,
   uint64_t generation)
{
   struct timespec until;
   flow_tick_result_t result = FLOW_TICK_OK;

   flow_timespec_after_us (&until, 3ull * tick->period_us + FLOW_TICK_PARK_TIMEOUT_US);

   while ((tick->cycles == generation) && !tick->abort)
   {
      if (pthread_cond_timedwait (&tick->cv, &tick->lock, &until) != 0)
      {
         result = FLOW_TICK_TIMEOUT;
         break;
      }
   }

   if (tick->abort)
   {
      result = FLOW_TICK_STOPPED;
   }
   else if (result == FLOW_TICK_TIMEOUT)
   {
      LOG_WARN (FLOW_TAG_SAFE,
         "M33 did not see a new safe tick within %llu us",
         (unsigned long long)(3ull * tick->period_us + FLOW_TICK_PARK_TIMEOUT_US));
   }

   return result;
}

flow_tick_result_t flow_safe_tick_wait (flow_safe_tick_t * tick, mu_node_t node)
{
   flow_tick_result_t result;
   uint64_t deadline;
   uint64_t generation;

   pthread_mutex_lock (&tick->lock);

   if (tick->abort)
   {
      pthread_mutex_unlock (&tick->lock);
      return FLOW_TICK_STOPPED;
   }

   tick->parked[node] = true;
   pthread_cond_broadcast (&tick->cv);

   deadline = tick->deadline_us;
   generation = tick->cycles;

   if (node == MU_NODE_M7)
   {
      pthread_mutex_unlock (&tick->lock);
      result = tick_wait_release (tick, deadline);
   }
   else
   {
      result = tick_wait_release_by_peer (tick, generation);
      pthread_mutex_unlock (&tick->lock);
   }

   return result;
}

void flow_safe_tick_stop (flow_safe_tick_t * tick)
{
   pthread_mutex_lock (&tick->lock);
   tick->abort = true;
   pthread_cond_broadcast (&tick->cv);
   pthread_mutex_unlock (&tick->lock);
}

void flow_safe_tick_rearm (flow_safe_tick_t * tick)
{
   pthread_mutex_lock (&tick->lock);
   tick->deadline_us = flow_deadline_after_us (tick->period_us);
   pthread_mutex_unlock (&tick->lock);
}
