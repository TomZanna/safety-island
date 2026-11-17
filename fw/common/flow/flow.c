#include "flow.h"

#include "board.h"
#include "flow_config.h"
#include "flow_icc.h"
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

static void log_status(flow_u32 cycles)
{
    const flow_icc_stats_t *icc = flow_icc_get_stats();
    const flow_mu18_stats_t *mu = flow_mu18_get_stats();

    flow_platform_log(FLOW_TAG, "cyc %u slip %u hb %u/%u/%u icc %u/%u to %u crc %u bar %u mu %u/%u",
        (unsigned)cycles, (unsigned)flow_platform_tick_slips(), (unsigned)heartbeat(FLOW_CORE_SAFE_A),
        (unsigned)heartbeat(FLOW_CORE_SAFE_B), (unsigned)heartbeat(FLOW_CORE_FIELDBUS), (unsigned)icc->posted,
        (unsigned)icc->received, (unsigned)icc->timeouts, (unsigned)icc->crc_errors,
        (unsigned)icc->barrier_timeouts, (unsigned)mu->sent, (unsigned)mu->received);
}

/* Lavoro di ogni ciclo: heartbeat e input da RTT. In N2 qui entra il passo 1..4
 * del diagramma (flow_safe_step) e, sul fieldbus, lo slave FSoE. */
static void cycle_body(flow_u32 heartbeat_value)
{
    flow_shm_update_heartbeat(s_shm, (flow_core_id_t)FLOW_CORE_ID, heartbeat_value);

    for (;;) {
        const int c = board_getkey();

        if (c < 0) {
            break;
        }
        flow_platform_log(FLOW_TAG, "key '%c'", ((c >= 32) && (c < 127)) ? (char)c : '?');
    }
}

#if (FLOW_CORE_ID != FLOW_CORE_FIELDBUS)
static void safe_loop(flow_core_id_t self)
{
    for (;;) {
        flow_u32 cycle;

        if (!flow_platform_tick_wait()) {
            break;
        }

        if (!flow_icc_cycle_wait(&cycle, FLOW_BARRIER_TIMEOUT_US)) {
            flow_platform_log(FLOW_TAG, "barrier timeout (peer fermo?)");
            continue;
        }

        cycle_body(cycle);
        flow_icc_cycle_done(self, cycle);

        if ((cycle % 100u) == 0u) {
            log_status(cycle);
        }
    }
}
#else /* FLOW_CORE_ID == FLOW_CORE_FIELDBUS */
static void fieldbus_loop(void)
{
    const flow_u32 bus_per_safe = FLOW_SAFE_CYCLE_MS / FLOW_FIELD_CYCLE_MS;
    flow_u32 bus = 0u;

    for (;;) {
        if (!flow_platform_tick_wait()) { /* 2 ms */
            break;
        }
        bus++;

        /* Una volta per ciclo safe: apre il ciclo successivo dopo che i due core
         * safe hanno finito il corrente (e, in N2, dopo aver pubblicato la RX). */
        if ((bus % bus_per_safe) == 0u) {
            (void)flow_icc_cycle_release(flow_icc_cycle_current(), FLOW_BARRIER_TIMEOUT_US);
        }

        cycle_body(flow_icc_cycle_current());

        if ((bus % 500u) == 0u) {
            log_status(bus);
        }
    }
}
#endif /* FLOW_CORE_ID */

void flow_main(void)
{
    const flow_core_id_t self = (flow_core_id_t)FLOW_CORE_ID;

    flow_platform_set_core(self);
    flow_platform_init();

    flow_platform_log(FLOW_TAG, "flow: PDU %u B, mailbox @0x%08x (%u B), ICC @0x%08x", (unsigned)FLOW_PDU_MAX,
        (unsigned)FLOW_SHM_BASE_ADDR, (unsigned)sizeof(flow_shm_t), (unsigned)FLOW_ICC_BASE_ADDR);

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    /* L'owner prepara prima l'ICC e poi la mailbox: il magic della mailbox e' il
     * "go" per i due core safe, cosi' quando lo vedono c'e' gia' anche l'ICC. */
    flow_icc_init(FLOW_CORE_FIELDBUS);
    flow_shm_init(s_shm, FLOW_CORE_FIELDBUS);
#else
    if (!flow_shm_wait_ready(s_shm, 2000000u) || !flow_icc_wait_ready(2000000u)) {
        flow_platform_log(FLOW_TAG, "mailbox/ICC non pronte: fail-safe");
        flow_platform_fatal();
    }
    flow_platform_log(FLOW_TAG, "mailbox validata (layout 0x%08x)", (unsigned)s_shm->header.layout_hash);
#endif

    if (!flow_selftest_run() || !flow_selftest_icc()) {
        flow_platform_fatal();
    }

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    flow_platform_log(FLOW_TAG, "fieldbus pronto (bus %u ms, ciclo safe %u ms)", (unsigned)FLOW_FIELD_CYCLE_MS,
        (unsigned)FLOW_SAFE_CYCLE_MS);
    fieldbus_loop();
#else
    flow_platform_log(FLOW_TAG, "core safe pronto (ciclo %u ms, watchdog %u ms)", (unsigned)FLOW_SAFE_CYCLE_MS,
        (unsigned)FLOW_WATCHDOG_MS);
    safe_loop(self);
#endif

    flow_platform_fatal();
}
