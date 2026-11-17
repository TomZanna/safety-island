/*
 * CRC software del sottosistema flow.
 *
 * - CRC-32/ISO-HDLC (polinomio Ethernet) protegge i messaggi di cross-check 1oo2
 *   e il CRC-32 portato dal doorbell MU18. Quei messaggi sono decisioni di
 *   sicurezza e non sono coperti dal CRC end-to-end FSoE.
 * - CRC-16/CCITT-FALSE protegge solo l'header dello slot della mailbox. Il
 *   payload FSoE non viene ricalcolato: e' gia' protetto da FSoE.
 */
#ifndef SI_FLOW_CRC_H
#define SI_FLOW_CRC_H

#include "flow_types.h"

flow_u32 flow_crc32(const void *data, flow_u32 size);
flow_u16 flow_crc16(const void *data, flow_u32 size);

#endif /* SI_FLOW_CRC_H */
