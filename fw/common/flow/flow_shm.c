#include "flow_shm.h"

#include "flow_crc.h"
#include "flow_mem.h"
#include "flow_platform.h"

#define SHM_LOAD_ACQUIRE(pointer) __atomic_load_n((pointer), __ATOMIC_ACQUIRE)
#define SHM_STORE_RELEASE(pointer, value) __atomic_store_n((pointer), (value), __ATOMIC_RELEASE)
#define SHM_LOAD_RELAXED(pointer) __atomic_load_n((pointer), __ATOMIC_RELAXED)
#define SHM_STORE_RELAXED(pointer, value) __atomic_store_n((pointer), (value), __ATOMIC_RELAXED)
#define SHM_FENCE_ACQUIRE() __atomic_thread_fence(__ATOMIC_ACQUIRE)
#define SHM_FENCE_RELEASE() __atomic_thread_fence(__ATOMIC_RELEASE)

#define SHM_SPIN_ATTEMPTS 20000u

/* Il CRC copre len, flags e fc (i primi otto byte dello slot). Il campo reserved
 * deve essere 0 ed e' verificato a parte. La PDU e' gia' protetta da FSoE. */
static flow_u16 shm_header_crc(const flow_shm_slot_t *slot)
{
    return flow_crc16(&slot->len, 8u);
}

static flow_u32 shm_bank_of(flow_u32 index)
{
    return (index - 1u) % FLOW_SHM_BANKS;
}

void flow_shm_init(flow_shm_t *shm, flow_core_id_t owner)
{
    flow_u32 k;
    flow_u32 node;

    if (shm == NULL) {
        return;
    }

    /* Azzera tutto: magic = 0 e state = UNINIT finche' l'header non e' completo,
     * cosi' chi valida in questo istante vede "non pronta" e riprova. */
    flow_memset(shm, 0, (flow_u32)sizeof(*shm));

    shm->header.protocol_id = FLOW_SHM_PROTOCOL_ID;
    shm->header.version = FLOW_SHM_VERSION;
    shm->header.layout_hash = FLOW_SHM_LAYOUT_HASH;
    shm->header.conn_count = FLOW_NUM_CONNECTIONS;
    shm->header.banks = FLOW_SHM_BANKS;
    shm->header.slot_size = FLOW_SHM_SLOT_SIZE;
    shm->header.header_size = FLOW_SHM_HEADER_SIZE;
    shm->header.state = FLOW_SHM_STATE_READY;
    if (owner < FLOW_CORE_COUNT) {
        shm->header.heartbeat[owner] = 0u;
    }

    for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
        for (node = 0u; node < FLOW_SHM_NODE_COUNT; node++) {
            shm->tx[k][node].count = 0u;
        }
        shm->rx[k].count = 0u;
    }

    /* Pubblicazione: il magic per ultimo, con release. Quando un lettore lo
     * vede, tutto il resto dell'header e' visibile. */
    SHM_FENCE_RELEASE();
    SHM_STORE_RELEASE(&shm->header.magic, FLOW_SHM_MAGIC);
}

bool flow_shm_validate(const flow_shm_t *shm)
{
    if (shm == NULL) {
        return false;
    }

    if (SHM_LOAD_ACQUIRE(&shm->header.magic) != FLOW_SHM_MAGIC) {
        return false;
    }
    if (shm->header.protocol_id != FLOW_SHM_PROTOCOL_ID) {
        return false;
    }
    if (shm->header.version != FLOW_SHM_VERSION) {
        return false;
    }
    if (shm->header.layout_hash != FLOW_SHM_LAYOUT_HASH) {
        return false;
    }
    if (shm->header.conn_count != FLOW_NUM_CONNECTIONS) {
        return false;
    }
    if (shm->header.banks != FLOW_SHM_BANKS) {
        return false;
    }
    if (shm->header.slot_size != FLOW_SHM_SLOT_SIZE) {
        return false;
    }
    if (shm->header.header_size != FLOW_SHM_HEADER_SIZE) {
        return false;
    }
    if (shm->header.state != FLOW_SHM_STATE_READY) {
        return false;
    }

    return true;
}

bool flow_shm_wait_ready(const flow_shm_t *shm, flow_u32 timeout_us)
{
    const flow_u32 start = flow_platform_time_us();

    while ((flow_u32)(flow_platform_time_us() - start) < timeout_us) {
        if (flow_shm_validate(shm)) {
            return true;
        }
    }

    return flow_shm_validate(shm);
}

flow_u32 flow_shm_count(const flow_shm_channel_t *channel)
{
    if (channel == NULL) {
        return 0u;
    }

    return SHM_LOAD_ACQUIRE(&channel->count);
}

bool flow_shm_stage(
    flow_shm_channel_t *channel, flow_u32 index, flow_u32 len, flow_u32 fc, flow_u16 flags, const flow_u8 *pdu)
{
    flow_shm_slot_t slot;
    flow_shm_slot_t check;
    flow_u32 bank;
    flow_u32 sequence;

    if ((channel == NULL) || (pdu == NULL) || (index == 0u) || (len == 0u) || (len > FLOW_PDU_MAX)) {
        return false;
    }

    bank = shm_bank_of(index);

    flow_memset(&slot, 0, (flow_u32)sizeof(slot));
    slot.len = (flow_u16)len;
    slot.flags = flags;
    slot.fc = fc;
    slot.reserved = 0u;
    flow_memcpy(slot.pdu, pdu, len);
    slot.hdr_crc = shm_header_crc(&slot);

    sequence = SHM_LOAD_RELAXED(&channel->bank_seq[bank]);

    /* Sequence dispari: scrittore attivo, i lettori riprovano. */
    SHM_STORE_RELAXED(&channel->bank_seq[bank], sequence + 1u);
    SHM_FENCE_RELEASE();

    flow_memcpy(&channel->bank[bank], &slot, (flow_u32)sizeof(slot));

    SHM_FENCE_RELEASE();
    SHM_STORE_RELEASE(&channel->bank_seq[bank], sequence + 2u);

    /* Read-back: si verifica esattamente cio' che vedra' il peer. */
    if (!flow_shm_read_index(channel, index, &check)) {
        return false;
    }

    if ((check.len != (flow_u16)len) || (check.flags != flags) || (check.fc != fc) ||
        (flow_memcmp(check.pdu, pdu, len) != 0)) {
        return false;
    }

    return true;
}

void flow_shm_commit(flow_shm_channel_t *channel, flow_u32 index)
{
    if (channel != NULL) {
        SHM_STORE_RELEASE(&channel->count, index);
    }
}

bool flow_shm_read_index(const flow_shm_channel_t *channel, flow_u32 index, flow_shm_slot_t *slot)
{
    flow_u32 bank;
    flow_u32 attempt;

    if ((channel == NULL) || (slot == NULL) || (index == 0u)) {
        return false;
    }

    bank = shm_bank_of(index);

    for (attempt = 0u; attempt < SHM_SPIN_ATTEMPTS; attempt++) {
        flow_u32 sequence_before;
        flow_u32 sequence_after;

        sequence_before = SHM_LOAD_ACQUIRE(&channel->bank_seq[bank]);

        if ((sequence_before & 1u) != 0u) {
            continue; /* scrittore in corso */
        }

        flow_memcpy(slot, &channel->bank[bank], (flow_u32)sizeof(*slot));

        SHM_FENCE_ACQUIRE();
        sequence_after = SHM_LOAD_RELAXED(&channel->bank_seq[bank]);

        if (sequence_before != sequence_after) {
            continue;
        }

        if (slot->reserved != 0u) {
            return false;
        }
        if ((slot->len == 0u) || (slot->len > FLOW_PDU_MAX)) {
            return false;
        }
        if (slot->hdr_crc != shm_header_crc(slot)) {
            return false;
        }

        return true;
    }

    return false;
}

bool flow_shm_publish(flow_shm_channel_t *channel, flow_u32 len, flow_u32 fc, flow_u16 flags, const flow_u8 *pdu,
    flow_u32 *index)
{
    flow_u32 next;

    if (channel == NULL) {
        return false;
    }

    next = flow_shm_count(channel) + 1u;

    if (!flow_shm_stage(channel, next, len, fc, flags, pdu)) {
        return false;
    }

    flow_shm_commit(channel, next);

    if (index != NULL) {
        *index = next;
    }

    return true;
}

void flow_shm_update_heartbeat(flow_shm_t *shm, flow_core_id_t core, flow_u32 value)
{
    if ((shm != NULL) && (core < FLOW_CORE_COUNT)) {
        SHM_STORE_RELEASE(&shm->header.heartbeat[core], value);
    }
}

void flow_shm_split(flow_u8 *destination, const flow_u8 *frame, flow_u32 len, bool crc_part)
{
    flow_u32 i;

    if (destination == NULL) {
        return;
    }

    flow_memset(destination, 0, FLOW_PDU_MAX);

    if ((frame == NULL) || (len > FLOW_PDU_MAX)) {
        return;
    }

    for (i = 0u; i < len; i++) {
        if ((flow_pdu_is_crc_byte(len, i) != 0) == crc_part) {
            destination[i] = frame[i];
        }
    }
}

void flow_shm_assemble(flow_u8 *destination, const flow_u8 *crc_part, const flow_u8 *data_part, flow_u32 len)
{
    flow_u32 i;

    if (destination == NULL) {
        return;
    }

    flow_memset(destination, 0, FLOW_PDU_MAX);

    if ((crc_part == NULL) || (data_part == NULL) || (len > FLOW_PDU_MAX)) {
        return;
    }

    for (i = 0u; i < len; i++) {
        destination[i] = (flow_pdu_is_crc_byte(len, i) != 0) ? crc_part[i] : data_part[i];
    }
}

bool flow_shm_part_matches(const flow_u8 *stored, const flow_u8 *frame, flow_u32 len, bool crc_part)
{
    flow_u32 i;

    if ((stored == NULL) || (frame == NULL) || (len > FLOW_PDU_MAX)) {
        return false;
    }

    for (i = 0u; i < len; i++) {
        if (((flow_pdu_is_crc_byte(len, i) != 0) == crc_part) && (stored[i] != frame[i])) {
            return false;
        }
    }

    return true;
}


