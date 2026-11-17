/**
 * @file flow_icc.h
 * @brief Canale inter-core del 1oo2 (ICC): payload protetto in OCRAM + doorbell
 * MU18.
 * @ingroup flow_icc
 *
 * Perche' non solo MU: una MU ha 4 registri TR e 4 RR da 32 bit (16 byte per
 * direzione), mentre i messaggi di cross-check sono 23..72 byte. Quindi
 *   - il messaggio (flow_mu_message_t, con seq e CRC-32) sta in uno slot OCRAM;
 *   - la MU18 porta il doorbell: TR0 = {seq, tag, channel}, TR1 = CRC-32 del
 *     payload. Il ricevente ricalcola il CRC del payload letto in OCRAM e lo
 *     confronta con quello arrivato per MU: due mezzi fisici indipendenti
 * devono dire la stessa cosa, altrimenti anomalia -> fail-safe.
 *
 * C'e' poi il rendezvous di ciclo (barrier a 3: SAFE_A, SAFE_B, FIELDBUS):
 *   - `cycle` lo scrive SOLO il FIELDBUS (scrittore unico: nessun RMW in gara);
 *   - ogni core safe pubblica `ready[me] = cycle` quando ha finito il ciclo;
 *   - il FIELDBUS apre il ciclo successivo dopo ready[A] e ready[B], cosi' i
 * due core safe vedono lo stesso `cycle` e la stessa RX (una per ciclo).
 *
 * Regione: FLOW_ICC_BASE_ADDR (0x204D5000, 1 KB nella finestra della patch SM
 * 0002). Tutto in memoria non cache: accessi con acquire/release.
 *
 * \mermaid
 * sequenceDiagram
 *     participant A as SAFE_A
 *     participant B as SAFE_B
 *     participant F as FIELDBUS
 *     A->>B: flow_icc_post(messaggio 72 B in OCRAM)
 *     A->>B: doorbell MU18 {seq, tag, channel, CRC-32}
 *     B->>B: ricalcola CRC-32 del payload
 *     B-->>A: risposta sul canale opposto
 *     F->>A: cycle++ (barrier, dopo ready[A] e ready[B])
 * \endmermaid
 */
#ifndef SI_FLOW_ICC_H
#define SI_FLOW_ICC_H

#include "flow_config.h"
#include "flow_mu.h"
#include "flow_types.h"

#define FLOW_ICC_MAGIC 0x49434331u /**< "ICC1" */
#define FLOW_ICC_DIRS 2u /**< 0 = SAFE_A -> SAFE_B, 1 = SAFE_B -> SAFE_A */

/** Lo slot porta l'INTERO messaggio MU (header + payload: 72 byte), non solo il
 * payload: e' quello che flow_mu_exchange() scrive e verifica. */
#define FLOW_ICC_PAYLOAD_MAX FLOW_MU_MESSAGE_SIZE

/** @brief Slot OCRAM: messaggio MU completo + seq, ciclo e CRC-32. */
typedef struct FLOW_PACKED flow_icc_slot
{
  flow_u16 seq; /**< contatore di trasporto, scritto per ultimo */
  flow_u16 length; /**< byte validi del payload */
  flow_u32 cycle; /**< ciclo a cui il messaggio appartiene */
  flow_u32 crc; /**< CRC-32 del payload (viaggia anche nel doorbell) */
  flow_u8 payload[FLOW_ICC_PAYLOAD_MAX];
  flow_u8 pad[4];
} flow_icc_slot_t;

/** @brief Canale ICC e barrier di ciclo. */
typedef struct FLOW_PACKED flow_icc
{
  flow_u32 magic;
  flow_u32 cycle; /**< ciclo aperto: scrittore = FIELDBUS */
  flow_u32 ready[FLOW_CORE_COUNT]; /**< ultimo ciclo completato da ogni ruolo */
  flow_icc_slot_t slot[FLOW_ICC_DIRS][FLOW_MU_CHAN_COUNT];
} flow_icc_t;

FLOW_STATIC_ASSERT(sizeof(flow_icc_slot_t) == 88u,
                   "flow_icc_slot_t must be 88 bytes");
FLOW_STATIC_ASSERT(sizeof(flow_icc_t) <= 1024u,
                   "flow_icc_t does not fit the reserved 1 KB");

/** @brief Contatori diagnostici del canale ICC. */
typedef struct flow_icc_stats
{
  flow_u32 posted;
  flow_u32 received;
  flow_u32 timeouts;
  flow_u32 crc_errors;
  flow_u32 seq_errors;
  flow_u32 cycle_errors;
  flow_u32 doorbell_errors;
  flow_u32 barrier_timeouts;
} flow_icc_stats_t;

/** @brief Inizializza la zona ICC e ne pubblica il magic.
 *
 * @param[in] self ruolo chiamante: solo FIELDBUS e' owner, gli altri ruoli
 *                 escono senza scrivere niente.
 * @post `cycle = 1` e `magic` pubblicato con release.
 */
void flow_icc_init(flow_core_id_t self);

/** @brief La zona ICC e' inizializzata e visibile a questo core?
 *
 * @return true se il magic letto con acquire e' quello atteso.
 */
bool flow_icc_ready(void);

/** @brief Attende che la zona ICC sia pronta, con timeout.
 *
 * @param[in] timeout_us attesa massima in microsecondi.
 * @return true se la zona diventa pronta entro il timeout.
 */
bool flow_icc_wait_ready(flow_u32 timeout_us);

/** @brief Pubblica un messaggio verso il peer e suona il doorbell MU18.
 *
 * Il payload va nello slot OCRAM, la MU18 porta solo `{seq, tag, channel}` e
 * il CRC-32 del payload: due mezzi fisici indipendenti devono concordare.
 *
 * @param[in] self        ruolo mittente (SAFE_A o SAFE_B);
 * @param[in] destination peer logico (oggi ignorato: c'e' un solo peer);
 * @param[in] channel     canale logico FLOW_MU_CHAN_*;
 * @param[in] tag         tag della connessione;
 * @param[in] cycle       ciclo a cui il messaggio appartiene;
 * @param[in] payload     messaggio MU completo, `length` byte;
 * @param[in] length      lunghezza del payload (<= FLOW_ICC_PAYLOAD_MAX).
 * @return true se payload e doorbell sono stati pubblicati.
 */
bool flow_icc_post(flow_core_id_t self,
                   flow_core_id_t destination,
                   flow_u8 channel,
                   flow_u8 tag,
                   flow_u32 cycle,
                   const void* payload,
                   flow_u16 length);

/** @brief Attende il messaggio del peer e lo verifica.
 *
 * Verifica doorbell (canale e tag), sequence, lunghezza, ciclo e CRC: quello
 * ricalcolato sul payload OCRAM deve coincidere sia con lo slot sia con il CRC
 * arrivato per MU.
 *
 * @param[in]  self       ruolo ricevente;
 * @param[in]  source     peer logico (SAFE_A o SAFE_B);
 * @param[in]  channel    canale logico FLOW_MU_CHAN_*;
 * @param[in]  tag        tag della connessione;
 * @param[in]  cycle      ciclo atteso;
 * @param[out] payload    buffer di destinazione;
 * @param[in]  max_length capacita' di `payload`;
 * @param[out] length     byte ricevuti (se non NULL);
 * @param[in]  timeout_us attesa massima in microsecondi.
 * @return true solo se il messaggio e' del peer, del ciclo atteso e integro.
 */
bool flow_icc_wait(flow_core_id_t self,
                   flow_core_id_t source,
                   flow_u8 channel,
                   flow_u8 tag,
                   flow_u32 cycle,
                   void* payload,
                   flow_u16 max_length,
                   flow_u16* length,
                   flow_u32 timeout_us);

/** @brief Attende che il FIELDBUS apra un ciclo nuovo.
 *
 * @param[out] cycle      numero del ciclo aperto (se non NULL);
 * @param[in]  timeout_us attesa massima in microsecondi.
 * @return true se il ciclo e' avanzato entro il timeout.
 */
bool flow_icc_cycle_wait(flow_u32* cycle, flow_u32 timeout_us);

/** @brief Pubblica `ready[self] = cycle` quando il ruolo ha finito il ciclo.
 *
 * @param[in] self  ruolo che ha completato il ciclo;
 * @param[in] cycle ciclo completato.
 */
void flow_icc_cycle_done(flow_core_id_t self, flow_u32 cycle);

/** @brief Rendezvous a 3: attende i due core safe e apre il ciclo successivo.
 *
 * Il `cycle` lo scrive solo il FIELDBUS. Finche' i due core safe non hanno
 * completato un ciclo il numero non avanza (gate d'avvio); al timeout il ciclo
 * si apre comunque e chi e' vivo lo rileva dal cross-check.
 *
 * @param[in] cycle      ciclo corrente;
 * @param[in] timeout_us attesa massima in microsecondi.
 * @return false se il timeout e' scaduto prima che entrambi i core fossero
 *         pronti (il ciclo viene comunque aperto).
 */
bool flow_icc_cycle_release(flow_u32 cycle, flow_u32 timeout_us);

/** @brief Ciclo attualmente aperto dal FIELDBUS.
 *
 * @return numero di ciclo letto con acquire.
 */
flow_u32 flow_icc_cycle_current(void);

/** @brief Contatori diagnostici del canale ICC.
 *
 * @return puntatore ai contatori interni (mai NULL); sola lettura.
 */
const flow_icc_stats_t* flow_icc_get_stats(void);

#endif /* SI_FLOW_ICC_H */
