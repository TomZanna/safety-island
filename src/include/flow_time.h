/*
 * flow_time.h
 *
 * Monotonic time helpers.
 *
 * The safe cores are driven by absolute deadlines on CLOCK_MONOTONIC, which
 * is exactly what the real target does with its hardware safe timer tick.
 * There is no hardware CRC in this prototype, so time is only used for
 * scheduling and for the watchdog inside the FSoE stack (which goes through
 * the PAL function fsoeport_current_time_us()).
 */

#ifndef FLOW_TIME_H
#define FLOW_TIME_H

#include <stdint.h>
#include <time.h>

/* Current monotonic time, in microseconds, since an arbitrary epoch. */
uint64_t flow_now_us (void);

/* Current monotonic time, in microseconds, since the first call of
 * flow_now_us(). Used by the logger. */
uint64_t flow_elapsed_us (void);

/* Absolute deadline: flow_now_us() style timestamp `duration_us` from now. */
uint64_t flow_deadline_after_us (uint64_t duration_us);

/* Sleep until the absolute monotonic deadline (clock_nanosleep TIMER_ABSTIME).
 * Returns 0 on success, an errno value on failure. */
int flow_sleep_until_us (uint64_t deadline_us);

/* Sleep for the given duration (used by polling loops). */
void flow_sleep_us (uint64_t duration_us);

/* Same deadline expressed as a struct timespec, for pthread_cond_timedwait(). */
void flow_timespec_after_us (struct timespec * ts, uint64_t duration_us);

#endif /* FLOW_TIME_H */
