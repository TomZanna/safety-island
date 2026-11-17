/**
 * @file flow_mem.c
 * @brief Helper di memoria del sottosistema flow.
 * @ingroup flow
 *
 */
#include "flow_mem.h"

#include "flow_config.h"

void* flow_memcpy(void* destination, const void* source, flow_u32 size)
{
  flow_u8* to = (flow_u8*)destination;
  const flow_u8* from = (const flow_u8*)source;
  flow_u32 i;

  for (i = 0u; i < size; i++) {
    to[i] = from[i];
  }

  return destination;
}

void* flow_memset(void* destination, flow_u8 value, flow_u32 size)
{
  flow_u8* to = (flow_u8*)destination;
  flow_u32 i;

  for (i = 0u; i < size; i++) {
    to[i] = value;
  }

  return destination;
}

int flow_memcmp(const void* left, const void* right, flow_u32 size)
{
  const flow_u8* a = (const flow_u8*)left;
  const flow_u8* b = (const flow_u8*)right;
  flow_u32 i;

  for (i = 0u; i < size; i++) {
    if (a[i] != b[i]) {
      return (a[i] < b[i]) ? -1 : 1;
    }
  }

  return 0;
}

#if FLOW_FREESTANDING
/* Solo se il build non linka una libc (le app linkano newlib-nano). */
void* memcpy(void* destination, const void* source, size_t size)
{
  return flow_memcpy(destination, source, (flow_u32)size);
}

void* memset(void* destination, int value, size_t size)
{
  return flow_memset(destination, (flow_u8)value, (flow_u32)size);
}

int memcmp(const void* left, const void* right, size_t size)
{
  return flow_memcmp(left, right, (flow_u32)size);
}
#endif
