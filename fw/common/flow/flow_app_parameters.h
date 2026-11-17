/**
 * @file flow_app_parameters.h
 * @brief Parametri applicativi della connessione FSoE (i 2 x uint16 dello stato
 * Parameter). Sono uguali per tutti i ruoli: li verifica sia il master sia lo
 * slave (fsoeapp_verify_parameters), quindi vivono fuori da fw/safe e
 * fw/fieldbus.
 * @ingroup flow
 */
#ifndef SI_FLOW_APP_PARAMETERS_H
#define SI_FLOW_APP_PARAMETERS_H

#include <stddef.h>

#include "flow_config.h"
#include "flow_types.h"

/** @brief Verifica i parametri applicativi della connessione FSoE.
 *
 * @param[in] timeout_ms timeout del watchdog dichiarato dallo stack (non usato:
 *                       i parametri non dipendono dal tempo);
 * @param[in] parameters due uint16 little endian (4 byte);
 * @param[in] size       dimensione di `parameters`.
 * @return FSOEAPP_STATUS_OK se coincidono con flow_config.h, altrimenti
 *         FSOEAPP_STATUS_BAD_APP_PARAMETER.
 */
flow_u8 flow_app_parameters_verify(flow_u16 timeout_ms,
                                   const void* parameters,
                                   size_t size);

#endif /* SI_FLOW_APP_PARAMETERS_H */
