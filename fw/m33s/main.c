/*
 * App M33_1: ruolo FIELDBUS del virtual safe PLC. Fa l'A55 del diagramma: unisce
 * le due meta' dei core safe (MIN dei contatori + interleave), esegue lo slave
 * FSoE e genera il processo di campo. Non e' un componente di sicurezza.
 *
 * Il codice sta in fw/common/flow/ (flow_main), identico per i tre ruoli; qui
 * solo il banner. RTT: canale 0 del control block condiviso (fw/common/rtt_shared.h).
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
