/*
 * flow_crc.h
 *
 * Software CRC helpers.
 *
 * - CRC-32/ISO-HDLC (Ethernet polynomial) protects the MU cross-check
 *   messages.  Those messages are safety decisions and are not covered by the
 *   FSoE end-to-end CRC.
 * - CRC-16/CCITT-FALSE protects only the mailbox slot header.  The FSoE PDU
 *   payload is intentionally not covered again: it is already protected by
 *   FSoE across the black channel.
 */

#ifndef FLOW_CRC_H
#define FLOW_CRC_H

#include "flow_types.h"

flow_u32 flow_crc32 (const void * data, flow_u32 size);
flow_u16 flow_crc16 (const void * data, flow_u32 size);

#endif /* FLOW_CRC_H */
