/*
 * flow_platform_target.c
 *
 * Target skeleton for the i.MX943 safety cores.  The tick, the hardware
 * counter, the TRNG, the MU and the external watchdog are intentionally left
 * as explicit TODOs: they are board/SoC specific and must be implemented and
 * verified before this code can run on silicon.
 *
 * The file is compiled twice, once with FLOW_CORE_ID=0 (M7) and once with
 * FLOW_CORE_ID=1 (M33).
 */

#include "flow_platform.h"
#include "flow_config.h"

#if (FLOW_CORE_ID != FLOW_CORE_M7) && (FLOW_CORE_ID != FLOW_CORE_M33)
#error "FLOW_CORE_ID must be FLOW_CORE_M7 or FLOW_CORE_M33"
#endif

static volatile flow_u32 target_tick_count;

static void target_wfi (void)
{
#if defined(__arm__)
   __asm volatile ("wfi");
#endif
}

static void target_disable_interrupts (void)
{
#if defined(__arm__)
   __asm volatile ("cpsid i" : : : "memory");
#endif
}

void flow_platform_set_core (flow_core_id_t core)
{
   if (core != (flow_core_id_t)FLOW_CORE_ID)
   {
      flow_platform_fatal ();
   }
}

void flow_platform_init (void)
{
   /* TODO(target): clocks, MPU/TRDC, ECC, MU, TRNG health tests and external
    * watchdog configuration. */
}

bool flow_platform_tick_wait (void)
{
   const flow_u32 seen = target_tick_count;

   while (target_tick_count == seen)
   {
      target_wfi ();
   }

   return true;
}

void flow_platform_tick_isr (void)
{
   target_tick_count++;
}

flow_u32 flow_platform_time_us (void)
{
   /* TODO(target): return an independent hardware counter, in microseconds.
    * Do not derive the FSoE watchdog time from the safe tick: the two sources
    * must not have a common cause failure. */
   return target_tick_count * (FLOW_SAFE_CYCLE_MS * 1000u);
}

flow_u16 flow_platform_random_u16 (void)
{
   /* TODO(target): read the hardware TRNG and run the required health tests.
    * A missing or failing TRNG must bring the system fail-safe.  There is no
    * pseudo-random fallback in the safety build. */
   flow_platform_fatal ();
}

bool flow_platform_mu_post (
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   const void * message,
   flow_u32 size)
{
   (void)self;
   (void)channel;
   (void)tag;
   (void)message;
   (void)size;

   /* TODO(target): write the message into the MU/shared SRAM registers and
    * raise the doorbell.  Return false if the transport is not available. */
   return false;
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
   (void)self;
   (void)channel;
   (void)tag;
   (void)cycle;
   (void)message;
   (void)max_size;
   (void)timeout_us;

   /* TODO(target): poll the peer MU registers with the deadline derived from
    * flow_platform_time_us().  Bounded wait, never an infinite loop. */
   return false;
}

void flow_platform_watchdog_kick (void)
{
   /* TODO(target): service the external hardware watchdog. */
}

void flow_platform_fatal (void)
{
   target_disable_interrupts ();

   for (;;)
   {
      /* Safe state: the outputs are already forced to 0 by the safe logic.
       * Wait for the external watchdog reset. */
   }
}

void flow_platform_shutdown (void)
{
   /* No shutdown on a bare metal target. */
}

#if FLOW_DEBUG
void flow_platform_log (const char * tag, const char * format, ...)
{
   (void)tag;
   (void)format;

   /* TODO(target): UART/semihosting hook, only in a debug build. */
}
#endif
