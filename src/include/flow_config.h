/*
 * flow_config.h
 *
 * Compile-time configuration of the imx943-flow prototype.
 *
 * The prototype emulates, on a Linux PC, the architecture described in
 * sequence diag.md:
 *
 *   A55 (black channel / fieldbus)  <->  SHMEM mailbox  <->  { M7, M33 } safe cores
 *
 * Every component is compiled on its own (see Makefile) and the rt-labs FSoE
 * stack is linked in as a static library.
 */

#ifndef FLOW_CONFIG_H
#define FLOW_CONFIG_H

/* -------------------------------------------------------------------------
 * FSoE connection parameters
 * ---------------------------------------------------------------------- */

/* Number of independent FSoE connections handled by each core/slave.
 * Override at build time: make CONNECTIONS=2 */
#ifndef FLOW_NUM_CONNECTIONS
#define FLOW_NUM_CONNECTIONS 1u
#endif

/* Size in bytes of the safety process data (SafeInputs and SafeOutputs).
 * Must be even (or 1), see ETG.5100 and fsoeoptions.h. */
#define FLOW_SAFE_DATA_SIZE 4u

/* Worst case FSoE PDU size, equals FSOEMASTER_FRAME_SIZE (FLOW_SAFE_DATA_SIZE).
 * Checked with a static assertion in glue/fsoe_app_glue.c */
#define FLOW_FSOE_PDU_MAX (2u * FLOW_SAFE_DATA_SIZE + 3u)

#define FLOW_SLAVE_ADDRESS 0xFFFEu
#define FLOW_CONNECTION_ID 0x0003u
#define FLOW_WATCHDOG_MS 100u

/* Application parameters exchanged in Parameter state (2 x uint16). */
#define FLOW_APP_PARAMETERS_SIZE 4u
#define FLOW_APP_PARAMETER_1 0x1234u
#define FLOW_APP_PARAMETER_2 0x5678u

/* -------------------------------------------------------------------------
 * Timing
 * ---------------------------------------------------------------------- */

/* Rigid safe cycle of the two safety cores (hardware timer tick on target) */
#define FLOW_SAFE_CYCLE_MS 10u

/* Asynchronous black channel (A55) cycle */
#define FLOW_A55_CYCLE_MS 2u

/* Inter core messaging unit: time a core waits for the peer message before
 * declaring the cross check failed (fail-safe). */
#define FLOW_MU_TIMEOUT_US 3000u

/* Time M7 waits for M33 to park on the safe tick barrier before releasing
 * the cycle anyway (jitter / desync detection). */
#define FLOW_TICK_PARK_TIMEOUT_US 3000u

/* -------------------------------------------------------------------------
 * SHMEM mailbox emulation
 * ---------------------------------------------------------------------- */

/* Double buffered bank count (ping-pong 2-phase commit) */
#define FLOW_SHM_BANKS 2u

/* Bounded retries when a seqlock protected bank is being written */
#define FLOW_SHM_READ_ATTEMPTS 10000u

/* Bounded retries when M33 waits for M7 to stage a TX bank */
#define FLOW_SHM_COMMIT_ATTEMPTS 200u

/* -------------------------------------------------------------------------
 * Mini safe logic (see safe/safe_logic.c)
 * ---------------------------------------------------------------------- */

/* Number of consecutive cycles with discrepant channels before the safe
 * logic latches a fault */
#define FLOW_DISCREPANCY_CYCLES 3u

/* Value of the safe output word when the safety function is enabled.
 * The fail-safe value is always 0x0000. */
#define FLOW_SAFE_OUTPUT_ENABLED 0x00C3u

/* -------------------------------------------------------------------------
 * Misc
 * ---------------------------------------------------------------------- */

#define FLOW_DEFAULT_DURATION_MS 2000u
#define FLOW_DEFAULT_SEED 0x1A2B3C4Du

#define FLOW_ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))

#endif /* FLOW_CONFIG_H */
