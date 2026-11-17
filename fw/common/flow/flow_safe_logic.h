/*
 * Logica safe eseguita da entrambi i core. Funzione pura: niente tempo, niente
 * I/O, niente casualita', cosi' SAFE_A e SAFE_B producono lo stesso risultato a
 * parita' di ingressi (e i cross-check hanno senso).
 *
 * Portata da imx-flow/bare-metal/common/flow_safe_logic.c.
 */
#ifndef SI_FLOW_SAFE_LOGIC_H
#define SI_FLOW_SAFE_LOGIC_H

#include "flow_config.h"
#include "flow_types.h"

typedef struct flow_safe_inputs
{
    flow_u16 ch_a; /* bit0 = E-stop OK, bit1 = porta chiusa, bit2 = reset */
    flow_u16 ch_b;
} flow_safe_inputs_t;

typedef struct flow_safe_outputs
{
    flow_u16 enable; /* FLOW_SAFE_OUTPUT_ENABLED oppure 0 (fail-safe) */
    flow_u16 status;
} flow_safe_outputs_t;

#define FLOW_SAFE_IN_ESTOP_OK 0x0001u
#define FLOW_SAFE_IN_DOOR_CLOSED 0x0002u
#define FLOW_SAFE_IN_RESET 0x0004u

#define FLOW_SAFE_STATUS_ENABLED 0x0001u
#define FLOW_SAFE_STATUS_LATCHED_FAULT 0x0002u
#define FLOW_SAFE_STATUS_DISCREPANCY 0x0004u
#define FLOW_SAFE_STATUS_ESTOP_OPEN 0x0008u
#define FLOW_SAFE_STATUS_DOOR_OPEN 0x0010u
#define FLOW_SAFE_STATUS_CH_A_OK 0x0020u
#define FLOW_SAFE_STATUS_CH_B_OK 0x0040u

typedef struct flow_safe_logic
{
    flow_u32 enabled;
    flow_u32 latched_fault;
    flow_u32 discrepancy_count;
    flow_u32 reset_previous;
    flow_u32 cycles;
} flow_safe_logic_t;

FLOW_STATIC_ASSERT(sizeof(flow_safe_inputs_t) == FLOW_SAFE_DATA_SIZE,
    "SafeInputs size must match the FSoE configuration");
FLOW_STATIC_ASSERT(sizeof(flow_safe_outputs_t) == FLOW_SAFE_DATA_SIZE,
    "SafeOutputs size must match the FSoE configuration");

void flow_safe_logic_init(flow_safe_logic_t *logic);

void flow_safe_logic_step(const flow_safe_inputs_t *inputs, flow_safe_logic_t *logic, flow_safe_outputs_t *outputs);

#endif /* SI_FLOW_SAFE_LOGIC_H */
