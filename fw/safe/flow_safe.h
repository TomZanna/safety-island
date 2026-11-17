/**
 * @file flow_safe.h
 * @brief Un core safe (m70 = SAFE_A, m71 = SAFE_B): stessa sorgente per
 * entrambi; il ruolo lo passa il main.c dell'app (safe_main), non una macro di
 * build. Ogni core ha la sua istanza dello stack FSoE master, il suo stato di
 * sequenza MU e i suoi canali TX nella mailbox.
 * @ingroup flow_safe
 *
 * I quattro passi del diagramma (flow_safe_step) sono:
 *   1. latch dei frame RX dalla mailbox
 *   2. sync con lo stack FSoE; dentro fsoeapp_send() -> cross-check del PDU in
 *      uscita e pubblicazione della propria meta' del frame
 *   3. cross-check delle SafeInputs decodificate
 *   4. logica safe e cross-check delle SafeOutputs
 *
 * Portato da imx-flow/bare-metal/safe/flow_safe.c, con il ciclo preso dalla
 * barrier (flow_icc) e il fault injection comandato da RTT.
 */
#ifndef SI_FLOW_SAFE_H
#define SI_FLOW_SAFE_H

#include <fsoemaster.h>

#include "flow_config.h"
#include "flow_fsoe_app.h"
#include "flow_mu.h"
#include "flow_safe_io.h"
#include "flow_safe_logic.h"
#include "flow_shm.h"
#include "flow_types.h"

typedef struct flow_safe flow_safe_t;

/** Riferimento passato a fsoemaster_init(): l'API rt-labs lo consegna alle
 * callback come `void *`. Tipo proprio del ruolo safe (in questo binario non
 * esiste lo slave), quindi nessun dispatch a runtime. */
typedef struct safe_fsoe_ref
{
  flow_u32 magic;
  flow_u32 k;
  flow_safe_t* safe;
} safe_fsoe_ref_t;

/** Copia locale di un frame RX/TX (passo 1 del diagramma). */
typedef struct flow_fsoe_slot
{
  flow_u8 pdu[FLOW_PDU_MAX];
  flow_u32 len;
  flow_u32 fc;
} flow_fsoe_slot_t;

/** Fault injection (solo con FLOW_DEBUG): un ciclo, sul ciclo richiesto da RTT.
 */
typedef enum flow_test_fault
{
  FLOW_TEST_FAULT_NONE = 0,
  FLOW_TEST_FAULT_PDU,
  FLOW_TEST_FAULT_INPUTS,
  FLOW_TEST_FAULT_LOGIC,
  FLOW_TEST_FAULT_SESSION
} flow_test_fault_t;

/** @brief Stato completo di un core safe (stack, mailbox, cross-check,
 * statistiche). */
struct flow_safe
{
  flow_core_id_t id;
  flow_shm_t* shm;

  flow_mu_state_t mu;

  fsoemaster_t master[FLOW_NUM_CONNECTIONS];
  safe_fsoe_ref_t app_ref[FLOW_NUM_CONNECTIONS];

  /** Copia in RAM locale dei frame RX. */
  flow_fsoe_slot_t rx_slot[FLOW_NUM_CONNECTIONS];
  bool rx_ready[FLOW_NUM_CONNECTIONS];
  flow_u32 rx_count[FLOW_NUM_CONNECTIONS];
  flow_u32 tx_count[FLOW_NUM_CONNECTIONS];

  flow_safe_inputs_t raw_inputs[FLOW_NUM_CONNECTIONS];
  flow_safe_inputs_t safe_inputs[FLOW_NUM_CONNECTIONS];
  flow_safe_outputs_t safe_outputs[FLOW_NUM_CONNECTIONS];
  flow_safe_logic_t logic[FLOW_NUM_CONNECTIONS];
  fsoemaster_syncstatus_t sync[FLOW_NUM_CONNECTIONS];
  bool send_enabled[FLOW_NUM_CONNECTIONS];

  flow_u32 cycle;
  flow_u32 anomalies;
  bool cycle_fault;
  flow_u32 session_id;

  flow_test_fault_t fault;
  flow_u32 fault_cycle;

  struct
  {
    flow_u32 cycles;
    flow_u32 data_cycles;
    flow_u32 tx_published;
    flow_u32 pdu_mismatches;
    flow_u32 inputs_mismatches;
    flow_u32 logic_mismatches;
    flow_u32 reset_requests;
    flow_u32 rx_latch_failures;
    flow_u32 session_requests;
  } stats;
};

/** @brief Inizializza lo stato del core safe e gli stack FSoE master.
 *
 * @param[out] safe stato del core;
 * @param[in]  id   identita' del ruolo (SAFE_A su m70, SAFE_B su m71);
 * @param[out] shm  mailbox condivisa di cui il core pubblica la propria meta'.
 * @note Se `fsoemaster_init` fallisce il core va in stato fatale (fail-safe).
 */
void flow_safe_init(flow_safe_t* safe, flow_core_id_t id, flow_shm_t* shm);

/** @brief Un ciclo completo: latch RX, sync FSoE, cross-check, logica safe.
 *
 * @param[in,out] safe  stato del core;
 * @param[in]     cycle ciclo aperto dalla barrier del FIELDBUS.
 * @note I quattro passi sono descritti in flow_safe.h; ogni anomalia porta al
 *       fail-safe del ciclo, senza attendere il ciclo successivo.
 */
void flow_safe_step(flow_safe_t* safe, flow_u32 cycle);

/** @brief Arma un fault injection per il ciclo indicato.
 *
 * @param[out] safe        stato del core;
 * @param[in]  fault       tipo di fault da iniettare;
 * @param[in]  fault_cycle ciclo in cui vale (0 = non armato; SESSION vale per
 *                         la prossima negoziazione).
 * @note Attivo solo con FLOW_DEBUG; si azzera dopo l'uso.
 */
void flow_safe_set_fault(flow_safe_t* safe,
                         flow_test_fault_t fault,
                         flow_u32 fault_cycle);

/** @brief Callback di trasmissione: cross-check del PDU e pubblicazione.
 *
 * @param[in,out] safe   stato del core;
 * @param[in]     k      indice della connessione;
 * @param[in]     buffer frame FSoE generato dallo stack;
 * @param[in]     size   lunghezza del frame.
 * @note Pubblica solo la propria meta' del frame: l'assemblaggio lo fa il
 *       FIELDBUS.
 */
void flow_safe_on_egress(flow_safe_t* safe,
                         flow_u32 k,
                         const void* buffer,
                         size_t size);

/** @brief Callback di ricezione: consegna il frame RX latched allo stack.
 *
 * @param[in,out] safe   stato del core;
 * @param[in]     k      indice della connessione;
 * @param[out]    buffer buffer dello stack da riempire;
 * @param[in]     size   capacita' di `buffer`.
 * @return byte consegnati; 0 se non c'e' un frame nuovo o la taglia non
 *         coincide.
 */
size_t flow_safe_on_recv(flow_safe_t* safe,
                         flow_u32 k,
                         void* buffer,
                         size_t size);

/** @brief Negozia il Session ID con il peer (somma dei due numeri casuali).
 *
 * @param[in,out] safe stato del core;
 * @param[in]     k    indice della connessione.
 * @return Session ID concordato; 0 se la negoziazione fallisce (il chiamante
 *         richiede un reset).
 */
flow_u16 flow_safe_on_session(flow_safe_t* safe, flow_u32 k);

#endif /* SI_FLOW_SAFE_H */
