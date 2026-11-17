/*
 * Canale inter-core del 1oo2 (ICC): payload protetto in OCRAM + doorbell MU18.
 *
 * Perche' non solo MU: una MU ha 4 registri TR e 4 RR da 32 bit (16 byte per
 * direzione), mentre i messaggi di cross-check sono 23..72 byte. Quindi
 *   - il messaggio (flow_mu_message_t, con seq e CRC-32) sta in uno slot OCRAM;
 *   - la MU18 porta il doorbell: TR0 = {seq, tag, channel}, TR1 = CRC-32 del
 *     payload. Il ricevente ricalcola il CRC del payload letto in OCRAM e lo
 *     confronta con quello arrivato per MU: due mezzi fisici indipendenti devono
 *     dire la stessa cosa, altrimenti anomalia -> fail-safe.
 *
 * C'e' poi il rendezvous di ciclo (barrier a 3: SAFE_A, SAFE_B, FIELDBUS):
 *   - `cycle` lo scrive SOLO il FIELDBUS (scrittore unico: nessun RMW in gara);
 *   - ogni core safe pubblica `ready[me] = cycle` quando ha finito il ciclo;
 *   - il FIELDBUS apre il ciclo successivo dopo ready[A] e ready[B], cosi' i due
 *     core safe vedono lo stesso `cycle` e la stessa RX (una per ciclo).
 *
 * Regione: FLOW_ICC_BASE_ADDR (0x204D5000, 1 KB nella finestra della patch SM
 * 0002). Tutto in memoria non cache: accessi con acquire/release.
 */
#ifndef SI_FLOW_ICC_H
#define SI_FLOW_ICC_H

#include "flow_config.h"
#include "flow_mu.h"
#include "flow_types.h"

#define FLOW_ICC_MAGIC 0x49434331u /* "ICC1" */
#define FLOW_ICC_DIRS 2u           /* 0 = SAFE_A -> SAFE_B, 1 = SAFE_B -> SAFE_A */

/* Lo slot porta l'INTERO messaggio MU (header + payload: 72 byte), non solo il
 * payload: e' quello che flow_mu_exchange() scrive e verifica. */
#define FLOW_ICC_PAYLOAD_MAX FLOW_MU_MESSAGE_SIZE

typedef struct FLOW_PACKED flow_icc_slot
{
    flow_u16 seq;      /* contatore di trasporto, scritto per ultimo          */
    flow_u16 length;   /* byte validi del payload                            */
    flow_u32 cycle;    /* ciclo a cui il messaggio appartiene                */
    flow_u32 crc;      /* CRC-32 del payload (viaggia anche nel doorbell)    */
    flow_u8 payload[FLOW_ICC_PAYLOAD_MAX];
    flow_u8 pad[4];
} flow_icc_slot_t;

typedef struct FLOW_PACKED flow_icc
{
    flow_u32 magic;
    flow_u32 cycle;                   /* ciclo aperto: scrittore = FIELDBUS   */
    flow_u32 ready[FLOW_CORE_COUNT];  /* ultimo ciclo completato da ogni ruolo */
    flow_icc_slot_t slot[FLOW_ICC_DIRS][FLOW_MU_CHAN_COUNT];
} flow_icc_t;

FLOW_STATIC_ASSERT(sizeof(flow_icc_slot_t) == 88u, "flow_icc_slot_t must be 88 bytes");
FLOW_STATIC_ASSERT(sizeof(flow_icc_t) <= 1024u, "flow_icc_t does not fit the reserved 1 KB");

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

/* Owner (FIELDBUS): azzera la zona e pubblica il magic. Gli altri ruoli non
 * scrivono niente qui. */
void flow_icc_init(flow_core_id_t self);

bool flow_icc_ready(void);
bool flow_icc_wait_ready(flow_u32 timeout_us);

/* Pubblica `length` byte verso il peer (solo fra SAFE_A e SAFE_B) e suona il
 * doorbell MU18. */
bool flow_icc_post(flow_core_id_t self, flow_core_id_t destination, flow_u8 channel, flow_u8 tag, flow_u32 cycle,
    const void *payload, flow_u16 length);

/* Attende il messaggio del peer per (channel, tag, cycle) e ne verifica seq, CRC
 * (doppio: OCRAM + MU) e lunghezza. */
bool flow_icc_wait(flow_core_id_t self, flow_core_id_t source, flow_u8 channel, flow_u8 tag, flow_u32 cycle,
    void *payload, flow_u16 max_length, flow_u16 *length, flow_u32 timeout_us);

/* Rendezvous di ciclo (vedi sopra). */
bool flow_icc_cycle_wait(flow_u32 *cycle, flow_u32 timeout_us);
void flow_icc_cycle_done(flow_core_id_t self, flow_u32 cycle);
bool flow_icc_cycle_release(flow_u32 cycle, flow_u32 timeout_us);

flow_u32 flow_icc_cycle_current(void);
const flow_icc_stats_t *flow_icc_get_stats(void);

#endif /* SI_FLOW_ICC_H */
