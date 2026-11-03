/*
 * safe_tick.h
 *
 * The "Safe HW Timer Tick" shared by M7 and M33.
 *
 * Both safety cores start a new cycle on the same tick. M7 is the master of
 * the timer (on the target: the hardware safe timer), M33 is released by the
 * same tick, so the two cores never drift apart.
 *
 * Implementation note: a plain pthread_barrier_t cannot be timed out without
 * losing its state consistency, so the same semantics is implemented with a
 * mutex + condition variable: each core parks, M7 sleeps on an *absolute*
 * CLOCK_MONOTONIC deadline and then releases the cycle. If M33 does not park
 * within FLOW_TICK_PARK_TIMEOUT_US, M7 releases the cycle anyway and the
 * desync is counted (fail-safe path in the cross-checks).
 */

#ifndef SAFE_TICK_H
#define SAFE_TICK_H

#include "mu_channel.h"

#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum flow_tick_result
{
   FLOW_TICK_OK = 0,
   FLOW_TICK_TIMEOUT,   /**< the peer did not arrive in time      */
   FLOW_TICK_STOPPED    /**< system shutting down                 */
} flow_tick_result_t;

typedef struct flow_safe_tick
{
   pthread_mutex_t lock;
   pthread_cond_t cv;

   uint64_t period_us;
   uint64_t deadline_us;      /**< next absolute deadline (M7 only)     */
   uint64_t cycles;           /**< released cycles                      */
   uint64_t release_count;
   uint64_t jitter_us;        /**< last release overshoot               */

   bool parked[MU_NODE_COUNT];
   bool abort;
} flow_safe_tick_t;

void flow_safe_tick_init (flow_safe_tick_t * tick, uint64_t period_us);

/**
 * \brief Wait for the next safe tick
 *
 * Must be called by both cores, in a loop, before starting their cycle.
 */
flow_tick_result_t flow_safe_tick_wait (flow_safe_tick_t * tick, mu_node_t node);

/** \brief Wake everybody up and stop the tick (shutdown) */
void flow_safe_tick_stop (flow_safe_tick_t * tick);

/** \brief Re-arm the next deadline `period_us` from now (start of the run) */
void flow_safe_tick_rearm (flow_safe_tick_t * tick);

#endif /* SAFE_TICK_H */
