/*
 * fsoe_slot.h
 *
 * An FSoE PDU as it is stored in the SHMEM mailbox, plus the metadata that
 * the black channel needs.
 *
 * Note that `fc` is the frame counter of the emulated black channel
 * (EtherCAT-like cyclic exchange). It is completely independent from the
 * FSoE sequence number, which is computed and checked inside the stack.
 */

#ifndef FSOE_SLOT_H
#define FSOE_SLOT_H

#include "flow_config.h"
#include "flow_digest.h"
#include "flow_log.h"

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct flow_fsoe_slot
{
   uint8_t pdu[FLOW_FSOE_PDU_MAX]; /**< Safety PDU as received/sent by the stack  */
   uint32_t len;                   /**< Number of valid bytes in pdu             */
   uint32_t fc;                    /**< Black channel frame counter              */
   uint32_t digest;                /**< flow_digest() of the first `len` bytes   */
} flow_fsoe_slot_t;

static inline void flow_fsoe_slot_clear (flow_fsoe_slot_t * slot)
{
   memset (slot, 0, sizeof (*slot));
}

static inline void flow_fsoe_slot_set (
   flow_fsoe_slot_t * slot,
   const void * pdu,
   uint32_t len,
   uint32_t fc)
{
   if (len > FLOW_FSOE_PDU_MAX)
   {
      len = FLOW_FSOE_PDU_MAX;
   }

   memset (slot->pdu, 0, sizeof (slot->pdu));
   memcpy (slot->pdu, pdu, len);
   slot->len = len;
   slot->fc = fc;
   slot->digest = flow_digest (slot->pdu, len);
}

static inline bool flow_fsoe_slot_is_empty (const flow_fsoe_slot_t * slot)
{
   return (slot->len == 0u);
}

static inline bool flow_fsoe_slot_equal (
   const flow_fsoe_slot_t * a,
   const flow_fsoe_slot_t * b)
{
   return (a->len == b->len) && (a->digest == b->digest);
}

/** \brief FSoE safety PDU command byte (ETG.5100 ch. 8.1.2 table 5) */
static inline uint8_t flow_fsoe_command (const flow_fsoe_slot_t * slot)
{
   return (slot->len > 0u) ? slot->pdu[0] : 0u;
}

/** \brief Human readable name of an FSoE command byte */
static inline const char * flow_fsoe_command_name (uint8_t command)
{
   switch (command)
   {
   case 0x36: return "ProcessData";
   case 0x2A: return "Reset";
   case 0x4E: return "Session";
   case 0x64: return "Connection";
   case 0x52: return "Parameter";
   case 0x08: return "FailSafeData";
   case 0x00: return "none";
   default:   return "invalid";
   }
}

/* ==========================================================================
 * Frame split / assembly
 * ==========================================================================
 *
 * The safety PDU is split between the two cores: one core contributes the CRC
 * words, the other one the command byte, the safety data and the Connection
 * ID. The A55 assembles the frame that goes on the fieldbus by interleaving
 * the two contributions.
 *
 * Consequence (this is the point of the mechanism): a single fault can never
 * produce a frame that the FSoE slave accepts, because the CRCs of one core
 * must match the data of the other core. The end-to-end check is therefore
 * performed by the standard FSoE CRC inside the slave, by a component that is
 * neither of the two cores nor the black channel.
 *
 * Byte layout (ETG.5100 ch. 8.1.1, see fsoeframe_set_data()/CalcCrc()):
 *   len == 6 : [0]=cmd [1]=data [2..3]=CRC_0 [4..5]=ConnId
 *   len >  6 : [0]=cmd [1..2]=data [3..4]=CRC_0 [5..6]=data [7..8]=CRC_1 ...
 *              i.e. CRC words at 3 + 4*j and the ConnId in the last 2 bytes.
 */

/** \brief Minimum FSoE PDU size (a single data byte) */
#define FLOW_FSOE_PDU_MIN_SIZE 6u

/** \brief Offset of the first safety data byte (Data[0]) */
#define FLOW_FSOE_PDU_FIRST_DATA_BYTE 1u

/** \brief true if the byte at \a offset of a frame of \a len bytes is a CRC byte */
static inline bool flow_fsoe_pdu_is_crc_byte (uint32_t len, uint32_t offset)
{
   if (len < FLOW_FSOE_PDU_MIN_SIZE)
   {
      return false;
   }

   if (len == FLOW_FSOE_PDU_MIN_SIZE)
   {
      return (offset == 2u) || (offset == 3u);
   }

   if ((offset < 3u) || (offset >= (len - 2u)))
   {
      return false; /* command + first data bytes, or Connection ID */
   }

   return (((offset - 3u) % 4u) < 2u);
}

/**
 * \brief Copy only one of the two contributions of a frame
 *
 * \param[out] dst       Buffer of at least FLOW_FSOE_PDU_MAX bytes
 * \param[in]  src       Complete frame
 * \param[in]  len       Frame length in bytes
 * \param[in]  crc_part  true = keep only the CRC bytes, false = keep the rest
 */
static inline void flow_fsoe_pdu_split_copy (
   uint8_t * dst,
   const uint8_t * src,
   uint32_t len,
   bool crc_part)
{
   uint32_t i;

   memset (dst, 0, FLOW_FSOE_PDU_MAX);

   if (len > FLOW_FSOE_PDU_MAX)
   {
      return;
   }

   for (i = 0; i < len; i++)
   {
      if (flow_fsoe_pdu_is_crc_byte (len, i) == crc_part)
      {
         dst[i] = src[i];
      }
   }
}

/**
 * \brief Assemble the final frame by interleaving the two contributions
 *
 * \param[out] dst        Buffer of at least FLOW_FSOE_PDU_MAX bytes
 * \param[in]  crc_part   Contribution of the CRC core
 * \param[in]  data_part  Contribution of the data core
 * \param[in]  len        Frame length in bytes
 */
static inline void flow_fsoe_pdu_assemble (
   uint8_t * dst,
   const uint8_t * crc_part,
   const uint8_t * data_part,
   uint32_t len)
{
   uint32_t i;

   memset (dst, 0, FLOW_FSOE_PDU_MAX);

   if (len > FLOW_FSOE_PDU_MAX)
   {
      return;
   }

   for (i = 0; i < len; i++)
   {
      dst[i] = flow_fsoe_pdu_is_crc_byte (len, i) ? crc_part[i] : data_part[i];
   }
}

/**
 * \brief Verify that the contribution of this core is intact in the stored copy
 *
 * \param[in] stored     Frame read back from the shared memory
 * \param[in] frame      Frame generated by the local stack
 * \param[in] len        Frame length in bytes
 * \param[in] crc_part   Which contribution this core owns
 */
static inline bool flow_fsoe_pdu_part_matches (
   const uint8_t * stored,
   const uint8_t * frame,
   uint32_t len,
   bool crc_part)
{
   uint32_t i;

   if (len > FLOW_FSOE_PDU_MAX)
   {
      return false;
   }

   for (i = 0; i < len; i++)
   {
      if ((flow_fsoe_pdu_is_crc_byte (len, i) == crc_part) &&
          (stored[i] != frame[i]))
      {
         return false;
      }
   }

   return true;
}

#endif /* FSOE_SLOT_H */
