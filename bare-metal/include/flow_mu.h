/*
 * flow_mu.h
 *
 * Inter-core messaging unit protocol used by the 1oo2 cross-checks.
 *
 * These messages are not covered by the FSoE end-to-end CRC: they carry
 * safety decisions between M7 and M33, so they have their own:
 *   - protocol id and version;
 *   - channel id, source, destination and connection tag;
 *   - sequence number (independent from the FSoE sequence);
 *   - CRC-32 over the whole message, header included.
 */

#ifndef FLOW_MU_H
#define FLOW_MU_H

#include "flow_config.h"
#include "flow_platform.h"
#include "flow_types.h"

#define FLOW_MU_MAGIC 0x4D553031u /* "MU01" */
#define FLOW_MU_VERSION 1u
#define FLOW_MU_PAYLOAD_MAX 48u
#define FLOW_MU_CHAN_COUNT 4u
#define FLOW_MU_TAG_MAX FLOW_NUM_CONNECTIONS

/* Logical channels. */
#define FLOW_MU_CHAN_SESSION 0u
#define FLOW_MU_CHAN_EGRESS 1u
#define FLOW_MU_CHAN_INPUTS 2u
#define FLOW_MU_CHAN_LOGIC 3u

typedef struct FLOW_PACKED flow_mu_message
{
   flow_u32 magic;
   flow_u16 version;
   flow_u8 channel;
   flow_u8 source;

   flow_u16 destination;
   flow_u16 tag;
   flow_u32 cycle;

   flow_u16 sequence;
   flow_u16 length;
   flow_u32 crc;

   flow_u8 payload[FLOW_MU_PAYLOAD_MAX];
} flow_mu_message_t;

FLOW_STATIC_ASSERT (sizeof (flow_mu_message_t) == 72u,
   "flow_mu_message_t must be 72 bytes");
FLOW_STATIC_ASSERT (FLOW_MU_PAYLOAD_MAX >= 32u,
   "MU payload must fit the cross-check messages");

typedef struct flow_mu_state
{
   flow_core_id_t self;
   flow_u16 tx_sequence[FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX];
   flow_u16 rx_sequence[FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX];
} flow_mu_state_t;

void flow_mu_init (flow_mu_state_t * state, flow_core_id_t self);

/*
 * Publish a message and wait for the peer one.  The peer message is verified
 * (magic, version, CRC, routing, length, sequence) before it is returned.
 */
bool flow_mu_exchange (
   flow_mu_state_t * state,
   flow_u8 channel,
   flow_u8 tag,
   flow_u32 cycle,
   const void * payload,
   flow_u16 length,
   flow_mu_message_t * peer,
   flow_u32 timeout_us);

bool flow_mu_verify (
   const flow_mu_message_t * message,
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   flow_u32 cycle);

#endif /* FLOW_MU_H */
