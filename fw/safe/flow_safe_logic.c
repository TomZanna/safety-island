/**
 * @file flow_safe_logic.c
 * @brief Logica safe eseguita da entrambi i core. Funzione pura: niente tempo,
 * niente I/O, niente casualita', cosi' SAFE_A e SAFE_B producono lo stesso
 * risultato a parita' di ingressi (e i cross-check hanno senso).
 * @ingroup flow_safe
 *
 */
#include "flow_safe_logic.h"

#include "flow_mem.h"

void flow_safe_logic_init(flow_safe_logic_t* logic)
{
  if (logic != NULL) {
    flow_memset(logic, 0, (flow_u32)sizeof(*logic));
  }
}

void flow_safe_logic_step(const flow_safe_inputs_t* inputs,
                          flow_safe_logic_t* logic,
                          flow_safe_outputs_t* outputs)
{
  const bool estop_a = (inputs->ch_a & FLOW_SAFE_IN_ESTOP_OK) != 0u;
  const bool estop_b = (inputs->ch_b & FLOW_SAFE_IN_ESTOP_OK) != 0u;
  const bool door_a = (inputs->ch_a & FLOW_SAFE_IN_DOOR_CLOSED) != 0u;
  const bool door_b = (inputs->ch_b & FLOW_SAFE_IN_DOOR_CLOSED) != 0u;
  const bool reset = (inputs->ch_a & FLOW_SAFE_IN_RESET) != 0u;
  const bool discrepancy = (estop_a != estop_b) || (door_a != door_b);
  bool safe_conditions;

  logic->cycles++;

  /* Monitoraggio di discrepanza 1oo2. */
  if (discrepancy) {
    logic->discrepancy_count++;
    if (logic->discrepancy_count >= FLOW_DISCREPANCY_CYCLES) {
      logic->latched_fault = 1u;
    }
  } else {
    logic->discrepancy_count = 0u;
  }

  safe_conditions =
      estop_a && estop_b && door_a && door_b && (logic->latched_fault == 0u);

  /* Reset manuale su fronte di salita, accettato solo con le condizioni di
   * sicurezza soddisfatte; azzera anche il fault latched. */
  if (reset && (logic->reset_previous == 0u)) {
    if (estop_a && estop_b && door_a && door_b) {
      logic->latched_fault = 0u;
      logic->discrepancy_count = 0u;
      logic->enabled = 1u;
    }
  }
  logic->reset_previous = reset ? 1u : 0u;

  /* Qualsiasi perdita delle condizioni toglie subito l'uscita. */
  if (!safe_conditions) {
    logic->enabled = 0u;
  }

  outputs->enable = (logic->enabled != 0u) ? FLOW_SAFE_OUTPUT_ENABLED : 0x0000u;
  outputs->status = 0u;

  if (logic->enabled != 0u) {
    outputs->status |= FLOW_SAFE_STATUS_ENABLED;
  }
  if (logic->latched_fault != 0u) {
    outputs->status |= FLOW_SAFE_STATUS_LATCHED_FAULT;
  }
  if (discrepancy) {
    outputs->status |= FLOW_SAFE_STATUS_DISCREPANCY;
  }
  if (!estop_a || !estop_b) {
    outputs->status |= FLOW_SAFE_STATUS_ESTOP_OPEN;
  }
  if (!door_a || !door_b) {
    outputs->status |= FLOW_SAFE_STATUS_DOOR_OPEN;
  }
  if (estop_a && door_a) {
    outputs->status |= FLOW_SAFE_STATUS_CH_A_OK;
  }
  if (estop_b && door_b) {
    outputs->status |= FLOW_SAFE_STATUS_CH_B_OK;
  }
}
