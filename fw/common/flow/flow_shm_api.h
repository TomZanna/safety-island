// clang-format off
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
// clang-format on
#ifndef SI_FLOW_SHM_API_H
#define SI_FLOW_SHM_API_H

#include "flow_config.h"
#include "flow_types.h"

/** ------------------------------------------------------------- identity ---
 */

#define FLOW_SHM_MAGIC 0x464C4F57u /**< "FLOW" */
#define FLOW_SHM_PROTOCOL_ID 0x494D5839u /**< "IMX9" */

/** Stato dell'header. Oggi si scrive un solo stato, 1 = READY: il valore 0 e'
 * il contenuto iniziale della memoria (mai pubblicata) e ogni altro valore e'
 * riservato all'ABI. Chi legge accetta solo READY e tratta il resto come "non
 * pronta", quindi una mailbox a meta' inizializzazione non viene mai usata. */
#define FLOW_SHM_STATE_READY 1u

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

/** Dimensioni d'ABI: non si ricavano a runtime e i static assert qui sotto le
 * confrontano con le struct. L'header copre se stesso, un banco e' uno slot, un
 * canale e' contatore + seqlock + banchi. */
#define FLOW_SHM_HEADER_SIZE 64u
#define FLOW_SHM_SLOT_SIZE (12u + FLOW_PDU_MAX + FLOW_SHM_PDU_PAD)
#define FLOW_SHM_CHANNEL_SIZE (16u + (FLOW_SHM_BANKS * FLOW_SHM_SLOT_SIZE))

/** @brief Header della mailbox: identita, versione, geometria e stato.
 *
 * `magic` e' l'ultimo campo pubblicato (store release): chi lo vede vede tutto
 * il resto. `layout_hash` identifica il layout atteso da questo build e si
 * confronta per uguaglianza, non si calcola a runtime.
 */
typedef struct FLOW_PACKED flow_shm_header
{
  flow_u32 magic; /**< FLOW_SHM_MAGIC, pubblicato per ultimo */
  flow_u32 protocol_id; /**< FLOW_SHM_PROTOCOL_ID ("IMX9") */
  flow_u32 version; /**< FLOW_SHM_VERSION: cambia con l'ABI */
  flow_u32 layout_hash; /**< FLOW_SHM_LAYOUT_HASH, confronto esatto */

  flow_u32 conn_count; /**< connessioni: FLOW_NUM_CONNECTIONS */
  flow_u32 banks; /**< banchi per canale: FLOW_SHM_BANKS */
  flow_u32 slot_size; /**< FLOW_SHM_SLOT_SIZE */
  flow_u32 header_size; /**< FLOW_SHM_HEADER_SIZE (offset del primo canale) */

  flow_u32 state; /**< FLOW_SHM_STATE_READY quando la mailbox e' valida */
  flow_u32 heartbeat[FLOW_CORE_COUNT]; /**< ultimo ciclo visto da ogni ruolo */
  flow_u32 reserved[4]; /**< riservato, deve essere 0 */
} flow_shm_header_t;

/** @brief Slot PDU con metadati protetti da CRC-16. */
typedef struct FLOW_PACKED flow_shm_slot
{
  flow_u16 len; /**< numero di byte validi della PDU */
  flow_u16 flags; /**< riservato, deve essere 0 */
  flow_u32 fc; /**< contatore di frame del black channel */
  flow_u16 hdr_crc; /**< CRC-16 su len, flags, fc, reserved */
  flow_u16 reserved; /**< deve essere 0 */
  flow_u8 pdu[FLOW_PDU_MAX]; /**< payload del frame FSoE */
  flow_u8 pad[FLOW_SHM_PDU_PAD]; /**< allinea lo slot a 4 byte */
} flow_shm_slot_t;

/** @brief Canale TX/RX: 3 banchi con seqlock e contatore. */
typedef struct FLOW_PACKED flow_shm_channel
{
  flow_u32 count; /**< frame pubblicati (monotono) */
  flow_u32 bank_seq[FLOW_SHM_BANKS]; /**< seqlock: dispari = scrittura */
  flow_shm_slot_t bank[FLOW_SHM_BANKS]; /**< 3 copie: mai quella in uso */
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
                           + (FLOW_NUM_CONNECTIONS * 2u
                              * FLOW_SHM_CHANNEL_SIZE)),
                   "RX channels must follow the TX channels");
FLOW_STATIC_ASSERT(sizeof(flow_shm_t)
                       == (FLOW_SHM_HEADER_SIZE
                           + (FLOW_NUM_CONNECTIONS * 2u * FLOW_SHM_CHANNEL_SIZE)
                           + (FLOW_NUM_CONNECTIONS * FLOW_SHM_CHANNEL_SIZE)),
                   "unexpected flow_shm_t size");

/** Layout del frame ETG.5100:
 *   len == 6: [0]=cmd [1]=data [2..3]=CRC_0 [4..5]=ConnId
 *   len >  6: parole CRC a 3 + 4*j, ConnId negli ultimi due byte.
 *
 * @param[in] len    lunghezza del frame FSoE (>= FLOW_PDU_MIN);
 * @param[in] offset posizione del byte nel frame.
 * @return true se `offset` e' un byte di CRC (contributo di SAFE_A), false se
 *         appartiene a comando/dati/ConnId (contributo di SAFE_B).
 */
FLOW_INLINE bool flow_pdu_is_crc_byte(flow_u32 len, flow_u32 offset)
{
  if (len < FLOW_PDU_MIN) {
    return false;
  }

  if (len == FLOW_PDU_MIN) {
    return (offset == 2u) || (offset == 3u);
  }

  if ((offset < 3u) || (offset >= (len - 2u))) {
    return false;
  }

  return (((offset - 3u) % 4u) < 2u);
}

#endif /* SI_FLOW_SHM_API_H */
