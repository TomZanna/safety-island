/**
 * @file flow_types.h
 * @brief Tipi a larghezza fissa e identita' dei ruoli del virtual safe PLC.
 * @ingroup flow
 *
 * Regole per tutto cio' che passa dalla mailbox condivisa (flow_shm_api.h):
 *   - solo flow_u8 / flow_u16 / flow_u32;
 *   - niente size_t, puntatori, enum, bool o bit field;
 *   - strutture packed, con offset verificati a compile time.
 *
 * Portato da imx-flow/bare-metal/include/flow_types.h.
 */
#ifndef SI_FLOW_TYPES_H
#define SI_FLOW_TYPES_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t flow_u8;
typedef uint16_t flow_u16;
typedef uint32_t flow_u32;
typedef int32_t flow_s32;

#define FLOW_PACKED __attribute__((packed))
#define FLOW_NORETURN __attribute__((noreturn))
#define FLOW_UNUSED __attribute__((unused))
#define FLOW_INLINE static inline

#define FLOW_STATIC_ASSERT(condition, message) \
  _Static_assert(condition, message)

/** Ruoli: uno per core secondario. Il ruolo e' una proprieta' dell'app (lo
 * passa il main.c al punto di ingresso del ruolo), non una macro che seleziona
 * codice.
 *
 *   SAFE_A (m70, M7_0)  master FSoE #1: pubblica la meta' CRC del frame
 *   SAFE_B (m71, M7_1)  master FSoE #2: pubblica la meta' dati del frame
 *   FIELDBUS (m33s)     fa l'A55: unisce le due meta', esegue lo slave FSoE e
 *                       genera il processo di campo. Non e' un componente safe.
 */
#define FLOW_CORE_SAFE_A 0u
#define FLOW_CORE_SAFE_B 1u
#define FLOW_CORE_FIELDBUS 2u

#define FLOW_CORE_COUNT 3u /**< ruoli totali (heartbeat nella mailbox) */
#define FLOW_SAFE_CORE_COUNT 2u /**< core che pubblicano una meta' */

typedef flow_u8 flow_core_id_t;

/** Tag breve dei log RTT: "m70/A", "m71/B", "m33s/fld". */
FLOW_INLINE const char* flow_role_log_tag(flow_core_id_t role)
{
  switch (role) {
    case FLOW_CORE_SAFE_A:
      return "m70/A";
    case FLOW_CORE_SAFE_B:
      return "m71/B";
    case FLOW_CORE_FIELDBUS:
      return "m33s/fld";
    default:
      return "?";
  }
}

#endif /* SI_FLOW_TYPES_H */
