/**
 * @file flow_platform.h
 * @brief Tutto cio' che il sottosistema flow chiede alla piattaforma.
 * @ingroup flow_platform
 *
 * Su queste app l'implementazione e' flow_platform_imx94.c: tick da SysTick
 * (board_millis), tempo da DWT->CYCCNT, RNG di banco, MU18, RTT per i log.
 */
#ifndef SI_FLOW_PLATFORM_H
#define SI_FLOW_PLATFORM_H

#include "flow_config.h"
#include "flow_types.h"

/** @brief Identita' del core che sta eseguendo.
 *
 * @param[in] core uno dei ruoli di flow_types.h.
 * @pre Va chiamata prima di flow_platform_init() e di ogni log.
 */
void flow_platform_set_core(flow_core_id_t core);

/** @brief Inizializzazione una tantum della piattaforma.
 *
 * @param[in] cycle_ms periodo del tick di ciclo del ruolo.
 */
void flow_platform_init(flow_u32 cycle_ms);

/** @brief Attende il prossimo tick di ciclo del ruolo corrente.
 *
 * @return false se la piattaforma si sta spegnendo (uscita dal loop).
 * @note 10 ms per i core safe, 2 ms per il fieldbus.
 */
bool flow_platform_tick_wait(void);

/** @brief Tempo monotono in microsecondi.
 *
 * Sorgente indipendente dal tick (DWT->CYCCNT): watchdog FSoE e timeout non
 * hanno una causa comune con il ciclo.
 *
 * @return microsecondi dall'avvio (a 32 bit, con wrap).
 */
flow_u32 flow_platform_time_us(void);

/** @brief Numero casuale per la negoziazione del Session ID.
 *
 * @return valore del RNG di banco.
 * @warning Con FLOW_BENCH_RNG=0 e' fatale: nel build safety non esiste un
 *          ripiego pseudo-casuale.
 */
flow_u16 flow_platform_random_u16(void);

/** @brief Condizione fatale: uscite gia' a 0, ci si ferma qui.
 *
 * @note Non ritorna (breakpoint/watchdog).
 */
void flow_platform_fatal(void) FLOW_NORETURN;

/** @brief Log su RTT del ruolo corrente.
 *
 * @param[in] tag    prefisso breve del ruolo;
 * @param[in] format formato printf-like;
 * @param[in] ...    argomenti del formato.
 */
void flow_platform_log(const char* tag, const char* format, ...)
    __attribute__((format(printf, 2, 3)));

/** @brief Trasporto di un messaggio di cross-check 1oo2.
 *
 * flow_mu.c costruisce e verifica sequenza, routing e CRC-32: qui c'e' solo il
 * trasporto (MU18 + payload ICC).
 *
 * @param[in] self    ruolo mittente;
 * @param[in] channel canale logico;
 * @param[in] tag     tag della connessione;
 * @param[in] cycle   ciclo a cui il messaggio appartiene;
 * @param[in] message messaggio da inviare;
 * @param[in] size    dimensione del messaggio.
 * @return true se il messaggio e' stato consegnato al trasporto.
 */
bool flow_platform_mu_post(flow_core_id_t self,
                           flow_u8 channel,
                           flow_u8 tag,
                           flow_u32 cycle,
                           const void* message,
                           flow_u32 size);

/** @brief Attende un messaggio di cross-check dal peer.
 *
 * @param[in]  self     ruolo ricevente;
 * @param[in]  channel  canale logico;
 * @param[in]  tag      tag della connessione;
 * @param[in]  cycle    ciclo atteso;
 * @param[out] message  buffer di destinazione;
 * @param[in]  max_size capacita' di `message`;
 * @param[in]  timeout_us attesa massima in microsecondi.
 * @return true se un messaggio del ciclo atteso e' arrivato integro.
 */
bool flow_platform_mu_wait(flow_core_id_t self,
                           flow_u8 channel,
                           flow_u8 tag,
                           flow_u32 cycle,
                           void* message,
                           flow_u32 max_size,
                           flow_u32 timeout_us);

/** @brief Servizio del watchdog esterno.
 *
 * @note Non abilitato: WDOG5/6/7 restano spenti (vedi @ref limiti) e la
 *       sorveglianza e' quella dello stack FSoE.
 */
void flow_platform_watchdog_kick(void);

/** @brief Contatori del tick di ciclo, per la telemetria su RTT.
 *
 * @return numero di tick eseguiti dall'avvio.
 */
flow_u32 flow_platform_cycles(void);

/** @brief Tick persi (il ciclo non ha rispettato il periodo).
 *
 * @return numero di scatti di periodo persi dall'avvio.
 */
flow_u32 flow_platform_tick_slips(void);

#endif /* SI_FLOW_PLATFORM_H */
