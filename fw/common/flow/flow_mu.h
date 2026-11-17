/**
 * @file flow_mu.h
 * @brief Messaggi di cross-check fra i due core safe (1oo2).
 * @ingroup flow_mu
 *
 * Non sono coperti dal CRC end-to-end FSoE: portano decisioni di sicurezza, quindi
 * hanno un proprio protocollo:
 *   - protocol id e versione;
 *   - canale, sorgente, destinazione e tag di connessione;
 *   - sequence number indipendente da quello FSoE;
 *   - CRC-32 su tutto il messaggio, header compreso.
 *
 * Portato da imx-flow/bare-metal/common/flow_mu.c; il peer e' l'altro core safe
 * (SAFE_A <-> SAFE_B). Il trasporto e' flow_icc.c.
 */
#ifndef SI_FLOW_MU_H
#define SI_FLOW_MU_H

#include "flow_config.h"
#include "flow_types.h"

#define FLOW_MU_MAGIC 0x4D553031u /**< "MU01" */
#define FLOW_MU_VERSION 1u
#define FLOW_MU_PAYLOAD_MAX 48u
#define FLOW_MU_MESSAGE_SIZE 72u
#define FLOW_MU_CHAN_COUNT 4u
#define FLOW_MU_TAG_MAX FLOW_NUM_CONNECTIONS

/** Canali logici. */
#define FLOW_MU_CHAN_SESSION 0u
#define FLOW_MU_CHAN_EGRESS 1u
#define FLOW_MU_CHAN_INPUTS 2u
#define FLOW_MU_CHAN_LOGIC 3u

/** @brief Messaggio di cross-check: header, routing, sequenza e CRC-32. */
typedef struct FLOW_PACKED flow_mu_message
{
    flow_u32 magic;
    flow_u16 version;
    flow_u8 channel;
    flow_u8 source;

    flow_u16 destination;
    flow_u16 tag;
    flow_u32 cycle;

    flow_u16 sequence;
    flow_u16 length;
    flow_u32 crc;

    flow_u8 payload[FLOW_MU_PAYLOAD_MAX];
} flow_mu_message_t;

FLOW_STATIC_ASSERT(sizeof(flow_mu_message_t) == FLOW_MU_MESSAGE_SIZE, "flow_mu_message_t must be 72 bytes");
FLOW_STATIC_ASSERT(FLOW_MU_PAYLOAD_MAX >= 32u, "MU payload must fit the cross-check messages");

/** @brief Stato delle sequenze TX/RX per canale e tag. */
typedef struct flow_mu_state
{
    flow_core_id_t self;
    flow_u16 tx_sequence[FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX];
    flow_u16 rx_sequence[FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX];
} flow_mu_state_t;

void flow_mu_init(flow_mu_state_t *state, flow_core_id_t self);

/**
 * Pubblica un messaggio e attende quello del peer. Il messaggio ricevuto viene
 * verificato (magic, versione, CRC, routing, lunghezza, sequence) prima di
 * essere restituito.
 */
bool flow_mu_exchange(flow_mu_state_t *state, flow_u8 channel, flow_u8 tag, flow_u32 cycle, const void *payload,
    flow_u16 length, flow_mu_message_t *peer, flow_u32 timeout_us);

bool flow_mu_verify(const flow_mu_message_t *message, flow_core_id_t self, flow_u8 channel, flow_u8 tag,
    flow_u32 cycle);

#endif /* SI_FLOW_MU_H */
