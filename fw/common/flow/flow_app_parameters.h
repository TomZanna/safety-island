/**
 * @file flow_app_parameters.h
 * @brief Parametri applicativi della connessione FSoE (i 2 x uint16 dello stato
 * Parameter). Sono uguali per tutti i ruoli: li verifica sia il master sia lo
 * slave (fsoeapp_verify_parameters), quindi vivono fuori da fw/safe e
 * fw/fieldbus.
 * @ingroup flow
 *
 */
#ifndef SI_FLOW_APP_PARAMETERS_H
#define SI_FLOW_APP_PARAMETERS_H

#include <stddef.h>

#include "flow_config.h"
#include "flow_types.h"

/** FSOEAPP_STATUS_OK (0) se i parametri sono quelli di flow_config.h. */
flow_u8 flow_app_parameters_verify(flow_u16 timeout_ms,
                                   const void* parameters,
                                   size_t size);

#endif /* SI_FLOW_APP_PARAMETERS_H */
