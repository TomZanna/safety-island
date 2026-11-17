#include "flow.h"

#include "board.h"
#include "flow_config.h"
#include "flow_mu18.h"
#include "flow_platform.h"
#include "flow_selftest.h"
#include "flow_shm.h"

#if (FLOW_CORE_ID == FLOW_CORE_SAFE_A)
#define FLOW_TAG "m70/A"
#elif (FLOW_CORE_ID == FLOW_CORE_SAFE_B)
#define FLOW_TAG "m71/B"
#else
#define FLOW_TAG "m33s/fld"
#endif

/* Unica istanza della mailbox, alla finestra OCRAM della patch SM 0002. */
static flow_shm_t *const s_shm = (flow_shm_t *)FLOW_SHM_BASE_ADDR;

static flow_u32 heartbeat(flow_core_id_t core)
{
    return __atomic_load_n(&s_shm->header.heartbeat[core], __ATOMIC_ACQUIRE);
}

static void log_status(flow_u32 cycle)
{
    const flow_mu18_stats_t *mu = flow_mu18_get_stats();

    flow_platform_log(FLOW_TAG, "cyc %u slip %u hb %u/%u/%u mu tx %u rx %u to %u/%u", (unsigned)cycle,
        (unsigned)flow_platform_tick_slips(), (unsigned)heartbeat(FLOW_CORE_SAFE_A),
        (unsigned)heartbeat(FLOW_CORE_SAFE_B), (unsigned)heartbeat(FLOW_CORE_FIELDBUS), (unsigned)mu->sent,
        (unsigned)mu->received, (unsigned)mu->tx_timeouts, (unsigned)mu->rx_timeouts);
}

/* Lavoro comune a ogni ciclo (M1: link MU18, heartbeat, input RTT).
 * In M2 qui dentro ci finisce flow_safe_step() / il ciclo dello slave. */
static void cycle_body(flow_u32 cycle)
{
    flow_u32 word0;
    flow_u32 word1;

    /* Eco del doorbell MU18: in M1 serve a validare il link fra M7_0 e M7_1;
     * in M2 il doorbell porta {seq, tag, channel} e il peer non fa l'eco ma
     * consuma il messaggio. */
    if (flow_mu18_poll(&word0, &word1)) {
        (void)flow_mu18_send(word0, word1, 1000u);
    }

    flow_shm_update_heartbeat(s_shm, (flow_core_id_t)FLOW_CORE_ID, cycle);

    /* Canale down dell'RTT: eco dei caratteri ricevuti. */
    for (;;) {
        const int c = board_getkey();

        if (c < 0) {
            break;
        }
        flow_platform_log(FLOW_TAG, "key '%c'", ((c >= 32) && (c < 127)) ? (char)c : '?');
    }
}

void flow_main(void)
{
    const flow_u32 log_period = (FLOW_CORE_ID == FLOW_CORE_FIELDBUS) ? 500u : 100u;

    flow_platform_set_core((flow_core_id_t)FLOW_CORE_ID);
    flow_platform_init();

    flow_platform_log(FLOW_TAG, "flow: PDU %u B, mailbox @0x%08x (%u B), ICC @0x%08x", (unsigned)FLOW_PDU_MAX,
        (unsigned)FLOW_SHM_BASE_ADDR, (unsigned)sizeof(flow_shm_t), (unsigned)FLOW_ICC_BASE_ADDR);

    if (!flow_selftest_run()) {
        flow_platform_fatal();
    }

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    /* Il ruolo fieldbus e' l'owner della mailbox: la azzera e la pubblica. */
    flow_shm_init(s_shm, FLOW_CORE_FIELDBUS);
    flow_platform_log(FLOW_TAG, "mailbox inizializzata (owner)");
#else
    /* I due core safe aspettano che l'owner l'abbia pubblicata: attesa limitata,
     * poi fail-safe (non si resta appesi a un core spento). */
    if (!flow_shm_wait_ready(s_shm, 2000000u)) {
        flow_platform_log(FLOW_TAG, "mailbox non pronta: fail-safe");
        flow_platform_fatal();
    }
    flow_platform_log(FLOW_TAG, "mailbox validata (layout 0x%08x)", (unsigned)s_shm->header.layout_hash);
#endif

    for (;;) {
        flow_u32 cycle;

        if (!flow_platform_tick_wait()) {
            break;
        }

        cycle = flow_platform_cycles();
        cycle_body(cycle);

        if ((cycle % log_period) == 0u) {
            log_status(cycle);
        }
    }

    flow_platform_fatal();
}
