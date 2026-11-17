/**
 * @file flow_mu.h
 * @brief Messaggi di cross-check fra i due core safe (1oo2).
 * @ingroup flow_mu
 *
 * Non sono coperti dal CRC end-to-end FSoE: portano decisioni di sicurezza,
 * quindi hanno un proprio protocollo:
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
#define FLOW_MU_VERSION 1u /**< versione del protocollo di cross-check */
#define FLOW_MU_PAYLOAD_MAX 48u /**< payload massimo (72 - 24 di header) */
#define FLOW_MU_MESSAGE_SIZE 72u /**< header 24 B + payload massimo */
#define FLOW_MU_CHAN_COUNT 4u /**< canali definiti qui sotto */
#define FLOW_MU_TAG_MAX FLOW_NUM_CONNECTIONS /**< un tag per connessione */

/** Canali logici: ognuno ha la sua sequence TX/RX. */
#define FLOW_MU_CHAN_SESSION 0u /**< negoziazione del Session ID */
#define FLOW_MU_CHAN_EGRESS 1u /**< PDU in uscita, prima di pubblicarlo */
#define FLOW_MU_CHAN_INPUTS 2u /**< SafeInputs e stato della connessione */
#define FLOW_MU_CHAN_LOGIC 3u /**< SafeOutputs e stato della logica */

/** @brief Messaggio di cross-check: header, routing, sequenza e CRC-32. */
typedef struct FLOW_PACKED flow_mu_message
{
  flow_u32 magic; /**< FLOW_MU_MAGIC */
  flow_u16 version; /**< FLOW_MU_VERSION */
  flow_u8 channel; /**< FLOW_MU_CHAN_* */
  flow_u8 source; /**< ruolo mittente */

  flow_u16 destination; /**< ruolo destinatario */
  flow_u16 tag; /**< tag della connessione */
  flow_u32 cycle; /**< ciclo a cui il messaggio appartiene */

  flow_u16 sequence; /**< indipendente da FSoE, per canale e tag */
  flow_u16 length; /**< byte validi del payload */
  flow_u32 crc; /**< CRC-32 di tutto il messaggio, questo campo a 0 */

  flow_u8 payload[FLOW_MU_PAYLOAD_MAX]; /**< decisioni da confrontare */
} flow_mu_message_t;

FLOW_STATIC_ASSERT(sizeof(flow_mu_message_t) == FLOW_MU_MESSAGE_SIZE,
                   "flow_mu_message_t must be 72 bytes");
FLOW_STATIC_ASSERT(FLOW_MU_PAYLOAD_MAX >= 32u,
                   "MU payload must fit the cross-check messages");

/** @brief Stato delle sequenze TX/RX per canale e tag. */
typedef struct flow_mu_state
{
  flow_core_id_t self; /**< ruolo mittente (SAFE_A o SAFE_B) */
  flow_u16 tx_sequence[FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX]; /**< inviate */
  flow_u16 rx_sequence[FLOW_MU_CHAN_COUNT][FLOW_MU_TAG_MAX]; /**< accettate */
} flow_mu_state_t;

/** @brief Inizializza lo stato delle sequenze e l'identita' del core.
 *
 * @param[out] state stato MU del core;
 * @param[in]  self  ruolo del core (SAFE_A o SAFE_B).
 */
void flow_mu_init(flow_mu_state_t* state, flow_core_id_t self);

/**
 * @brief Pubblica un messaggio e attende quello del peer.
 *
 * Il messaggio ricevuto viene verificato (magic, versione, CRC, routing,
 * lunghezza, sequence) prima di essere restituito. La sequence e' indipendente
 * da quella FSoE e il confronto e' wrap-aware.
 *
 * @param[in,out] state     sequenze TX/RX del core;
 * @param[in]     channel   canale logico FLOW_MU_CHAN_*;
 * @param[in]     tag       tag della connessione;
 * @param[in]     cycle     ciclo a cui il messaggio appartiene;
 * @param[in]     payload   payload da inviare (`length` byte, puo' essere NULL
 *                          se `length` e' 0);
 * @param[in]     length    lunghezza del payload (<= FLOW_MU_PAYLOAD_MAX);
 * @param[out]    peer      messaggio verificato del peer;
 * @param[in]     timeout_us attesa massima del peer in microsecondi.
 * @return true solo se il peer ha risposto con un messaggio valido, del ciclo
 *         atteso e con sequence piu' recente.
 */
bool flow_mu_exchange(flow_mu_state_t* state,
                      flow_u8 channel,
                      flow_u8 tag,
                      flow_u32 cycle,
                      const void* payload,
                      flow_u16 length,
                      flow_mu_message_t* peer,
                      flow_u32 timeout_us);

/** @brief Verifica un messaggio ricevuto senza usare lo stato del core.
 *
 * @param[in] message messaggio da verificare;
 * @param[in] self    ruolo ricevente;
 * @param[in] channel canale atteso;
 * @param[in] tag     tag atteso;
 * @param[in] cycle   ciclo atteso.
 * @return true se magic, versione, CRC, routing, lunghezza e ciclo coincidono.
 */
bool flow_mu_verify(const flow_mu_message_t* message,
                    flow_core_id_t self,
                    flow_u8 channel,
                    flow_u8 tag,
                    flow_u32 cycle);

#endif /* SI_FLOW_MU_H */
