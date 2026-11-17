/**
 * @file flow_safe.h
 * @brief Un core safe (m70 = SAFE_A, m71 = SAFE_B): stessa sorgente per entrambi; il ruolo lo passa il main.c dell'app (safe_main), non una macro di build. Ogni core ha la sua istanza dello stack FSoE master, il suo stato di sequenza MU e i suoi canali TX nella mailbox.
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

#include "flow_config.h"
#include "flow_fsoe_app.h"
#include "flow_mu.h"
#include "flow_safe_io.h"
#include "flow_safe_logic.h"
#include "flow_shm.h"
#include "flow_types.h"

#include <fsoemaster.h>

typedef struct flow_safe flow_safe_t;

/** Riferimento passato a fsoemaster_init(): l'API rt-labs lo consegna alle
 * callback come `void *`. Tipo proprio del ruolo safe (in questo binario non
 * esiste lo slave), quindi nessun dispatch a runtime. */
typedef struct safe_fsoe_ref
{
    flow_u32 magic;
    flow_u32 k;
    flow_safe_t *safe;
} safe_fsoe_ref_t;

/** Copia locale di un frame RX/TX (passo 1 del diagramma). */
typedef struct flow_fsoe_slot
{
    flow_u8 pdu[FLOW_PDU_MAX];
    flow_u32 len;
    flow_u32 fc;
} flow_fsoe_slot_t;

/** Fault injection (solo con FLOW_DEBUG): un ciclo, sul ciclo richiesto da RTT. */
typedef enum flow_test_fault
{
    FLOW_TEST_FAULT_NONE = 0,
    FLOW_TEST_FAULT_PDU,
    FLOW_TEST_FAULT_INPUTS,
    FLOW_TEST_FAULT_LOGIC,
    FLOW_TEST_FAULT_SESSION
} flow_test_fault_t;

/** @brief Stato completo di un core safe (stack, mailbox, cross-check, statistiche). */
typedef struct flow_safe
{
    flow_core_id_t id;
    flow_shm_t *shm;

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
} flow_safe_t;

void flow_safe_init(flow_safe_t *safe, flow_core_id_t id, flow_shm_t *shm);

/** Un ciclo completo (passi 1..4). `cycle` viene dalla barrier. */
void flow_safe_step(flow_safe_t *safe, flow_u32 cycle);

/** Fault injection: vale sul ciclo `fault_cycle`, poi si azzera. */
void flow_safe_set_fault(flow_safe_t *safe, flow_test_fault_t fault, flow_u32 fault_cycle);

/** Callback usate dal glue FSoE. */
void flow_safe_on_egress(flow_safe_t *safe, flow_u32 k, const void *buffer, size_t size);
size_t flow_safe_on_recv(flow_safe_t *safe, flow_u32 k, void *buffer, size_t size);
flow_u16 flow_safe_on_session(flow_safe_t *safe, flow_u32 k);

#endif /* SI_FLOW_SAFE_H */
