/*
 * flow_config.h
 *
 * Compile-time configuration of the bare metal safety firmware.
 *
 * The safety core code is the same for M7 and M33; the only difference is
 * FLOW_CORE_ID, passed by the build system (-DFLOW_CORE_ID=0 or 1).
 */

#ifndef FLOW_CONFIG_H
#define FLOW_CONFIG_H

#include "flow_types.h"

/* Number of independent FSoE connections handled by each core. */
#ifndef FLOW_NUM_CONNECTIONS
#define FLOW_NUM_CONNECTIONS 1u
#endif

/* Safety process data size, in bytes (must be even or 1). */
#define FLOW_SAFE_DATA_SIZE 4u

/* FSoE PDU size, ETG.5100 ch. 8.1.1.  For 4 data bytes this is 11. */
#define FLOW_FSOE_PDU_SIZE(data_size) \
   (((data_size) == 1u) ? 6u : (2u * (data_size) + 3u))
#define FLOW_PDU_MAX FLOW_FSOE_PDU_SIZE (FLOW_SAFE_DATA_SIZE)

/* FSoE connection parameters. */
#define FLOW_SLAVE_ADDRESS 0xFFFEu
#define FLOW_CONNECTION_ID 0x0003u
#define FLOW_WATCHDOG_MS 100u

/* Application parameters exchanged in Parameter state (2 x uint16). */
#define FLOW_APP_PARAMETERS_SIZE 4u
#define FLOW_APP_PARAMETER_1 0x1234u
#define FLOW_APP_PARAMETER_2 0x5678u

/* SHMEM mailbox: triple buffering.  With three banks the bank selected by
 * MIN(count_m7, count_m33) is never the one being written, even when one core
 * is one or two publications behind. */
#define FLOW_SHM_BANKS 3u

/* Timing.  The real tick and the time source come from the platform
 * (hardware safe timer, TODO on the target). */
#define FLOW_SAFE_CYCLE_MS 10u
#define FLOW_A55_CYCLE_MS 2u

/* Time a core waits for the peer MU message before declaring the cross-check
 * failed.  Kept configurable so the host test can use a larger value. */
#ifndef FLOW_MU_TIMEOUT_US
#define FLOW_MU_TIMEOUT_US 3000u
#endif

/* Safety logic. */
#define FLOW_DISCREPANCY_CYCLES 3u
#define FLOW_SAFE_OUTPUT_ENABLED 0x00C3u

/* Debug features (logging, fault injection).  The production safety build
 * uses FLOW_DEBUG=0 and the related code is not linked. */
#ifndef FLOW_DEBUG
#define FLOW_DEBUG 0
#endif

/* Host-only test hooks. */
#ifndef FLOW_HOST_TEST
#define FLOW_HOST_TEST 0
#endif

/* Freestanding build: provide the memory functions that the compiler may
 * emit instead of linking a C library. */
#ifndef FLOW_FREESTANDING
#define FLOW_FREESTANDING 0
#endif

/* Core identity, overridden by the build system for M7/M33. */
#ifndef FLOW_CORE_ID
#define FLOW_CORE_ID FLOW_CORE_M7
#endif

/* Physical base of the shared mailbox on the target.  TODO(target): take it
 * from the device tree / memory map, or from the MPU configuration. */
#define FLOW_SHM_BASE_ADDR 0x80000000u

#define FLOW_ARRAY_SIZE(a) (sizeof (a) / sizeof ((a)[0]))

#endif /* FLOW_CONFIG_H */
