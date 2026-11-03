/*
 * shmem_mailbox.h
 *
 * Emulation of the shared memory mailbox that, on the i.MX943 target, is
 * placed in a physical memory region visible to the A55 and protected for the
 * two safety cores by the MPU.
 *
 * On the PC the mailbox lives in an anonymous MAP_SHARED mapping (real shared
 * memory, same semantics as the target) and is accessed as follows:
 *
 *   A55  --[rx banks]-->  M7 / M33      (frames received from the fieldbus)
 *   M7   --[tx banks]-->  M33 --> A55   (frames to be put on the fieldbus)
 *
 * Every channel is double buffered (ping-pong) and every bank is protected by
 * a seqlock (even/odd counter). Publishing a bank is a 2-phase commit:
 *
 *   - A55  : writes the *inactive* bank, then publishes it (rx direction).
 *            Publication is deferred while a safety core is inside a safe
 *            cycle (cores_active != 0) so both cores always latch the very
 *            same snapshot.
 *   - M7   : stages the frame it wants to send in the inactive bank.
 *   - M33  : verifies the staged bank (digest) and commits it, i.e. it flips
 *            the active index. Only then A55 can read the frame.
 */

#ifndef SHMEM_MAILBOX_H
#define SHMEM_MAILBOX_H

#include "flow_config.h"
#include "fsoe_slot.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct flow_shm_chan
{
   flow_fsoe_slot_t bank[FLOW_SHM_BANKS];      /**< Ping-pong data banks       */
   atomic_uint bank_seq[FLOW_SHM_BANKS];       /**< Seqlock, odd while writing */
   atomic_uint active;                         /**< Index of the published bank*/
   atomic_uint published;                      /**< Number of publications     */
} flow_shm_chan_t;

/** \brief Telemetry written by A55 while it reads the SHMEM */
typedef struct flow_shm_a55_telemetry
{
   atomic_uint bus_cycles;
   atomic_uint slave_state[FLOW_NUM_CONNECTIONS];
   atomic_uint slave_frames_in[FLOW_NUM_CONNECTIONS];
   atomic_uint slave_frames_out[FLOW_NUM_CONNECTIONS];
   atomic_uint tx_frames_read;                 /**< Frames taken from SHMEM TX  */
   atomic_uint tx_frames_dropped;              /**< Dropped on purpose (fault)  */
   atomic_uint safe_outputs_word0;             /**< Last SafeOutputs read       */
   atomic_uint safe_outputs_word1;
   atomic_uint actuator_enabled;
} flow_shm_a55_telemetry_t;

typedef struct flow_shmem
{
   flow_shm_chan_t rx[FLOW_NUM_CONNECTIONS];   /**< A55 -> safe cores            */
   flow_shm_chan_t tx[FLOW_NUM_CONNECTIONS];   /**< safe cores -> A55            */

   atomic_uint cores_active;                   /**< Cores currently in a cycle   */
   atomic_uint cores_cycle;                    /**< Last completed safe cycle    */
   atomic_uint core_state[2];                  /**< [0]=M7, [1]=M33 (for reports)*/
   atomic_uint core_outputs[2];                /**< Last SafeOutputs word0       */

   atomic_uint rx_publishes;                   /**< A55 rx publications          */
   atomic_uint rx_publish_deferred;            /**< A55 publications postponed   */
   atomic_uint tx_stages;                      /**< M7 stagings                  */
   atomic_uint tx_commits;                     /**< M33 commits                  */

   flow_shm_a55_telemetry_t a55;               /**< Written/read by A55          */
} flow_shmem_t;

/** \brief Allocate the SHMEM mailbox in a shared mapping (like the target) */
flow_shmem_t * flow_shmem_create (void);
void flow_shmem_destroy (flow_shmem_t * shm);

/* ---------------------------------------------------------------- A55 side */

/**
 * \brief Publish a frame received from the fieldbus into the RX channel
 *
 * The write is performed only when no safety core is inside a safe cycle, so
 * that M7 and M33 always latch the same snapshot. Returns false when the
 * publication has been deferred (the caller must retry later) or failed.
 */
bool flow_shmem_rx_publish (
   flow_shmem_t * shm,
   uint32_t k,
   const flow_fsoe_slot_t * slot);

/** \brief Read the last committed TX frame (what the cores want to send) */
bool flow_shmem_tx_read (
   flow_shmem_t * shm,
   uint32_t k,
   flow_fsoe_slot_t * slot,
   uint32_t * published);

/* -------------------------------------------------------------- cores side */

/** \brief Latch the published RX frame into local RAM (diagram step 1) */
bool flow_shmem_rx_latch (
   flow_shmem_t * shm,
   uint32_t k,
   flow_fsoe_slot_t * slot,
   uint32_t * published);

/**
 * \brief Stage a TX frame in the inactive bank (M7, phase 1)
 *
 * The frame is *not* visible to A55 until M33 commits the bank.
 * \param[out] bank   Index of the bank that has been written
 */
bool flow_shmem_tx_stage (
   flow_shmem_t * shm,
   uint32_t k,
   const flow_fsoe_slot_t * slot,
   uint32_t * bank,
   uint32_t * staged_digest);

/**
 * \brief Validate and commit a staged TX bank (M33, phase 2)
 *
 * Reads the staged bank back from SHMEM through the seqlock, checks that its
 * digest is the one agreed over the messaging unit and, only in that case,
 * publishes the bank.
 */
bool flow_shmem_tx_commit (
   flow_shmem_t * shm,
   uint32_t k,
   uint32_t bank,
   uint32_t expected_digest,
   uint32_t * published);

/** \brief Read a specific bank (used to re-read the snapshot latched by M7) */
bool flow_shmem_rx_read_bank (
   flow_shmem_t * shm,
   uint32_t k,
   uint32_t bank,
   flow_fsoe_slot_t * slot);

/** \brief Index of the RX bank currently published */
uint32_t flow_shmem_rx_active (flow_shmem_t * shm, uint32_t k);

void flow_shmem_cores_enter_cycle (flow_shmem_t * shm, uint32_t cycle);
void flow_shmem_cores_exit_cycle (flow_shmem_t * shm);
bool flow_shmem_cores_idle (flow_shmem_t * shm);

#endif /* SHMEM_MAILBOX_H */
