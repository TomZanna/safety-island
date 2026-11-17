/**
 * @file safe_platform.h
 * @brief Aggancio alla piattaforma del ruolo safe.
 * @ingroup flow_safe
 *
 * La MU18 ha un lato per M7: lato B su M7_0 (SAFE_A), lato A su M7_1 (SAFE_B).
 * E' un vincolo hardware, quindi la mappa sta qui e non nel driver
 * (fw/common/flow/flow_mu18.c), che resta generico.
 */
#ifndef SI_SAFE_PLATFORM_H
#define SI_SAFE_PLATFORM_H

#include "flow_types.h"

/** @brief Lega la MU18 al lato fisico del core safe.
 *
 * SAFE_A (M7_0) usa il lato B, SAFE_B (M7_1) il lato A: e' un vincolo
 * hardware, per questo la mappa sta qui e non nel driver generico.
 *
 * @param[in] self ruolo del core.
 * @note Un ruolo diverso dai due safe e' una condizione fatale.
 */
void safe_platform_init_mu18(flow_core_id_t self);

#endif /* SI_SAFE_PLATFORM_H */
