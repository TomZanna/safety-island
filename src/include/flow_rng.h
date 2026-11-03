/*
 * flow_rng.h
 *
 * Deterministic pseudo random generator (xorshift32).
 *
 * Each safe core owns its own generator: the FSoE Session ID is the sum of
 * the two independent random numbers (see session id negotiation in
 * safe/safe_core.c). A fixed seed makes the whole simulation reproducible.
 */

#ifndef FLOW_RNG_H
#define FLOW_RNG_H

#include <stdint.h>

typedef struct flow_rng
{
   uint32_t state;
} flow_rng_t;

static inline void flow_rng_seed (flow_rng_t * rng, uint32_t seed)
{
   rng->state = (seed == 0u) ? 0xDEADBEEFu : seed;
}

static inline uint32_t flow_rng_next (flow_rng_t * rng)
{
   uint32_t x = rng->state;

   x ^= x << 13;
   x ^= x >> 17;
   x ^= x << 5;
   rng->state = (x == 0u) ? 0xDEADBEEFu : x;

   return rng->state;
}

static inline uint16_t flow_rng_next_u16 (flow_rng_t * rng)
{
   return (uint16_t)(flow_rng_next (rng) >> 16);
}

#endif /* FLOW_RNG_H */
