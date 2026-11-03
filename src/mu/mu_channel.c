/*
 * mu_channel.c
 *
 * See mu_channel.h
 */

#include "mu_channel.h"
#include "flow_digest.h"
#include "flow_log.h"
#include "flow_time.h"

#include <errno.h>
#include <string.h>

/* ------------------------------------------------------------------- names */

const char * mu_node_name (mu_node_t node)
{
   switch (node)
   {
   case MU_NODE_M7:  return "M7";
   case MU_NODE_M33: return "M33";
   default:          return "?";
   }
}

const char * mu_chan_name (mu_chan_t chan)
{
   switch (chan)
   {
   case MU_CHAN_SESSION_ID:  return "SESSION_ID";
   case MU_CHAN_RX_SNAPSHOT: return "RX_SNAPSHOT";
   case MU_CHAN_EGRESS_PDU:  return "EGRESS_PDU";
   case MU_CHAN_SAFE_INPUTS: return "SAFE_INPUTS";
   case MU_CHAN_SAFE_LOGIC:  return "SAFE_LOGIC";
   default:                  return "?";
   }
}

mu_node_t mu_node_peer (mu_node_t node)
{
   return (node == MU_NODE_M7) ? MU_NODE_M33 : MU_NODE_M7;
}

/* ----------------------------------------------------------------- helpers */

/* The digest covers the whole message, header included, with the digest
 * field itself zeroed. */
static uint32_t mu_message_digest (const mu_message_t * message)
{
   mu_message_t copy = *message;

   copy.digest = 0u;

   return flow_digest (&copy, sizeof (copy));
}

static void mu_message_seal (mu_message_t * message)
{
   message->digest = mu_message_digest (message);
}

/* Has the runtime requested the shutdown? In that case a missing peer message
 * is not a cross-check failure. */
static bool mu_is_aborted (const mu_bus_t * bus)
{
   return (bus->abort != NULL) &&
      atomic_load_explicit (bus->abort, memory_order_relaxed);
}

bool mu_message_verify (
   const mu_message_t * message,
   mu_node_t self,
   mu_chan_t chan)
{
   mu_message_t copy = *message;

   if (copy.digest != mu_message_digest (message))
   {
      return false;
   }
   if ((copy.dst != (uint16_t)self) || (copy.chan_id != (uint16_t)chan))
   {
      return false;
   }
   if (copy.len > MU_PAYLOAD_MAX)
   {
      return false;
   }

   return true;
}

/* --------------------------------------------------------------- lifecycle */

void mu_bus_init (mu_bus_t * bus)
{
   uint32_t c;
   uint32_t n;
   pthread_condattr_t attr;

   pthread_condattr_init (&attr);
   pthread_condattr_setclock (&attr, CLOCK_MONOTONIC);

   bus->abort = NULL;

   for (c = 0; c < (uint32_t)MU_CHAN_COUNT; c++)
   {
      mu_channel_t * ch = &bus->chan[c];

      pthread_mutex_init (&ch->lock, NULL);
      pthread_cond_init (&ch->cv, &attr);
      for (n = 0; n < (uint32_t)MU_NODE_COUNT; n++)
      {
         uint32_t t;

         for (t = 0; t < (uint32_t)MU_TAG_SLOTS; t++)
         {
            memset (&ch->reg[n][t], 0, sizeof (ch->reg[n][t]));
            ch->valid[n][t] = false;
         }
         ch->next_seq[n] = 0u;
      }
      memset (&ch->stats, 0, sizeof (ch->stats));
   }

   pthread_condattr_destroy (&attr);

   LOG_INFO (FLOW_TAG_MU,
      "messaging unit ready: %u channels, %u nodes",
      (unsigned)MU_CHAN_COUNT,
      (unsigned)MU_NODE_COUNT);
}

void mu_bus_close (mu_bus_t * bus)
{
   uint32_t c;

   for (c = 0; c < (uint32_t)MU_CHAN_COUNT; c++)
   {
      pthread_mutex_destroy (&bus->chan[c].lock);
      pthread_cond_destroy (&bus->chan[c].cv);
   }
}

void mu_bus_set_abort (mu_bus_t * bus, atomic_bool * abort)
{
   bus->abort = abort;
}

/* -------------------------------------------------------------- messaging */

bool mu_publish (
   mu_bus_t * bus,
   mu_node_t self,
   mu_chan_t chan,
   uint32_t cycle,
   uint32_t tag,
   const void * payload,
   uint32_t len)
{
   mu_channel_t * ch;
   mu_message_t * message;

   if ((bus == NULL) || (chan >= MU_CHAN_COUNT) ||
       (self >= MU_NODE_COUNT) || (len > MU_PAYLOAD_MAX) ||
       (tag >= MU_TAG_SLOTS) ||
       ((payload == NULL) && (len > 0u)))
   {
      return false;
   }

   ch = &bus->chan[chan];

   pthread_mutex_lock (&ch->lock);

   message = &ch->reg[self][tag];
   memset (message, 0, sizeof (*message));
   message->chan_id = (uint16_t)chan;
   message->src = (uint16_t)self;
   message->dst = (uint16_t)mu_node_peer (self);
   message->seq = ch->next_seq[self];
   message->cycle = cycle;
   message->tag = tag;
   message->len = len;
   if (len > 0u)
   {
      memcpy (message->payload, payload, len);
   }
   mu_message_seal (message);

   ch->next_seq[self] = (uint16_t)(ch->next_seq[self] + 1u);
   ch->valid[self][tag] = true;
   ch->stats.sent++;

   pthread_cond_broadcast (&ch->cv);
   pthread_mutex_unlock (&ch->lock);

   LOG_TRACE (FLOW_TAG_MU,
      "%s -> %s %s cycle=%u tag=%u seq=%u len=%u digest=0x%08x",
      mu_node_name (self),
      mu_node_name (mu_node_peer (self)),
      mu_chan_name (chan),
      cycle,
      tag,
      (unsigned)message->seq,
      len,
      message->digest);

   return true;
}

bool mu_wait (
   mu_bus_t * bus,
   mu_node_t self,
   mu_chan_t chan,
   uint32_t cycle,
   uint32_t tag,
   mu_message_t * message,
   uint32_t timeout_us)
{
   mu_channel_t * ch;
   mu_node_t peer;
   struct timespec deadline;
   bool found = false;
   bool corrupted = false;

   if ((bus == NULL) || (chan >= MU_CHAN_COUNT) || (self >= MU_NODE_COUNT) ||
       (tag >= MU_TAG_SLOTS) || (message == NULL))
   {
      return false;
   }

   ch = &bus->chan[chan];
   peer = mu_node_peer (self);
   flow_timespec_after_us (&deadline, timeout_us);

   pthread_mutex_lock (&ch->lock);

   for (;;)
   {
      const mu_message_t * candidate = &ch->reg[peer][tag];

      if (ch->valid[peer][tag] &&
          (candidate->cycle == cycle) &&
          (candidate->tag == tag))
      {
         if (candidate->digest != mu_message_digest (candidate))
         {
            ch->stats.digest_errors++;
            corrupted = true;
         }
         else if ((candidate->dst != (uint16_t)self) ||
                  (candidate->chan_id != (uint16_t)chan) ||
                  (candidate->src != (uint16_t)peer))
         {
            ch->stats.routing_errors++;
         }
         else
         {
            *message = *candidate;
            ch->stats.delivered++;
            found = true;
         }
         break;
      }

      if (pthread_cond_timedwait (&ch->cv, &ch->lock, &deadline) != 0)
      {
         break; /* timeout */
      }
   }

   if (!found && !corrupted && !mu_is_aborted (bus))
   {
      ch->stats.timeouts++;
   }

   pthread_mutex_unlock (&ch->lock);

   if (corrupted)
   {
      LOG_ERROR (FLOW_TAG_MU,
         "%s REJECTED corrupted %s message (digest mismatch) cycle=%u tag=%u",
         mu_node_name (self),
         mu_chan_name (chan),
         cycle,
         tag);
   }
   else if (!found && !mu_is_aborted (bus))
   {
      LOG_WARN (FLOW_TAG_MU,
         "%s timeout (%u us) waiting for %s from %s cycle=%u tag=%u",
         mu_node_name (self),
         timeout_us,
         mu_chan_name (chan),
         mu_node_name (peer),
         cycle,
         tag);
   }

   return found;
}

bool mu_exchange (
   mu_bus_t * bus,
   mu_node_t self,
   mu_chan_t chan,
   uint32_t cycle,
   uint32_t tag,
   const void * payload,
   uint32_t len,
   mu_message_t * peer,
   uint32_t timeout_us)
{
   if (!mu_publish (bus, self, chan, cycle, tag, payload, len))
   {
      return false;
   }

   return mu_wait (bus, self, chan, cycle, tag, peer, timeout_us);
}

void mu_bus_stats (const mu_bus_t * bus, mu_stats_t * total)
{
   uint32_t c;

   memset (total, 0, sizeof (*total));

   for (c = 0; c < (uint32_t)MU_CHAN_COUNT; c++)
   {
      mu_channel_t * ch = (mu_channel_t *)&bus->chan[c];

      pthread_mutex_lock (&ch->lock);
      total->sent += ch->stats.sent;
      total->delivered += ch->stats.delivered;
      total->timeouts += ch->stats.timeouts;
      total->digest_errors += ch->stats.digest_errors;
      total->routing_errors += ch->stats.routing_errors;
      pthread_mutex_unlock (&ch->lock);
   }
}
