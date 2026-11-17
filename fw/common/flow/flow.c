/*
 * Cicli dei tre ruoli del virtual safe PLC.
 *
 *   SAFE_A (m70) / SAFE_B (m71): ogni 10 ms, dopo il rendezvous di ciclo,
 *   eseguono flow_safe_step() (i 4 passi del diagramma).
 *   FIELDBUS (m33s): ogni 2 ms unisce le due meta' del frame (flow_a55), fa
 *   girare slave + campo, tiene da parte la risposta e la pubblica UNA volta per
 *   ciclo safe, poi apre il ciclo successivo (barrier).
 *
 * Comandi RTT (canale del core):
 *   m70/m71: p/i/l/x = fault injection (PDU, inputs, logic, session), '?' = stato
 *   m33s:    s = discrepanza sugli ingressi, d = scarta 10 frame (watchdog), '?'
 */
#include "flow.h"

#include "board.h"
#include "flow_a55.h"
#include "flow_config.h"
#include "flow_field.h"
#include "flow_icc.h"
#include "flow_mem.h"
#include "flow_mu18.h"
#include "flow_platform.h"
#include "flow_safe.h"
#include "flow_selftest.h"
#include "flow_shm.h"

#if (FLOW_CORE_ID == FLOW_CORE_SAFE_A)
#define FLOW_TAG "m70/A"
#elif (FLOW_CORE_ID == FLOW_CORE_SAFE_B)
#define FLOW_TAG "m71/B"
#else
#define FLOW_TAG "m33s/fld"
#endif

static flow_shm_t *const s_shm = (flow_shm_t *)FLOW_SHM_BASE_ADDR;

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
static flow_field_t s_field;
static flow_a55_t s_a55;
#else
static flow_safe_t s_safe;
#endif

static flow_u32 heartbeat(flow_core_id_t core)
{
    return __atomic_load_n(&s_shm->header.heartbeat[core], __ATOMIC_ACQUIRE);
}

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)

static void log_status(flow_u32 bus)
{
    const char *state = fsoeslave_state_description(s_field.sync[0].current_state);

    flow_platform_log(FLOW_TAG, "bus %u %s out %u en %u ok %u drop %u a55 %u hb %u/%u/%u", (unsigned)bus, state,
        (unsigned)s_field.outputs_valid, (unsigned)s_field.enabled_seen, (unsigned)s_field.frames_ok,
        (unsigned)s_field.frames_dropped, (unsigned)s_a55.invalid_frames, (unsigned)heartbeat(FLOW_CORE_SAFE_A),
        (unsigned)heartbeat(FLOW_CORE_SAFE_B), (unsigned)heartbeat(FLOW_CORE_FIELDBUS));
}

static void fieldbus_keys(void)
{
    for (;;) {
        const int c = board_getkey();

        if (c < 0) {
            return;
        }
        if ((c < 32) || (c > 126)) {
            continue; /* newline e caratteri di controllo del terminale */
        }

        switch (c) {
            case 's':
                s_field.inject_discrepancy = !s_field.inject_discrepancy;
                flow_platform_log(FLOW_TAG, "discrepanza sugli ingressi: %s",
                    s_field.inject_discrepancy ? "ON" : "OFF");
                break;
            case 'd':
                flow_field_drop_frames(&s_field, 60u); /* ~120 ms: fa scattare il watchdog FSoE */
                flow_platform_log(FLOW_TAG, "drop di 60 frame (test watchdog FSoE)");
                break;
            case '?':
                log_status(s_field.bus_cycles);
                break;
            default:
                flow_platform_log(FLOW_TAG, "comando '%c' sconosciuto (s, d, ?)", (char)c);
                break;
        }
    }
}

static void fieldbus_loop(void)
{
    const flow_u32 bus_per_safe = FLOW_SAFE_CYCLE_MS / FLOW_FIELD_CYCLE_MS;
    flow_u8 staged[FLOW_PDU_MAX];
    flow_u32 staged_len = 0u;
    bool staged_ready = false;
    flow_u32 bus = 0u;

    for (;;) {
        flow_a55_frame_t frame;
        flow_u8 reply[FLOW_PDU_MAX];
        flow_u32 reply_len = 0u;

        if (!flow_platform_tick_wait()) { /* 2 ms */
            break;
        }
        bus++;

        /* Frame del master: solo quando ENTRAMBI i core hanno pubblicato. */
        if (flow_a55_get_tx(&s_a55, 0u, &frame) == FLOW_A55_OK) {
            flow_field_provide_rx(&s_field, frame.pdu, frame.len);
        }

        flow_field_step(&s_field);

        if (flow_field_take_tx(&s_field, reply, &reply_len)) {
            flow_memcpy(staged, reply, reply_len);
            staged_len = reply_len;
            staged_ready = true;
        }

        fieldbus_keys();

        /* Una RX per ciclo safe, poi si apre il ciclo successivo: cosi' i due core
         * leggono la stessa RX e lo stesso `cycle`. */
        if ((bus % bus_per_safe) == 0u) {
            if (staged_ready) {
                (void)flow_a55_put_rx(&s_a55, 0u, staged, staged_len);
                staged_ready = false;
            }

            (void)flow_icc_cycle_release(flow_icc_cycle_current(), FLOW_BARRIER_TIMEOUT_US);
            flow_shm_update_heartbeat(s_shm, FLOW_CORE_FIELDBUS, flow_icc_cycle_current());

            if ((bus % 500u) == 0u) {
                log_status(bus);
            }
        }
    }
}

#else /* SAFE_A / SAFE_B */

static void log_status(flow_u32 cycle)
{
    flow_safe_t *s = &s_safe;
    const flow_icc_stats_t *icc = flow_icc_get_stats();
    const flow_mu18_stats_t *mu = flow_mu18_get_stats();
    const char *state = fsoemaster_state_description(s->sync[0].current_state);

    flow_platform_log(FLOW_TAG, "cyc %u %s anom %u icc p%u r%u to%u crc%u sq%u cy%u db%u mu to%u/%u sess 0x%04x hb %u/%u/%u",
        (unsigned)cycle, state, (unsigned)s->anomalies, (unsigned)icc->posted, (unsigned)icc->received,
        (unsigned)icc->timeouts, (unsigned)icc->crc_errors, (unsigned)icc->seq_errors, (unsigned)icc->cycle_errors,
        (unsigned)icc->doorbell_errors, (unsigned)mu->tx_timeouts, (unsigned)mu->rx_timeouts,
        (unsigned)s->session_id, (unsigned)heartbeat(FLOW_CORE_SAFE_A), (unsigned)heartbeat(FLOW_CORE_SAFE_B),
        (unsigned)heartbeat(FLOW_CORE_FIELDBUS));
}

static void safe_keys(void)
{
    for (;;) {
        const int c = board_getkey();
        const flow_u32 at = flow_icc_cycle_current() + 2u;

        if (c < 0) {
            return;
        }
        if ((c < 32) || (c > 126)) {
            continue; /* newline e caratteri di controllo del terminale */
        }

        switch (c) {
            case 'p':
                flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_PDU, at);
                flow_platform_log(FLOW_TAG, "fault injection: PDU al ciclo %u", (unsigned)at);
                break;
            case 'i':
                flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_INPUTS, at);
                flow_platform_log(FLOW_TAG, "fault injection: inputs al ciclo %u", (unsigned)at);
                break;
            case 'l':
                flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_LOGIC, at);
                flow_platform_log(FLOW_TAG, "fault injection: logic al ciclo %u", (unsigned)at);
                break;
            case 'x':
                flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_SESSION, at);
                flow_platform_log(FLOW_TAG, "fault injection: session al ciclo %u", (unsigned)at);
                break;
            case '?':
                log_status(s_safe.cycle);
                break;
            default:
                flow_platform_log(FLOW_TAG, "comando '%c' sconosciuto (p, i, l, x, ?)", (char)c);
                break;
        }
    }
}

static void safe_loop(flow_core_id_t self)
{
    for (;;) {
        flow_u32 cycle;

        if (!flow_platform_tick_wait()) { /* 10 ms */
            break;
        }

        if (!flow_icc_cycle_wait(&cycle, FLOW_BARRIER_TIMEOUT_US)) {
            flow_platform_log(FLOW_TAG, "barrier timeout (peer fermo?)");
            continue;
        }

        flow_safe_step(&s_safe, cycle);
        flow_icc_cycle_done(self, cycle);

        safe_keys();

        if ((cycle % 100u) == 0u) {
            log_status(cycle);
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
     * "go" per i due core safe. */
    flow_icc_init(FLOW_CORE_FIELDBUS);
    flow_shm_init(s_shm, FLOW_CORE_FIELDBUS);
#else
    if (!flow_shm_wait_ready(s_shm, 2000000u) || !flow_icc_wait_ready(2000000u)) {
        flow_platform_log(FLOW_TAG, "mailbox/ICC non pronte: fail-safe");
        flow_platform_fatal();
    }
#endif

    if (!flow_selftest_run() || !flow_selftest_icc()) {
        flow_platform_fatal();
    }

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    flow_a55_init(&s_a55, s_shm);
    flow_field_init(&s_field, FLOW_SLAVE_ADDRESS);
    flow_platform_log(FLOW_TAG, "fieldbus pronto (bus %u ms, ciclo safe %u ms)", (unsigned)FLOW_FIELD_CYCLE_MS,
        (unsigned)FLOW_SAFE_CYCLE_MS);
    fieldbus_loop();
#else
    flow_safe_init(&s_safe, self, s_shm);
    flow_platform_log(FLOW_TAG, "core safe pronto (ciclo %u ms)", (unsigned)FLOW_SAFE_CYCLE_MS);
    safe_loop(self);
#endif

    flow_platform_fatal();
}
