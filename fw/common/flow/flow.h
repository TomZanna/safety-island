/*
 * Entry point del sottosistema flow per i tre ruoli.
 *
 * Milestone M1 (bring-up): verifica della piattaforma e della memoria condivisa
 * (selftest, tick, DWT, MU18, heartbeat fra i core). I cicli FSoE veri arrivano
 * con M2 (libreria fsoe + flow_safe.c + flow_field.c + flow_icc.c).
 */
#ifndef SI_FLOW_H
#define SI_FLOW_H

#include "flow_types.h"

/* Non ritorna: ciclo infinito del ruolo scelto a compile time (FLOW_CORE_ID). */
void flow_main(void) FLOW_NORETURN;

#endif /* SI_FLOW_H */
