/**
 * @file main.c
 * @brief App M7_1: ruolo SAFE_B del virtual safe PLC (master FSoE #2, pubblica
 * la meta' dati del frame) e lato A della MU18 verso M7_0.
 * @ingroup flow
 *
 * Qui solo il wiring dell'app: board_init, banner e punto di ingresso del ruolo
 * (fw/safe/safe_main.c). RTT: canale 2 del control block condiviso
 * (fw/common/rtt_shared.h).
 */
#include "board.h"
#include "safe.h"

int main(void)
{
  board_init();
  LOG("\n[%s] avvio: agente SCMI \"%s\", core %u MHz\n",
      BOARD_CORE_NAME,
      board_scmi_agent(),
      (unsigned)(SystemCoreClock / 1000000U));

  safe_main(FLOW_CORE_SAFE_B); /* non ritorna */
}
