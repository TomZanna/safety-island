/**
 * @file flow_safe.h
 * @brief Un core safe (m70 = SAFE_A, m71 = SAFE_B): stessa sorgente per
 * entrambi; il ruolo lo passa il main.c dell'app (safe_main), non una macro di
 * build. Ogni core ha la sua istanza dello stack FSoE master, il suo stato di
 * sequenza MU e i suoi canali TX nella mailbox.
 * @ingroup flow_safe
 *
 * Un ciclo (flow_safe_step) esegue quattro passi in ordine:
 *   1. latcha i frame RX pubblicati dal FIELDBUS nella mailbox;
 *   2. sincronizza lo stack FSoE: dentro fsoeapp_send() fa il cross-check del
 *      PDU in uscita e pubblica la propria meta' del frame;
 *   3. confronta con il peer le SafeInputs decodificate;
 *   4. esegue la logica safe e confronta con il peer le SafeOutputs.
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
  flow_u32 magic; /**< FLOW_FSOE_REF_MAGIC: guardia sull'app_ref */
  flow_u32 k; /**< indice della connessione */
  flow_safe_t* safe; /**< stato del core (mai NULL) */
} safe_fsoe_ref_t;

/** Copia locale di un frame RX/TX (passo 1 del diagramma). */
typedef struct flow_fsoe_slot
{
  flow_u8 pdu[FLOW_PDU_MAX]; /**< frame FSoE */
  flow_u32 len; /**< byte validi del frame */
  flow_u32 fc; /**< frame counter del black channel */
} flow_fsoe_slot_t;

/** Fault injection (solo con FLOW_DEBUG): vale per il ciclo scelto da RTT. */
typedef enum flow_test_fault
{
  FLOW_TEST_FAULT_NONE = 0, /**< nessun fault armato */
  /** Corrompe il PDU in uscita nel ciclo scelto. */
  FLOW_TEST_FAULT_PDU,
  /** Corrompe le SafeInputs nel ciclo scelto. */
  FLOW_TEST_FAULT_INPUTS,
  /** Corrompe le SafeOutputs nel ciclo scelto. */
  FLOW_TEST_FAULT_LOGIC,
  /** Forza un Session ID diverso alla prossima negoziazione. */
  FLOW_TEST_FAULT_SESSION
} flow_test_fault_t;

/** @brief Stato completo di un core safe (stack, mailbox, cross-check,
 * statistiche).
 *
 * Una sola istanza per core, allocata staticamente dall'app: niente
 * allocazione dinamica e il puntatore non cambia mai. `shm` e' la mailbox
 * condivisa, tutto il resto e' RAM privata del core.
 */
struct flow_safe
{
  flow_core_id_t id; /**< ruolo: SAFE_A (m70) o SAFE_B (m71) */
  flow_shm_t* shm; /**< mailbox di cui pubblica la propria meta' */

  flow_mu_state_t mu; /**< sequenze TX/RX del cross-check 1oo2 */

  /** Un master FSoE e il suo riferimento per connessione. */
  fsoemaster_t master[FLOW_NUM_CONNECTIONS];
  safe_fsoe_ref_t app_ref[FLOW_NUM_CONNECTIONS];

  /** Copia locale dell'ultimo frame RX. `rx_ready` dice se e' nuovo e non
   * ancora consegnato allo stack, `rx_count` da quale pubblicazione arriva. */
  flow_fsoe_slot_t rx_slot[FLOW_NUM_CONNECTIONS];
  bool rx_ready[FLOW_NUM_CONNECTIONS];
  flow_u32 rx_count[FLOW_NUM_CONNECTIONS];
  /** Frame pubblicati nella propria meta' TX: resta allineato al peer e su
   * divergenza si riallinea al MIN (poi si chiede il reset). */
  flow_u32 tx_count[FLOW_NUM_CONNECTIONS];

  flow_safe_inputs_t raw_inputs[FLOW_NUM_CONNECTIONS]; /**< dal frame FSoE */
  flow_safe_inputs_t safe_inputs[FLOW_NUM_CONNECTIONS]; /**< cross-check ok */
  flow_safe_outputs_t safe_outputs[FLOW_NUM_CONNECTIONS]; /**< verso lo stack */
  flow_safe_logic_t logic[FLOW_NUM_CONNECTIONS]; /**< latch e reset */
  fsoemaster_syncstatus_t sync[FLOW_NUM_CONNECTIONS]; /**< stato FSoE */
  bool send_enabled[FLOW_NUM_CONNECTIONS]; /**< flag inviato allo stack */

  flow_u32 cycle; /**< ciclo aperto dalla barrier */
  flow_u32 anomalies; /**< anomalie cumulate (allineate al MAX col peer) */
  bool cycle_fault; /**< un'anomalia ha gia' portato il ciclo in fail-safe */
  flow_u32 session_id; /**< Session ID negoziato (somma dei due random) */

  flow_test_fault_t fault; /**< fault injection armato (solo FLOW_DEBUG) */
  flow_u32 fault_cycle; /**< ciclo in cui scatta; 0 = non armato */

  /** Telemetria del core, azzerata all'init e stampata da log_status(). */
  struct
  {
    flow_u32 cycles; /**< cicli eseguiti */
    flow_u32 data_cycles; /**< cicli in DATA con dati di processo */
    flow_u32 tx_published; /**< meta' TX pubblicate nella mailbox */
    flow_u32 pdu_mismatches; /**< PDU in uscita diverso dal peer */
    flow_u32 inputs_mismatches; /**< SafeInputs o contatori divergenti */
    flow_u32 logic_mismatches; /**< SafeOutputs o logica divergenti */
    flow_u32 reset_requests; /**< reset chiesti allo stack FSoE */
    flow_u32 rx_latch_failures; /**< slot RX non valido o taglia errata */
    flow_u32 session_requests; /**< negoziazioni di Session ID avviate */
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
