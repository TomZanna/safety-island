/*
 * flow_types.h
 *
 * Fixed width types and compiler helpers used by both the bare metal safety
 * firmware and the Linux A55 driver.  The driver is built with __KERNEL__
 * defined, the safety core without it.
 *
 * Rules for everything that is shared through the mailbox:
 *   - only flow_u8/flow_u16/flow_u32
 *   - no size_t, no pointers, no enums, no bool, no bit fields
 *   - structures are packed and their offsets are checked at compile time.
 */

#ifndef FLOW_TYPES_H
#define FLOW_TYPES_H

#ifdef __KERNEL__

#include <linux/stddef.h>
#include <linux/types.h>

typedef u8 flow_u8;
typedef u16 flow_u16;
typedef u32 flow_u32;
typedef s32 flow_s32;

#else /* __KERNEL__ */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t flow_u8;
typedef uint16_t flow_u16;
typedef uint32_t flow_u32;
typedef int32_t flow_s32;

#endif /* __KERNEL__ */

#define FLOW_PACKED __attribute__ ((packed))
#define FLOW_NORETURN __attribute__ ((noreturn))
#define FLOW_UNUSED __attribute__ ((unused))
#define FLOW_INLINE static inline

#define FLOW_STATIC_ASSERT(condition, message) \
   _Static_assert (condition, message)

/* Core identity.  Kept as macros so that flow_config.h remains usable from
 * both worlds without pulling in <stdbool.h>/<linux/types.h>. */
#define FLOW_CORE_M7 0u
#define FLOW_CORE_M33 1u
#define FLOW_CORE_COUNT 2u

typedef flow_u8 flow_core_id_t;

#endif /* FLOW_TYPES_H */
