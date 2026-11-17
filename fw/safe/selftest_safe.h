/*
 * Scambio ICC fra i due core safe (payload in OCRAM + doorbell MU18 + CRC).
 *
 * Esercita in un colpo solo OCRAM, doorbell MU18, seq e i due CRC-32. Va
 * chiamata dopo la sincronizzazione con l'owner della mailbox ed e' l'unico
 * selftest specifico del ruolo safe: il fieldbus non ha un lato MU18.
 */
#ifndef SI_SAFE_SELFTEST_H
#define SI_SAFE_SELFTEST_H

#include "flow_types.h"

bool safe_selftest_icc(flow_core_id_t self);

#endif /* SI_SAFE_SELFTEST_H */
