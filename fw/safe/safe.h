/**
 * @file safe.h
 * @brief Ruolo safe del virtual safe PLC: SAFE_A (m70) e SAFE_B (m71).
 * @ingroup flow_safe
 *
 * Un solo punto di ingresso per entrambi; il ruolo e' l'argomento, non una
 * macro: ogni app linka questo codice e passa la propria identita' dal main.c.
 * Il codice e' in fw/safe/ (ruolo) + fw/common/flow/ (protocolli condivisi).
 */
#ifndef SI_SAFE_H
#define SI_SAFE_H

#include "flow_types.h"

/** Non ritorna: ciclo infinito del core safe. self = FLOW_CORE_SAFE_A (m70) o
 * FLOW_CORE_SAFE_B (m71). */
void safe_main(flow_core_id_t self) FLOW_NORETURN;

#endif /* SI_SAFE_H */
