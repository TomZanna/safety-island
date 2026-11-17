/*
 * Prove all'avvio (KAT) del sottosistema flow.
 *
 * Come nel prototipo (imx-flow/src/app/self_test.c): se una prova fallisce il
 * sistema non parte. Qui serve anche a validare le due assunzioni hardware del
 * porting, prima di avere il codice FSoE:
 *   - la finestra di OCRAM condivisa (0x204D4000) e' scrivibile e leggibile da
 *     tutti e tre i core (patch SM 0002, TRDC/DID);
 *   - il link MU18 (M7_0 lato B <-> M7_1 lato A) risponde.
 */
#ifndef SI_FLOW_SELFTEST_H
#define SI_FLOW_SELFTEST_H

#include "flow_types.h"

/* Esegue tutte le prove, logga ogni voce su RTT e ritorna true se sono passate. */
bool flow_selftest_run(void);

/* Scambio ICC fra i due core safe (payload in OCRAM + doorbell MU18 + CRC). Va
 * chiamata dopo la sincronizzazione con l'owner della mailbox. Sul fieldbus e'
 * uno skip che ritorna true. */
bool flow_selftest_icc(void);

#endif /* SI_FLOW_SELFTEST_H */
