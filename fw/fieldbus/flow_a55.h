/**
 * @file flow_a55.h
 * @brief Lato "A55" della mailbox: il ruolo FIELDBUS legge l'ultimo frame che
 * ENTRAMBI i core safe hanno pubblicato, unisce le due meta' e lo consegna allo
 * slave FSoE. Non prende decisioni di sicurezza e non ripara niente.
 * @ingroup flow_field
 *
 * Portato da imx-flow/bare-metal/a55/flow_a55.{c,h}.
 */
#ifndef SI_FLOW_A55_H
#define SI_FLOW_A55_H

#include "flow_config.h"
#include "flow_shm.h"
#include "flow_types.h"

typedef enum flow_a55_result
{
  FLOW_A55_OK = 0,
  FLOW_A55_NO_DATA,
  FLOW_A55_INVALID
} flow_a55_result_t;

/** @brief Frame completo assemblato dal fieldbus. */
typedef struct flow_a55_frame
{
  flow_u32 conn;
  flow_u32 seq;
  flow_u32 fc;
  flow_u32 len;
  flow_u8 pdu[FLOW_PDU_MAX];
} flow_a55_frame_t;

/** @brief Merge delle due meta del frame e contatore degli invalidi. */
typedef struct flow_a55
{
  flow_shm_t* shm;
  flow_u32 last_delivered[FLOW_NUM_CONNECTIONS];
  flow_u32 invalid_frames;
} flow_a55_t;

void flow_a55_init(flow_a55_t* a55, flow_shm_t* shm);

/** Prossimo frame completo (MIN dei due contatori + interleave delle due
 * meta'). */
flow_a55_result_t flow_a55_get_tx(flow_a55_t* a55,
                                  flow_u32 connection,
                                  flow_a55_frame_t* frame);

/** Pubblica la risposta del campo nella mailbox RX (una per ciclo safe). */
flow_a55_result_t flow_a55_put_rx(flow_a55_t* a55,
                                  flow_u32 connection,
                                  const flow_u8* pdu,
                                  flow_u32 len);

#endif /* SI_FLOW_A55_H */
