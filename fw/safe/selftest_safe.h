/**
 * @file selftest_safe.h
 * @brief Scambio ICC fra i due core safe (payload in OCRAM + doorbell MU18 +
 * CRC).
 * @ingroup flow_safe
 *
 * Esercita in un colpo solo OCRAM, doorbell MU18, seq e i due CRC-32. Va
 * chiamata dopo la sincronizzazione con l'owner della mailbox ed e' l'unico
 * selftest specifico del ruolo safe: il fieldbus non ha un lato MU18.
 */
#ifndef SI_SAFE_SELFTEST_H
#define SI_SAFE_SELFTEST_H

#include "flow_types.h"

/** @brief Scambio ICC di prova fra i due core safe.
 *
 * Esercita OCRAM, doorbell MU18, sequence e i due CRC-32 pubblicando un probe
 * e attendendo quello del peer (fino a 50 tentativi).
 *
 * @param[in] self ruolo del core che esegue il selftest.
 * @return true se il peer ha risposto con il probe atteso.
 * @pre Va chiamata dopo la sincronizzazione con l'owner della mailbox.
 */
bool safe_selftest_icc(flow_core_id_t self);

#endif /* SI_SAFE_SELFTEST_H */
