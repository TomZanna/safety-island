/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2019 rt-labs AB, Sweden.
 * See LICENSE file in the project root for full license information.
 ********************************************************************/

/**
 * \file
 * \brief FSoE watchdog timer implementation
 *
 * See public header file for description.
 * All comments below are implementation details only.
 *
 * This is a simple timer which only uses the system time
 * to operate.
 *
 * Naming convention
 * -----------------
 *
 * Two different naming conventions are used:
 * - For variables and functions defined by the standard, the names used by the
 *   standard are used (UpperCamelCase for functions and variables,
 *   UPPER_CASE_WITH_UNDERSCORES for "macro" functions.
 * - In all other cases, lower_case_with_underscores are used for functions
 *   and variables and UPPER_CASE_WITH_UNDERSCORES for macros and enum fields.
 */

/* Replace function call with call to mock function when building
 * unit tests.
 */
#ifdef UNIT_TEST
#define fsoeport_current_time_us mock_fsoeport_current_time_us
#endif

#include "fsoewatchdog.h"
#include "fsoeport.h"
#include "fsoelog.h"
#include <string.h>

/* \brief See public header file for description
 */
void fsoewatchdog_START_WD (fsoewatchdog_t * watchdog)
{
   watchdog->start_time_us = fsoeport_current_time_us ();
   watchdog->is_started = true;
}

/* \brief Stop the watchdog timer
 *
 * See public header file for description
 */
void fsoewatchdog_STOP_WD (fsoewatchdog_t * watchdog)
{
   watchdog->is_started = false;
}

/* \brief Get configured timeout, in milliseconds
 *
 * See public header file for description
 */
uint32_t fsoewatchdog_timeout_ms (const fsoewatchdog_t * watchdog)
{
   return watchdog->timeout_ms;
}

/* \brief Get time remaining until watchdog timer expires, in milliseconds
 *
 * See public header file for description
 */
uint32_t fsoewatchdog_time_until_timeout_ms (
   const fsoewatchdog_t * watchdog)
{
   uint32_t remaining_ms;

   if (watchdog->is_started)
   {
      uint32_t elapsed_us;

      elapsed_us = fsoeport_current_time_us () - watchdog->start_time_us;
      if (elapsed_us < 1000u * watchdog->timeout_ms)
      {
         remaining_ms = watchdog->timeout_ms - elapsed_us / 1000u;
      }
      else
      {
         remaining_ms = 0;
      }
   }
   else
   {
      remaining_ms = UINT32_MAX;
   }

   return remaining_ms;
}

/* \brief Set watchdog timeout in milliseconds
 *
 * See public header file for description
 */
void fsoewatchdog_set_timeout_ms (
   fsoewatchdog_t * watchdog, uint32_t timeout_ms)
{
   FSOEPORT_ASSERT (timeout_ms > 0);

   watchdog->timeout_ms = timeout_ms;
}

/* \brief Initialise watchdog timer
 *
 * See public header file for description
 */
void fsoewatchdog_init (fsoewatchdog_t * watchdog)
{
   FSOEPORT_ASSERT (watchdog != NULL);

   watchdog->timeout_ms = 0;
   watchdog->is_started = false;
   watchdog->start_time_us = 0;
}
