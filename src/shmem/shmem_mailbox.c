/*
 * shmem_mailbox.c
 *
 * See shmem_mailbox.h
 *
 * Portability note: the mailbox is placed in a MAP_SHARED anonymous mapping
 * so that it has exactly the semantics of the shared RAM region used on the
 * target. All the accesses go through C11 atomics, therefore the code would
 * work unchanged also with the region mapped from /dev/mem or from a reserved
 * memory carve-out.
 */

#include "shmem_mailbox.h"
#include "flow_time.h"

#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>

/* Number of retries when the seqlock shows a write in progress. */
#define SHM_SPIN_ATTEMPTS 20000u

/* ------------------------------------------------------------------ helpers */

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

/* Read the published bank together with its publication counter. */
static bool shm_chan_read (
   const flow_shm_chan_t * chan,
   flow_fsoe_slot_t * slot,
   uint32_t * published)
{
   uint32_t attempt;

   for (attempt = 0; attempt < SHM_SPIN_ATTEMPTS; attempt++)
   {
      uint32_t published_before;
      uint32_t published_after;
      uint32_t bank;

      published_before = atomic_load_explicit (
         &chan->published, memory_order_acquire);
      bank = atomic_load_explicit (&chan->active, memory_order_acquire);

      if (bank >= FLOW_SHM_BANKS)
      {
         return false;
      }

      if (!shm_bank_read (chan, bank, slot))
      {
         continue;
      }

      atomic_thread_fence (memory_order_acquire);
      published_after = atomic_load_explicit (
         &chan->published, memory_order_relaxed);

      if (published_before == published_after)
      {
         if (published != NULL)
         {
            *published = published_after;
         }
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
   atomic_init (&chan->active, 0u);
   atomic_init (&chan->published, 0u);
}

/* ---------------------------------------------------------------- lifecycle */

flow_shmem_t * flow_shmem_create (void)
{
   flow_shmem_t * shm;
   uint32_t k;

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
      shm_chan_init (&shm->tx[k]);
   }

   atomic_init (&shm->cores_active, 0u);
   atomic_init (&shm->cores_cycle, 0u);
   atomic_init (&shm->core_state[0], 0u);
   atomic_init (&shm->core_state[1], 0u);
   atomic_init (&shm->core_outputs[0], 0u);
   atomic_init (&shm->core_outputs[1], 0u);
   atomic_init (&shm->rx_publishes, 0u);
   atomic_init (&shm->rx_publish_deferred, 0u);
   atomic_init (&shm->tx_stages, 0u);
   atomic_init (&shm->tx_commits, 0u);

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

/* ---------------------------------------------------------------- A55 side */

bool flow_shmem_rx_publish (
   flow_shmem_t * shm,
   uint32_t k,
   const flow_fsoe_slot_t * slot)
{
   flow_shm_chan_t * chan;
   uint32_t active;
   uint32_t bank;

   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS))
   {
      return false;
   }

   /* 2-phase commit gate: never publish while a safety core is inside a
    * cycle, otherwise the two cores could latch two different snapshots. */
   if (atomic_load_explicit (&shm->cores_active, memory_order_acquire) != 0u)
   {
      atomic_fetch_add_explicit (
         &shm->rx_publish_deferred, 1u, memory_order_relaxed);
      return false;
   }

   chan = &shm->rx[k];
   active = atomic_load_explicit (&chan->active, memory_order_acquire);
   bank = (active + 1u) % FLOW_SHM_BANKS;

   shm_bank_write (chan, bank, slot);
   atomic_store_explicit (&chan->active, bank, memory_order_release);
   atomic_fetch_add_explicit (&chan->published, 1u, memory_order_release);
   atomic_fetch_add_explicit (&shm->rx_publishes, 1u, memory_order_relaxed);

   return true;
}

bool flow_shmem_tx_read (
   flow_shmem_t * shm,
   uint32_t k,
   flow_fsoe_slot_t * slot,
   uint32_t * published)
{
   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS))
   {
      return false;
   }

   return shm_chan_read (&shm->tx[k], slot, published);
}

/* -------------------------------------------------------------- cores side */

bool flow_shmem_rx_latch (
   flow_shmem_t * shm,
   uint32_t k,
   flow_fsoe_slot_t * slot,
   uint32_t * published)
{
   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS))
   {
      return false;
   }

   return shm_chan_read (&shm->rx[k], slot, published);
}

uint32_t flow_shmem_rx_active (flow_shmem_t * shm, uint32_t k)
{
   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS))
   {
      return 0u;
   }

   return atomic_load_explicit (&shm->rx[k].active, memory_order_acquire);
}

bool flow_shmem_rx_read_bank (
   flow_shmem_t * shm,
   uint32_t k,
   uint32_t bank,
   flow_fsoe_slot_t * slot)
{
   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS) || (bank >= FLOW_SHM_BANKS))
   {
      return false;
   }

   return shm_bank_read (&shm->rx[k], bank, slot);
}

bool flow_shmem_tx_stage (
   flow_shmem_t * shm,
   uint32_t k,
   const flow_fsoe_slot_t * slot,
   uint32_t * bank,
   uint32_t * staged_digest)
{
   flow_shm_chan_t * chan;
   uint32_t active;
   uint32_t staged_bank;

   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS) || (bank == NULL))
   {
      return false;
   }

   chan = &shm->tx[k];
   active = atomic_load_explicit (&chan->active, memory_order_acquire);
   staged_bank = (active + 1u) % FLOW_SHM_BANKS;

   shm_bank_write (chan, staged_bank, slot);

   *bank = staged_bank;
   if (staged_digest != NULL)
   {
      *staged_digest = slot->digest;
   }

   atomic_fetch_add_explicit (&shm->tx_stages, 1u, memory_order_relaxed);

   return true;
}

bool flow_shmem_tx_commit (
   flow_shmem_t * shm,
   uint32_t k,
   uint32_t bank,
   uint32_t expected_digest,
   uint32_t * published)
{
   flow_shm_chan_t * chan;
   flow_fsoe_slot_t staged;
   uint32_t attempt;

   if ((shm == NULL) || (k >= FLOW_NUM_CONNECTIONS) || (bank >= FLOW_SHM_BANKS))
   {
      return false;
   }

   chan = &shm->tx[k];

   /* M7 and M33 leave the messaging unit rendezvous at the same time, so M7
    * may not have finished writing the bank yet: retry for a while. */
   for (attempt = 0; attempt < FLOW_SHM_COMMIT_ATTEMPTS; attempt++)
   {
      if (shm_bank_read (chan, bank, &staged) &&
          (staged.digest == expected_digest) &&
          (staged.len > 0u))
      {
         atomic_store_explicit (&chan->active, bank, memory_order_release);
         atomic_fetch_add_explicit (
            &chan->published, 1u, memory_order_release);
         atomic_fetch_add_explicit (
            &shm->tx_commits, 1u, memory_order_relaxed);

         if (published != NULL)
         {
            *published = atomic_load_explicit (
               &chan->published, memory_order_relaxed);
         }
         return true;
      }

      flow_sleep_us (1u);
   }

   return false;
}

void flow_shmem_cores_enter_cycle (flow_shmem_t * shm, uint32_t cycle)
{
   atomic_fetch_add_explicit (&shm->cores_active, 1u, memory_order_seq_cst);
   atomic_store_explicit (&shm->cores_cycle, cycle, memory_order_relaxed);
}

void flow_shmem_cores_exit_cycle (flow_shmem_t * shm)
{
   atomic_fetch_sub_explicit (&shm->cores_active, 1u, memory_order_seq_cst);
}

bool flow_shmem_cores_idle (flow_shmem_t * shm)
{
   return atomic_load_explicit (&shm->cores_active, memory_order_acquire) == 0u;
}
