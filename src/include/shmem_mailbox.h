/*
 * shmem_mailbox.h
 *
 * Emulation of the shared memory mailbox that, on the i.MX943 target, is
 * placed in a physical memory region visible to the A55 and protected for the
 * two safety cores by the MPU.
 *
 * Protocol (no master, no coordinator, no announcement between the cores):
 *
 *   TX  core -> A55 : every core owns its own bank array and its own counter
 *                     `count` (frames published so far). Frame number n is
 *                     stored in bank (n-1) % FLOW_SHM_BANKS. The A55 computes
 *                     `common = MIN(count_m7, count_m33)` and reads frame
 *                     `common` from the core that is at `common`: that core
 *                     has just written that bank, so the bank is intact and
 *                     the content is the frame it generated.
 *   RX  A55 -> core : single writer, A55 owns the counter. Every core reads
 *                     the newest published frame using its own copy of the
 *                     counter; nothing is announced between the cores.
 *
 * The two cores keep their counters aligned by the protocol: a frame is
 * published only after the FSoE PDU cross-check over the messaging unit has
 * succeeded. A divergence of the counters is a fault (detected by the cores in
 * the messaging unit), which brings the system fail-safe and re-aligns the
 * counters to MIN(own, peer) - a commutative operation, so both cores take the
 * same decision in the same cycle without talking to a master.
 *
 * Every bank is protected by a seqlock (even/odd counter) so that the reader
 * never sees a partially written frame.
 */

#ifndef SHMEM_MAILBOX_H
#define SHMEM_MAILBOX_H

#include "flow_config.h"
#include "fsoe_slot.h"
#include "mu_channel.h"

#include <stdatomic.h>
#include <stdbool.h>
#include <stdint.h>

/** \brief One direction of the mailbox: N banks + a free running counter */
typedef struct flow_shm_chan
{
   flow_fsoe_slot_t bank[FLOW_SHM_BANKS];
   atomic_uint bank_seq[FLOW_SHM_BANKS]; /**< Seqlock, odd while writing */
   atomic_uint count;                    /**< Frames published so far     */
} flow_shm_chan_t;

/** \brief Telemetry written by A55 while it reads the SHMEM */
typedef struct flow_shm_a55_telemetry
{
   atomic_uint bus_cycles;
   atomic_uint slave_state[FLOW_NUM_CONNECTIONS];
   atomic_uint slave_frames_in[FLOW_NUM_CONNECTIONS];
   atomic_uint slave_frames_out[FLOW_NUM_CONNECTIONS];
   atomic_uint tx_frames_read;    /**< Frames merged from the two cores     */
   atomic_uint tx_frames_dropped; /**< Dropped on purpose (fault injection) */
   atomic_uint safe_outputs_word0;/**< Last SafeOutputs read                */
   atomic_uint safe_outputs_word1;
   atomic_uint actuator_enabled;
} flow_shm_a55_telemetry_t;

typedef struct flow_shmem
{
   flow_shm_chan_t tx[FLOW_NUM_CONNECTIONS][MU_NODE_COUNT]; /**< [k][core]   */
   flow_shm_chan_t rx[FLOW_NUM_CONNECTIONS];                /**< A55 -> core */

   atomic_uint core_state[2];   /**< [0]=M7, [1]=M33 (telemetry)             */
   atomic_uint core_outputs[2];
   atomic_uint tx_delivered;    /**< Frames merged and sent to the fieldbus  */
   atomic_uint rx_publishes;    /**< Frames published by A55                 */

   flow_shm_a55_telemetry_t a55;
} flow_shmem_t;

/** \brief Allocate the SHMEM mailbox in a shared mapping (like the target) */
flow_shmem_t * flow_shmem_create (void);
void flow_shmem_destroy (flow_shmem_t * shm);

/* ------------------------------------------------------- generic producers */

/** \brief Number of frames published on this channel so far */
uint32_t flow_shm_count (const flow_shm_chan_t * chan);

/**
 * \brief Write a frame in the bank of the given frame number (do not publish)
 *
 * \param[in]  chan           Channel
 * \param[in]  index          1-based frame number that will be published
 * \param[in]  slot           Frame
 * \param[out] stored_digest  Digest of the bytes as stored in SHMEM
 *                            (read back, catches corruption of the copy)
 */
bool flow_shm_stage (
   flow_shm_chan_t * chan,
   uint32_t index,
   const flow_fsoe_slot_t * slot,
   uint32_t * stored_digest);

/**
 * \brief Publish a staged frame number
 *
 * Separate from flow_shm_stage() because the two safety cores publish only
 * after the FSoE PDU cross-check over the messaging unit succeeded.
 * It can also be used to step the counter back (re-alignment to MIN).
 */
void flow_shm_commit (flow_shm_chan_t * chan, uint32_t index);

/** \brief Stage + commit in one call (single writer channels, i.e. A55 -> cores) */
bool flow_shm_publish_frame (
   flow_shm_chan_t * chan,
   const flow_fsoe_slot_t * slot,
   uint32_t * index);

/** \brief Read frame number `index` (1-based) through the seqlock */
bool flow_shm_read_index (
   const flow_shm_chan_t * chan,
   uint32_t index,
   flow_fsoe_slot_t * slot);

#endif /* SHMEM_MAILBOX_H */

