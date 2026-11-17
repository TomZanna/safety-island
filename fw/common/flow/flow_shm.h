/*
 * Accessori alla mailbox descritta in flow_shm_api.h.
 *
 * Il payload della PDU non viene ricalcolato: e' gia' protetto da FSoE. Il
 * seqlock protegge da letture strappate e il CRC-16 dell'header dello slot
 * protegge i metadati che il ruolo FIELDBUS usa per unire le due meta'.
 */
#ifndef SI_FLOW_SHM_H
#define SI_FLOW_SHM_H

#include "flow_shm_api.h"
#include "flow_types.h"

/* Inizializza la mailbox. La chiama solo il ruolo FIELDBUS (owner): gli altri
 * due la validano. */
void flow_shm_init(flow_shm_t *shm, flow_core_id_t owner);

/* magic, protocol id, version, layout hash, geometria e stato READY. */
bool flow_shm_validate(const flow_shm_t *shm);

/* Attesa limitata: true quando la mailbox e' valida. Non blocca mai per sempre:
 * i core safe la chiamano prima di partire e vanno in fault se non arriva. */
bool flow_shm_wait_ready(const flow_shm_t *shm, flow_u32 timeout_us);

/* Frame pubblicati su un canale. */
flow_u32 flow_shm_count(const flow_shm_channel_t *channel);

/* Scrive uno slot. `index` parte da 1; il banco e' (index - 1) % banks.
 * `pdu` puo' contenere solo il contributo di questo core (meta' CRC o meta'
 * dati), il resto e' zero. Ritorna true se la copia e' arrivata in memoria e
 * l'header dello slot e' valido (read-back). */
bool flow_shm_stage(flow_shm_channel_t *channel, flow_u32 index, flow_u32 len, flow_u32 fc, flow_u16 flags,
    const flow_u8 *pdu);

/* Pubblica uno slot gia' scritto (index 0 = riallineamento verso il basso). */
void flow_shm_commit(flow_shm_channel_t *channel, flow_u32 index);

/* Legge uno slot passando dal seqlock e verifica il CRC-16 dell'header. */
bool flow_shm_read_index(const flow_shm_channel_t *channel, flow_u32 index, flow_shm_slot_t *slot);

/* stage + commit in una chiamata (canali con un solo scrittore). */
bool flow_shm_publish(flow_shm_channel_t *channel, flow_u32 len, flow_u32 fc, flow_u16 flags, const flow_u8 *pdu,
    flow_u32 *index);

/* Heartbeat di telemetria, scritto da ogni ruolo a ogni ciclo. */
void flow_shm_update_heartbeat(flow_shm_t *shm, flow_core_id_t core, flow_u32 value);

/* Lettura del heartbeat di un ruolo (per la telemetria degli altri due). */
flow_u32 flow_shm_heartbeat(const flow_shm_t *shm, flow_core_id_t core);

/* Copia solo il contributo di un core: crc_part true tiene i byte CRC, false
 * tiene comando + dati + Connection ID. */
void flow_shm_split(flow_u8 *destination, const flow_u8 *frame, flow_u32 len, bool crc_part);

/* Unisce i due contributi in un frame FSoE completo. */
void flow_shm_assemble(flow_u8 *destination, const flow_u8 *crc_part, const flow_u8 *data_part, flow_u32 len);

/* Verifica che i byte del contributo di questo core siano intatti. */
bool flow_shm_part_matches(const flow_u8 *stored, const flow_u8 *frame, flow_u32 len, bool crc_part);

#endif /* SI_FLOW_SHM_H */
