/**
 * @file board.c
 * @brief Board glue comune alle app dei core secondari i.MX94 (M33S, M7_0,
 * M7_1) con il System Manager.
 * @ingroup board
 *
 */
#include "board.h"

#include "scmi.h"

#ifndef WAIT_DEBUGGER
#  define WAIT_DEBUGGER 0
#endif

static volatile uint32_t s_millis;
static char s_agent[SCMI_BASE_MAX_NAME] = "?";

void SysTick_Handler(void)
{
  s_millis++;
}

uint32_t board_millis(void)
{
  return s_millis;
}

void board_fault(uint32_t what, int32_t status)
{
  volatile uint32_t fault_what = what;
  volatile int32_t fault_status = status;
  (void)fault_what;
  (void)fault_status;
  for (;;) {
    __BKPT(0); /* col debugger attaccato si ferma qui; senza, e' una HardFault
                  -> stesso effetto */
  }
}

/*
 * Con WAIT_DEBUGGER=1 il core aspetta il debugger in SystemInitHook(), chiamato
 * da SystemInit() del SDK prima di .data/.bss e di main(): cosi' i breakpoint
 * in tutto il codice C (main, board_init, ...) si colpiscono anche con l'attach
 * a core gia' avviato. Il debugger, all'attach, scrive g_debugger_go = 1
 * (launch.json: postAttachCommands; make debug-<app>: automatico).
 * Non si usa DHCSR.C_DEBUGEN: resta a 1 dopo una sessione precedente anche
 * attraverso il reset della board, e l'attesa verrebbe saltata. g_debugger_go
 * sta in .bss: qui va azzerata a mano (la RAM tiene il valore della sessione
 * precedente), poi lo startup la riazzera.
 */
volatile uint32_t g_debugger_go;

void SystemInitHook(void)
{
  /* Sempre referenziata: il debugger la scrive all'attach (postAttachCommands)
   * anche senza WAIT_DEBUGGER, e senza riferimenti --gc-sections la toglierebbe
   * dall'ELF (gdb: "Missing ELF symbol" e l'attach di Cortex-Debug si blocca).
   */
  (void)g_debugger_go;
#if WAIT_DEBUGGER
  g_debugger_go = 0U;
  while (g_debugger_go == 0U) {
  }
#endif
}

void board_init(void)
{
  uint32_t agentId = SCMI_BASE_ID_DISCOVER;

  rtt_init();

  /* Canale SCMI con l'SM (SDK, system_MIMX94398_<core>.c) */
  SystemPlatformInit();
  int32_t status =
      SCMI_BaseDiscoverAgent(SCMI_A2P, &agentId, (uint8_t*)s_agent);
  if (status != SCMI_ERR_SUCCESS) {
    s_agent[0] = '?';
    s_agent[1] = '\0';
    LOG("[%s] SCMI BaseDiscoverAgent: errore %d\n",
        BOARD_CORE_NAME,
        (int)status);
  }

  SystemCoreClockUpdate();
  SysTick_Config(SystemCoreClock / 1000U);
}

int board_getkey(void)
{
  return rtt_getkey();
}

const char* board_scmi_agent(void)
{
  return s_agent;
}
