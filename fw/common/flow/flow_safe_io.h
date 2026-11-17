/**
 * @file flow_safe_io.h
 * @brief Ingressi e uscite di sicurezza scambiati con il campo.
 * @ingroup flow_safe
 *
 * Sono tipi condivisi fra i due lati: i core safe li decodificano dal frame FSoE,
 * il fieldbus li genera e li pubblica. La funzione di sicurezza pura che li
 * elabora sta invece in fw/safe/flow_safe_logic.[ch].
 */
#ifndef SI_FLOW_SAFE_IO_H
#define SI_FLOW_SAFE_IO_H

#include "flow_config.h"
#include "flow_types.h"

/** @brief Ingressi di sicurezza decodificati (4 byte). */
typedef struct flow_safe_inputs
{
    flow_u16 ch_a; /**< bit0 = E-stop OK, bit1 = porta chiusa, bit2 = reset */
    flow_u16 ch_b;
} flow_safe_inputs_t;

/** @brief Uscite di sicurezza calcolate (4 byte). */
typedef struct flow_safe_outputs
{
    flow_u16 enable; /**< FLOW_SAFE_OUTPUT_ENABLED oppure 0 (fail-safe) */
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

FLOW_STATIC_ASSERT(sizeof(flow_safe_inputs_t) == FLOW_SAFE_DATA_SIZE,
    "SafeInputs size must match the FSoE configuration");
FLOW_STATIC_ASSERT(sizeof(flow_safe_outputs_t) == FLOW_SAFE_DATA_SIZE,
    "SafeOutputs size must match the FSoE configuration");

#endif /* SI_FLOW_SAFE_IO_H */
