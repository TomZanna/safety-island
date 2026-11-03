/*
 * safe_logic.h
 *
 * The "mini safe logic" executed, in lockstep, by both safety cores at every
 * safe cycle (diagram step 4).
 *
 * It is a pure, deterministic function of (SafeInputs, previous state): no
 * time, no I/O, no randomness. That is what makes the M7/M33 output digests
 * comparable.
 *
 * Function: 1oo2 evaluation of a two channel E-stop + guard door with
 * discrepancy detection, latched fault and manual reset. The safe output word
 * is 0x0000 (fail-safe) unless the safety function is enabled.
 */

#ifndef SAFE_LOGIC_H
#define SAFE_LOGIC_H

#include "flow_config.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct flow_safe_inputs
{
   uint16_t ch_a; /**< Channel A: bit0 = E-stop OK, bit1 = door closed,
                   *   bit2 = reset push button                            */
   uint16_t ch_b; /**< Channel B: same encoding, independent channel       */
} flow_safe_inputs_t;

typedef struct flow_safe_outputs
{
   uint16_t enable; /**< FLOW_SAFE_OUTPUT_ENABLED or 0x0000 (fail-safe)      */
   uint16_t status; /**< Diagnostic status word                             */
} flow_safe_outputs_t;

/* Input bit assignment (per channel) */
#define FLOW_SAFE_IN_ESTOP_OK 0x0001u
#define FLOW_SAFE_IN_DOOR_CLOSED 0x0002u
#define FLOW_SAFE_IN_RESET 0x0004u

/* Status word bit assignment */
#define FLOW_SAFE_STATUS_ENABLED 0x0001u
#define FLOW_SAFE_STATUS_LATCHED_FAULT 0x0002u
#define FLOW_SAFE_STATUS_DISCREPANCY 0x0004u
#define FLOW_SAFE_STATUS_ESTOP_OPEN 0x0008u
#define FLOW_SAFE_STATUS_DOOR_OPEN 0x0010u
#define FLOW_SAFE_STATUS_CH_A_OK 0x0020u
#define FLOW_SAFE_STATUS_CH_B_OK 0x0040u

/* Internal state of the safety function */
typedef struct flow_safe_logic
{
   uint32_t enabled;
   uint32_t latched_fault;
   uint32_t discrepancy_count;
   uint32_t reset_previous;
   uint32_t cycles;
} flow_safe_logic_t;

void flow_safe_logic_init (flow_safe_logic_t * logic);

/**
 * \brief Execute the safety function for one cycle
 *
 * \param[in]     inputs   SafeInputs received from the field (through FSoE)
 * \param[in,out] logic    Internal state (must be equal on both cores)
 * \param[out]    outputs  SafeOutputs to be sent to the field (through FSoE)
 */
void flow_safe_logic_step (
   const flow_safe_inputs_t * inputs,
   flow_safe_logic_t * logic,
   flow_safe_outputs_t * outputs);

/** \brief Human readable description of the outputs, for logging */
const char * flow_safe_logic_describe (
   const flow_safe_outputs_t * outputs,
   char * buffer,
   unsigned size);

#endif /* SAFE_LOGIC_H */
