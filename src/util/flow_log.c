/*
 * flow_log.c
 *
 * See flow_log.h
 */

#include "flow_log.h"
#include "flow_time.h"

#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>

static pthread_mutex_t log_lock = PTHREAD_MUTEX_INITIALIZER;

/* Runtime configurable log level. Not atomic on purpose: it is written once
 * at startup, before the threads are created. */
static flow_log_level_t current_level = FLOW_LOG_LEVEL_INFO;

void flow_log_set_level (flow_log_level_t level)
{
   current_level = level;
}

flow_log_level_t flow_log_level (void)
{
   return current_level;
}

int flow_log_is_enabled (flow_log_level_t level)
{
   return (int)level <= (int)current_level;
}

void flow_log_printf (
   flow_log_level_t level,
   const char * tag,
   const char * fmt,
   ...)
{
   va_list list;
   unsigned long long elapsed_us;

   if (!flow_log_is_enabled (level))
   {
      return;
   }

   elapsed_us = flow_elapsed_us ();

   pthread_mutex_lock (&log_lock);
   printf ("[%3llu.%03llu] %s | ",
      elapsed_us / 1000000ull,
      (elapsed_us / 1000ull) % 1000ull,
      tag);
   va_start (list, fmt);
   vprintf (fmt, list);
   va_end (list);
   printf ("\n");
   fflush (stdout);
   pthread_mutex_unlock (&log_lock);
}
