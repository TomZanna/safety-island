/*
 * flow_shm.h
 *
 * Accessors for the shared memory mailbox described in flow_shm_api.h.
 *
 * The PDU payload is not checksummed here: it is already protected end-to-end
 * by FSoE.  The seqlock protects against torn reads and the slot header CRC
 * protects the metadata used by the A55 to merge the two halves.
 */

#ifndef FLOW_SHM_H
#define FLOW_SHM_H

#include "flow_shm_api.h"
#include "flow_types.h"

/* Initialise the whole mailbox.  Only the owner core (M7) calls this. */
void flow_shm_init (flow_shm_t * shm, flow_core_id_t owner);

/* Validate magic, protocol id, version, layout hash and geometry. */
bool flow_shm_validate (const flow_shm_t * shm);

/* Number of frames published on a channel. */
flow_u32 flow_shm_count (const flow_shm_channel_t * channel);

/*
 * Write one slot.  `index` is 1-based; the bank is (index - 1) % banks.
 * `pdu` may contain only the contribution of this core (CRC half or data
 * half); the other bytes are expected to be zero.  Returns true if the copy
 * landed in the shared memory and the header CRC is valid.
 */
bool flow_shm_stage (
   flow_shm_channel_t * channel,
   flow_u32 index,
   flow_u32 len,
   flow_u32 fc,
   flow_u16 flags,
   const flow_u8 * pdu);

/* Publish a staged frame.  index may be 0 to re-align a channel downwards. */
void flow_shm_commit (flow_shm_channel_t * channel, flow_u32 index);

/* Read a slot through the seqlock.  Verifies the slot header CRC. */
bool flow_shm_read_index (
   const flow_shm_channel_t * channel,
   flow_u32 index,
   flow_shm_slot_t * slot);

/* Stage + commit in one call (single writer channels). */
bool flow_shm_publish (
   flow_shm_channel_t * channel,
   flow_u32 len,
   flow_u32 fc,
   flow_u16 flags,
   const flow_u8 * pdu,
   flow_u32 * index);

/* Heartbeat telemetry, written by the owner core every safe cycle. */
void flow_shm_update_heartbeat (
   flow_shm_t * shm,
   flow_core_id_t core,
   flow_u32 value);

/* Copy only the contribution of one core: crc_part true keeps the CRC bytes,
 * false keeps command + data + Connection ID. */
void flow_shm_split (
   flow_u8 * destination,
   const flow_u8 * frame,
   flow_u32 len,
   bool crc_part);

/* Merge the two contributions into a complete FSoE frame. */
void flow_shm_assemble (
   flow_u8 * destination,
   const flow_u8 * crc_part,
   const flow_u8 * data_part,
   flow_u32 len);

/* Verify that the bytes contributed by this core are intact. */
bool flow_shm_part_matches (
   const flow_u8 * stored,
   const flow_u8 * frame,
   flow_u32 len,
   bool crc_part);

#endif /* FLOW_SHM_H */
