/*
 * flow_a55_uapi.h
 *
 * Stable ioctl ABI between the Linux A55 driver and the fieldbus process.
 *
 * The frame payload is an FSoE PDU, already protected end-to-end by FSoE.
 * The driver does not add or remove protection; it only validates lengths and
 * the mailbox metadata.
 */

#ifndef FLOW_A55_UAPI_H
#define FLOW_A55_UAPI_H

#include <linux/ioctl.h>
#include <linux/types.h>

#include "flow_config.h"

#define FLOW_A55_ABI_VERSION 1u
#define FLOW_A55_IOCTL_MAGIC 0xF5u
#define FLOW_A55_PDU_MAX FLOW_PDU_MAX

struct flow_a55_abi
{
   __u32 abi_version;
   __u32 protocol_id;
   __u32 layout_hash;
   __u32 conn_count;
   __u32 slot_size;
   __u32 pdu_max;
   __u32 reserved[2];
};

struct flow_a55_frame_u
{
   __u32 conn;
   __u32 seq;
   __u32 fc;
   __u32 len;
   __u32 flags;
   __u8 pdu[FLOW_A55_PDU_MAX];
   __u8 reserved[4];
};

struct flow_a55_status
{
   __u32 header_state;
   __u32 heartbeat[2];
   __u32 tx_count[2];
   __u32 rx_count;
   __u32 invalid_frames;
   __u32 reserved[3];
};

FLOW_STATIC_ASSERT (sizeof (struct flow_a55_abi) == 32u,
   "flow_a55_abi ABI size mismatch");
FLOW_STATIC_ASSERT (sizeof (struct flow_a55_frame_u) == 36u,
   "flow_a55_frame_u ABI size mismatch");
FLOW_STATIC_ASSERT (sizeof (struct flow_a55_status) == 40u,
   "flow_a55_status ABI size mismatch");

#define FLOW_A55_IOCTL_GET_ABI \
   _IOR (FLOW_A55_IOCTL_MAGIC, 0, struct flow_a55_abi)
#define FLOW_A55_IOCTL_GET_TX \
   _IOWR (FLOW_A55_IOCTL_MAGIC, 1, struct flow_a55_frame_u)
#define FLOW_A55_IOCTL_PUT_RX \
   _IOW (FLOW_A55_IOCTL_MAGIC, 2, struct flow_a55_frame_u)
#define FLOW_A55_IOCTL_GET_STATUS \
   _IOR (FLOW_A55_IOCTL_MAGIC, 3, struct flow_a55_status)

#endif /* FLOW_A55_UAPI_H */
