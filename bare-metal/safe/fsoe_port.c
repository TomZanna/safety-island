/*
 * fsoe_port.c
 *
 * Platform abstraction layer required by the rt-labs FSoE stack.
 *
 * The memory helpers are our freestanding versions, the time source comes
 * from the platform (hardware counter on the target) and an assertion failure
 * is a fatal safety condition, not a printf/debug break.
 */

#include "flow_mem.h"
#include "flow_platform.h"

#include "fsoeport.h"

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

void fsoeport_log (uint8_t type, const char * format, ...)
{
   /* FSoE internal logging is disabled by default (FSOE_LOG_ENABLE is not
    * defined in options.h).  Keeping the symbol makes the library linkable
    * also when somebody enables it; on the safety target it is a no-op. */
   (void)type;
   (void)format;
}

uint32_t fsoeport_current_time_us (void)
{
   return (uint32_t)flow_platform_time_us ();
}

void fsoeport_memcpy (void * destination, const void * source, size_t size)
{
   (void)flow_memcpy (destination, source, (flow_u32)size);
}

int32_t fsoeport_memcmp (
   const void * left,
   const void * right,
   size_t size)
{
   return (int32_t)flow_memcmp (left, right, (flow_u32)size);
}

void fsoeport_memset (void * destination, uint8_t value, size_t size)
{
   (void)flow_memset (destination, (flow_u8)value, (flow_u32)size);
}
