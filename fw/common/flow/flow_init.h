/**
 * @file flow_init.h
 * @brief Inizializzazione comune del sottosistema flow, identica per i tre
 * ruoli: identita' del core, piattaforma (DWT, tick) e banner con la geometria
 * della memoria condivisa. L'ordine di inizializzazione di mailbox/ICC e il
 * selftest sono invece specifici del ruolo (fw/safe/safe_main.c,
 * fw/fieldbus/fieldbus_main.c).
 * @ingroup flow
 */
#ifndef SI_FLOW_INIT_H
#define SI_FLOW_INIT_H

#include "flow_types.h"

/** @brief Inizializzazione comune del sottosistema flow.
 *
 * Imposta l'identita' del core, inizializza la piattaforma (DWT, tick) e
 * stampa il banner con la geometria della memoria condivisa.
 *
 * @param[in] self     ruolo del core che esegue il boot;
 * @param[in] cycle_ms periodo del ruolo (10 ms safe, 2 ms fieldbus).
 * @pre Va chiamata una sola volta, prima di mailbox, ICC e selftest.
 */
void flow_init(flow_core_id_t self, flow_u32 cycle_ms);

#endif /* SI_FLOW_INIT_H */
