/*
 * flow_mem.h
 *
 * Small freestanding memory helpers.  They are used instead of the C library
 * so the safety build does not depend on libc.  In the Linux driver they are
 * normal kernel-safe functions.
 */

#ifndef FLOW_MEM_H
#define FLOW_MEM_H

#include "flow_types.h"

void * flow_memcpy (void * destination, const void * source, flow_u32 size);
void * flow_memset (void * destination, flow_u8 value, flow_u32 size);
int flow_memcmp (const void * left, const void * right, flow_u32 size);

#endif /* FLOW_MEM_H */
