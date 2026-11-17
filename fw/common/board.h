/**
 * @file board.h
 * @brief Board glue comune alle app dei core secondari i.MX94 (M33S, M7_0,
 * M7_1) con il System Manager.
 * @ingroup board
 *
 * - Canale SCMI con l'SM (SystemPlatformInit() del SDK): serve per qualsiasi
 * pin mux/clock/power, che con l'SM sono protetti dal TRDC e si chiedono
 * all'SM.
 * - Log e input via SEGGER RTT, un canale per core in un control block
 * condiviso (fw/common/rtt_shared.h): una sola sessione RTT vede tutti i core.
 * VS Code: un terminale per core durante il debug; da terminale: make rtt.
 * - SysTick a 1 kHz, attesa opzionale del debugger (WAIT_DEBUGGER) prima di
 * main().
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

/** Init: canale RTT del core, canale SCMI con l'SM, SysTick a 1 kHz */
void board_init(void);

/** Millisecondi da board_init() (SysTick) */
uint32_t board_millis(void);

/** Carattere ricevuto via RTT (canale del core), o -1 se non ce n'e' (non
 * bloccante) */
int board_getkey(void);

/** Nome dell'agente SCMI di questo core secondo l'SM (es. "M7"), o "?" */
const char* board_scmi_agent(void);

/** Scritta a 1 dal debugger all'attach: sblocca l'attesa di WAIT_DEBUGGER */
extern volatile uint32_t g_debugger_go;

/** Errore fatale: si ferma qui (breakpoint utile). what/status visibili dal
 * debugger. */
void board_fault(uint32_t what, int32_t status) __attribute__((noreturn));

#endif
