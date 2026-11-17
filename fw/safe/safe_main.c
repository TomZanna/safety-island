/**
 * @file safe_main.c
 * @brief Ciclo del core safe (SAFE_A su m70, SAFE_B su m71).
 * @ingroup flow_safe
 *
 * Ogni 10 ms, dopo il rendezvous di ciclo, esegue flow_safe_step() (i 4 passi
 * del diagramma). Fuori da qui restano solo la scelta della piattaforma
 * (safe_platform) e le callback FSoE (fsoe_app).
 *
 * Comandi RTT (canale del core): p/i/l/x = fault injection (PDU, inputs, logic,
 * session), '?' = stato.
 */

/**
 * @page rtt_log Formato dei log RTT
 *
 * Ogni secondo ogni ruolo stampa una riga sul proprio canale RTT. Le righe dei
 * due core safe sono volutamente identiche: se un contatore differisce, si vede
 * subito.
 *
 * | Campo | Significato |
 * |---|---|
 * | `cyc` / `bus` | cicli safe eseguiti / cicli da 2 ms del fieldbus |
 * | `Reset...Data` | stato FSoE corrente |
 * | `anom` | anomalie di cross-check cumulate (deve restare 0 in run pulito) |
 * | `icc p/r` | messaggi pubblicati/ricevuti sul canale inter-core |
 * | `to/crc/sq/cy/db` | timeout, CRC errati, seq incoerente, ciclo diverso,
 * doorbell inatteso | | `mu to` | timeout del doorbell MU18 (tx/rx) | | `sess`
 * | Session ID negoziato (deve essere uguale sui due core) | | `hb` | heartbeat
 * dei tre ruoli, letti dalla mailbox | | `out/en/ok/drop/a55` | fieldbus:
 * uscite valide, abilitate, frame buoni, frame scartati, frame invalidi |
 *
 * Il formato e' prodotto da `log_status()` in questo file (safe) e in
 * `fieldbus_main.c` (campo). I comandi RTT sono nel README; l'effetto di ogni
 * handler e' accanto al parser (`safe_keys()`, `fieldbus_keys()`).
 */
#include "safe.h"

#include <fsoemaster.h>

#include "board.h"
#include "flow_config.h"
#include "flow_icc.h"
#include "flow_init.h"
#include "flow_mu18.h"
#include "flow_platform.h"
#include "flow_safe.h"
#include "flow_selftest.h"
#include "flow_shm.h"
#include "safe_platform.h"
#include "selftest_safe.h"

static flow_shm_t* const s_shm = (flow_shm_t*)FLOW_SHM_BASE_ADDR;
static flow_safe_t s_safe;
static const char* s_tag;

static void log_status(flow_u32 cycle)
{
  const flow_safe_t* s = &s_safe;
  const flow_icc_stats_t* icc = flow_icc_get_stats();
  const flow_mu18_stats_t* mu = flow_mu18_get_stats();
  const char* state = fsoemaster_state_description(s->sync[0].current_state);

  flow_platform_log(s_tag,
                    "cyc %u %s anom %u icc p%u r%u to%u crc%u sq%u cy%u db%u "
                    "mu to%u/%u sess 0x%04x hb %u/%u/%u",
                    (unsigned)cycle,
                    state,
                    (unsigned)s->anomalies,
                    (unsigned)icc->posted,
                    (unsigned)icc->received,
                    (unsigned)icc->timeouts,
                    (unsigned)icc->crc_errors,
                    (unsigned)icc->seq_errors,
                    (unsigned)icc->cycle_errors,
                    (unsigned)icc->doorbell_errors,
                    (unsigned)mu->tx_timeouts,
                    (unsigned)mu->rx_timeouts,
                    (unsigned)s->session_id,
                    (unsigned)flow_shm_heartbeat(s_shm, FLOW_CORE_SAFE_A),
                    (unsigned)flow_shm_heartbeat(s_shm, FLOW_CORE_SAFE_B),
                    (unsigned)flow_shm_heartbeat(s_shm, FLOW_CORE_FIELDBUS));
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
        flow_platform_log(
            s_tag, "fault injection: PDU al ciclo %u", (unsigned)at);
        break;
      case 'i':
        flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_INPUTS, at);
        flow_platform_log(
            s_tag, "fault injection: inputs al ciclo %u", (unsigned)at);
        break;
      case 'l':
        flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_LOGIC, at);
        flow_platform_log(
            s_tag, "fault injection: logic al ciclo %u", (unsigned)at);
        break;
      case 'x':
        flow_safe_set_fault(&s_safe, FLOW_TEST_FAULT_SESSION, at);
        flow_platform_log(
            s_tag, "fault injection: session al ciclo %u", (unsigned)at);
        break;
      case '?':
        log_status(s_safe.cycle);
        break;
      default:
        flow_platform_log(
            s_tag, "comando '%c' sconosciuto (p, i, l, x, ?)", (char)c);
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
      flow_platform_log(s_tag, "barrier timeout (peer fermo?)");
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

void safe_main(flow_core_id_t self)
{
  if ((self != FLOW_CORE_SAFE_A) && (self != FLOW_CORE_SAFE_B)) {
    flow_platform_fatal();
  }

  s_tag = flow_role_log_tag(self);
  flow_init(self, FLOW_SAFE_CYCLE_MS);
  safe_platform_init_mu18(self);

  if (!flow_shm_wait_ready(s_shm, 2000000u) || !flow_icc_wait_ready(2000000u)) {
    flow_platform_log(s_tag, "mailbox/ICC non pronte: fail-safe");
    flow_platform_fatal();
  }

  if (!flow_selftest_run(self) || !safe_selftest_icc(self)) {
    flow_platform_fatal();
  }

  flow_safe_init(&s_safe, self, s_shm);
  flow_platform_log(
      s_tag, "core safe pronto (ciclo %u ms)", (unsigned)FLOW_SAFE_CYCLE_MS);
  safe_loop(self);

  flow_platform_fatal();
}
