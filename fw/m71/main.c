/*
 * App M7_1: ruolo SAFE_B del virtual safe PLC (master FSoE #2, pubblica la meta'
 * dati del frame) e lato A della MU18 verso M7_0.
 *
 * Il codice sta in fw/common/flow/ (flow_main), identico per i tre ruoli; qui
 * solo il banner. RTT: canale 2 del control block condiviso (fw/common/rtt_shared.h).
 */
#include "board.h"
#include "flow.h"

int main(void)
{
    board_init();
    LOG("\n[%s] avvio: agente SCMI \"%s\", core %u MHz\n", BOARD_CORE_NAME, board_scmi_agent(),
        (unsigned)(SystemCoreClock / 1000000U));

    flow_main(); /* non ritorna */
}
