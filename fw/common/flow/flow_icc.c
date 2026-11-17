/**
 * @file flow_icc.c
 * @brief Canale inter-core del 1oo2 (ICC): payload protetto in OCRAM + doorbell
 * MU18.
 * @ingroup flow_icc
 *
 */
#include "flow_icc.h"

#include "flow_crc.h"
#include "flow_mem.h"
#include "flow_mu18.h"
#include "flow_platform.h"

#define ICC_ACQUIRE(pointer) __atomic_load_n((pointer), __ATOMIC_ACQUIRE)
#define ICC_RELEASE(pointer, value) \
  __atomic_store_n((pointer), (value), __ATOMIC_RELEASE)

static flow_icc_t* const s_icc = (flow_icc_t*)FLOW_ICC_BASE_ADDR;
static flow_icc_stats_t s_stats;
static flow_u16 s_tx_sequence[FLOW_ICC_DIRS][FLOW_MU_CHAN_COUNT];
static flow_u32 s_cycle_seen;

static flow_u32 icc_dir_of(flow_core_id_t sender)
{
  return (sender == FLOW_CORE_SAFE_A) ? 0u : 1u;
}

void flow_icc_init(flow_core_id_t self)
{
  if (self != FLOW_CORE_FIELDBUS) {
    return; /* l'owner e' il fieldbus: gli altri ruoli non scrivono qui */
  }

  flow_memset(s_icc, 0, (flow_u32)sizeof(*s_icc));
  s_icc->cycle = 1u;
  __atomic_thread_fence(__ATOMIC_RELEASE);
  ICC_RELEASE(&s_icc->magic, FLOW_ICC_MAGIC);
}

bool flow_icc_ready(void)
{
  return ICC_ACQUIRE(&s_icc->magic) == FLOW_ICC_MAGIC;
}

bool flow_icc_wait_ready(flow_u32 timeout_us)
{
  const flow_u32 start = flow_platform_time_us();

  while (!flow_icc_ready()) {
    if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
      return false;
    }
  }

  return true;
}

bool flow_icc_post(flow_core_id_t self,
                   flow_core_id_t destination,
                   flow_u8 channel,
                   flow_u8 tag,
                   flow_u32 cycle,
                   const void* payload,
                   flow_u16 length)
{
  flow_icc_slot_t* slot;
  flow_u32 dir;
  flow_u16 sequence;
  flow_u32 crc;
  flow_u32 doorbell;

  (void)destination; /* un solo peer: SAFE_A <-> SAFE_B */

  if (!flow_icc_ready() || (payload == NULL) || (channel >= FLOW_MU_CHAN_COUNT)
      || (length > FLOW_ICC_PAYLOAD_MAX))
  {
    return false;
  }

  dir = icc_dir_of(self);
  slot = &s_icc->slot[dir][channel];

  sequence = (flow_u16)(++s_tx_sequence[dir][channel]);
  crc = flow_crc32(payload, length);

  /* Payload prima, `seq` per ultimo con release: chi vede il nuovo seq vede
   * tutto il resto. */
  slot->length = length;
  slot->cycle = cycle;
  slot->crc = crc;
  flow_memcpy(slot->payload, payload, length);
  __atomic_thread_fence(__ATOMIC_RELEASE);
  ICC_RELEASE(&slot->seq, (flow_u32)sequence);

  /* Doorbell: {seq, tag, channel} + CRC-32 del payload. */
  doorbell =
      ((flow_u32)sequence << 16) | ((flow_u32)tag << 8) | (flow_u32)channel;

  if (!flow_mu18_send(doorbell, crc, FLOW_MU_TIMEOUT_US)) {
    return false;
  }

  s_stats.posted++;
  return true;
}

bool flow_icc_wait(flow_core_id_t self,
                   flow_core_id_t source,
                   flow_u8 channel,
                   flow_u8 tag,
                   flow_u32 cycle,
                   void* payload,
                   flow_u16 max_length,
                   flow_u16* length,
                   flow_u32 timeout_us)
{
  const flow_u32 dir = icc_dir_of(source);
  const flow_u32 start = flow_platform_time_us();
  const flow_icc_slot_t* slot = &s_icc->slot[dir][channel];

  (void)self;

  if ((payload == NULL) || (channel >= FLOW_MU_CHAN_COUNT)) {
    return false;
  }

  for (;;) {
    flow_u32 word0 = 0u;
    flow_u32 word1 = 0u;

    if (flow_mu18_poll(&word0, &word1)) {
      const flow_u16 sequence = (flow_u16)(word0 >> 16);
      const flow_u8 doorbell_channel = (flow_u8)(word0 & 0xFFu);
      const flow_u8 doorbell_tag = (flow_u8)((word0 >> 8) & 0xFFu);

      if ((doorbell_channel != channel) || (doorbell_tag != tag)) {
        s_stats.doorbell_errors++;
      } else if ((flow_u16)ICC_ACQUIRE(&slot->seq) != sequence) {
        s_stats.seq_errors++;
      } else if ((slot->length > max_length)
                 || (slot->length > FLOW_ICC_PAYLOAD_MAX))
      {
        s_stats.seq_errors++;
      } else if (slot->cycle != cycle) {
        s_stats.cycle_errors++;
      } else {
        flow_u32 crc;

        flow_memcpy(payload, slot->payload, slot->length);
        crc = flow_crc32(payload, slot->length);

        /* Doppio controllo: CRC ricalcolato == slot (OCRAM) == doorbell (MU).
         */
        if ((crc != slot->crc) || (crc != word1)) {
          s_stats.crc_errors++;
        } else {
          if (length != NULL) {
            *length = slot->length;
          }
          s_stats.received++;
          return true;
        }
      }
    }

    if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
      s_stats.timeouts++;
      return false;
    }
  }
}

bool flow_icc_cycle_wait(flow_u32* cycle, flow_u32 timeout_us)
{
  const flow_u32 start = flow_platform_time_us();

  for (;;) {
    const flow_u32 current = ICC_ACQUIRE(&s_icc->cycle);

    if (current != s_cycle_seen) {
      s_cycle_seen = current;
      if (cycle != NULL) {
        *cycle = current;
      }
      return true;
    }

    if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
      s_stats.barrier_timeouts++;
      return false;
    }
  }
}

void flow_icc_cycle_done(flow_core_id_t self, flow_u32 cycle)
{
  if (self < FLOW_CORE_COUNT) {
    ICC_RELEASE(&s_icc->ready[self], cycle);
  }
}

bool flow_icc_cycle_release(flow_u32 cycle, flow_u32 timeout_us)
{
  const flow_u32 start = flow_platform_time_us();
  bool ok = true;

  /* Fase di avvio: finche' i due core safe non hanno completato un ciclo il
   * numero di ciclo NON avanza. Altrimenti il fieldbus lo farebbe correre
   * mentre i core sono ancora nel boot/selftest e i due partirebbero con numeri
   * diversi (e i cross-check, che confrontano il `cycle`, fallirebbero sempre).
   */
  if ((ICC_ACQUIRE(&s_icc->ready[FLOW_CORE_SAFE_A]) == 0u)
      || (ICC_ACQUIRE(&s_icc->ready[FLOW_CORE_SAFE_B]) == 0u))
  {
    return true;
  }

  /* Attende i due core safe. Su timeout si apre comunque il ciclo successivo:
   * chi e' vivo lo rileva dal cross-check e va in fail-safe, senza che un core
   * morto blocchi tutto. */
  for (;;) {
    const flow_u32 ready_a = ICC_ACQUIRE(&s_icc->ready[FLOW_CORE_SAFE_A]);
    const flow_u32 ready_b = ICC_ACQUIRE(&s_icc->ready[FLOW_CORE_SAFE_B]);

    if (((flow_s32)(ready_a - cycle) >= 0)
        && ((flow_s32)(ready_b - cycle) >= 0))
    {
      break;
    }

    if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
      s_stats.barrier_timeouts++;
      ok = false;
      break;
    }
  }

  ICC_RELEASE(&s_icc->cycle, cycle + 1u);
  return ok;
}

flow_u32 flow_icc_cycle_current(void)
{
  return ICC_ACQUIRE(&s_icc->cycle);
}

const flow_icc_stats_t* flow_icc_get_stats(void)
{
  return &s_stats;
}
