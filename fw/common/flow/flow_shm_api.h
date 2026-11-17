/**
 * @file flow_shm_api.h
 * @brief FONTE UNICA della mailbox in memoria condivisa fra i core secondari.
 * @ingroup flow_shm
 *
 * La includono il firmware (M7_0, M7_1, M33_1) e, se in futuro servisse, un
 * driver lato AP. Definisce il layout in byte, il protocol id e la versione:
 * non c'e' serializzazione, i due mondi accedono alle stesse strutture packed.
 *
 * La PDU FSoE e' gia' protetta end-to-end da FSoE (CRC, sequence, watchdog). Il
 * CRC nell'header dello slot protegge solo i metadati della mailbox (len,
 * flags, frame counter), non il payload.
 *
 * Regole (core a 32 bit e AP a 64 bit):
 *   - strutture packed;
 *   - solo campi flow_u8/flow_u16/flow_u32, allineati naturalmente;
 *   - offset e dimensioni verificati con FLOW_STATIC_ASSERT.
 *
 * Portato da imx-flow/bare-metal/include/flow_shm_api.h con FLOW_SHM_VERSION 2:
 * heartbeat per tutti e tre i ruoli e finestra di OCRAM condivisa (patch SM
 * 0002).
 *
 * \mermaid
 * flowchart LR
 *   A["SAFE_A<br/>meta CRC"] --> TXA["tx[A]"]
 *   B["SAFE_B<br/>meta dati"] --> TXB["tx[B]"]
 *   TXA --> F["FIELDBUS<br/>MIN(count_A, count_B)<br/>+ interleave"]
 *   TXB --> F
 *   F --> RX["rx<br/>una per ciclo safe"]
 * \endmermaid
 */
#ifndef SI_FLOW_SHM_API_H
#define SI_FLOW_SHM_API_H

#include "flow_config.h"
#include "flow_types.h"

/** ------------------------------------------------------------- identity ---
 */

#define FLOW_SHM_MAGIC 0x464C4F57u /**< "FLOW" */
#define FLOW_SHM_PROTOCOL_ID 0x494D5839u /**< "IMX9" */

/** Stato dell'header. */
#define FLOW_SHM_STATE_UNINIT 0u
#define FLOW_SHM_STATE_READY 1u
#define FLOW_SHM_STATE_FAULT 2u

/** Nodi che possiedono una meta' del frame TX (indici di tx[conn][node]). */
#define FLOW_SHM_NODE_SAFE_A 0u
#define FLOW_SHM_NODE_SAFE_B 1u
#define FLOW_SHM_NODE_COUNT FLOW_SAFE_CORE_COUNT

/** Split del frame: SAFE_A mette i byte CRC, SAFE_B il resto. Nessuno dei due
 * ha da solo un frame FSoE valido: lo assembla il ruolo FIELDBUS. */
#define FLOW_SHM_CRC_NODE FLOW_SHM_NODE_SAFE_A
#define FLOW_SHM_DATA_NODE FLOW_SHM_NODE_SAFE_B

/** Padding che tiene ogni slot allineato a 4 byte. */
#define FLOW_SHM_PDU_PAD ((4u - (FLOW_PDU_MAX % 4u)) % 4u)
#define FLOW_SHM_HEADER_SIZE 64u
#define FLOW_SHM_SLOT_SIZE (12u + FLOW_PDU_MAX + FLOW_SHM_PDU_PAD)
#define FLOW_SHM_CHANNEL_SIZE (16u + FLOW_SHM_BANKS * FLOW_SHM_SLOT_SIZE)

/** @brief Header della mailbox: identita, versione, geometria e stato. */
typedef struct FLOW_PACKED flow_shm_header
{
  flow_u32 magic;
  flow_u32 protocol_id;
  flow_u32 version;
  flow_u32 layout_hash;

  flow_u32 conn_count;
  flow_u32 banks;
  flow_u32 slot_size;
  flow_u32 header_size;

  flow_u32 state;
  flow_u32 heartbeat[FLOW_CORE_COUNT];
  flow_u32 reserved[4];
} flow_shm_header_t;

/** @brief Slot PDU con metadati protetti da CRC-16. */
typedef struct FLOW_PACKED flow_shm_slot
{
  flow_u16 len; /**< numero di byte validi della PDU */
  flow_u16 flags; /**< riservato, deve essere 0 */
  flow_u32 fc; /**< contatore di frame del black channel */
  flow_u16 hdr_crc; /**< CRC-16 su len, flags, fc, reserved */
  flow_u16 reserved; /**< deve essere 0 */
  flow_u8 pdu[FLOW_PDU_MAX];
  flow_u8 pad[FLOW_SHM_PDU_PAD];
} flow_shm_slot_t;

/** @brief Canale TX/RX: 3 banchi con seqlock e contatore. */
typedef struct FLOW_PACKED flow_shm_channel
{
  flow_u32 count; /**< frame pubblicati (monotono) */
  flow_u32 bank_seq[FLOW_SHM_BANKS]; /**< seqlock: dispari = scrittura */
  flow_shm_slot_t bank[FLOW_SHM_BANKS];
} flow_shm_channel_t;

/** @brief Mailbox completa: header, canali TX per nodo e RX. */
typedef struct FLOW_PACKED flow_shm
{
  flow_shm_header_t header;

  /** TX: [connessione][nodo] - un solo scrittore per canale (quel core). */
  flow_shm_channel_t tx[FLOW_NUM_CONNECTIONS][FLOW_SHM_NODE_COUNT];

  /** RX: [connessione] - un solo scrittore: il ruolo FIELDBUS. */
  flow_shm_channel_t rx[FLOW_NUM_CONNECTIONS];
} flow_shm_t;

#define FLOW_SHM_TOTAL_SIZE ((flow_u32)sizeof(flow_shm_t))
#define FLOW_SHM_TX_OFFSET ((flow_u32)offsetof(flow_shm_t, tx))
#define FLOW_SHM_RX_OFFSET ((flow_u32)offsetof(flow_shm_t, rx))

FLOW_STATIC_ASSERT(sizeof(flow_shm_header_t) == FLOW_SHM_HEADER_SIZE,
                   "flow_shm_header_t size mismatch");
FLOW_STATIC_ASSERT(offsetof(flow_shm_header_t, protocol_id) == 4u,
                   "protocol_id must be at offset 4");
FLOW_STATIC_ASSERT(offsetof(flow_shm_header_t, version) == 8u,
                   "version must be at offset 8");
FLOW_STATIC_ASSERT(offsetof(flow_shm_header_t, layout_hash) == 12u,
                   "layout_hash must be at offset 12");
FLOW_STATIC_ASSERT(sizeof(flow_shm_slot_t) == FLOW_SHM_SLOT_SIZE,
                   "flow_shm_slot_t size mismatch");
FLOW_STATIC_ASSERT(sizeof(flow_shm_channel_t) == FLOW_SHM_CHANNEL_SIZE,
                   "flow_shm_channel_t size mismatch");
FLOW_STATIC_ASSERT(offsetof(flow_shm_channel_t, bank_seq) == 4u,
                   "bank_seq must be at offset 4");
FLOW_STATIC_ASSERT(offsetof(flow_shm_channel_t, bank) == 16u,
                   "bank must be at offset 16");
FLOW_STATIC_ASSERT(offsetof(flow_shm_slot_t, pdu) == 12u,
                   "PDU must start at offset 12");
FLOW_STATIC_ASSERT(offsetof(flow_shm_t, tx) == FLOW_SHM_HEADER_SIZE,
                   "TX channels must follow the header");
FLOW_STATIC_ASSERT(offsetof(flow_shm_t, rx)
                       == (FLOW_SHM_HEADER_SIZE
                           + FLOW_NUM_CONNECTIONS * 2u * FLOW_SHM_CHANNEL_SIZE),
                   "RX channels must follow the TX channels");
FLOW_STATIC_ASSERT(sizeof(flow_shm_t)
                       == (FLOW_SHM_HEADER_SIZE
                           + FLOW_NUM_CONNECTIONS * 2u * FLOW_SHM_CHANNEL_SIZE
                           + FLOW_NUM_CONNECTIONS * FLOW_SHM_CHANNEL_SIZE),
                   "unexpected flow_shm_t size");

/** Layout del frame ETG.5100:
 *   len == 6: [0]=cmd [1]=data [2..3]=CRC_0 [4..5]=ConnId
 *   len >  6: parole CRC a 3 + 4*j, ConnId negli ultimi due byte.
 */
FLOW_INLINE int flow_pdu_is_crc_byte(flow_u32 len, flow_u32 offset)
{
  if (len < FLOW_PDU_MIN) {
    return 0;
  }

  if (len == FLOW_PDU_MIN) {
    return (offset == 2u) || (offset == 3u);
  }

  if ((offset < 3u) || (offset >= (len - 2u))) {
    return 0;
  }

  return (((offset - 3u) % 4u) < 2u);
}

#endif /* SI_FLOW_SHM_API_H */
