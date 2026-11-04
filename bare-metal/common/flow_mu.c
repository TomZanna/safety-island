/*
 * flow_mu.c
 *
 * See flow_mu.h.  The transport is provided by the platform; this file owns
 * the safety-relevant message format and its validation.
 */

#include "flow_mu.h"
#include "flow_crc.h"
#include "flow_mem.h"

#define MU_SEQUENCE_HALF 0x8000u

static flow_u32 mu_message_crc (const flow_mu_message_t * message)
{
   flow_mu_message_t copy = *message;

   copy.crc = 0u;

   return flow_crc32 (&copy, (flow_u32)sizeof (copy));
}

void flow_mu_init (flow_mu_state_t * state, flow_core_id_t self)
{
   if (state == NULL)
   {
      return;
   }

   flow_memset (state, 0, (flow_u32)sizeof (*state));
   state->self = self;
}

bool flow_mu_verify (
   const flow_mu_message_t * message,
   flow_core_id_t self,
   flow_u8 channel,
   flow_u8 tag,
   flow_u32 cycle)
{
   flow_core_id_t peer;

   if (message == NULL)
   {
      return false;
   }

   peer = (self == FLOW_CORE_M7) ? FLOW_CORE_M33 : FLOW_CORE_M7;

   if (message->magic != FLOW_MU_MAGIC)
   {
      return false;
   }
   if (message->version != FLOW_MU_VERSION)
   {
      return false;
   }
   if (message->crc != mu_message_crc (message))
   {
      return false;
   }
   if ((message->source != peer) ||
       (message->destination != self) ||
       (message->channel != channel) ||
       (message->tag != tag) ||
       (message->cycle != cycle))
   {
      return false;
   }
   if (message->length > FLOW_MU_PAYLOAD_MAX)
   {
      return false;
   }

   return true;
}

bool flow_mu_exchange (
   flow_mu_state_t * state,
   flow_u8 channel,
   flow_u8 tag,
   flow_u32 cycle,
   const void * payload,
   flow_u16 length,
   flow_mu_message_t * peer,
   flow_u32 timeout_us)
{
   flow_mu_message_t message;
   flow_mu_message_t received;
   flow_u16 difference;

   if ((state == NULL) || (peer == NULL) ||
       (channel >= FLOW_MU_CHAN_COUNT) || (tag >= FLOW_MU_TAG_MAX) ||
       (length > FLOW_MU_PAYLOAD_MAX) ||
       ((payload == NULL) && (length != 0u)))
   {
      return false;
   }

   flow_memset (&message, 0, (flow_u32)sizeof (message));

   message.magic = FLOW_MU_MAGIC;
   message.version = FLOW_MU_VERSION;
   message.channel = channel;
   message.source = state->self;
   message.destination = (state->self == FLOW_CORE_M7) ?
      FLOW_CORE_M33 : FLOW_CORE_M7;
   message.tag = tag;
   message.cycle = cycle;
   message.sequence = ++state->tx_sequence[channel][tag];
   message.length = length;

   if (length != 0u)
   {
      flow_memcpy (message.payload, payload, length);
   }

   message.crc = mu_message_crc (&message);

   if (!flow_platform_mu_post (
          state->self, channel, tag, &message, (flow_u32)sizeof (message)))
   {
      return false;
   }

   if (!flow_platform_mu_wait (
          state->self, channel, tag, cycle, &received,
          (flow_u32)sizeof (received), timeout_us))
   {
      return false;
   }

   if (!flow_mu_verify (&received, state->self, channel, tag, cycle))
   {
      return false;
   }

   /* Sequence number independent from FSoE: must be newer than the last one
    * seen on this channel/tag.  The check is wrap-aware. */
   difference = (flow_u16)(received.sequence -
      state->rx_sequence[channel][tag]);

   if ((difference == 0u) || (difference >= MU_SEQUENCE_HALF))
   {
      return false;
   }

   state->rx_sequence[channel][tag] = received.sequence;
   *peer = received;

   return true;
}
