/*
 * flow_shm_api.h
 *
 * SINGLE SOURCE OF TRUTH of the shared memory mailbox.
 *
 * This header is included by the bare metal safety firmware (M7/M33) and by
 * the Linux A55 driver.  It defines the exact byte layout, protocol id and
 * version.  There is no serialization: both sides access the same shared
 * memory through these packed structures.
 *
 * The safety PDU is already protected end-to-end by FSoE (CRC, sequence
 * number, watchdog).  The CRC in the slot header protects only the mailbox
 * metadata (len, flags, frame counter), not the PDU payload.
 *
 * Layout rules (32-bit safety cores and 64-bit A55):
 *   - packed structures;
 *   - only flow_u8/flow_u16/flow_u32 fields;
 *   - 32-bit fields naturally aligned to avoid unaligned accesses;
 *   - offsets and sizes verified by FLOW_STATIC_ASSERT.
 */

#ifndef FLOW_SHM_API_H
#define FLOW_SHM_API_H

#include "flow_config.h"
#include "flow_types.h"

/* ------------------------------------------------------------- identity --- */

#define FLOW_SHM_MAGIC 0x464C4F57u        /* "FLOW" */
#define FLOW_SHM_PROTOCOL_ID 0x494D5839u  /* "IMX9" */
/* Increment on any interface/semantic change. */
#define FLOW_SHM_VERSION 1u
/* Increment when the byte layout changes; checked by the A55 driver. */
#define FLOW_SHM_LAYOUT_HASH 0x00000001u

/* Header state. */
#define FLOW_SHM_STATE_UNINIT 0u
#define FLOW_SHM_STATE_READY 1u
#define FLOW_SHM_STATE_FAULT 2u

/* Nodes that own a TX half. */
#define FLOW_SHM_NODE_M7 0u
#define FLOW_SHM_NODE_M33 1u
#define FLOW_SHM_NODE_COUNT 2u

/* Frame split: M7 contributes the CRC bytes, M33 the rest. */
#define FLOW_SHM_CRC_NODE FLOW_SHM_NODE_M7
#define FLOW_SHM_DATA_NODE FLOW_SHM_NODE_M33

/* Minimum FSoE PDU size (one data byte). */
#define FLOW_PDU_MIN_SIZE 6u

/* Padding that keeps every slot aligned to 4 bytes. */
#define FLOW_SHM_PDU_PAD ((4u - (FLOW_PDU_MAX % 4u)) % 4u)
#define FLOW_SHM_HEADER_SIZE 64u
#define FLOW_SHM_SLOT_SIZE (12u + FLOW_PDU_MAX + FLOW_SHM_PDU_PAD)
#define FLOW_SHM_CHANNEL_SIZE (16u + FLOW_SHM_BANKS * FLOW_SHM_SLOT_SIZE)

typedef struct FLOW_PACKED flow_shm_header
{
   flow_u32 magic;
   flow_u32 protocol_id;
   flow_u32 version;
   flow_u32 layout_hash;

   flow_u32 conn_count;
   flow_u32 banks;
   flow_u32 slot_size;
   flow_u32 header_size;

   flow_u32 state;
   flow_u32 heartbeat[FLOW_CORE_COUNT];
   flow_u32 reserved[5];
} flow_shm_header_t;

typedef struct FLOW_PACKED flow_shm_slot
{
   flow_u16 len;          /* number of valid PDU bytes                  */
   flow_u16 flags;        /* reserved for future use, must be 0          */
   flow_u32 fc;           /* black channel frame counter                 */
   flow_u16 hdr_crc;      /* CRC-16 over len, flags, fc, reserved        */
   flow_u16 reserved;     /* must be 0                                   */
   flow_u8 pdu[FLOW_PDU_MAX];
   flow_u8 pad[FLOW_SHM_PDU_PAD];
} flow_shm_slot_t;

typedef struct FLOW_PACKED flow_shm_channel
{
   flow_u32 count;                     /* published frames (monotonic)    */
   flow_u32 bank_seq[FLOW_SHM_BANKS];  /* seqlock: odd = writer active    */
   flow_shm_slot_t bank[FLOW_SHM_BANKS];
} flow_shm_channel_t;

typedef struct FLOW_PACKED flow_shm
{
   flow_shm_header_t header;

   /* TX: [connection][core] - each channel has a single writer (that core) */
   flow_shm_channel_t tx[FLOW_NUM_CONNECTIONS][FLOW_SHM_NODE_COUNT];

   /* RX: [connection] - single writer is the A55                            */
   flow_shm_channel_t rx[FLOW_NUM_CONNECTIONS];
} flow_shm_t;

#define FLOW_SHM_TOTAL_SIZE ((flow_u32)sizeof (flow_shm_t))
#define FLOW_SHM_TX_OFFSET ((flow_u32)offsetof (flow_shm_t, tx))
#define FLOW_SHM_RX_OFFSET ((flow_u32)offsetof (flow_shm_t, rx))

FLOW_STATIC_ASSERT (sizeof (flow_shm_header_t) == FLOW_SHM_HEADER_SIZE,
   "flow_shm_header_t size mismatch");
FLOW_STATIC_ASSERT (offsetof (flow_shm_header_t, protocol_id) == 4u,
   "protocol_id must be at offset 4");
FLOW_STATIC_ASSERT (offsetof (flow_shm_header_t, version) == 8u,
   "version must be at offset 8");
FLOW_STATIC_ASSERT (offsetof (flow_shm_header_t, layout_hash) == 12u,
   "layout_hash must be at offset 12");
FLOW_STATIC_ASSERT (sizeof (flow_shm_slot_t) == FLOW_SHM_SLOT_SIZE,
   "flow_shm_slot_t size mismatch");
FLOW_STATIC_ASSERT (sizeof (flow_shm_channel_t) == FLOW_SHM_CHANNEL_SIZE,
   "flow_shm_channel_t size mismatch");
FLOW_STATIC_ASSERT (offsetof (flow_shm_channel_t, bank_seq) == 4u,
   "bank_seq must be at offset 4");
FLOW_STATIC_ASSERT (offsetof (flow_shm_channel_t, bank) == 16u,
   "bank must be at offset 16");
FLOW_STATIC_ASSERT (offsetof (flow_shm_slot_t, pdu) == 12u,
   "PDU must start at offset 12");
FLOW_STATIC_ASSERT (offsetof (flow_shm_t, tx) == FLOW_SHM_HEADER_SIZE,
   "TX channels must follow the header");
FLOW_STATIC_ASSERT (offsetof (flow_shm_t, rx) ==
   (FLOW_SHM_HEADER_SIZE +
    FLOW_NUM_CONNECTIONS * 2u * FLOW_SHM_CHANNEL_SIZE),
   "RX channels must follow the TX channels");
FLOW_STATIC_ASSERT (sizeof (flow_shm_t) ==
   (FLOW_SHM_HEADER_SIZE +
    FLOW_NUM_CONNECTIONS * 2u * FLOW_SHM_CHANNEL_SIZE +
    FLOW_NUM_CONNECTIONS * FLOW_SHM_CHANNEL_SIZE),
   "unexpected flow_shm_t size");

/* ETG.5100 frame layout:
 *   len == 6 : [0]=cmd [1]=data [2..3]=CRC_0 [4..5]=ConnId
 *   len >  6 : CRC words at 3 + 4*j, ConnId in the last two bytes.
 */
FLOW_INLINE int flow_pdu_is_crc_byte (flow_u32 len, flow_u32 offset)
{
   if (len < 6u)
   {
      return 0;
   }

   if (len == 6u)
   {
      return (offset == 2u) || (offset == 3u);
   }

   if ((offset < 3u) || (offset >= (len - 2u)))
   {
      return 0;
   }

   return (((offset - 3u) % 4u) < 2u);
}

#endif /* FLOW_SHM_API_H */
