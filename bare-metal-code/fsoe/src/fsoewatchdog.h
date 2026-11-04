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
 * \brief FSoE watchdog timer
 *
 * Implementation of an FSoE watchdog timer.
 *
 * Provides the following functionality:
 * - Starting and stopping the time.
 * - Checking if timer has expired.
 */

#ifndef FSOEWATCHDOG_H
#define FSOEWATCHDOG_H

#include <fsoetypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Get time remaining until watchdog timer expires, in milliseconds
 *
 * Returned value is rounded up; e.g. if 1 microseconds remains it is
 * counted as 1 millisecond.
 *
 * \param[in]     watchdog       Watchdog timer
 * \return                       Time in milliseconds until timer expires
 *                               if timer is started but not yet expired.
 *                               0 if timer is started but has expired.
 *                               UINT32_MAX if timer is not started.
 */
uint32_t fsoewatchdog_time_until_timeout_ms (
   const fsoewatchdog_t * watchdog);

/**
 * \brief Restart the watchdog timer
 *
 * Timer is reset and then restarted with configured timeout.
 *
 * This function is similar to the macro START_WD(). See ETG.5100
 * ch. 8.4.1.4 table 33: "Macros in the FSoE Master state table" and
 * ch. 8.5.1.4 table 38: "Macros in the FSoE Slave state table".
 *
 * \pre Timeout has been set using fsoewatchdog_set_timeout_ms().
 *
 * \param[in,out] watchdog       Watchdog timer
 */
void fsoewatchdog_START_WD (fsoewatchdog_t * watchdog);

/**
 * \brief Stop the watchdog timer
 *
 * A stopped timer can't expire and is counted as not started.
 *
 * This function is similar to the macro STOP_WD(). See ETG.5100
 * ch. 8.5.1.4 table 38: "Macros in the FSoE Slave state table".
 *
 * \param[in,out] watchdog       Watchdog timer
 */
void fsoewatchdog_STOP_WD (fsoewatchdog_t * watchdog);

/**
 * \brief Get timeout in milliseconds
 *
 * \param[in]     watchdog       Watchdog timer
 * \return                       Timeout in milliseconds
 */
uint32_t fsoewatchdog_timeout_ms (const fsoewatchdog_t * watchdog);

/**
 * \brief Set watchdog timeout in milliseconds
 *
 * Note that this does not start or stop the timer.
 *
 * \param[in,out] watchdog       Watchdog timer
 * \param[in]     timeout_ms     Timeout in milliseconds
 */
void fsoewatchdog_set_timeout_ms (
   fsoewatchdog_t * watchdog, uint32_t timeout_ms);

/**
 * \brief Initialise watchdog timer
 *
 * \param[out]    watchdog       Watchdog timer
 */
void fsoewatchdog_init (fsoewatchdog_t * watchdog);

#ifdef __cplusplus
}
#endif

#endif /* FSOEWATCHDOG_H */
