/*
 * main.c
 *
 * Bare metal entry point of a safety core.  The external safe timer is the
 * only thing that advances the cycle; no RTOS, no heap, no libc.
 */

#include "flow_config.h"
#include "flow_platform.h"
#include "flow_safe.h"
#include "flow_shm.h"

#if (FLOW_CORE_ID != FLOW_CORE_M7) && (FLOW_CORE_ID != FLOW_CORE_M33)
#error "FLOW_CORE_ID must be defined to FLOW_CORE_M7 or FLOW_CORE_M33"
#endif

static flow_safe_t safe_core;

int main (void)
{
   flow_shm_t * shm = (flow_shm_t *)FLOW_SHM_BASE_ADDR;

   flow_platform_set_core ((flow_core_id_t)FLOW_CORE_ID);
   flow_platform_init ();

#if FLOW_CORE_ID == FLOW_CORE_M7
   /* M7 is the owner of the SHMEM initialisation.  TODO(target): make this
    * robust against the actual M7/M33 boot order (hardware semaphore or a
    * documented boot sequence). */
   flow_shm_init (shm, (flow_core_id_t)FLOW_CORE_ID);
#else
   if (!flow_shm_validate (shm))
   {
      /* TODO(target): wait for M7 to publish the mailbox header instead of
       * failing immediately. */
      flow_platform_fatal ();
   }
#endif

   flow_safe_init (&safe_core, (flow_core_id_t)FLOW_CORE_ID, shm);

   for (;;)
   {
      if (flow_platform_tick_wait ())
      {
         flow_safe_step (&safe_core);
         flow_platform_watchdog_kick ();
      }
   }
}
