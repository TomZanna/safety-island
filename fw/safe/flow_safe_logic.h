/**
 * @file flow_safe_logic.h
 * @brief Logica safe eseguita da entrambi i core. Funzione pura: niente tempo,
 * niente I/O, niente casualita', cosi' SAFE_A e SAFE_B producono lo stesso
 * risultato a parita' di ingressi (e i cross-check hanno senso).
 * @ingroup flow_safe
 *
 * I tipi di ingresso/uscita (condivisi con il fieldbus) stanno in
 * fw/common/flow/flow_safe_io.h.
 *
 * Ingressi (`flow_safe_inputs_t`):
 *   - `ch_a`/`ch_b` bit 0: E-stop rilasciato (necessario per abilitare);
 *   - `ch_a`/`ch_b` bit 1: porta chiusa (necessario per abilitare);
 *   - `ch_a` bit 2: pulsante di reset (fronte di salita: abilita e azzera il
 * latch).
 *
 * Uscite: `enable = 0x00C3` quando le condizioni sono soddisfatte, `0x0000` in
 * fail-safe; `status` porta i bit di diagnostica (abilitato, fault latched,
 * discrepanza, E-stop aperto, porta aperta, canale A/B ok).
 *
 * La valutazione e' 1oo2 sui due canali con monitoraggio di discrepanza: tre
 * cicli consecutivi diversi producono un fault *latchato*, che si azzera solo
 * con un reset a condizioni di sicurezza soddisfatte. Qualsiasi perdita delle
 * condizioni toglie subito l'uscita.
 *
 * Portata da imx-flow/bare-metal/common/flow_safe_logic.c.
 */
#ifndef SI_FLOW_SAFE_LOGIC_H
#define SI_FLOW_SAFE_LOGIC_H

#include "flow_config.h"
#include "flow_safe_io.h"
#include "flow_types.h"

/** Stato della logica safe (una istanza per connessione). */
typedef struct flow_safe_logic
{
  flow_u32 enabled; /**< 1 se le condizioni di sicurezza sono soddisfatte */
  flow_u32 latched_fault; /**< fault latchato: serve un reset */
  flow_u32 discrepancy_count; /**< cicli consecutivi con i due canali diversi */
  flow_u32 reset_previous; /**< fronte del reset al ciclo precedente */
  flow_u32 cycles; /**< cicli eseguiti (telemetria) */
} flow_safe_logic_t;

/** @brief Azzera lo stato della logica, latch compreso.
 *
 * @param[out] logic stato della logica di una connessione.
 */
void flow_safe_logic_init(flow_safe_logic_t* logic);

/** @brief Un ciclo di logica safe: 1oo2, latch su discrepanza, reset su fronte.
 *
 * @param[in]     inputs  ingressi decodificati dal frame FSoE;
 * @param[in,out] logic   stato (abilitazione, latch, fronte del reset);
 * @param[out]    outputs uscite calcolate: enable 0 se le condizioni non sono
 *                        soddisfatte.
 * @note Funzione pura: niente tempo, I/O o casualita', cosi' i due core
 *       producono lo stesso risultato a parita' di ingressi.
 */
void flow_safe_logic_step(const flow_safe_inputs_t* inputs,
                          flow_safe_logic_t* logic,
                          flow_safe_outputs_t* outputs);

#endif /* SI_FLOW_SAFE_LOGIC_H */
