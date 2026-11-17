/**
 * @file flow_mu18.h
 * @brief MU18: l'unica MU fra due core secondari dell'i.MX94 EVK (config SM
 * mx94evk).
 * @ingroup flow_mu18
 *
 *   lato A  MU18_A  0x42B70000  M7_1  (SAFE_B)
 *   lato B  MU18_B  0x42B80000  M7_0  (SAFE_A)
 *
 * Qui c'e' solo il driver dei registri: il lato lo sceglie chi usa la MU
 * (fw/safe/safe_platform.c, con flow_mu18_bind()). Gli altri ruoli non la
 * bindano e le funzioni falliscono chiuse.
 *
 * Ogni core raggiunge fisicamente solo il proprio lato. La MU ha 4 registri TR
 * e 4 RR da 32 bit (16 byte per direzione), quindi NON puo' portare i nostri
 * messaggi di cross-check (23..72 byte): porta il doorbell
 *   TR0 = {seq[15:0], tag[7:0], channel[7:0]}
 *   TR1 = CRC-32 del payload che sta in flow_icc (OCRAM)
 * e il ricevente ricalcola il CRC del payload: due mezzi indipendenti devono
 * dire la stessa cosa.
 *
 * Accesso diretto ai registri: niente MU_Init()/CLOCK_EnableClock(), il clock
 * e' dell'SM (se il registro non risponde si chiede il clock via SCMI).
 *
 * \mermaid
 * flowchart LR
 *   ICC["flow_icc slot<br/>88 B in OCRAM"] -->|payload| MU["MU18<br/>doorbell
 * 16 B"] MU -->|"TR0 = {seq, tag, channel}<br/>TR1 = CRC-32"| PEER["peer"] PEER
 * -->|"ricalcola CRC-32"| ICC
 * \endmermaid
 */
#ifndef SI_FLOW_MU18_H
#define SI_FLOW_MU18_H

#include "flow_types.h"

/** @brief Lega il driver a un lato della MU18.
 *
 * @param[in] base indirizzo base del proprio lato (FLOW_MU18_A_BASE o
 *                 FLOW_MU18_B_BASE); 0 = nessun lato (fieldbus).
 */
void flow_mu18_bind(flow_u32 base);

/** @brief Questo core ha un lato MU18 configurato?
 *
 * @return true se flow_mu18_bind() e' gia' stato chiamato con un indirizzo.
 */
bool flow_mu18_available(void);

/** @brief Scrive due parole nel proprio lato della MU.
 *
 * @param[in] word0      TR0 (doorbell `{seq, tag, channel}`);
 * @param[in] word1      TR1 (CRC-32 del payload);
 * @param[in] timeout_us attesa massima che TR0/TR1 siano liberi.
 * @return true se le due parole sono state scritte.
 */
bool flow_mu18_send(flow_u32 word0, flow_u32 word1, flow_u32 timeout_us);

/** @brief Legge due parole dal proprio lato quando sono arrivate.
 *
 * @param[out] word0      RF0;
 * @param[out] word1      RF1;
 * @param[in]  timeout_us attesa massima in microsecondi.
 * @return true se le due parole sono state lette entro il timeout.
 */
bool flow_mu18_recv(flow_u32* word0, flow_u32* word1, flow_u32 timeout_us);

/** @brief Come flow_mu18_recv(), ma senza attesa e senza contatori.
 *
 * Serve ai cicli (eco del doorbell, servizio della barrier): non deve alterare
 * le statistiche dei timeout.
 *
 * @param[out] word0 RF0 (se non NULL);
 * @param[out] word1 RF1 (se non NULL).
 * @return true se un messaggio era disponibile.
 */
bool flow_mu18_poll(flow_u32* word0, flow_u32* word1);

/** @brief Round-trip di prova sul link MU18.
 *
 * Invia `value` e attende il valore del peer. Usato dal selftest di bring-up
 * per validare il link fra M7_0 e M7_1.
 *
 * @param[in]  value      valore da inviare;
 * @param[in]  timeout_us attesa massima in microsecondi;
 * @param[out] peer_value valore ricevuto dal peer (se non NULL).
 * @return true se il peer ha risposto entro il timeout.
 */
bool flow_mu18_ping(flow_u32 value, flow_u32 timeout_us, flow_u32* peer_value);

/** Contatori diagnostici del link. */
typedef struct flow_mu18_stats
{
  flow_u32 sent;
  flow_u32 received;
  flow_u32 tx_timeouts;
  flow_u32 rx_timeouts;
} flow_mu18_stats_t;

/** @brief Contatori diagnostici del link MU18.
 *
 * @return puntatore ai contatori interni (mai NULL); sola lettura.
 */
const flow_mu18_stats_t* flow_mu18_get_stats(void);

#endif /* SI_FLOW_MU18_H */
