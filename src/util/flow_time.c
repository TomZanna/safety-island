/*
 * flow_time.c
 *
 * See flow_time.h
 */

#include "flow_time.h"

#include <errno.h>

#define FLOW_NS_PER_US 1000ull

static int time_initialised;
static uint64_t time_start_us;

uint64_t flow_now_us (void)
{
   struct timespec ts;

   clock_gettime (CLOCK_MONOTONIC, &ts);

   return (uint64_t)ts.tv_sec * 1000000ull + (uint64_t)ts.tv_nsec / FLOW_NS_PER_US;
}

uint64_t flow_elapsed_us (void)
{
   uint64_t now = flow_now_us ();

   if (!time_initialised)
   {
      time_start_us = now;
      time_initialised = 1;
   }

   return now - time_start_us;
}

uint64_t flow_deadline_after_us (uint64_t duration_us)
{
   return flow_now_us () + duration_us;
}

int flow_sleep_until_us (uint64_t deadline_us)
{
   struct timespec ts;
   int result;

   ts.tv_sec = (time_t)(deadline_us / 1000000ull);
   ts.tv_nsec = (long)((deadline_us % 1000000ull) * FLOW_NS_PER_US);

   do
   {
      result = clock_nanosleep (CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
   }
   while (result == EINTR);

   return result;
}

void flow_sleep_us (uint64_t duration_us)
{
   (void)flow_sleep_until_us (flow_deadline_after_us (duration_us));
}

void flow_timespec_after_us (struct timespec * ts, uint64_t duration_us)
{
   uint64_t deadline_us = flow_deadline_after_us (duration_us);

   ts->tv_sec = (time_t)(deadline_us / 1000000ull);
   ts->tv_nsec = (long)((deadline_us % 1000000ull) * FLOW_NS_PER_US);
}
