/*
 * Ciclo del core di campo (FIELDBUS, m33s).
 *
 * Ogni 2 ms unisce le due meta' del frame (flow_a55), fa girare slave + campo,
 * tiene da parte la risposta e la pubblica UNA volta per ciclo safe, poi apre il
 * ciclo successivo (barrier).
 *
 * Comandi RTT (canale del core): s = discrepanza sugli ingressi, d = scarta 60
 * frame (watchdog FSoE), '?' = stato.
 */
#include "fieldbus.h"

#include "board.h"
#include "flow_a55.h"
#include "flow_config.h"
#include "flow_field.h"
#include "flow_icc.h"
#include "flow_init.h"
#include "flow_mem.h"
#include "flow_platform.h"
#include "flow_selftest.h"
#include "flow_shm.h"

#include <fsoeslave.h>

static flow_shm_t *const s_shm = (flow_shm_t *)FLOW_SHM_BASE_ADDR;
static flow_field_t s_field;
static flow_a55_t s_a55;
static const char *const s_tag = "m33s/fld";

static void log_status(flow_u32 bus)
{
    const char *state = fsoeslave_state_description(s_field.sync[0].current_state);

    flow_platform_log(s_tag, "bus %u %s out %u en %u ok %u drop %u a55 %u hb %u/%u/%u", (unsigned)bus, state,
        (unsigned)s_field.outputs_valid, (unsigned)s_field.enabled_seen, (unsigned)s_field.frames_ok,
        (unsigned)s_field.frames_dropped, (unsigned)s_a55.invalid_frames,
        (unsigned)flow_shm_heartbeat(s_shm, FLOW_CORE_SAFE_A),
        (unsigned)flow_shm_heartbeat(s_shm, FLOW_CORE_SAFE_B),
        (unsigned)flow_shm_heartbeat(s_shm, FLOW_CORE_FIELDBUS));
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
                flow_platform_log(s_tag, "discrepanza sugli ingressi: %s",
                    s_field.inject_discrepancy ? "ON" : "OFF");
                break;
            case 'd':
                flow_field_drop_frames(&s_field, 60u); /* ~120 ms: fa scattare il watchdog FSoE */
                flow_platform_log(s_tag, "drop di 60 frame (test watchdog FSoE)");
                break;
            case '?':
                log_status(s_field.bus_cycles);
                break;
            default:
                flow_platform_log(s_tag, "comando '%c' sconosciuto (s, d, ?)", (char)c);
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

void fieldbus_main(void)
{
    flow_init(FLOW_CORE_FIELDBUS, FLOW_FIELD_CYCLE_MS);

    /* L'owner prepara prima l'ICC e poi la mailbox: il magic della mailbox e' il
     * "go" per i due core safe. */
    flow_icc_init(FLOW_CORE_FIELDBUS);
    flow_shm_init(s_shm, FLOW_CORE_FIELDBUS);

    if (!flow_selftest_run(FLOW_CORE_FIELDBUS)) {
        flow_platform_fatal();
    }

    flow_a55_init(&s_a55, s_shm);
    flow_field_init(&s_field, FLOW_SLAVE_ADDRESS);
    flow_platform_log(s_tag, "fieldbus pronto (bus %u ms, ciclo safe %u ms)", (unsigned)FLOW_FIELD_CYCLE_MS,
        (unsigned)FLOW_SAFE_CYCLE_MS);
    fieldbus_loop();

    flow_platform_fatal();
}
