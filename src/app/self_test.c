/*
 * self_test.c
 *
 * Known answer tests (KAT) executed at startup, before the safe cycle begins.
 *
 * Why: the digest, the frame split rules and the safe logic are code that both
 * channels share. A fault (or a wrong build) in one of them would silently
 * defeat the 1oo2 cross-check, i.e. it would be a common cause failure. A KAT
 * verifies those mechanisms against known-good answers while the machine is
 * still in a safe state.
 */

#include "self_test.h"
#include "flow_digest.h"
#include "flow_log.h"
#include "fsoe_slot.h"
#include "safe_logic.h"

#include <string.h>

/* ------------------------------------------------------------ digest KAT */

typedef struct self_test_vector
{
   const char * text;
   uint32_t digest;
} self_test_vector_t;

/* Reference values of FNV-1a, 32 bit (well known test vectors). */
static const self_test_vector_t digest_vectors[] =
{
   { "",       0x811C9DC5u },
   { "a",      0xE40C292Cu },
   { "foobar", 0xBF9CF968u },
};

static bool self_test_digest (void)
{
   uint32_t i;

   for (i = 0; i < (uint32_t)FLOW_ARRAY_SIZE (digest_vectors); i++)
   {
      const self_test_vector_t * vector = &digest_vectors[i];
      const uint32_t digest =
         flow_digest (vector->text, strlen (vector->text));

      if (digest != vector->digest)
      {
         LOG_ERROR (FLOW_TAG_MAIN,
            "KAT FAILED (digest): '%s' -> 0x%08x, expected 0x%08x",
            vector->text,
            digest,
            vector->digest);
         return false;
      }
   }

   LOG_INFO (FLOW_TAG_MAIN,
      "KAT digest: %u reference vectors OK",
      (unsigned)FLOW_ARRAY_SIZE (digest_vectors));

   return true;
}

/* ------------------------------------------------------- frame split KAT */

/* A general 11 byte PDU: cmd, 4 data bytes, 2 CRC words, ConnID */
static const uint8_t self_test_frame_11[11] =
{
   0x36, 0x11, 0x22, 0x5A, 0xA5, 0x33, 0x44, 0xC3, 0x3C, 0x03, 0x00
};

/* A minimum size 6 byte PDU: cmd, 1 data byte, CRC_0, ConnID */
static const uint8_t self_test_frame_6[6] =
{
   0x08, 0x7E, 0x1D, 0xE2, 0x03, 0x00
};

static bool self_test_frame_case (const uint8_t * frame, uint32_t len)
{
   flow_fsoe_slot_t crc_part;
   flow_fsoe_slot_t data_part;
   flow_fsoe_slot_t assembled;
   uint32_t i;

   memset (&crc_part, 0, sizeof (crc_part));
   memset (&data_part, 0, sizeof (data_part));
   memset (&assembled, 0, sizeof (assembled));

   flow_fsoe_pdu_split_copy (crc_part.pdu, frame, len, true);
   flow_fsoe_pdu_split_copy (data_part.pdu, frame, len, false);

   for (i = 0; i < len; i++)
   {
      const bool crc_byte = flow_fsoe_pdu_is_crc_byte (len, i);

      if (crc_byte)
      {
         /* the CRC half must contain this byte, the data half must not */
         if ((crc_part.pdu[i] != frame[i]) || (data_part.pdu[i] != 0u))
         {
            LOG_ERROR (FLOW_TAG_MAIN,
               "KAT FAILED (frame split, len %u, byte %u)",
               (unsigned)len, (unsigned)i);
            return false;
         }
      }
      else
      {
         /* the data half must contain this byte, the CRC half must not */
         if ((data_part.pdu[i] != frame[i]) || (crc_part.pdu[i] != 0u))
         {
            LOG_ERROR (FLOW_TAG_MAIN,
               "KAT FAILED (frame split, len %u, byte %u)",
               (unsigned)len, (unsigned)i);
            return false;
         }
      }
   }

   /* Re-assembly must reproduce the original frame */
   flow_fsoe_pdu_assemble (assembled.pdu, crc_part.pdu, data_part.pdu, len);

   if (memcmp (assembled.pdu, frame, len) != 0)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (frame assembly, len %u)", (unsigned)len);
      return false;
   }

   /* The assembly must be sensitive to both halves: a corrupted data byte
    * must not survive the round trip. */
   data_part.pdu[FLOW_FSOE_PDU_FIRST_DATA_BYTE] ^= 0xFFu;
   flow_fsoe_pdu_assemble (assembled.pdu, crc_part.pdu, data_part.pdu, len);

   if (memcmp (assembled.pdu, frame, len) == 0)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (frame assembly is not sensitive to the data half)");
      return false;
   }

   /* A corrupted CRC byte must be visible as well */
   data_part.pdu[FLOW_FSOE_PDU_FIRST_DATA_BYTE] ^= 0xFFu;
   for (i = 0; i < len; i++)
   {
      if (flow_fsoe_pdu_is_crc_byte (len, i))
      {
         crc_part.pdu[i] ^= 0xFFu;
         break;
      }
   }
   flow_fsoe_pdu_assemble (assembled.pdu, crc_part.pdu, data_part.pdu, len);

   if (memcmp (assembled.pdu, frame, len) == 0)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (frame assembly is not sensitive to the CRC half)");
      return false;
   }

   return true;
}

static bool self_test_frame_split (void)
{
   if (!self_test_frame_case (self_test_frame_11, sizeof (self_test_frame_11)))
   {
      return false;
   }

   if (!self_test_frame_case (self_test_frame_6, sizeof (self_test_frame_6)))
   {
      return false;
   }

   LOG_INFO (FLOW_TAG_MAIN,
      "KAT frame split/assembly: 11 byte and 6 byte frames OK");

   return true;
}

/* -------------------------------------------------------- safe logic KAT */

static bool self_test_safe_logic (void)
{
   flow_safe_logic_t logic;
   flow_safe_inputs_t inputs;
   flow_safe_outputs_t outputs;
   uint32_t i;

   /* --- E-stop pressed on both channels: the safety function stays off --- */
   flow_safe_logic_init (&logic);
   inputs.ch_a = FLOW_SAFE_IN_DOOR_CLOSED;
   inputs.ch_b = FLOW_SAFE_IN_DOOR_CLOSED;

   for (i = 0; i < 5u; i++)
   {
      flow_safe_logic_step (&inputs, &logic, &outputs);
   }

   if ((outputs.enable != 0u) ||
       ((outputs.status & FLOW_SAFE_STATUS_ENABLED) != 0u))
   {
      LOG_ERROR (FLOW_TAG_MAIN, "KAT FAILED (safe logic: must stay off)");
      return false;
   }

   /* --- E-stop released + reset on a rising edge: ENABLED --- */
   inputs.ch_a = FLOW_SAFE_IN_ESTOP_OK | FLOW_SAFE_IN_DOOR_CLOSED |
      FLOW_SAFE_IN_RESET;
   inputs.ch_b = FLOW_SAFE_IN_ESTOP_OK | FLOW_SAFE_IN_DOOR_CLOSED;
   flow_safe_logic_step (&inputs, &logic, &outputs);

   if (outputs.enable != FLOW_SAFE_OUTPUT_ENABLED)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (safe logic: reset must enable, got 0x%04x)",
         (unsigned)outputs.enable);
      return false;
   }

   /* --- reset released: must stay ENABLED --- */
   inputs.ch_a = FLOW_SAFE_IN_ESTOP_OK | FLOW_SAFE_IN_DOOR_CLOSED;
   flow_safe_logic_step (&inputs, &logic, &outputs);

   if (outputs.enable != FLOW_SAFE_OUTPUT_ENABLED)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (safe logic: output must stay enabled)");
      return false;
   }

   /* --- guard door open: immediate safe stop --- */
   inputs.ch_a = FLOW_SAFE_IN_ESTOP_OK;
   inputs.ch_b = FLOW_SAFE_IN_ESTOP_OK;
   flow_safe_logic_step (&inputs, &logic, &outputs);

   if (outputs.enable != 0u)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (safe logic: door open must drop the output)");
      return false;
   }

   /* --- channel discrepancy for FLOW_DISCREPANCY_CYCLES: latched fault --- */
   flow_safe_logic_init (&logic);
   inputs.ch_a = FLOW_SAFE_IN_ESTOP_OK | FLOW_SAFE_IN_DOOR_CLOSED;
   inputs.ch_b = FLOW_SAFE_IN_DOOR_CLOSED; /* channel B says E-stop pressed */

   for (i = 0; i < FLOW_DISCREPANCY_CYCLES; i++)
   {
      flow_safe_logic_step (&inputs, &logic, &outputs);
   }

   if (logic.latched_fault == 0u)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (safe logic: discrepancy must latch a fault)");
      return false;
   }

   /* --- channels agree again, but the fault stays latched until a reset --- */
   inputs.ch_a = FLOW_SAFE_IN_ESTOP_OK | FLOW_SAFE_IN_DOOR_CLOSED;
   inputs.ch_b = FLOW_SAFE_IN_ESTOP_OK | FLOW_SAFE_IN_DOOR_CLOSED;
   flow_safe_logic_step (&inputs, &logic, &outputs);

   if ((outputs.enable != 0u) ||
       ((outputs.status & FLOW_SAFE_STATUS_LATCHED_FAULT) == 0u))
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "KAT FAILED (safe logic: latched fault must inhibit the output)");
      return false;
   }

   LOG_INFO (FLOW_TAG_MAIN, "KAT safe logic: state machine OK");

   return true;
}

/* ------------------------------------------------------------------- API */

bool flow_self_test_run (void)
{
   bool ok = true;

   LOG_INFO (FLOW_TAG_MAIN, "startup self-test (KAT) of the shared mechanisms");

   if (!self_test_digest ())
   {
      ok = false;
   }
   if (!self_test_frame_split ())
   {
      ok = false;
   }
   if (!self_test_safe_logic ())
   {
      ok = false;
   }

   if (!ok)
   {
      LOG_ERROR (FLOW_TAG_MAIN,
         "STARTUP SELF-TEST FAILED: refusing to start the safe cycle");
   }
   else
   {
      LOG_INFO (FLOW_TAG_MAIN, "startup self-test passed");
   }

   return ok;
}

