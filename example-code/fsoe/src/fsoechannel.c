/*********************************************************************
 *        _       _         _
 *  _ __ | |_  _ | |  __ _ | |__   ___
 * | '__|| __|(_)| | / _` || '_ \ / __|
 * | |   | |_  _ | || (_| || |_) |\__ \
 * |_|    \__|(_)|_| \__,_||_.__/ |___/
 *
 * www.rt-labs.com
 * Copyright 2019 rt-labs AB, Sweden.
 * See LICENSE file in the project root for full license information.
 ********************************************************************/

/**
 * \file
 * \brief FSoE black channel implementation
 *
 * See public header file for description.
 * All comments below are implementation details only.
 *
 * Naming convention
 * -----------------
 *
 * Two different naming conventions are used:
 * - For variables and functions defined by the standard, the names used by the
 *   standard are used (UpperCamelCase for functions and variables,
 *   UPPER_CASE_WITH_UNDERSCORES for "macro" functions.
 * - In all other cases, lower_case_with_underscores are used for functions
 *   and variables and UPPER_CASE_WITH_UNDERSCORES for macros and enum fields.
 */

/* Replace function call with call to mock function when building
 * unit tests.
 */
#ifdef UNIT_TEST
#define fsoeframe_add_crc_words mock_fsoeframe_add_crc
#endif

#include "fsoechannel.h"
#include "fsoeframe.h"
#include "fsoeport.h"
#include "fsoelog.h"
#include <fsoeapp.h>
#include <string.h>

#ifdef FSOE_LOG_ENABLE
/** \brief Get description of boolean value as a string
 *
 * \param[in]     boolean        Boolean value (true or false)
 * \return                       String constant "TRUE" or "FALSE"
 */
static const char * fsoechannel_boolean_as_string (bool boolean)
{
   const char * string;

   if (boolean == false)
   {
      string = "FALSE";
   }
   else
   {
      string = "TRUE";
   }

   return string;
}
#endif /* FSOE_LOG_ENABLE */

/* \brief Try to receive a frame from black channel
 *
 * See public header file for description
 */
const fsoeframe_t * fsoechannel_receive_frame (
   fsoechannel_t * channel)
{
   size_t received_bytes;
   const fsoeframe_t * received_frame;

   received_bytes = fsoeapp_recv (
      channel->app_ref,
      fsoeframe_buffer (&channel->received_frame),
      fsoeframe_size (&channel->received_frame));
   FSOEPORT_ASSERT (!fsoeframe_is_overflowed (&channel->received_frame));

   /* Was new frame received? */
   if (received_bytes == 0)
   {
      FSOELOG_DEBUG (FSOE_LOG_CHANNEL, "No frame received\n");
      received_frame =  NULL;
   }
   else if (fsoeframe_equals (&channel->received_frame, &channel->last_received_frame))
   {
      /* See ETG.5100 ch. 8.1.1 "Safety PDU structure" */
      FSOELOG_DEBUG (FSOE_LOG_CHANNEL,
         "Previous frame received again. Discarding\n");
      received_frame = NULL;
   }
   else
   {
      FSOELOG_DEBUG (FSOE_LOG_CHANNEL, "New frame received:\n");
      FSOELOG_DEBUG (FSOE_LOG_CHANNEL,
         "Frame.Command=%s, .Crc0=0x%04"PRIx16", .ConnId=%"PRIu16"\n",
         fsoeframe_command_name (fsoeframe_command (&channel->received_frame)),
         fsoeframe_crc0 (&channel->received_frame),
         fsoeframe_connection_id (&channel->received_frame));
      fsoeframe_copy (&channel->received_frame, &channel->last_received_frame);
      received_frame = &channel->received_frame;
   }

   return received_frame;
}

/* \brief Construct frame and send it over black channel
 *
 * See public header file for description
 */
const fsoeframe_t * fsoechannel_SendFrame (
   fsoechannel_t * channel,
   uint8_t cmd,
   const void * safeData,
   size_t safeData_size,
   uint16_t lastCrc,
   uint16_t connId,
   uint16_t * seqNo,
   uint16_t * oldCrc,
   bool bNew)
{
   FSOEPORT_ASSERT (safeData != NULL);
   FSOEPORT_ASSERT (safeData_size > 0);
   FSOEPORT_ASSERT (seqNo != NULL);
   FSOEPORT_ASSERT (oldCrc != NULL);

   FSOELOG_DEBUG (FSOE_LOG_CHANNEL,
      "SendFrame(cmd=%s, lastCrc=0x%04"PRIx16", connId=%"PRIu16",\n",
      fsoeframe_command_name (cmd),
      lastCrc,
      connId);
   FSOELOG_DEBUG (FSOE_LOG_CHANNEL,
      "          seqNo=%"PRIu16", oldCrc=0x%04"PRIx16", bNew=%s)\n",
      *seqNo,
      *oldCrc,
      fsoechannel_boolean_as_string (bNew));

   /* Construct frame with command, data and Connection ID */
   fsoeframe_zeroise (&channel->sent_frame);
   fsoeframe_set_command (&channel->sent_frame, cmd);
   if (cmd == FSOEFRAME_COMMAND_FailSafeData)
   {
      /* Fail-safe data is all zeros, so nothing to write */
   }
   else
   {
      fsoeframe_set_data (&channel->sent_frame, safeData, safeData_size);
   }
   fsoeframe_set_connection_id (&channel->sent_frame, connId);

   /* Write CRCs and update CRC_0 and sequence number */
   fsoeframe_add_crc_words (
      &channel->sent_frame,
      lastCrc, /* Last received CRC_0 */
      seqNo,   /* Expected sequence number. Will be updated */
      oldCrc,  /* Last sent CRC_0. Will be updated */
      bNew);   /* Sent CRC_0 must be different from last sent CRC_0? */

   FSOELOG_DEBUG (FSOE_LOG_CHANNEL,
      "     New: seqNo=%"PRIu16", oldCrc=0x%04"PRIx16"\n",
      *seqNo,
      *oldCrc);

   /* Send frame through black channel */
   fsoeapp_send (
      channel->app_ref,
      fsoeframe_buffer (&channel->sent_frame),
      fsoeframe_size (&channel->sent_frame));

   return &channel->sent_frame;
}

/* \brief Initialise black channel instance
 *
 * See public header file for description
 */
void fsoechannel_init (
   fsoechannel_t * channel,
   size_t received_data_size,
   size_t sent_data_size,
   void * app_ref)
{
   FSOEPORT_ASSERT (channel != NULL);

   channel->app_ref = app_ref;

   fsoeframe_init (&channel->received_frame, received_data_size);
   fsoeframe_zeroise (&channel->received_frame);

   fsoeframe_init (&channel->last_received_frame, received_data_size);
   fsoeframe_zeroise (&channel->last_received_frame);

   fsoeframe_init (&channel->sent_frame, sent_data_size);
   fsoeframe_zeroise (&channel->sent_frame);
}
