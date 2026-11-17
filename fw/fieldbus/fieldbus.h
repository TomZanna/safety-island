/**
 * @file fieldbus.h
 * @brief Ruolo fieldbus del virtual safe PLC (m33s).
 * @ingroup flow_field
 *
 * Fa l'A55 del diagramma: unisce le due meta' dei core safe, esegue lo slave
 * FSoE e genera il processo di campo. Non e' un componente di sicurezza.
 */
#ifndef SI_FIELDBUS_H
#define SI_FIELDBUS_H

#include "flow_types.h"

/** @brief Punto di ingresso del core di campo: non ritorna.
 *
 * Inizializza mailbox e ICC (e' l'owner), poi esegue il ciclo infinito a 2 ms:
 * apre il ciclo safe, serve lo slave FSoE e pubblica la risposta.
 */
void fieldbus_main(void) FLOW_NORETURN;

#endif /* SI_FIELDBUS_H */
