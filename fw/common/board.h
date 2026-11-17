/**
 * @file board.h
 * @brief Board glue comune alle app dei core secondari i.MX94 (M33S, M7_0,
 * M7_1) con il System Manager.
 * @ingroup board
 *
 * Il canale SCMI con l'SM (SystemPlatformInit() del SDK) e' la via per ogni
 * pin mux, clock o power: con l'SM quelle risorse sono protette dal TRDC e si
 * chiedono, non si configurano. Log e input passano da SEGGER RTT con un canale
 * per core in un control block condiviso (fw/common/rtt_shared.h), cosi' una
 * sola sessione RTT vede tutti i core: un terminale per core in VS Code oppure
 * `make rtt` da terminale. Il tick e' SysTick a 1 kHz; WAIT_DEBUGGER aggiunge
 * un'attesa opzionale del debugger prima di main().
 */
#ifndef SI_BOARD_H
#define SI_BOARD_H

#include <stdint.h>

#include "fsl_device_registers.h"
#include "rtt_shared.h"

/** Nome breve del core (m33s, m70, m71), dal define CPU_* del build */
#if defined(CPU_MIMX94398AVKM_cm33_core1)
#  define BOARD_CORE_NAME "m33s"
#elif defined(CPU_MIMX94398AVKM_cm7_core0)
#  define BOARD_CORE_NAME "m70"
#elif defined(CPU_MIMX94398AVKM_cm7_core1)
#  define BOARD_CORE_NAME "m71"
#endif

/** Log sul canale RTT del core (printf di newlib-nano, max 255 caratteri per
 * chiamata). Non blocca: se nessuno legge e il buffer (4 KB) e' pieno, il
 * messaggio si perde. */
#define LOG(...) rtt_printf(__VA_ARGS__)

/** @brief Init: canale RTT del core, canale SCMI con l'SM, SysTick a 1 kHz. */
void board_init(void);

/** @brief Millisecondi dall'avvio.
 *
 * @return tick di SysTick (1 kHz) da board_init().
 */
uint32_t board_millis(void);

/** @brief Carattere ricevuto via RTT sul canale del core.
 *
 * @return carattere (0..255), oppure -1 se non ce n'e' (non bloccante).
 */
int board_getkey(void);

/** @brief Nome dell'agente SCMI di questo core secondo l'SM.
 *
 * @return stringa dell'agente (es. "M7"), oppure "?" se non disponibile.
 */
const char* board_scmi_agent(void);

/** @brief Flag scritto a 1 dal debugger all'attach.
 *
 * Sblocca l'attesa opzionale di WAIT_DEBUGGER prima di main().
 *
 * @warning Deve restare nell'ELF: e' referenziata da SystemInitHook() in
 *          board.c, e un attach SWD che non la trova non rilascia il core.
 */
extern volatile uint32_t g_debugger_go;

/** @brief Errore fatale della board: si ferma qui.
 *
 * @param[in] what   codice dell'errore (visibile dal debugger);
 * @param[in] status stato/errno associato.
 * @note Non ritorna: breakpoint utile per l'analisi.
 */
void board_fault(uint32_t what, int32_t status) __attribute__((noreturn));

#endif
