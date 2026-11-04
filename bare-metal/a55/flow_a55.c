/*
 * flow_a55.c
 *
 * See flow_a55.h.  A55-side merge of the two independent TX halves.
 */

#include "flow_a55.h"
#include "flow_mem.h"

static flow_u32 a55_min (flow_u32 a, flow_u32 b)
{
   return (a < b) ? a : b;
}

void flow_a55_init (flow_a55_t * a55, flow_shm_t * shm)
{
   flow_u32 k;

   if (a55 == NULL)
   {
      return;
   }

   flow_memset (a55, 0, (flow_u32)sizeof (*a55));

   if (shm == NULL)
   {
      return;
   }

   a55->shm = shm;

   for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++)
   {
      flow_u32 count_crc = flow_shm_count (&shm->tx[k][FLOW_SHM_CRC_NODE]);
      flow_u32 count_data = flow_shm_count (&shm->tx[k][FLOW_SHM_DATA_NODE]);

      a55->last_delivered[k] = a55_min (count_crc, count_data);
   }
}

flow_a55_result_t flow_a55_get_tx (
   flow_a55_t * a55,
   flow_u32 connection,
   flow_a55_frame_t * frame)
{
   const flow_shm_channel_t * crc_channel;
   const flow_shm_channel_t * data_channel;
   flow_shm_slot_t crc_slot;
   flow_shm_slot_t data_slot;
   flow_u32 count_crc;
   flow_u32 count_data;
   flow_u32 common;
   flow_u32 len;

   if ((a55 == NULL) || (a55->shm == NULL) || (frame == NULL) ||
       (connection >= FLOW_NUM_CONNECTIONS))
   {
      return FLOW_A55_INVALID;
   }

   crc_channel = &a55->shm->tx[connection][FLOW_SHM_CRC_NODE];
   data_channel = &a55->shm->tx[connection][FLOW_SHM_DATA_NODE];

   count_crc = flow_shm_count (crc_channel);
   count_data = flow_shm_count (data_channel);

   if ((count_crc == 0u) || (count_data == 0u))
   {
      return FLOW_A55_NO_DATA;
   }

   common = a55_min (count_crc, count_data);

   if (common < a55->last_delivered[connection])
   {
      /* The safety cores re-aligned their counters downwards after a fault.
       * Do not replay an old frame: resynchronise to the common value. */
      a55->last_delivered[connection] = common;
      return FLOW_A55_NO_DATA;
   }

   if (common == a55->last_delivered[connection])
   {
      return FLOW_A55_NO_DATA;
   }

   if (!flow_shm_read_index (crc_channel, common, &crc_slot) ||
       !flow_shm_read_index (data_channel, common, &data_slot))
   {
      a55->invalid_frames++;
      a55->last_delivered[connection] = common;
      return FLOW_A55_INVALID;
   }

   len = data_slot.len;

   if ((crc_slot.len != data_slot.len) ||
       (crc_slot.fc != data_slot.fc) ||
       (len < FLOW_PDU_MIN_SIZE) || (len > FLOW_PDU_MAX))
   {
      a55->invalid_frames++;
      a55->last_delivered[connection] = common;
      return FLOW_A55_INVALID;
   }

   flow_memset (frame, 0, (flow_u32)sizeof (*frame));
   frame->conn = connection;
   frame->seq = common;
   frame->fc = common;
   frame->len = len;
   flow_shm_assemble (frame->pdu, crc_slot.pdu, data_slot.pdu, len);

   a55->last_delivered[connection] = common;

   return FLOW_A55_OK;
}

flow_a55_result_t flow_a55_put_rx (
   flow_a55_t * a55,
   flow_u32 connection,
   const flow_u8 * pdu,
   flow_u32 len)
{
   flow_u32 frame;

   if ((a55 == NULL) || (a55->shm == NULL) || (pdu == NULL) ||
       (connection >= FLOW_NUM_CONNECTIONS) ||
       (len < FLOW_PDU_MIN_SIZE) || (len > FLOW_PDU_MAX))
   {
      return FLOW_A55_INVALID;
   }

   frame = flow_shm_count (&a55->shm->rx[connection]) + 1u;

   if (!flow_shm_publish (
          &a55->shm->rx[connection], len, frame, 0u, pdu, NULL))
   {
      return FLOW_A55_INVALID;
   }

   return FLOW_A55_OK;
}
