/**
 * @file rtt_shared.h
 * @brief SEGGER RTT con un solo control block condiviso da tutti i core
 * secondari, un canale per core.
 * @ingroup rtt
 *
 * Il motore RTT del J-Link segue un solo control block per sonda: se ogni core
 * avesse il suo (nella sua TCM), una sessione ne vedrebbe uno solo. Qui il
 * control block e i buffer stanno in 16 KB di OCRAM condivisi da M33S, M7_0,
 * M7_1 (patch SM sm/patches/0001-mx94evk-rtt-shared-ocram.patch), allo stesso
 * indirizzo per tutti: una sola sessione RTT, collegata a qualunque core, legge
 * tutti i canali con gli strumenti standard (Cortex-Debug, RTT Viewer,
 * J-Scope).
 *
 *   canale (up e down)   0 = m33s   1 = m70   2 = m71
 *
 * Il formato in memoria e' quello di SEGGER_RTT.h. Non si usa SEGGER_RTT.c: la
 * sua init scrive l'intero control block e cancellerebbe i canali degli altri
 * core. Ogni core scrive solo i propri descrittori (e l'header, identico per
 * tutti). Nessuna cache dati attiva sull'area (gli M7 abilitano solo la
 * I-cache, l'M33S solo la cache del bus codice): se si abilita una D-cache,
 * l'area va resa non-cacheable via MPU.
 *
 * Il canale e' no-block-skip: se il buffer da 4 KB e' pieno il messaggio intero
 * si perde (mai righe spezzate). Per questo i log per-ciclo sono dietro
 * `FLOW_LOG_VERBOSE`, spento di default. Due lettori sullo stesso canale si
 * rubano i dati; su canali diversi convivono.
 */
#ifndef SI_RTT_SHARED_H
#define SI_RTT_SHARED_H

#include <stddef.h>
#include <stdint.h>

#define RTT_SHARED_BASE 0x204D0000U /**< = _SEGGER_RTT (linker), 16 KB */
#define RTT_SHARED_SIZE 0x4000U
#define RTT_NUM_CHANNELS 3U /**< uno per core */
#define RTT_UP_BUFFER_SIZE 4096U /**< target -> host, per core */
#define RTT_DOWN_BUFFER_SIZE 64U /**< host -> target (input), per core */

#if defined(CPU_MIMX94398AVKM_cm33_core1)
#  define RTT_CHANNEL 0U
#elif defined(CPU_MIMX94398AVKM_cm7_core0)
#  define RTT_CHANNEL 1U
#elif defined(CPU_MIMX94398AVKM_cm7_core1)
#  define RTT_CHANNEL 2U
#endif

/** @brief Prepara il canale RTT di questo core e l'header comune.
 *
 * @pre Va chiamata una volta all'avvio, prima di ogni log.
 * @note Scrive solo i propri descrittori: non azzera i canali degli altri core.
 */
void rtt_init(void);

/** @brief Scrive n byte nel canale del core.
 *
 * @param[in] data byte da scrivere;
 * @param[in] n    numero di byte.
 * @note Non blocca: se non c'e' spazio il messaggio intero si perde (mai
 *       righe spezzate).
 */
void rtt_write(const char* data, size_t n);

/** @brief printf nel canale del core.
 *
 * @param[in] fmt formato printf-like;
 * @param[in] ... argomenti del formato.
 * @return caratteri scritti, oppure un valore negativo in caso di errore.
 * @note vsnprintf di newlib-nano, massimo 255 caratteri per chiamata.
 */
int rtt_printf(const char* fmt, ...) __attribute__((format(printf, 1, 2)));

/** @brief Carattere dall'input del core (buffer down).
 *
 * @return carattere (0..255), oppure -1 se non ce n'e' (non bloccante).
 */
int rtt_getkey(void);

#endif
