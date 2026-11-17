/*
 * Prove all'avvio (KAT) comuni ai tre ruoli.
 *
 * Come nel prototipo (imx-flow/src/app/self_test.c): se una prova fallisce il
 * sistema non parte. Qui si validano CRC, split/assemble del frame e le due
 * assunzioni hardware del porting:
 *   - la finestra di OCRAM condivisa (0x204D4000) e' scrivibile e leggibile da
 *     tutti e tre i core (patch SM 0002, TRDC/DID);
 *   - il link MU18 (M7_0 lato B <-> M7_1 lato A) risponde: e' lo scambio ICC in
 *     fw/safe/selftest_safe.h (solo i due core safe).
 */
#ifndef SI_FLOW_SELFTEST_H
#define SI_FLOW_SELFTEST_H

#include "flow_types.h"

/* Esegue tutte le prove comuni, logga ogni voce su RTT e ritorna true se sono
 * passate. */
bool flow_selftest_run(flow_core_id_t self);

/* Logga PASS/FAIL di una prova: la usano anche i selftest di ruolo. */
void flow_selftest_check(const char *name, bool ok);

#endif /* SI_FLOW_SELFTEST_H */
