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

#endif /* FSOE_SLOT_H */
