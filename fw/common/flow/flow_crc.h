/**
 * @file flow_crc.h
 * @brief CRC software del sottosistema flow.
 * @ingroup flow
 *
 * - CRC-32/ISO-HDLC (polinomio Ethernet) protegge i messaggi di cross-check
 * 1oo2 e il CRC-32 portato dal doorbell MU18. Quei messaggi sono decisioni di
 *   sicurezza e non sono coperti dal CRC end-to-end FSoE.
 * - CRC-16/CCITT-FALSE protegge solo l'header dello slot della mailbox. Il
 *   payload FSoE non viene ricalcolato: e' gia' protetto da FSoE.
 */
#ifndef SI_FLOW_CRC_H
#define SI_FLOW_CRC_H

#include "flow_types.h"

/** @brief CRC-32/ISO-HDLC (polinomio Ethernet) su un buffer.
 *
 * @param[in] data puntatore ai byte da elaborare;
 * @param[in] size numero di byte.
 * @return CRC-32 del buffer.
 */
flow_u32 flow_crc32(const void* data, flow_u32 size);

/** @brief CRC-16/CCITT-FALSE su un buffer.
 *
 * @param[in] data puntatore ai byte da elaborare;
 * @param[in] size numero di byte.
 * @return CRC-16 del buffer.
 */
flow_u16 flow_crc16(const void* data, flow_u32 size);

#endif /* SI_FLOW_CRC_H */
