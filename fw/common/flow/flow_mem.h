/*
 * Helper di memoria del sottosistema flow.
 *
 * Il codice safe non dipende dalla libc: con FLOW_FREESTANDING=1 (default 0 per
 * queste app, che linkano newlib-nano) fornisce anche i simboli memcpy/memset/
 * memcmp che il compilatore puo' emettere.
 */
#ifndef SI_FLOW_MEM_H
#define SI_FLOW_MEM_H

#include "flow_types.h"

void *flow_memcpy(void *destination, const void *source, flow_u32 size);
void *flow_memset(void *destination, flow_u8 value, flow_u32 size);
int flow_memcmp(const void *left, const void *right, flow_u32 size);

#endif /* SI_FLOW_MEM_H */
