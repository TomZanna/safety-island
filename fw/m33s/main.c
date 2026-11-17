/**
 * @file main.c
 * @brief App M33_1: ruolo FIELDBUS del virtual safe PLC. Fa l'A55 del diagramma: unisce le due meta' dei core safe (MIN dei contatori + interleave), esegue lo slave FSoE e genera il processo di campo. Non e' un componente di sicurezza.
 * @ingroup flow
 *
 * Qui solo il wiring dell'app: board_init, banner e punto di ingresso del ruolo
 * (fw/fieldbus/fieldbus_main.c). RTT: canale 0 del control block condiviso
 * (fw/common/rtt_shared.h).
 */
#include "board.h"
#include "fieldbus.h"

int main(void)
{
    board_init();
    LOG("\n[%s] avvio: agente SCMI \"%s\", core %u MHz\n", BOARD_CORE_NAME, board_scmi_agent(),
        (unsigned)(SystemCoreClock / 1000000U));

    fieldbus_main(); /* non ritorna */
}
