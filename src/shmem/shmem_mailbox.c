/*
 * shmem_mailbox.c
 *
 * See shmem_mailbox.h
 *
 * Portability note: the mailbox is placed in a MAP_SHARED anonymous mapping so
 * that it has exactly the semantics of the shared RAM region used on the
 * target. All the accesses go through C11 atomics, therefore the code works
 * unchanged also with the region mapped from /dev/mem or from a reserved
 * memory carve-out.
 */

#include "shmem_mailbox.h"
#include "flow_log.h"

#include <string.h>
#include <sys/mman.h>

/* Number of retries when the seqlock shows a write in progress. */
#define SHM_SPIN_ATTEMPTS 20000u

/* ------------------------------------------------------------------ helpers */

/* Frame number `index` is 1-based: frame 1 lives in bank 0. */
static uint32_t shm_bank_of (uint32_t index)
{
   return (index - 1u) % FLOW_SHM_BANKS;
}

static void shm_bank_write (
   flow_shm_chan_t * chan,
   uint32_t bank,
   const flow_fsoe_slot_t * slot)
{
   uint32_t seq;

   seq = atomic_load_explicit (&chan->bank_seq[bank], memory_order_relaxed);

   /* Odd value: a writer is active, readers with a matching counter retry. */
   atomic_store_explicit (
      &chan->bank_seq[bank], seq + 1u, memory_order_relaxed);
   atomic_thread_fence (memory_order_release);

   chan->bank[bank] = *slot;

   atomic_thread_fence (memory_order_release);
   atomic_store_explicit (
      &chan->bank_seq[bank], seq + 2u, memory_order_release);
}

static bool shm_bank_read (
   const flow_shm_chan_t * chan,
   uint32_t bank,
   flow_fsoe_slot_t * slot)
{
   uint32_t attempt;

   for (attempt = 0; attempt < SHM_SPIN_ATTEMPTS; attempt++)
   {
      uint32_t seq_before;
      uint32_t seq_after;

      seq_before = atomic_load_explicit (
         &chan->bank_seq[bank], memory_order_acquire);

      if ((seq_before & 1u) != 0u)
      {
         continue; /* writer in progress */
      }

      *slot = chan->bank[bank];

      atomic_thread_fence (memory_order_acquire);
      seq_after = atomic_load_explicit (
         &chan->bank_seq[bank], memory_order_relaxed);

      if (seq_before == seq_after)
      {
         return true;
      }
   }

   return false;
}

static void shm_chan_init (flow_shm_chan_t * chan)
{
   uint32_t bank;

   for (bank = 0; bank < FLOW_SHM_BANKS; bank++)
   {
      flow_fsoe_slot_clear (&chan->bank[bank]);
      atomic_init (&chan->bank_seq[bank], 0u);
   }
   atomic_init (&chan->count, 0u);
}

/* ---------------------------------------------------------------- lifecycle */

flow_shmem_t * flow_shmem_create (void)
{
   flow_shmem_t * shm;
   uint32_t k;
   uint32_t node;

   shm = mmap (
      NULL,
      sizeof (*shm),
      PROT_READ | PROT_WRITE,
      MAP_SHARED | MAP_ANONYMOUS,
      -1,
      0);

   if (shm == MAP_FAILED)
   {
      return NULL;
   }

   memset (shm, 0, sizeof (*shm));

   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      shm_chan_init (&shm->rx[k]);
      for (node = 0; node < (uint32_t)MU_NODE_COUNT; node++)
      {
         shm_chan_init (&shm->tx[k][node]);
      }
   }

   atomic_init (&shm->core_state[0], 0u);
   atomic_init (&shm->core_state[1], 0u);
   atomic_init (&shm->core_outputs[0], 0u);
   atomic_init (&shm->core_outputs[1], 0u);
   atomic_init (&shm->tx_delivered, 0u);
   atomic_init (&shm->rx_publishes, 0u);

   atomic_init (&shm->a55.bus_cycles, 0u);
   for (k = 0; k < FLOW_NUM_CONNECTIONS; k++)
   {
      atomic_init (&shm->a55.slave_state[k], 0u);
      atomic_init (&shm->a55.slave_frames_in[k], 0u);
      atomic_init (&shm->a55.slave_frames_out[k], 0u);
   }
   atomic_init (&shm->a55.tx_frames_read, 0u);
   atomic_init (&shm->a55.tx_frames_dropped, 0u);
   atomic_init (&shm->a55.safe_outputs_word0, 0u);
   atomic_init (&shm->a55.safe_outputs_word1, 0u);
   atomic_init (&shm->a55.actuator_enabled, 0u);

   LOG_INFO (FLOW_TAG_SHM,
      "mailbox mapped at %p (%zu bytes, %u connection(s), %u banks/channel)",
      (void *)shm,
      sizeof (*shm),
      (unsigned)FLOW_NUM_CONNECTIONS,
      (unsigned)FLOW_SHM_BANKS);

   return shm;
}

void flow_shmem_destroy (flow_shmem_t * shm)
{
   if (shm != NULL)
   {
      (void)munmap (shm, sizeof (*shm));
   }
}

/* ------------------------------------------------------- generic producers */

uint32_t flow_shm_count (const flow_shm_chan_t * chan)
{
   return atomic_load_explicit (&chan->count, memory_order_acquire);
}

bool flow_shm_stage (
   flow_shm_chan_t * chan,
   uint32_t index,
   const flow_fsoe_slot_t * slot,
   uint32_t * stored_digest)
{
   flow_fsoe_slot_t stored;
   uint32_t bank;

   if ((chan == NULL) || (slot == NULL) || (index == 0u))
   {
      return false;
   }

   bank = shm_bank_of (index);

   shm_bank_write (chan, bank, slot);

   /* Read the frame back from SHMEM: what the cores cross-check is the data
    * as it really landed in the shared memory, not the local copy the stack
    * generated. This catches a corruption of the SHMEM copy. */
   if (!shm_bank_read (chan, bank, &stored))
   {
      return false;
   }

   if (stored_digest != NULL)
   {
      *stored_digest = stored.digest;
   }

   return true;
}

void flow_shm_commit (flow_shm_chan_t * chan, uint32_t index)
{
   if ((chan != NULL) && (index != 0u))
   {
      atomic_store_explicit (&chan->count, index, memory_order_release);
   }
}

bool flow_shm_publish_frame (
   flow_shm_chan_t * chan,
   const flow_fsoe_slot_t * slot,
   uint32_t * index)
{
   uint32_t next;

   if (chan == NULL)
   {
      return false;
   }

   next = flow_shm_count (chan) + 1u;

   if (!flow_shm_stage (chan, next, slot, NULL))
   {
      return false;
   }

   flow_shm_commit (chan, next);

   if (index != NULL)
   {
      *index = next;
   }

   return true;
}

bool flow_shm_read_index (
   const flow_shm_chan_t * chan,
   uint32_t index,
   flow_fsoe_slot_t * slot)
{
   if ((chan == NULL) || (slot == NULL) || (index == 0u))
   {
      return false;
   }

   return shm_bank_read (chan, shm_bank_of (index), slot);
}

