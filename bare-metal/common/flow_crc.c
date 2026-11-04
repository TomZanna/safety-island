/*
 * flow_crc.c
 *
 * See flow_crc.h.
 */

#include "flow_crc.h"

flow_u32 flow_crc32 (const void * data, flow_u32 size)
{
   const flow_u8 * bytes = (const flow_u8 *)data;
   flow_u32 crc = 0xFFFFFFFFu;
   flow_u32 i;
   flow_u32 bit;

   for (i = 0u; i < size; i++)
   {
      crc ^= (flow_u32)bytes[i];

      for (bit = 0u; bit < 8u; bit++)
      {
         if ((crc & 1u) != 0u)
         {
            crc = (crc >> 1u) ^ 0xEDB88320u;
         }
         else
         {
            crc >>= 1u;
         }
      }
   }

   return crc ^ 0xFFFFFFFFu;
}

flow_u16 flow_crc16 (const void * data, flow_u32 size)
{
   const flow_u8 * bytes = (const flow_u8 *)data;
   flow_u16 crc = 0xFFFFu;
   flow_u32 i;
   flow_u32 bit;

   for (i = 0u; i < size; i++)
   {
      crc ^= (flow_u16)((flow_u16)bytes[i] << 8u);

      for (bit = 0u; bit < 8u; bit++)
      {
         if ((crc & 0x8000u) != 0u)
         {
            crc = (flow_u16)((flow_u16)(crc << 1u) ^ 0x1021u);
         }
         else
         {
            crc = (flow_u16)(crc << 1u);
         }
      }
   }

   return crc;
}
