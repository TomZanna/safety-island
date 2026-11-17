/*
 * Ruolo fieldbus del virtual safe PLC (m33s).
 *
 * Fa l'A55 del diagramma: unisce le due meta' dei core safe, esegue lo slave
 * FSoE e genera il processo di campo. Non e' un componente di sicurezza.
 */
#ifndef SI_FIELDBUS_H
#define SI_FIELDBUS_H

#include "flow_types.h"

/* Non ritorna: ciclo infinito del core di campo. */
void fieldbus_main(void) FLOW_NORETURN;

#endif /* SI_FIELDBUS_H */
