/*
 * App M7_0. Punto di partenza: banner, heartbeat ogni secondo, eco dei caratteri ricevuti.
 * Log e input via RTT, canale di questo core nel control block condiviso (fw/common/rtt_shared.h).
 * VS Code: un terminale RTT per core durante il debug; terminale: make rtt (tutti i core).
 */
#include "board.h"

static uint32_t s_ticks; /* contatore heartbeat: comodo da guardare in Cortex Live Watch */

int main(void)
{
    board_init();
    LOG("\n[%s] avvio: agente SCMI \"%s\", core %u MHz\n", BOARD_CORE_NAME, board_scmi_agent(),
        (unsigned)(SystemCoreClock / 1000000U));

    uint32_t next = board_millis() + 1000U;
    for (;;)
    {
        int c = board_getkey();
        if (c >= 0)
        {
            LOG("%c", c); /* eco */
        }
        if ((int32_t)(board_millis() - next) >= 0)
        {
            next += 1000U;
            s_ticks++;
            LOG("[%s] tick %u\n", BOARD_CORE_NAME, (unsigned)s_ticks);
        }
    }
}
