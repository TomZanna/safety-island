/*
 * flow_platform.h
 *
 * Everything the safety firmware needs from the outside world.  The target
 * implementation is responsible for the hardware; the host implementation is
 * used only by the validation test.
 *
 * The tick, the monotonic clock, the TRNG, the MU and the external watchdog
 * are deliberately behind this interface: they are the points where the real
 * i.MX943 platform has to be wired (see TODO markers in platform/target).
 */

#ifndef FLOW_PLATFORM_H
#define FLOW_PLATFORM_H

#include "flow_config.h"
#include "flow_types.h"

/* Set the identity of the core that is executing.  On the target it must
 * match FLOW_CORE_ID; on the host it is thread local. */
void flow_platform_set_core (flow_core_id_t core);

/* One-time platform initialisation (clocks, MPU, TRNG, MU, watchdog). */
void flow_platform_init (void);

/* Wait for the next external safe tick.  Returns false when the platform is
 * shutting down (host test). */
bool flow_platform_tick_wait (void);

/* Monotonic time in microseconds, used by the FSoE watchdog.  TODO(target):
 * independent hardware counter, not derived from the safe tick. */
flow_u32 flow_platform_time_us (void);

/* Hardware random number for the Session ID negotiation.  TODO(target): TRNG
 * with health tests.  A failure must bring the system fail-safe, never fall
 * back to a pseudo random generator. */
flow_u16 flow_platform_random_u16 (void);

/* Fatal condition: the safety build must stop the outputs and wait for the
 * external watchdog.  Never returns. */
void flow_platform_fatal (void) FLOW_NORETURN;

#if FLOW_DEBUG
void flow_platform_log (
   const char * tag,
   const char * format,
   ...) __attribute__ ((format (printf, 2, 3)));
#endif

/* Inter-core messaging unit transport.  The message content (sequence, CRC,
 * routing, length) is built and checked by common/flow_mu.c. */
bool flow_platform_mu_post (
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   const void * message,
   flow_u32 size);

bool flow_platform_mu_wait (
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   flow_u32 cycle,
   void * message,
   flow_u32 max_size,
   flow_u32 timeout_us);

/* Kick the external watchdog.  TODO(target): hardware watchdog service. */
void flow_platform_watchdog_kick (void);

/* Host test shutdown hook.  The target implementation is a no-op. */
void flow_platform_shutdown (void);

/* Target interrupt entry point: the external safe timer calls this.  TODO:
 * connect it to the real timer interrupt. */
void flow_platform_tick_isr (void);

#endif /* FLOW_PLATFORM_H */
