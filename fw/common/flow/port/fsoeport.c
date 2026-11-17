/*
 * Port della libreria FSoE di rt-labs per le app dei core secondari.
 *
 * - tempo: flow_platform_time_us (DWT, indipendente dal tick);
 * - memcpy/memcmp/memset: la libc di newlib-nano (le app non sono freestanding);
 * - log: FSOE_LOG_ENABLE e' spento in options.h, quindi no-op (il simbolo serve
 *   solo al link se qualcuno lo accende).
 */
#include "fsoeport.h"

#include <stdarg.h>
#include <string.h>

#include "flow_platform.h"

void fsoeport_log(uint8_t type, const char *fmt, ...)
{
    (void)type;
    (void)fmt;
}

uint32_t fsoeport_current_time_us(void)
{
    return (uint32_t)flow_platform_time_us();
}

void fsoeport_memcpy(void *dest, const void *src, size_t num)
{
    (void)memcpy(dest, src, num);
}

int32_t fsoeport_memcmp(const void *ptr1, const void *ptr2, size_t num)
{
    return (int32_t)memcmp(ptr1, ptr2, num);
}

void fsoeport_memset(void *ptr, uint8_t value, size_t num)
{
    (void)memset(ptr, value, num);
}
