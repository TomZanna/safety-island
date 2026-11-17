/**
 * @file flow_shm.h
 * @brief Accessori alla mailbox descritta in flow_shm_api.h.
 * @ingroup flow_shm
 *
 * Il payload della PDU non viene ricalcolato: e' gia' protetto da FSoE. Il
 * seqlock protegge da letture strappate e il CRC-16 dell'header dello slot
 * protegge i metadati che il ruolo FIELDBUS usa per unire le due meta'.
 */
#ifndef SI_FLOW_SHM_H
#define SI_FLOW_SHM_H

#include "flow_shm_api.h"
#include "flow_types.h"

/** @brief Inizializza la mailbox e ne pubblica l'header.
 *
 * Azzera la struttura, scrive geometria e stato READY, poi pubblica il magic
 * per ultimo con una barrier di release: chi vede il magic vede tutto il resto.
 *
 * @param[out] shm   mailbox nella finestra OCRAM condivisa;
 * @param[in]  owner ruolo che possiede l'inizializzazione (FIELDBUS).
 * @pre `shm` punta alla finestra della patch SM 0002, non alla DDR.
 * @note La chiama solo l'owner: gli altri ruoli si limitano a validarla.
 */
void flow_shm_init(flow_shm_t* shm, flow_core_id_t owner);

/** @brief Verifica identita', versione, geometria e stato della mailbox.
 *
 * @param[in] shm mailbox da validare.
 * @return true se magic, protocol id, versione, layout hash, geometria e stato
 *         READY coincidono con quelli attesi da questo build.
 */
bool flow_shm_validate(const flow_shm_t* shm);

/** @brief Attende che la mailbox diventi valida, con timeout.
 *
 * Non blocca mai per sempre: i core safe la chiamano prima di partire e vanno
 * in fail-safe se l'owner non pubblica la mailbox.
 *
 * @param[in] shm        mailbox da attendere;
 * @param[in] timeout_us attesa massima in microsecondi.
 * @return true se la mailbox e' valida entro il timeout.
 */
bool flow_shm_wait_ready(const flow_shm_t* shm, flow_u32 timeout_us);

/** @brief Numero di frame pubblicati su un canale.
 *
 * @param[in] channel canale TX o RX.
 * @return contatore monotono pubblicato con release; 0 se `channel` e' NULL.
 */
flow_u32 flow_shm_count(const flow_shm_channel_t* channel);

/** @brief Scrive uno slot e ne verifica il read-back.
 *
 * `index` parte da 1 e sceglie il banco: `(index - 1) % FLOW_SHM_BANKS`. Il
 * payload puo' contenere solo il contributo di questo core (meta' CRC o meta'
 * dati), il resto e' zero.
 *
 * @param[out] channel canale TX del core;
 * @param[in]  index   numero di frame da pubblicare (>= 1);
 * @param[in]  len     lunghezza del frame FSoE in byte;
 * @param[in]  fc      frame counter del black channel;
 * @param[in]  flags   metadati dello slot: riservati all'ABI, oggi 0;
 * @param[in]  pdu     contributo del core, `len` byte.
 * @return true se la copia e' arrivata in memoria e l'header dello slot
 *         supera il controllo di CRC-16 (read-back).
 */
bool flow_shm_stage(flow_shm_channel_t* channel,
                    flow_u32 index,
                    flow_u32 len,
                    flow_u32 fc,
                    flow_u16 flags,
                    const flow_u8* pdu);

/** @brief Pubblica uno slot gia' scritto, aggiornando il contatore.
 *
 * @param[out] channel canale TX del core;
 * @param[in]  index   numero di frame pubblicato (0 = riallineamento verso il
 *                     basso dopo un fault).
 * @note Il contatore e' l'unico dato che i lettori usano per sapere quale slot
 *       leggere: va scritto dopo lo stage, mai prima.
 */
void flow_shm_commit(flow_shm_channel_t* channel, flow_u32 index);

/** @brief Legge uno slot passando dal seqlock e ne verifica l'header.
 *
 * Riprova finche' lo scrittore non ha finito (sequence dispari) o finche' la
 * sequence non e' stabile; poi verifica `reserved`, lunghezza e CRC-16.
 *
 * @param[in]  channel canale da leggere (TX del core o RX del fieldbus);
 * @param[in]  index   frame da leggere (>= 1);
 * @param[out] slot    copia locale dello slot.
 * @return true se lo slot e' consistente e l'header e' valido; false su
 *         seqlock instabile, metadati incoerenti o CRC-16 errato.
 */
bool flow_shm_read_index(const flow_shm_channel_t* channel,
                         flow_u32 index,
                         flow_shm_slot_t* slot);

/** @brief Scrive e pubblica uno slot in una sola chiamata.
 *
 * @param[out] channel canale con un solo scrittore;
 * @param[in]  len     lunghezza del frame FSoE in byte;
 * @param[in]  fc      frame counter del black channel;
 * @param[in]  flags   metadati dello slot: riservati all'ABI, oggi 0;
 * @param[in]  pdu     payload del frame;
 * @param[out] index   se non NULL, numero di frame pubblicato.
 * @return true se stage e read-back sono andati a buon fine.
 */
bool flow_shm_publish(flow_shm_channel_t* channel,
                      flow_u32 len,
                      flow_u32 fc,
                      flow_u16 flags,
                      const flow_u8* pdu,
                      flow_u32* index);

/** @brief Aggiorna l'heartbeat di telemetria di un ruolo.
 *
 * @param[out] shm   mailbox;
 * @param[in]  core  ruolo che scrive il proprio heartbeat;
 * @param[in]  value valore del ciclo corrente.
 */
void flow_shm_update_heartbeat(flow_shm_t* shm,
                               flow_core_id_t core,
                               flow_u32 value);

/** @brief Legge l'heartbeat di un ruolo.
 *
 * @param[in] shm  mailbox;
 * @param[in] core ruolo di cui leggere il heartbeat.
 * @return ultimo valore pubblicato; 0 se gli argomenti non sono validi.
 */
flow_u32 flow_shm_heartbeat(const flow_shm_t* shm, flow_core_id_t core);

/** @brief Copia solo il contributo di un core del frame FSoE.
 *
 * @param[out] destination area di `FLOW_PDU_MAX` byte; i byte non coperti
 *                         restano a zero;
 * @param[in]  frame       frame FSoE completo;
 * @param[in]  len         lunghezza del frame in byte;
 * @param[in]  crc_part    true copia i byte CRC (contributo di SAFE_A), false
 *                         copia comando + dati + Connection ID (SAFE_B).
 * @pre `destination` ha spazio per FLOW_PDU_MAX byte.
 */
void flow_shm_split(flow_u8* destination,
                    const flow_u8* frame,
                    flow_u32 len,
                    bool crc_part);

/** @brief Unisce i due contributi in un frame FSoE completo.
 *
 * @param[out] destination frame assemblato (`FLOW_PDU_MAX` byte, azzerato
 *                         prima di scrivere);
 * @param[in]  crc_part    contributo con i byte CRC (SAFE_A);
 * @param[in]  data_part   contributo con comando, dati e Connection ID
 *                         (SAFE_B);
 * @param[in]  len         lunghezza del frame in byte.
 * @post `destination` e' l'originale se i due contributi sono intatti.
 */
void flow_shm_assemble(flow_u8* destination,
                       const flow_u8* crc_part,
                       const flow_u8* data_part,
                       flow_u32 len);

/** @brief Verifica che i byte del contributo di questo core siano intatti.
 *
 * @param[in] stored   contributo letto dalla mailbox;
 * @param[in] frame    frame originale del core;
 * @param[in] len      lunghezza del frame in byte;
 * @param[in] crc_part true confronta i byte CRC, false gli altri.
 * @return true se tutti i byte del contributo coincidono.
 */
bool flow_shm_part_matches(const flow_u8* stored,
                           const flow_u8* frame,
                           flow_u32 len,
                           bool crc_part);

#endif /* SI_FLOW_SHM_H */
