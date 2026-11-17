/*
 * Logica safe eseguita da entrambi i core. Funzione pura: niente tempo, niente
 * I/O, niente casualita', cosi' SAFE_A e SAFE_B producono lo stesso risultato a
 * parita' di ingressi (e i cross-check hanno senso).
 *
 * I tipi di ingresso/uscita (condivisi con il fieldbus) stanno in
 * fw/common/flow/flow_safe_io.h.
 *
 * Portata da imx-flow/bare-metal/common/flow_safe_logic.c.
 */
#ifndef SI_FLOW_SAFE_LOGIC_H
#define SI_FLOW_SAFE_LOGIC_H

#include "flow_config.h"
#include "flow_safe_io.h"
#include "flow_types.h"

typedef struct flow_safe_logic
{
    flow_u32 enabled;
    flow_u32 latched_fault;
    flow_u32 discrepancy_count;
    flow_u32 reset_previous;
    flow_u32 cycles;
} flow_safe_logic_t;

void flow_safe_logic_init(flow_safe_logic_t *logic);

void flow_safe_logic_step(const flow_safe_inputs_t *inputs, flow_safe_logic_t *logic, flow_safe_outputs_t *outputs);

#endif /* SI_FLOW_SAFE_LOGIC_H */
