/*
 * flow_digest.h
 *
 * Software integrity check used everywhere the real target would use a
 * hardware CRC (which, by design decision, is not emulated here).
 *
 * - protects MU messages in their entirety (header + payload)
 * - is the cross-check quantity exchanged between M7 and M33
 * - protects the SHMEM slots together with the seqlock
 *
 * FNV-1a, 32 bit: tiny, deterministic and dependency free.
 */

#ifndef FLOW_DIGEST_H
#define FLOW_DIGEST_H

#include <stddef.h>
#include <stdint.h>

#define FLOW_DIGEST_INIT 0x811C9DC5u
#define FLOW_DIGEST_PRIME 0x01000193u

static inline uint32_t flow_digest_update (
   uint32_t digest,
   const void * data,
   size_t size)
{
   const uint8_t * bytes = (const uint8_t *)data;
   size_t i;

   for (i = 0; i < size; i++)
   {
      digest ^= (uint32_t)bytes[i];
      digest *= FLOW_DIGEST_PRIME;
   }

   return digest;
}

static inline uint32_t flow_digest (const void * data, size_t size)
{
   return flow_digest_update (FLOW_DIGEST_INIT, data, size);
}

#endif /* FLOW_DIGEST_H */
