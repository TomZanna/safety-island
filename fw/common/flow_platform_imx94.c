/*
 * Piattaforma i.MX94 per il sottosistema flow (M7_0, M7_1, M33_1).
 *
 * Scelte:
 *   - tick di ciclo: SysTick a 1 kHz (board_millis, fw/common/board.c), 10 ms per i
 *     due core safe e 2 ms per il ruolo fieldbus. Un ritardo oltre due periodi
 *     riallinea il contatore invece di accumulare (s_slips lo conta).
 *   - tempo in us: DWT->CYCCNT. E' una sorgente indipendente dal tick (nessuna
 *     causa comune con il watchdog FSoE) e non richiede periferiche dell'SM.
 *   - RNG: solo di banco (DWT + SysTick + ruolo), vedi FLOW_BENCH_RNG. Con
 *     l'opzione a 0 la funzione e' fatale: nel build safety non c'e' ripiego.
 *   - log: una riga per chiamata su rtt_write, cosi' il no-block-skip del canale
 *     RTT o la scrive intera o la scarta (mai righe spezzate).
 *   - trasporto dei cross-check: TODO, arriva con flow_icc.c (M2). Per ora un
 *     post/wait che fallisce chiuso.
 */
#include "flow_platform.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#include "board.h"
#include "fsl_device_registers.h"
#include "rtt_shared.h"

static flow_u32 s_tick_ms;
static flow_u32 s_cycles;
static flow_u32 s_slips;
static flow_u32 s_rng_state = 0x9E3779B9u;
static const char *s_role = "?";

static const char *role_name(flow_core_id_t id)
{
    switch (id) {
        case FLOW_CORE_SAFE_A: return "A";   /* m70 */
        case FLOW_CORE_SAFE_B: return "B";   /* m71 */
        case FLOW_CORE_FIELDBUS: return "FLD"; /* m33s */
        default: return "?";
    }
}

void flow_platform_set_core(flow_core_id_t core)
{
    if (core != (flow_core_id_t)FLOW_CORE_ID) {
        flow_platform_fatal();
    }
    s_role = role_name(core);
}

void flow_platform_init(void)
{
    /* DWT cycle counter: base di tempo indipendente da SysTick. */
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0u;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    s_tick_ms = board_millis();
    s_cycles = 0u;
    s_slips = 0u;
    s_rng_state ^= DWT->CYCCNT ^ ((flow_u32)FLOW_CORE_ID << 8);
}

bool flow_platform_tick_wait(void)
{
    const flow_u32 period = (FLOW_CORE_ID == FLOW_CORE_FIELDBUS) ? FLOW_FIELD_CYCLE_MS : FLOW_SAFE_CYCLE_MS;

    for (;;) {
        const flow_u32 now = board_millis();
        const flow_u32 elapsed = now - s_tick_ms;

        if (elapsed >= period) {
            if (elapsed > (2u * period)) {
                /* Troppo indietro: si riallinea (e si conta), non si accumula. */
                s_slips++;
                s_tick_ms = now;
            } else {
                s_tick_ms += period;
            }
            s_cycles++;
            return true;
        }

        __WFI(); /* sveglia il core al prossimo interrupt (SysTick) */
    }
}

flow_u32 flow_platform_time_us(void)
{
    const flow_u32 cycles_per_us = (flow_u32)(SystemCoreClock / 1000000u);

    return (flow_u32)((uint64_t)DWT->CYCCNT / (uint64_t)((cycles_per_us != 0u) ? cycles_per_us : 1u));
}

flow_u16 flow_platform_random_u16(void)
{
#if FLOW_BENCH_RNG
    /* Banco, non safety: DWT + SysTick + stato precedente. Serve solo a dare ai
     * due master due numeri diversi (il Session ID e' la loro somma). */
    flow_u32 x = s_rng_state;

    x ^= DWT->CYCCNT + 0x9E3779B9u + (x << 6) + (x >> 2);
    x ^= board_millis() * 2654435761u;
    if (x == 0u) {
        x = 0xA5A50001u;
    }
    s_rng_state = x;

    return (flow_u16)(x ^ (x >> 16));
#else
    flow_platform_fatal(); /* nessun ripiego pseudo-casuale nel build safety */
#endif
}

void flow_platform_fatal(void)
{
    flow_platform_log(s_role, "FATAL, uscite in stato sicuro");
    board_fault(0xF10Fu, 0);
}

void flow_platform_log(const char *tag, const char *format, ...)
{
    char line[200];
    int prefix;
    int body;
    size_t length;
    va_list ap;

    prefix = snprintf(line, sizeof(line), "[%s] ", (tag != NULL) ? tag : "?");
    if (prefix < 0) {
        return;
    }
    if ((size_t)prefix >= sizeof(line)) {
        prefix = (int)sizeof(line) - 1;
    }

    va_start(ap, format);
    body = vsnprintf(&line[prefix], sizeof(line) - (size_t)prefix, format, ap);
    va_end(ap);
    if (body <= 0) {
        return;
    }

    length = (size_t)prefix + ((size_t)body < (sizeof(line) - (size_t)prefix) ? (size_t)body : (sizeof(line) - (size_t)prefix - 1u));
    if (length < (sizeof(line) - 1u)) {
        line[length++] = '\n';
    }

    rtt_write(line, length);
}

bool flow_platform_mu_post(flow_core_id_t self, flow_u8 channel, flow_u8 tag, const void *message, flow_u32 size)
{
    (void)self;
    (void)channel;
    (void)tag;
    (void)message;
    (void)size;

    return false; /* TODO(M2): flow_icc.c (payload in OCRAM + doorbell MU18) */
}

bool flow_platform_mu_wait(flow_core_id_t self, flow_u8 channel, flow_u8 tag, flow_u32 cycle, void *message,
    flow_u32 max_size, flow_u32 timeout_us)
{
    (void)self;
    (void)channel;
    (void)tag;
    (void)cycle;
    (void)message;
    (void)max_size;
    (void)timeout_us;

    return false; /* TODO(M2): flow_icc.c */
}

void flow_platform_watchdog_kick(void)
{
    /* WDOG5/WDOG6 non sono abilitati: quando lo saranno va servito qui. */
}

flow_u32 flow_platform_cycles(void)
{
    return s_cycles;
}

flow_u32 flow_platform_tick_slips(void)
{
    return s_slips;
}
