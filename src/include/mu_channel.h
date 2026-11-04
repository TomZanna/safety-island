/*
 * mu_channel.h
 *
 * Emulation of the inter-core Messaging Unit (MU) of the i.MX943: the IPC
 * hardware that M7 and M33 use for their 1oo2 cross-checks.
 *
 * Each logical channel is a pair of single-entry registers (one per
 * direction). A message is protected in its entirety by a digest, carries its
 * own sequence number (independent from the FSoE sequence number) and carries
 * a channel id + source + destination so that messages of different
 * destinations/connections cannot be confused.
 *
 * On the target this would be spin-waiting on MU status registers; on the PC
 * it is a mutex + condition variable per channel, with a bounded timeout:
 * if the peer message does not arrive in time the cross-check fails and the
 * caller must go fail-safe. That keeps the system deadlock free even when one
 * core is late or stopped.
 */

#ifndef MU_CHANNEL_H
#define MU_CHANNEL_H

#include "flow_config.h"

#include <pthread.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum mu_node
{
   MU_NODE_M7 = 0,
   MU_NODE_M33 = 1,
   MU_NODE_COUNT = 2
} mu_node_t;

typedef enum mu_chan
{
   MU_CHAN_SESSION_ID = 0, /**< Session ID negotiation (M7 <-> M33)          */
   MU_CHAN_EGRESS_PDU,     /**< Digest of the FSoE PDU generated for k       */
   MU_CHAN_SAFE_INPUTS,    /**< SafeInputs + status + counters of k          */
   MU_CHAN_SAFE_LOGIC,     /**< Safe logic outputs + internal state          */
   MU_CHAN_COUNT
} mu_chan_t;

#define MU_PAYLOAD_MAX 64u

/**
 * \brief Number of sub-addresses (tags) per channel and per sender
 *
 * One tag per FSoE connection, so that the messages of different connections
 * never overwrite each other in the emulated registers.
 */
#define MU_TAG_SLOTS FLOW_NUM_CONNECTIONS

typedef struct mu_message
{
   uint16_t chan_id;              /**< Logical channel (mu_chan_t)           */
   uint16_t src;                  /**< Sender node                          */
   uint16_t dst;                  /**< Destination node                     */
   uint16_t seq;                  /**< MU sequence number (not FSoE)        */
   uint32_t cycle;                /**< Safe cycle the message refers to     */
   uint32_t tag;                  /**< Sub-address (FSoE connection index)  */
   uint32_t len;                  /**< Payload length in bytes              */
   uint32_t digest;               /**< Digest of the whole message          */
   uint8_t payload[MU_PAYLOAD_MAX];
} mu_message_t;

typedef struct mu_stats
{
   uint32_t sent;
   uint32_t delivered;
   uint32_t timeouts;
   uint32_t digest_errors;
   uint32_t routing_errors;
} mu_stats_t;

typedef struct mu_channel
{
   pthread_mutex_t lock;
   pthread_cond_t cv;
   mu_message_t reg[MU_NODE_COUNT][MU_TAG_SLOTS];
   bool valid[MU_NODE_COUNT][MU_TAG_SLOTS];
   uint16_t next_seq[MU_NODE_COUNT];
   mu_stats_t stats;
} mu_channel_t;

typedef struct mu_bus
{
   mu_channel_t chan[MU_CHAN_COUNT];
   atomic_bool * abort; /**< Runtime stop flag: silences shutdown timeouts  */
} mu_bus_t;

const char * mu_node_name (mu_node_t node);
const char * mu_chan_name (mu_chan_t chan);
mu_node_t mu_node_peer (mu_node_t node);

/* Frame split between the two cores (see the "Frame split / assembly" section
 * of fsoe_slot.h):
 *  - MU_CRC_NODE  contributes the CRC words of the assembled FSoE frame
 *  - MU_DATA_NODE contributes command + safety data + Connection ID
 * The A55 interleaves the two contributions. */
#define MU_CRC_NODE MU_NODE_M7
#define MU_DATA_NODE MU_NODE_M33

void mu_bus_init (mu_bus_t * bus);
void mu_bus_close (mu_bus_t * bus);

/**
 * \brief Provide the runtime stop flag
 *
 * When the flag is set a pending wait is not reported as a cross-check
 * failure: the system is shutting down, not diverging.
 */
void mu_bus_set_abort (mu_bus_t * bus, atomic_bool * abort);

/** \brief Put a message into the sender register and wake the peer */
bool mu_publish (
   mu_bus_t * bus,
   mu_node_t self,
   mu_chan_t chan,
   uint32_t cycle,
   uint32_t tag,
   const void * payload,
   uint32_t len);

/**
 * \brief Wait for the peer message of the given channel/cycle/tag
 *
 * The received message is verified (digest, source, destination, length)
 * before being handed to the caller.
 */
bool mu_wait (
   mu_bus_t * bus,
   mu_node_t self,
   mu_chan_t chan,
   uint32_t cycle,
   uint32_t tag,
   mu_message_t * message,
   uint32_t timeout_us);

/** \brief Publish + wait: the symmetric rendezvous used by both cores */
bool mu_exchange (
   mu_bus_t * bus,
   mu_node_t self,
   mu_chan_t chan,
   uint32_t cycle,
   uint32_t tag,
   const void * payload,
   uint32_t len,
   mu_message_t * peer,
   uint32_t timeout_us);

/** \brief Verify integrity and routing of a received message */
bool mu_message_verify (
   const mu_message_t * message,
   mu_node_t self,
   mu_chan_t chan);

void mu_bus_stats (const mu_bus_t * bus, mu_stats_t * total);

#endif /* MU_CHANNEL_H */
