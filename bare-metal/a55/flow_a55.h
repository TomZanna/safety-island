/*
 * flow_a55.h
 *
 * Portable A55 side of the shared memory mailbox.  This code is used by the
 * Linux kernel driver and by the host test; it does not contain any kernel or
 * POSIX dependency.
 *
 * The A55 does not make safety decisions.  It reads the last frame that both
 * safe cores published, merges the two halves and hands the PDU to the
 * fieldbus stack.  A structurally invalid slot is reported, never repaired.
 */

#ifndef FLOW_A55_H
#define FLOW_A55_H

#include "flow_config.h"
#include "flow_shm.h"
#include "flow_types.h"

typedef enum flow_a55_result
{
   FLOW_A55_OK = 0,
   FLOW_A55_NO_DATA,
   FLOW_A55_INVALID
} flow_a55_result_t;

typedef struct flow_a55_frame
{
   flow_u32 conn;
   flow_u32 seq;
   flow_u32 fc;
   flow_u32 len;
   flow_u8 pdu[FLOW_PDU_MAX];
} flow_a55_frame_t;

typedef struct flow_a55
{
   flow_shm_t * shm;
   flow_u32 last_delivered[FLOW_NUM_CONNECTIONS];
   flow_u32 invalid_frames;
} flow_a55_t;

void flow_a55_init (flow_a55_t * a55, flow_shm_t * shm);

/* Read the next complete frame assembled from the two cores. */
flow_a55_result_t flow_a55_get_tx (
   flow_a55_t * a55,
   flow_u32 connection,
   flow_a55_frame_t * frame);

/* Publish a frame received from the fieldbus into the RX mailbox. */
flow_a55_result_t flow_a55_put_rx (
   flow_a55_t * a55,
   flow_u32 connection,
   const flow_u8 * pdu,
   flow_u32 len);

#endif /* FLOW_A55_H */
