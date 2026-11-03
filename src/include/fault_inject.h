/*
 * fault_inject.h
 *
 * Fault injection used to validate the fail-safe paths of the architecture
 * (the `else` branches of the sequence diagram).
 *
 * A fault is armed for a given safe cycle (and repeated for `count` cycles)
 * so that the demonstration runs normally first and then shows the reaction.
 */

#ifndef FAULT_INJECT_H
#define FAULT_INJECT_H

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

typedef enum flow_fault_type
{
   FLOW_FAULT_NONE = 0,
   FLOW_FAULT_SESSION,  /**< M33 corrupts its Session ID random number      */
   FLOW_FAULT_PDU,      /**< M33 corrupts the generated FSoE PDU digest     */
   FLOW_FAULT_INPUTS,   /**< M33 corrupts the decoded SafeInputs            */
   FLOW_FAULT_LOGIC,    /**< M33 corrupts the computed SafeOutputs digest   */
   FLOW_FAULT_BUSDROP,  /**< A55 drops frames coming from the safe cores    */
} flow_fault_type_t;

typedef struct flow_fault_cfg
{
   flow_fault_type_t type;
   uint32_t first_cycle;
   uint32_t count;
   atomic_uint fired; /**< Number of times the fault actually fired */
} flow_fault_cfg_t;

const char * flow_fault_name (flow_fault_type_t type);
bool flow_fault_parse (const char * name, flow_fault_type_t * type);
bool flow_fault_expected (const flow_fault_cfg_t * cfg, flow_fault_type_t type);

void flow_fault_init (
   flow_fault_cfg_t * cfg,
   flow_fault_type_t type,
   uint32_t first_cycle,
   uint32_t count);

/** \brief true if the given fault must be injected in this cycle */
bool flow_fault_hits (
   const flow_fault_cfg_t * cfg,
   flow_fault_type_t type,
   uint32_t cycle);

/** \brief Register that the fault has been injected */
void flow_fault_note (flow_fault_cfg_t * cfg);

#endif /* FAULT_INJECT_H */
