/*
 * Tutto cio' che il sottosistema flow chiede alla piattaforma.
 *
 * Su queste app l'implementazione e' flow_platform_imx94.c: tick da SysTick
 * (board_millis), tempo da DWT->CYCCNT, RNG di banco, MU18, RTT per i log.
 */
#ifndef SI_FLOW_PLATFORM_H
#define SI_FLOW_PLATFORM_H

#include "flow_config.h"
#include "flow_types.h"

/* Identita' del core che sta eseguendo: deve combaciare con FLOW_CORE_ID. */
void flow_platform_set_core(flow_core_id_t core);

/* Inizializzazione una tantum (DWT, contatori). */
void flow_platform_init(void);

/* Attende il prossimo tick di ciclo del ruolo corrente (10 ms safe, 2 ms
 * fieldbus). Ritorna false se la piattaforma si sta spegnendo. */
bool flow_platform_tick_wait(void);

/* Tempo monotono in microsecondi (DWT), usato dal watchdog FSoE e dai timeout.
 * Sorgente indipendente dal tick: nessuna causa comune. */
flow_u32 flow_platform_time_us(void);

/* Numero casuale per la negoziazione del Session ID. Con FLOW_BENCH_RNG=0 e'
 * fatale: nel build safety non esiste un ripiego pseudo-casuale. */
flow_u16 flow_platform_random_u16(void);

/* Condizione fatale: uscite gia' a 0, si ferma qui (breakpoint/watchdog). */
void flow_platform_fatal(void) FLOW_NORETURN;

void flow_platform_log(const char *tag, const char *format, ...) __attribute__((format(printf, 2, 3)));

/* Trasporto dei messaggi di cross-check 1oo2 (flow_mu.c costruisce e verifica
 * sequenza, routing e CRC-32; qui c'e' solo il trasporto). `cycle` e' il ciclo
 * a cui il messaggio appartiene: chi riceve lo confronta con il proprio. */
bool flow_platform_mu_post(flow_core_id_t self, flow_u8 channel, flow_u8 tag, flow_u32 cycle, const void *message,
    flow_u32 size);

bool flow_platform_mu_wait(flow_core_id_t self, flow_u8 channel, flow_u8 tag, flow_u32 cycle, void *message,
    flow_u32 max_size, flow_u32 timeout_us);

/* Servizio del watchdog esterno (oggi non abilitato). */
void flow_platform_watchdog_kick(void);

/* Contatori del tick di ciclo, per la telemetria su RTT. */
flow_u32 flow_platform_cycles(void);
flow_u32 flow_platform_tick_slips(void);

#endif /* SI_FLOW_PLATFORM_H */
