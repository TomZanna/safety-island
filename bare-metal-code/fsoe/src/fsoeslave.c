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
 * \brief FSoE slave state machine implementation
 *
 * See public header file for description.
 * All comments below are implementation details only.
 *
 * \par State machine
 *
 * This file implements the state machine described in ETG.5100.
 * The code implementing a particular transition will have a comment
 * with the name of the transition. Each transition is also covered by
 * at least one test case, with name of transition supplied in comment.
 * For reference, all the transitions are listed below:
 * - RESET_FAIL1:    Received Session request with bad CRC.
 * - RESET_FAIL2:    Received invalid frame.
 * - RESET_FAIL3:    Received unknown frame.
 * - RESET_OK:       Received valid first Session request.
 * - RESET_START:    Application requested connection to be reset.
 * - RESET_STAY1:    Received Reset request.
 * - RESET_STAY2:    Application set new data command.
 * - SESSION_FAIL1:  Received Connection request with bad CRC.
 * - SESSION_FAIL2:  Received Connection request with bad Connection ID.
 * - SESSION_FAIL3:  Received Connection request before final Session request.
 * - SESSION_FAIL4:  Received Session request with bad CRC.
 * - SESSION_FAIL5:  Received unexpected Session request.
 * - SESSION_FAIL6:  Received Reset request with bad CRC.
 * - SESSION_FAIL7:  Received invalid frame.
 * - SESSION_FAIL8:  Received unknown frame.
 * - SESSION_OK:     Received valid first Connection request.
 * - SESSION_RESET1: Received valid Reset request.
 * - SESSION_RESET2: Application requested connection to be reset.
 * - SESSION_STAY1:  Received valid final Session request.
 * - SESSION_STAY2:  Received valid Session restart request.
 * - SESSION_STAY3:  Application requested new data command.
 * - CONN_FAIL1:     Received Parameter request with bad CRC.
 * - CONN_FAIL2:     Received Parameter request while Slave Address is bad.
 * - CONN_FAIL3:     Received Parameter request with bad Connection ID.
 * - CONN_FAIL4:     Received Parameter request before final Connection request.
 * - CONN_FAIL5:     Received subsequent Connection request with bad CRC.
 * - CONN_FAIL6:     Received subsequent Connection request with bad Connection ID.
 * - CONN_FAIL7:     Received unexpected Connection request.
 * - CONN_FAIL8:     Received Reset request with bad CRC.
 * - CONN_FAIL9:     Received Session restart request with bad CRC.
 * - CONN_FAIL10:    Received invalid frame.
 * - CONN_FAIL11:    Received unknown frame.
 * - CONN_OK:        Received valid first Parameter request.
 * - CONN_RESET1:    Received valid Reset request.
 * - CONN_RESET2:    Received valid Session restart request.
 * - CONN_RESET3:    Application requested connection to be reset.
 * - CONN_STAY1:     Received valid subsequent Connection request.
 * - CONN_STAY2:     Application requested new data command.
 * - PARA_FAIL1:     Received first ProcessData/FailSafeData with bad CRC.
 * - PARA_FAIL2:     Received first ProcessData/FailSafeData while previously
 *                   received Parameters are bad.
 * - PARA_FAIL3:     Received first ProcessData/FailSafeData with bad Connection ID.
 * - PARA_FAIL4:     Received first ProcessData/FailSafeData before final
 *                   Parameter request.
 * - PARA_FAIL5:     Received subsequent Parameter request with bad CRC.
 * - PARA_FAIL6:     Received subsequent Parameter request with bad Connection ID.
 * - PARA_FAIL7:     Received Parameter request after final Parameter request.
 * - PARA_FAIL8:     Received Reset request with bad CRC.
 * - PARA_FAIL9:     Received Session restart request with bad CRC.
 * - PARA_FAIL10:    Received Connection request.
 * - PARA_FAIL11:    Received unknown frame.
 * - PARA_OK1:       Received valid first ProcessData.
 * - PARA_OK2:       Received valid first FailSafeData.
 * - PARA_RESET1:    Received valid Reset request.
 * - PARA_RESET2:    Received valid Session restart request.
 * - PARA_RESET3:    Application requested connection to be reset.
 * - PARA_STAY1:     Received valid subsequent Parameter request.
 * - PARA_STAY2:     Application requested new data command.
 * - DATA_FAIL1:     Received ProcessData/FailSafeData with bad CRC.
 * - DATA_FAIL2:     Received ProcessData/FailSafeData with bad Connection ID.
 * - DATA_FAIL3:     Received Reset request with bad CRC.
 * - DATA_FAIL4:     Received Session restart request with bad CRC.
 * - DATA_FAIL5:     Received Connection or Parameter request.
 * - DATA_FAIL6:     Received unknown frame.
 * - DATA_OK1:       Received valid ProcessData.
 * - DATA_OK2:       Received valid FailSafeData.
 * - DATA_RESET1:    Received valid Reset request.
 * - DATA_RESET2:    Received valid Session restart request.
 * - DATA_RESET3:    Application requested connection to be reset.
 * - DATA_STAY:      Application requested new data command.
 * - DATA_WD:        Watchdog timer expired.
 *
 * \par Naming convention
 *
 * Two different naming conventions are used:
 * - For variables and functions defined by the standard, the names used by the
 *   standard are used (UpperCamelCase for functions and variables,
 *   UPPER_CASE_WITH_UNDERSCORES for "macro" functions.
 * - In all other cases, lower_case_with_underscores are used for functions
 *   and variables and UPPER_CASE_WITH_UNDERSCORES for macros and enum fields.
 */

/* Replace function calls with calls to mock function when building
 * unit test suite for slave.
 */
#ifdef UNIT_TEST
#define fsoeframe_IS_CRC_CORRECT mock_fsoeframe_is_crc_correct
#endif

#include <fsoeslave.h>
#include <fsoeapp.h>
#include "fsoeframe.h"
#include "fsoechannel.h"
#include "fsoewatchdog.h"
#include "fsoesracrc.h"
#include "fsoeport.h"
#include "fsoelog.h"
#include "fsoeutil.h"
#include <string.h>

/**
 * \brief Magic number.
 *
 * All slave instances will have their first field set to this value.
 * API functions will check that the field has this value as to ensure
 * the instance was initialised using fsoeslave_init().
 * It also serves as a check for memory corruption.
 */
#define FSOESLAVE_MAGIC (0xF50E5678u)

/******************************************************************************
 *
 * Miscellaneous functions
 *
 *****************************************************************************/

/**
 * \brief Create a new Session ID
 *
 * Note that function is called from state transitions in the standard,
 * but it is not itself defined by the standard.
 * See ETG.5100 ch. 8.5.
 *
 * \param[in,out] slave          Slave instance
 * \return                       New Session ID
 */
static fsoeframe_uint16_t fsoeslave_CREATE_SESSION_ID (const fsoeslave_t * slave)
{
   fsoeframe_uint16_t session_id;

   session_id.little_endian =
         fsoeutil_to_le16 (fsoeapp_generate_session_id (slave->app_ref));
   return session_id;
}

/**
 * \brief Check if stored Parameters are correct
 *
 * The user-implemented callback fsoeapp_verify_parameters() may be called.
 *
 * This function is similar to the macro IS_SAFE_PARA_CORRECT().
 * See ETG.5100 ch. 8.5.1.4 table 38 "Macros in the FSoE Slave state table".
 *
 * \pre The parameters have previously been received from master
 *
 * \param[in]     slave       Slave instance
 * \param[in]     SafePara    Parameters to check
 * \param[out]    error       Error code set
 */
static bool fsoeslave_IS_SAFE_PARA_CORRECT (
   const fsoeslave_t * slave,
   const fsoeframe_safepara_t * SafePara,
   uint8_t * const error)
{
   const size_t expected_app_parameters_size =
      slave->ExpectedSafeParaSize -
      offsetof (fsoeframe_encoded_safepara_t, app_parameters);
   bool is_correct;

   if (fsoeutil_from_le16 (SafePara->members.watchdog_size.little_endian) !=
      sizeof (SafePara->members.watchdog))
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received communication parameters has size %"PRIu16", expected %"PRIu16" \n",
         fsoeutil_from_le16 (SafePara->members.watchdog_size.little_endian),
         (uint16_t)sizeof (SafePara->members.watchdog));
      is_correct = false;
      *error = FSOEFRAME_ERROR_INVALID_COMPARALEN;
   }
   else if (fsoeutil_from_le16 (SafePara->members.app_parameters_size.little_endian) !=
      expected_app_parameters_size)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received application parameters has size %"PRIu16", expected %"PRIu16" \n",
         fsoeutil_from_le16 (SafePara->members.app_parameters_size.little_endian),
         (uint16_t)expected_app_parameters_size);
      is_correct = false;
      *error = FSOEFRAME_ERROR_INVALID_USERPARALEN;
   }
   else
   {
      uint8_t app_error = fsoeapp_verify_parameters (
         slave->app_ref,
         fsoeutil_from_le16 (SafePara->members.watchdog.little_endian),
         SafePara->members.app_parameters,
         expected_app_parameters_size);
      if (app_error == FSOEAPP_STATUS_OK)
      {
         FSOELOG_INFO (FSOE_LOG_SLAVE,
            "Received parameters are valid\n");
         is_correct = true;
         *error = 0;
      }
      else if (app_error == FSOEAPP_STATUS_BAD_TIMOUT)
      {
         FSOELOG_ERROR (FSOE_LOG_SLAVE,
            "Received watchdog timeout %"PRIu16" is invalid\n",
            fsoeutil_from_le16 (SafePara->members.watchdog.little_endian));
         is_correct = false;
         *error = FSOEFRAME_ERROR_INVALID_COMPARA;
      }
      else if (app_error >= 0x80)
      {
         FSOELOG_ERROR (FSOE_LOG_SLAVE,
            "Received application parameters are invalid (code: 0x%"PRIx8")\n",
            app_error);
         is_correct = false;
         *error = app_error;
      }
      else
      {
         FSOELOG_ERROR (FSOE_LOG_SLAVE,
            "Received application parameters are invalid\n");
         is_correct = false;
         *error = FSOEFRAME_ERROR_INVALID_USERPARA;
      }
   }

   return is_correct;
}

/**
 * \brief Store received data to buffer.
 *
 * This function is called STORE_DATA() in the standard.
 * See ETG.5100 ch. 8.5.1.4 table 38: "Macros in the FSoE Slave state table".
 *
 * \param[out]    dst            Destination buffer where data will be stored
 * \param[in]     dst_size       Size in bytes of destination buffer
 * \param[in]     src            Source buffer containing data
 * \param[in]     src_size       Size in bytes of source buffer
 */
static void fsoeslave_STORE_DATA (
   void * dst, size_t dst_size, const void * src, size_t src_size)
{
   fsoeport_memcpy (dst, src, FSOEUTIL_MIN (dst_size, src_size));
}

/**
 * \brief Return how many bytes are still to be sent before state is changed
 *
 * Used when establishing connection as data sent may not fit in a
 * single frame.
 *
 * This function is called UPDATE_BYTES_TO_BE_SENT() in the standard.
 * See ETG.5100 ch. 8.5.1.4 table 38: "Macros in the FSoE Slave state table".
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     bytesSent      Number of bytes yet to be sent
 *                               before sending frame
 * \return                       Number of bytes yet to send
 *                               after sending frame
 */
static size_t fsoeslave_UPDATE_BYTES_TO_BE_SENT (
   const fsoeslave_t * slave, size_t bytesSent)
{
   size_t remaining_bytes_to_be_sent;
   size_t max_bytes_to_send = FSOEUTIL_MIN (slave->inputs_size, slave->outputs_size);

   if (bytesSent <= max_bytes_to_send)
   {
      /* All data fits in one frame, so no more data to send */
      remaining_bytes_to_be_sent = 0;
   }
   else
   {
      /* More data to send after filling a complete frame */
      remaining_bytes_to_be_sent = bytesSent - max_bytes_to_send;
   }

   return remaining_bytes_to_be_sent;
}

/**
 * \brief Check if user supplied a valid configuration
 *
 * \param[in]     cfg            Configuration of FSoE slave
 * \return                       true if configuration is valid,
 *                               false if any field is invalid
 */
static bool fsoeslave_is_configuration_valid (const fsoeslave_cfg_t * cfg)
{
   bool is_valid = false;

   if (cfg->application_parameters_size >
      FSOE_APPLICATION_PARAMETERS_MAX_SIZE)
   {
       FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Application parameters are too large (%zu > %zu)\n",
          cfg->application_parameters_size,
          FSOE_APPLICATION_PARAMETERS_MAX_SIZE);
   }
   else if (cfg->inputs_size == 0)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE, "Size of inputs is 0\n");
   }
   else if (cfg->inputs_size > FSOE_PROCESS_DATA_MAX_SIZE)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Size of inputs is too large (%zu > %zu)\n",
          cfg->inputs_size, FSOE_PROCESS_DATA_MAX_SIZE);
   }
   else if ((cfg->inputs_size % 2 == 1) && (cfg->inputs_size != 1))
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Size of inputs is odd (%zu) and is not 1\n",
          cfg->inputs_size);
   }
   else if (cfg->outputs_size == 0)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE, "Size of outputs is 0\n");
   }
   else if (cfg->outputs_size > FSOE_PROCESS_DATA_MAX_SIZE)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Size of outputs is too large (%zu > %zu)\n",
          cfg->outputs_size, FSOE_PROCESS_DATA_MAX_SIZE);
   }
   else if ((cfg->outputs_size % 2 == 1) && (cfg->outputs_size != 1))
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Size of outputs is odd (%zu) and is not 1\n",
          cfg->outputs_size);
   }
   else
   {
      FSOELOG_INFO (FSOE_LOG_SLAVE, "Configuration is valid\n");
      is_valid = true;
   }

   return is_valid;
}

/**
 * \brief Initialise variables in slave instance
 *
 * No memory allocation is done in this function.
 *
 * \param[out]    slave          FSoE slave instance
 * \param[in]     cfg            Configuration of FSoE slave
 * \param[in]     app_ref        Application reference
 */
static void fsoeslave_initialise (
   fsoeslave_t * slave,
   const fsoeslave_cfg_t * cfg,
   void * app_ref)
{
   fsoeport_memset (slave, 0, sizeof (*slave));

   /* Initialise standard variables
    *
    * See ETG.5100 ch. 8.5.1.3 table 37: "Variables in the FSoE Slave state table".
    */
   slave->LastCrc = 0;
   slave->OldMasterCrc = 0;
   slave->OldSlaveCrc = 0;
   slave->MasterSeqno = 0;
   slave->SlaveSeqNo = 0;
   slave->InitSeqNo = 0;
   slave->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   slave->BytesToBeSent = 0;
   slave->ConnectionId = 0;
   slave->SlaveAddress = cfg->slave_address;
   slave->ExpectedSafeParaSize =
      offsetof (fsoeframe_encoded_safepara_t, app_parameters) +
      cfg->application_parameters_size;
   slave->CommFaultReason = FSOEFRAME_ERROR_LOCAL_RESET_OR_RESET_REPLY;
   fsoeport_memset (&slave->ConnectionData, 0, sizeof (slave->ConnectionData));
   fsoeport_memset (&slave->SafePara, 0, slave->ExpectedSafeParaSize);
   fsoeport_memset (slave->SafeOutputs, 0, cfg->outputs_size);

   /* Initialise other variables */
   fsoewatchdog_init (&slave->watchdog);
   fsoechannel_init (
      &slave->channel, cfg->outputs_size, cfg->inputs_size, app_ref);
   slave->is_reset_requested = false;
   slave->sync_status.reset_event = FSOESLAVE_RESETEVENT_NONE;
   slave->sync_status.reset_reason = FSOESLAVE_RESETREASON_LOCAL_RESET;
   slave->sync_status.current_state = FSOESLAVE_STATE_RESET;
   slave->sync_status.is_process_data_received = false;
   slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_NONE;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_NONE;

   /* Initialise other constants */
   slave->magic = FSOESLAVE_MAGIC;
   slave->inputs_size = cfg->inputs_size;
   slave->outputs_size = cfg->outputs_size;
   slave->app_ref = app_ref;
}

/******************************************************************************
 *
 * State machine Actions
 *
 * For each state machine transition, an Action will be performed.
 * An Action corresponds to a function below.
 * See ETG.5100 ch. 8.5.1 "FSoE Slave state machine".
 *
 *****************************************************************************/

/**
 * \brief Send Reset reply to master
 *
 * This will reset CRC generation, disable process-data inputs,
 * send Reset reply to master and start waiting for Session request.
 *
 * Note that the watchdog timer is also stopped. This is only needed in
 * Data state, as it is always stopped in any other state. Always stopping
 * it does no harm though, and simplifies the logic.
 *
 * \pre A Reset request is received from master
 *
 * \param[in,out] slave         Slave instance
 */
static void fsoeslave_send_reset_reply (fsoeslave_t * slave)
{
   FSOELOG_INFO (FSOE_LOG_SLAVE, "Sending Reset reply\n");

   slave->LastCrc = 0;
   slave->OldMasterCrc = 0;
   slave->OldSlaveCrc = 0;
   slave->MasterSeqno = 1;
   slave->SlaveSeqNo = 1;
   slave->InitSeqNo = 1;
   slave->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   fsoewatchdog_STOP_WD (&slave->watchdog);
   slave->CommFaultReason = 0;
   (void) fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Reset,
      &slave->CommFaultReason,
      sizeof (slave->CommFaultReason),
      slave->LastCrc,
      0,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      false
   );
   slave->SlaveSeqNo = 1;

   slave->sync_status.current_state = FSOESLAVE_STATE_RESET;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_RESET;
}

/**
 * \brief Send Reset request to master
 *
 * This will reset CRC generation, disable process-data inputs,
 * send Reset request to master and start waiting indefinitely
 * (no timeout) for Session request.
 *
 * Note that the watchdog timer is also stopped. This is only needed in
 * Data state, as it is always stopped in any other state. Always stopping
 * it does no harm though, and simplifies the logic.
 *
 * Note that the sequence number for initial received frames (InitSeqNo)
 * is also reset to 1. This is only needed in case Reset or Session requests
 * were received, as it is not otherwise modified. Resetting it
 * does no harm though, and simplifies the logic.
 *
 * \param[in,out] slave           Slave instance
 * \param[in]     CommFaultReason Reason for resetting connection
 */
static void fsoeslave_send_reset_request (
   fsoeslave_t * slave,
   uint8_t CommFaultReason)
{
   FSOELOG_INFO (FSOE_LOG_SLAVE,
      "Sending Reset request. Cause: %s (0x%02"PRIx8")\n",
      fsoeframe_error_description (CommFaultReason),
      CommFaultReason);

   slave->LastCrc = 0;
   slave->OldMasterCrc = 0;
   slave->OldSlaveCrc = 0;
   slave->MasterSeqno = 1;
   slave->InitSeqNo = 1;
   slave->SlaveSeqNo = 1;
   slave->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   slave->CommFaultReason = CommFaultReason;
   fsoewatchdog_STOP_WD (&slave->watchdog);
   (void) fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Reset,
      &slave->CommFaultReason,
      sizeof (slave->CommFaultReason),
      slave->LastCrc,
      0,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      false
   );
   slave->SlaveSeqNo = 1;

   slave->sync_status.current_state = FSOESLAVE_STATE_RESET;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_RESET;

   /* Report information about connection reset event to user */
   slave->sync_status.reset_event = FSOESLAVE_RESETEVENT_BY_SLAVE;
   slave->sync_status.reset_reason = CommFaultReason;
}

/**
 * \brief Send initial Session start reply to master
 *
 * This will create the slave Session ID, send it in Session reply to master
 * and start waiting for additional Session request or
 * Connection request.
 *
 * \pre Slave is in Reset state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_send_initial_session_start_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_RESET);

   /* Note that MasterSeqNo is not guaranteed to be 2 here,
    * nor is SlaveSeqNo guaranteed to be 1.
    */
   slave->LastCrc = fsoeframe_crc0 (Frame);
   slave->SessionId = fsoeslave_CREATE_SESSION_ID (slave);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Session,
      &slave->SessionId,
      FSOEUTIL_MIN (sizeof (slave->SessionId), slave->outputs_size),
      slave->LastCrc,
      0,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true
   );
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent = fsoeslave_UPDATE_BYTES_TO_BE_SENT (slave, 2);

   FSOELOG_INFO (FSOE_LOG_SLAVE,
      "Sending initial Session reply with Session ID 0x%04"PRIx16".\n",
      fsoeutil_from_le16 (slave->SessionId.little_endian));

   slave->sync_status.current_state = FSOESLAVE_STATE_SESSION;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_SESSION;
}

/**
 * \brief Send initial Session restart reply to master
 *
 * Called upon master request for session to be restarted with a
 * new Session ID while in Session state.
 *
 * This will reset sequence numbers and data command, create a new Session ID,
 * send it to master and start waiting indefinitely (no timeout) for
 * new requests from master.
 * See ETG.5100 ch. 8.5.3.1, transition SESSION_STAY2.
 *
 * \pre Slave is in Session state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_send_initial_session_reply_in_session_state (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_SESSION);

   /* Note that InitSeqNo is not guaranteed to be 2 here */
   slave->LastCrc = fsoeframe_crc0 (Frame);
   slave->MasterSeqno = slave->InitSeqNo;
   slave->InitSeqNo = 1;
   slave->SlaveSeqNo = 1;
   slave->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   slave->SessionId = fsoeslave_CREATE_SESSION_ID (slave);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Session,
      &slave->SessionId,
      FSOEUTIL_MIN (sizeof (slave->SessionId), slave->outputs_size),
      slave->LastCrc,
      0,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true
   );
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent = fsoeslave_UPDATE_BYTES_TO_BE_SENT (slave, 2);

   FSOELOG_INFO (FSOE_LOG_SLAVE,
      "Sending initial Session reply with new Session ID 0x%04"PRIx16".\n",
      fsoeutil_from_le16 (slave->SessionId.little_endian));

   slave->sync_status.current_state = FSOESLAVE_STATE_SESSION;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_SESSION;
}

/**
 * \brief Send initial Session restart reply to master
 *
 * Called upon master request for session to be restarted with a
 * new Session ID.
 *
 * This will reset sequence numbers and data command, create a new Session ID,
 * send it to master and start waiting indefinitely (no timeout) for
 * new requests from master.
 *
 * Note that the watchdog timer is also stopped. This is only needed in
 * Data state, as it is always stopped in any other state. Always stopping
 * it does no harm though, and simplifies the logic.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_send_initial_session_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   slave->LastCrc = fsoeframe_crc0 (Frame);
   slave->MasterSeqno = 2;
   slave->InitSeqNo = 1;
   slave->SlaveSeqNo = 1;
   slave->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   fsoewatchdog_STOP_WD (&slave->watchdog);
   slave->SessionId = fsoeslave_CREATE_SESSION_ID (slave);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Session,
      &slave->SessionId,
      FSOEUTIL_MIN (sizeof (slave->SessionId), slave->outputs_size),
      slave->LastCrc,
      0,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true
   );
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent = fsoeslave_UPDATE_BYTES_TO_BE_SENT (slave, 2);

   FSOELOG_INFO (FSOE_LOG_SLAVE,
      "Sending initial Session reply with new Session ID 0x%04"PRIx16".\n",
      fsoeutil_from_le16 (slave->SessionId.little_endian));

   slave->sync_status.current_state = FSOESLAVE_STATE_SESSION;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_SESSION;
}

/**
 * \brief Send next Session reply to master
 *
 * This is only needed if either received or sent frames have the
 * minimum size allowed (6 bytes, with 5 bytes overhead).
 *
 * \pre Slave is in Session state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_send_next_session_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_SESSION);
   FSOEPORT_ASSERT (slave->BytesToBeSent <= 2u);

   FSOELOG_INFO (FSOE_LOG_SLAVE, "Sending next Session reply\n");

   slave->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Session,
      &slave->SessionId.bytes[2u - slave->BytesToBeSent],
      slave->BytesToBeSent,
      slave->LastCrc,
      0,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true
   );
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent = fsoeslave_UPDATE_BYTES_TO_BE_SENT (
      slave, slave->BytesToBeSent);

   slave->sync_status.current_state = FSOESLAVE_STATE_SESSION;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_SESSION;
}

/**
 * \brief Send initial Connection reply to master and wait for next Connection request
 * from master (or Parameter request if this was the last Connection request).
 *
 * The connection data is the Connection ID and the Slave Address.
 * Note that multiple frames may be required to send all.
 *
 * \pre Slave is in Session state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Connection request frame
 * \param[in]     FrameSafeData  Received Connection data
 */
static void fsoeslave_send_initial_connection_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData
)
{
   const size_t FrameSafeData_size = slave->outputs_size;
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_SESSION);

   FSOELOG_INFO (FSOE_LOG_SLAVE, "Sending initial Connection reply\n");

   slave->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Connection,
      FrameSafeData,
      FrameSafeData_size,
      slave->LastCrc,
      slave->ConnectionId,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true);
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent = fsoeslave_UPDATE_BYTES_TO_BE_SENT (slave, 4);

   slave->sync_status.current_state = FSOESLAVE_STATE_CONNECTION;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_CONNECTION;
}

/**
 * \brief Send next Connection reply to master and wait for next Connection request
 * from master (or Parameter request if this was the last Connection request).
 *
 * The connection data is the Connection ID and the Slave Address.
 * Note that multiple frames may be required to send all.
 *
 * \pre Slave is in Connection state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Connection request frame
 * \param[in]     FrameSafeData  Received Connection data
 */
static void fsoeslave_send_next_connection_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const size_t FrameSafeData_size = slave->outputs_size;
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_CONNECTION);

   FSOELOG_INFO (FSOE_LOG_SLAVE,
      "Sending next Connection reply\n");

   slave->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Connection,
      FrameSafeData,
      FrameSafeData_size,
      slave->LastCrc,
      slave->ConnectionId,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true);
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent =
      fsoeslave_UPDATE_BYTES_TO_BE_SENT (slave, slave->BytesToBeSent);

   slave->sync_status.current_state = FSOESLAVE_STATE_CONNECTION;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_CONNECTION;
}

/**
 * \brief Send initial Parameter reply to master and wait for next
 * Parameter request (or FailSafeData/ProcessData if this was the
 * last Parameter reply).
 *
 * The parameters include both the watchdog timeout and the
 * device-specific application parameters.
 * Note that multiple frames may be required to send them all.
 *
 * \pre Slave is in Connection state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Parameter request frame
 * \param[in]     FrameSafeData  Received Parameter data
 */
static void fsoeslave_send_initial_parameter_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_CONNECTION);

   FSOELOG_INFO (FSOE_LOG_SLAVE, "Sending initial Parameter reply\n");

   slave->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Parameter,
      FrameSafeData,
      FSOEUTIL_MIN (slave->outputs_size, slave->ExpectedSafeParaSize),
      slave->LastCrc,
      slave->ConnectionId,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true);
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent = fsoeslave_UPDATE_BYTES_TO_BE_SENT (
         slave, slave->ExpectedSafeParaSize);

   slave->sync_status.current_state = FSOESLAVE_STATE_PARAMETER;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_PARAMETER;
}

/**
 * \brief Send next Parameter reply to master and wait for next
 * Parameter request (or FailSafeData/ProcessData if this was the
 * last Parameter reply).
 *
 * The parameters include both the watchdog timeout and the
 * device-specific application parameters.
 * Note that multiple frames may be required to send them all.
 *
 * \pre Slave is in Parameter state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Parameter request frame
 * \param[in]     FrameSafeData  Received Parameter data
 */
static void fsoeslave_send_next_parameter_reply (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const size_t FrameSafeData_size = slave->outputs_size;
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_PARAMETER);
   FSOEPORT_ASSERT (slave->BytesToBeSent <= slave->ExpectedSafeParaSize);

   FSOELOG_INFO (FSOE_LOG_SLAVE, "Sending next Parameter reply\n");

   slave->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      FSOEFRAME_COMMAND_Parameter,
      FrameSafeData,
      FrameSafeData_size,
      slave->LastCrc,
      slave->ConnectionId,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true);
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   slave->BytesToBeSent =
      fsoeslave_UPDATE_BYTES_TO_BE_SENT (slave, slave->BytesToBeSent);

   slave->sync_status.current_state = FSOESLAVE_STATE_PARAMETER;
   slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_PARAMETER;
}

/**
 * \brief Send data frame (ProcessData or FailSafeData)
 *
 * Send inputs to master and start waiting for ACK (i.e. outputs)
 * Note that the watchdog timer is also started.
 *
 * \pre Connection with master is fully established.
 * \pre A ProcessData or FailSafeData frame is received.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received frame
 * \param[in]     SafeInputs     Data to (possibly) send to master
 */
static void fsoeslave_send_data_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * SafeInputs)
{
   const fsoeframe_t * SendFrame;

   slave->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &slave->channel,
      slave->DataCommand,
      SafeInputs,
      slave->inputs_size,
      slave->LastCrc,
      slave->ConnectionId,
      &slave->SlaveSeqNo,
      &slave->OldSlaveCrc,
      true);
   slave->LastCrc = fsoeframe_crc0 (SendFrame);
   fsoewatchdog_START_WD (&slave->watchdog);

   FSOELOG_INFO (FSOE_LOG_SLAVE,
      "Sending %s frame (%"PRIu32" bytes SafeInputs). Timeout: %"PRIu32"ms\n",
      fsoeframe_command_name (fsoeframe_command (SendFrame)),
      (uint32_t)slave->inputs_size,
      fsoewatchdog_timeout_ms (&slave->watchdog));

   slave->sync_status.current_state = FSOESLAVE_STATE_DATA;
   if (slave->DataCommand == FSOEFRAME_COMMAND_ProcessData)
   {
      slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_PROCESS_DATA;
   }
   else
   {
      slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_FAILSAFE_DATA;
   }
}

/******************************************************************************
 *
 * State machine event handling
 *
 * Four types of events can occur:
 * - A frame of some type is received.
 * - The watchdog timer expires (i.e. no frame received in time).
 * - Application requests connection to be reset.
 * - Application requests the data command to be changed.
 * See ETG.5100 table 35: "Events in the FSoE Slave state table".
 *
 *****************************************************************************/

/**
 * \brief Act on received start-up request
 *
 * A Reset request was received from master, beginning the procedure to
 * establish a connection between master and slave.
 * A Reset reply is sent back as an ACK.
 *
 * The following transition in the state machine is implemented here:
 * - RESET_STAY1
 * See ETG.5100 ch. 8.5.2.1.
 *
 * \pre Slave is in Reset state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Reset request frame
 */
static void fsoeslave_act_on_received_reset_startup_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   uint8_t error_code;

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_RESET);

   /* Report information about connection reset event to user */
   error_code = fsoeframe_error_code (Frame);
   slave->sync_status.reset_event = FSOESLAVE_RESETEVENT_BY_MASTER;
   slave->sync_status.reset_reason = error_code;

   /* Report that a new valid frame was received */
   slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_RESET;

   FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received Reset request. Cause: %s (0x%02"PRIx8")\n",
         fsoeframe_error_description (error_code),
         error_code);

   fsoeslave_send_reset_reply (slave);
}

/**
 * \brief Act on received error
 *
 * A Reset request was received from master after it detected some error.
 * If the frame is valid, a Reset reply is sent back as an ACK.
 * Otherwise, a Reset request with error code will be sent back.
 * In either case, the connection is reset.
 * See ETG.5100 ch. 8.5.[3-6].1.
 *
 * \pre Slave has completed transition from Reset state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Reset request frame
 */
static void fsoeslave_act_on_received_reset_error_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   FSOEPORT_ASSERT (slave->sync_status.current_state != FSOESLAVE_STATE_RESET);

   if (fsoeframe_IS_CRC_CORRECT (
      Frame, 0, &slave->InitSeqNo, &slave->OldMasterCrc, false))
   {
      uint8_t error_code;

      /* Report information about connection reset event to user */
      error_code = fsoeframe_error_code (Frame);
      slave->sync_status.reset_event = FSOESLAVE_RESETEVENT_BY_MASTER;
      slave->sync_status.reset_reason = error_code;

      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_RESET;

      FSOELOG_WARNING (FSOE_LOG_SLAVE,
         "Received Reset request. Cause: %s (0x%02"PRIx8")\n",
         fsoeframe_error_description (error_code),
         error_code);

      /* Transitions:
       * - SESSION_RESET1
       * - CONN_RESET1
       * - PARA_RESET1
       * - DATA_RESET1
       */
      fsoeslave_send_reset_reply (slave);
   }
   else
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received Reset request with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));

      /* Transitions:
       * - SESSION_FAIL6
       * - CONN_FAIL8
       * - PARA_FAIL8
       * - DATA_FAIL3
       */
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received Session start request
 *
 * An initial Session request was received from master, containing at least
 * parts of the master Session ID.
 * If no errors are detected, a new slave Session ID is generated and
 * sent to master in a Session reply and current state is advanced to
 * Session state.
 * In any other case, master will be informed of the error and connection
 * will be reset.
 * See ETG.5100 ch. 8.5.2.1.
 *
 * \pre Slave is in Reset state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_act_on_received_session_start_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_RESET);

   if (fsoeframe_IS_CRC_CORRECT (
      Frame, slave->LastCrc, &slave->MasterSeqno, &slave->OldMasterCrc, false))
   {
      /* Save whole or least significant byte of received Master Session ID */
      fsoeframe_get_data (
         Frame,
         &slave->master_session_id,
         FSOEUTIL_MIN (sizeof (slave->master_session_id), slave->inputs_size));

      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_SESSION;

      /* Transition RESET_OK */
      FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received initial Session start request\n");
      fsoeslave_send_initial_session_start_reply (slave, Frame);
   }
   else
   {
      /* Transition RESET_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Session start request with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received Session request while in Session state
 *
 * A Session request was received from master containing at least parts of
 * the master Session ID.
 * If no errors are detected, a Session reply is sent back to master,
 * possibly with a new slave Session ID.
 * In any other case, master will be informed of the error and connection
 * will be reset.
 * See ETG.5100 ch. 8.5.3.1.
 *
 * \pre Slave is in Session state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_act_on_received_session_request_in_session_state (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_SESSION);

   /* This code is tricky, as we may call IS_CRC_CORRECT() twice or once,
    * and with different arguments depending on the transition.
    * IS_CRC_CORRECT() has the side effects of updating two variables.
    * Note that the value of OldMasterCrc which IS_CRC_CORRECT()
    * is called with the second time will be incorrect in the code below.
    * This has no consequence though as it is not read (bNew is set to false).
    */
   if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      if (slave->BytesToBeSent != 0)
      {
         /* Save most significant byte of received Master Session ID */
         fsoeframe_get_data (Frame, &slave->master_session_id.bytes[1], 1);

         /* Report that a new valid frame was received */
         slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_SESSION;

         /* Transition SESSION_STAY1 */
         FSOELOG_INFO (FSOE_LOG_SLAVE, "Received next Session request\n");
         fsoeslave_send_next_session_reply (slave, Frame);
      }
      else if (fsoeframe_IS_CRC_CORRECT (
         Frame, 0, &slave->InitSeqNo, &slave->OldMasterCrc, false))
      {
         /* Save whole or least significant byte of received Master Session ID */
         fsoeframe_get_data (
            Frame,
            &slave->master_session_id,
            FSOEUTIL_MIN (sizeof (slave->master_session_id), slave->inputs_size));

         /* Report that a new valid frame was received */
         slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_SESSION;

         /* Transition SESSION_STAY2 */
         FSOELOG_WARNING (FSOE_LOG_SLAVE,
               "Received initial Session restart request\n");
         fsoeslave_send_initial_session_reply_in_session_state (slave, Frame);
      }
      else
      {
         /* Transition SESSION_FAIL5 */
         FSOELOG_ERROR (FSOE_LOG_SLAVE,
               "Received Session request but expected initial Connection request\n");
         fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
      }
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame, 0, &slave->InitSeqNo, &slave->OldMasterCrc, false))
   {
      /* Save whole or least significant byte of received Master Session ID */
      fsoeframe_get_data (
         Frame,
         &slave->master_session_id,
         FSOEUTIL_MIN (sizeof (slave->master_session_id), slave->inputs_size));

      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_SESSION;

      /* Transition SESSION_STAY2 */
      FSOELOG_WARNING (FSOE_LOG_SLAVE,
            "Received initial Session restart request\n");
      fsoeslave_send_initial_session_reply_in_session_state (slave, Frame);
   }
   else
   {
      /* Transition SESSION_FAIL4 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
            "Received Session request with bad CRC (0x%04"PRIx16")\n",
            fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received Session restart request
 *
 * A Session request was received from master after the exchange of
 * Session IDs between master and slave has already completed.
 * If no errors are detected, a new slave Session ID is generated and
 * sent to master and current state is reverted back to Session state.
 * In any other case, master will be informed of the error and connection
 * will be reset.
 * See ETG.5100 ch. 8.5.[4-6].1.
 *
 * \pre Slave has completed transition from Session state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session request frame
 */
static void fsoeslave_act_on_received_session_restart_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   if (fsoeframe_IS_CRC_CORRECT (
      Frame, 0, &slave->InitSeqNo, &slave->OldMasterCrc, false))
   {
      /* Save whole or least significant byte of received Master Session ID */
      fsoeframe_get_data (
         Frame,
         &slave->master_session_id,
         FSOEUTIL_MIN (sizeof (slave->master_session_id), slave->inputs_size));

      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_SESSION;

      /* Transitions:
       * - CONN_RESET2
       * - PARA_RESET2
       * - DATA_RESET2
       */
      FSOELOG_WARNING (FSOE_LOG_SLAVE,
            "Received initial Session restart request\n");
      fsoeslave_send_initial_session_reply (slave, Frame);
   }
   else
   {
      /* Transitions:
       * - CONN_FAIL9
       * - PARA_FAIL9
       * - DATA_FAIL4
       */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
            "Received Session request with bad CRC (0x%04"PRIx16")\n",
            fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received initial Connection request
 *
 * The first frame of type Connection request has been received from master.
 * If this was expected and no errors are detected, the received Connection
 * data will be stored for later, a corresponding Connection reply will be
 * sent back to master and Connection state will be entered.
 * In any other case, master will be informed of the error and connection
 * will be reset.
 * See ETG.5100 ch. 8.5.3.1.
 *
 * \pre Slave is in Session state
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Connection request frame
 * \param[in]     FrameSafeData  Received Connection data
 */
static void fsoeslave_act_on_received_initial_connection_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_SESSION);

   if (slave->BytesToBeSent != 0)
   {
      /* Transition SESSION_FAIL3 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Connection request but expected Session request\n");
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
   }
   else if (FrameConnId == 0)
   {
      /* Transition SESSION_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Connection request with bad ConnID (0)\n");
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_CONNECTION;

      /* Transition SESSION_OK */
      FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received Session ID 0x%04"PRIx16"\n",
         fsoeutil_from_le16 (slave->master_session_id.little_endian));
      FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received initial Connection request. ConnID: %"PRIu16"\n",
         FrameConnId);
      fsoeslave_STORE_DATA (
         &slave->ConnectionData,
         sizeof (slave->ConnectionData),
         FrameSafeData,
         FSOEUTIL_MIN (slave->inputs_size, slave->outputs_size));
      slave->ConnectionId = FrameConnId;
      fsoeslave_send_initial_connection_reply (slave, Frame, FrameSafeData);
   }
   else
   {
      /* Transition SESSION_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received Connection request with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on next received Connection request
 *
 * Next frame of type Connection request has been received from master.
 * If this was expected and no errors are detected, the received Connection
 * data will be stored for later and a corresponding Connection reply will be
 * sent back to master. In any other case, master will be informed of the error
 * and connection will be reset.
 * See ETG.5100 ch. 8.5.4.1.
 *
 * \pre Slave is in Connection state
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Connection request frame
 * \param[in]     FrameSafeData  Received Connection data
 */
static void fsoeslave_act_on_received_next_connection_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_CONNECTION);

   if (slave->BytesToBeSent == 0)
   {
      /* Transition CONN_FAIL7 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next Connection request but expected Parameter request\n");
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
   }
   else if (FrameConnId != slave->ConnectionId)
   {
      /* Transition CONN_FAIL6 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next Connection request with invalid ConnID (%"PRIu16"), expected %"PRIu16"\n",
         FrameConnId, slave->ConnectionId);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_CONNECTION;

      /* Transition CONN_STAY1 */
      FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received next Connection request\n");
      fsoeslave_STORE_DATA (
         &slave->ConnectionData.bytes[4u - slave->BytesToBeSent],
         slave->BytesToBeSent,
         FrameSafeData,
         FSOEUTIL_MIN (slave->inputs_size, slave->outputs_size));
      fsoeslave_send_next_connection_reply (slave, Frame, FrameSafeData);
   }
   else
   {
      /* Transition CONN_FAIL5 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next Connection request with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received initial Parameter request
 *
 * The first frame of type Parameter request has been received from master.
 * If this was expected and no errors are detected, the received Parameter
 * data will be stored for later, a corresponding Parameter reply will be
 * sent back to master and Parameter state will be entered.
 * In any other case, master will be informed of the error and connection
 * will be reset.
 * See ETG.5100 ch. 8.5.4.1.
 *
 * \pre Slave is in Connection state
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Parameter request frame
 * \param[in]     FrameSafeData  Received Parameter data
 */
static void fsoeslave_act_on_received_initial_parameter_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_CONNECTION);

   if (slave->BytesToBeSent != 0)
   {
      /* Transition CONN_FAIL4 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Parameter request but expected Connection request\n");
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
   }
   else if (FrameConnId != slave->ConnectionId)
   {
      /* Transition CONN_FAIL3 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Parameter request with ConnID %"PRIu16", expected %"PRIu16"\n",
         FrameConnId,
         slave->ConnectionId);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (slave->ConnectionId !=
      fsoeutil_from_le16 (slave->ConnectionData.members.ConnId.little_endian))
   {
      /* Transition CONN_FAIL3 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Parameter request. ConnID verification failed\n");
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "(ConnectionData.ConnId: %"PRIu16", expected %"PRIu16")\n",
         fsoeutil_from_le16 (slave->ConnectionData.members.ConnId.little_endian),
         slave->ConnectionId);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (slave->SlaveAddress !=
      fsoeutil_from_le16 (slave->ConnectionData.members.SlaveAddress.little_endian))
   {
      /* Transition CONN_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Parameter request. Slave Address verification failed\n");
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "(ConnectionData.SlaveAddress: %"PRIu16", expected %"PRIu16")\n",
         fsoeutil_from_le16 (slave->ConnectionData.members.SlaveAddress.little_endian),
         slave->SlaveAddress);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_ADDRESS);
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_PARAMETER;

      /* Transition CONN_OK */
      FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received initial Parameter request\n");
      fsoeslave_STORE_DATA (
         &slave->SafePara,
         slave->ExpectedSafeParaSize,
         FrameSafeData,
         FSOEUTIL_MIN (slave->inputs_size, slave->outputs_size));
      fsoeslave_send_initial_parameter_reply (slave, Frame, FrameSafeData);
   }
   else
   {
      /* Transition CONN_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial Parameter request with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on next received Parameter request
 *
 * Next frame of type Parameter request has been received from master.
 * If this was expected and no errors are detected, the received Parameter
 * data will be stored for later and a corresponding Parameter reply will be
 * sent back to master. In any other case, master will be informed of the error
 * and connection will be reset.
 * See ETG.5100 ch. 8.5.5.1.
 *
 * \pre Slave is in Parameter state
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Parameter request frame
 * \param[in]     FrameSafeData  Received Parameter data
 */
static void fsoeslave_act_on_received_next_parameter_request (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_PARAMETER);

   if (slave->BytesToBeSent == 0)
   {
      /* Transition PARA_FAIL7 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next Parameter request but expected Process/FailSafeData\n");
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
   }
   else if (FrameConnId != slave->ConnectionId)
   {
      /* Transition PARA_FAIL6 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next Parameter request but ConnID is %"PRIu16", expected %"PRIu16"\n",
         FrameConnId, slave->ConnectionId);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      /* Report that a new valid frame was received */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_PARAMETER;

      /* Transition PARA_STAY1 */
      FSOELOG_INFO (FSOE_LOG_SLAVE,
         "Received next Parameter request\n");
      fsoeslave_STORE_DATA (
         &slave->SafePara.bytes[
            slave->ExpectedSafeParaSize - slave->BytesToBeSent],
         slave->BytesToBeSent,
               FrameSafeData,
               FSOEUTIL_MIN (slave->inputs_size, slave->outputs_size));
      fsoeslave_send_next_parameter_reply (slave, Frame, FrameSafeData);
   }
   else
   {
      /* Transition PARA_FAIL5 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next Parameter request with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received initial data frame (ProcessData or FailSafeData)
 *
 * The first frame of type ProcessData or FailSafeData has been received
 * from master, containing outputs data of respective type.
 * If stored Parameters previously received are correct and no other errors
 * are detected, the connection with master is now fully established and
 * Data state is entered, where master and slave periodically exchange
 * process or fail-safe data. Received outputs data will be stored and
 * inputs data will be sent back to master.
 * In any other case, master will be informed of the error and
 * connection will be reset.
 * See ETG.5100 ch. 8.5.5.1.
 *
 * \pre Slave is in Parameter state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received ProcessData or FailSafeData frame
 * \param[in]     SafeInputs     Data to (possibly) send to master
 */
static void fsoeslave_act_on_received_initial_data (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * SafeInputs)
{
   const uint8_t FrameCommand = fsoeframe_command (Frame);
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_PARAMETER);

   uint8_t para_fault;

   if (slave->BytesToBeSent != 0)
   {
      /* Transition PARA_FAIL4 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial %s frame, expected Parameter request\n",
         fsoeframe_command_name (FrameCommand));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
   }
   else if (FrameConnId != slave->ConnectionId)
   {
      /* Transition PARA_FAIL3 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial %s frame with ConnID %"PRIu16", expected %"PRIu16"\n",
         fsoeframe_command_name (FrameCommand),
         FrameConnId,
         slave->ConnectionId);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (fsoeslave_IS_SAFE_PARA_CORRECT (
      slave, &slave->SafePara, &para_fault) == false)
   {
      /* Transition PARA_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial %s frame but configured parameters are invalid\n",
         fsoeframe_command_name (FrameCommand));
      fsoeslave_send_reset_request (slave, para_fault);
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      if (FrameCommand == FSOEFRAME_COMMAND_ProcessData)
      {
         /* Report that a new valid frame was received */
         slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_PROCESS_DATA;

         /* Transition PARA_OK1 */
         FSOELOG_INFO (FSOE_LOG_SLAVE,
               "Received initial ProcessData frame (%"PRIu32" bytes SafeOutputs)\n",
               (uint32_t)slave->outputs_size);
         slave->sync_status.is_process_data_received = true;
      }
      else
      {
         /* Report that a new valid frame was received */
         slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_FAILSAFE_DATA;

         /* Transition PARA_OK2 */
         FSOELOG_INFO (FSOE_LOG_SLAVE,
               "Received initial FailSafeData frame (%"PRIu32" bytes SafeOutputs)\n",
               (uint32_t)slave->outputs_size);
      }
      FSOELOG_INFO (FSOE_LOG_SLAVE,
            "Setting watchdog timeout to %"PRIu16"ms\n",
            fsoeutil_from_le16 (slave->SafePara.members.watchdog.little_endian));
      fsoewatchdog_set_timeout_ms (
         &slave->watchdog,
         fsoeutil_from_le16 (slave->SafePara.members.watchdog.little_endian));
      fsoeslave_send_data_frame (slave, Frame, SafeInputs);
   }
   else
   {
      /* Transition PARA_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received initial %s frame with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_command_name (FrameCommand),
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on next received data frame (ProcessData or FailSafeData)
 *
 * Next frame of type ProcessData or FailSafeData has been received from master,
 * containing outputs data of respective type.
 * If no errors are detected, inputs data will be sent back to master.
 * In any other case, master will be informed of the error and
 * connection will be reset.
 * See ETG.5100 ch. 8.5.6.1.
 *
 * \pre Slave is in Data state.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received ProcessData or FailSafeData frame
 * \param[in]     SafeInputs     Data to (possibly) send to master
 */
static void fsoeslave_act_on_received_next_data (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * SafeInputs)
{
   const uint8_t FrameCommand = fsoeframe_command (Frame);
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_DATA);

   if (FrameConnId != slave->ConnectionId)
   {
      /* Transition DATA_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next %s frame with ConnID %"PRIu16", expected %"PRIu16"\n",
         fsoeframe_command_name (FrameCommand),
         FrameConnId,
         slave->ConnectionId);
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (fsoeframe_IS_CRC_CORRECT (
      Frame,
      slave->LastCrc,
      &slave->MasterSeqno,
      &slave->OldMasterCrc,
      true))
   {
      if (FrameCommand == FSOEFRAME_COMMAND_ProcessData)
      {
         /* Report that a new valid frame was received */
         slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_PROCESS_DATA;

         /* Transition DATA_OK1 */
         FSOELOG_INFO (FSOE_LOG_SLAVE,
               "Received ProcessData frame (%"PRIu32" bytes SafeOutputs)\n",
               (uint32_t)slave->outputs_size);
         slave->sync_status.is_process_data_received = true;
      }
      else
      {
         /* Report that a new valid frame was received */
         slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_FAILSAFE_DATA;

         /* Transition DATA_OK2 */
         FSOELOG_INFO (FSOE_LOG_SLAVE,
               "Received FailSafeData frame (%"PRIu32" bytes SafeOutputs)\n",
               (uint32_t)slave->outputs_size);
      }
      fsoeslave_send_data_frame (slave, Frame, SafeInputs);
   }
   else
   {
      /* Transition DATA_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received next %s frame with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_command_name (FrameCommand),
         fsoeframe_crc0 (Frame));
      fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CRC);
   }
}

/**
 * \brief Act on received invalid frame
 *
 * A frame of a type which is not valid for the current state
 * has been received from master.
 * Master will be informed about the error and connection will be reset.
 *
 * The following transitions in the state machine are implemented here:
 * - RESET_FAIL2
 * - SESSION_FAIL7
 * - CONN_FAIL10
 * - PARA_FAIL10
 * - DATA_FAIL5
 * See ETG.5100 ch. 8.5.[2-6].1.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received frame of type which is not
 *                               valid for current state
 */
static void fsoeslave_act_on_received_invalid_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   (void) Frame;

   FSOELOG_ERROR (FSOE_LOG_SLAVE, "Received %s frame in %s state\n",
         fsoeframe_command_name (fsoeframe_command (Frame)),
         fsoeslave_state_description (slave->sync_status.current_state));

   fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_INVALID_CMD);
}

/**
 * \brief Act on received Reset frame
 *
 * A frame of type Reset has been received from master.
 * If this was expected and no errors are detected, a Reset reply
 * will be sent back to master.
 * In any other case, master will be informed of the error and
 * connection will be reset.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Reset frame
 */
static void fsoeslave_act_on_received_reset_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   switch (slave->sync_status.current_state)
   {
   case FSOESLAVE_STATE_RESET:
      fsoeslave_act_on_received_reset_startup_request (slave, Frame);
      break;
   default:
      fsoeslave_act_on_received_reset_error_request (slave, Frame);
      break;
   }
}

/**
 * \brief Act on received Session frame
 *
 * A frame of type Session has been received from master.
 * If this was expected and no errors are detected, a generated slave
 * Session ID  will be sent back to master in a Session reply.
 * In any other case, master will be informed of the error and
 * connection will be reset.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Session frame
 */
static void fsoeslave_act_on_received_session_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   switch (slave->sync_status.current_state)
   {
   case FSOESLAVE_STATE_RESET:
      fsoeslave_act_on_received_session_start_request (slave, Frame);
      break;
   case FSOESLAVE_STATE_SESSION:
      fsoeslave_act_on_received_session_request_in_session_state (slave, Frame);
      break;
   default:
      fsoeslave_act_on_received_session_restart_request (slave, Frame);
      break;
   }
}

/**
 * \brief Act on received Connection frame
 *
 * A frame of type Connection has been received from master.
 * If this was expected and no errors are detected, the received Connection
 * data will be stored for later and a corresponding Connection reply will be
 * sent back to master. In any other case, master will be informed of the error
 * and connection will be reset.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Connection frame
 * \param[in]     FrameSafeData  Received Connection data
 */
static void fsoeslave_act_on_received_connection_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   switch (slave->sync_status.current_state)
   {
   case FSOESLAVE_STATE_SESSION:
      fsoeslave_act_on_received_initial_connection_request (slave, Frame, FrameSafeData);
      break;
   case FSOESLAVE_STATE_CONNECTION:
      fsoeslave_act_on_received_next_connection_request (slave, Frame, FrameSafeData);
      break;
   default:
      fsoeslave_act_on_received_invalid_frame (slave, Frame);
      break;
   }
}

/**
 * \brief Act on received Parameter request
 *
 * A frame of type Parameter request has been received from master.
 * If this was expected and no errors are detected, the received Parameter
 * data will be stored for later and a corresponding Parameter reply will be
 * sent back to master. In any other case, master will be informed of the error
 * and connection will be reset.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received Parameter frame
 * \param[in]     FrameSafeData  Received Parameter data
 */
static void fsoeslave_act_on_received_parameter_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * FrameSafeData)
{
   switch (slave->sync_status.current_state)
   {
   case FSOESLAVE_STATE_CONNECTION:
      fsoeslave_act_on_received_initial_parameter_request (slave, Frame, FrameSafeData);
      break;
   case FSOESLAVE_STATE_PARAMETER:
      fsoeslave_act_on_received_next_parameter_request (slave, Frame, FrameSafeData);
      break;
   default:
      fsoeslave_act_on_received_invalid_frame (slave, Frame);
      break;
   }
}

/**
 * \brief Act on received outputs data
 *
 * A frame of type ProcessData or FailSafeData has been received from master,
 * containing outputs data of respective type.
 * If this was expected and no errors are detected, the received outputs data
 * will be stored and inputs data will be sent back to master. In any other case,
 * master will be informed of the error and connection will be reset.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received ProcessData or FailSafeData frame
 * \param[in]     SafeInputs     Data to (possibly) send to master
 */
static void fsoeslave_act_on_received_data_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * SafeInputs)
{
   switch (slave->sync_status.current_state)
   {
   case FSOESLAVE_STATE_PARAMETER:
      fsoeslave_act_on_received_initial_data (
         slave, Frame, SafeInputs);
      break;
   case FSOESLAVE_STATE_DATA:
      fsoeslave_act_on_received_next_data (
         slave, Frame, SafeInputs);
      break;
   default:
      fsoeslave_act_on_received_invalid_frame (slave, Frame);
      break;
   }
}

/**
 * \brief Act on received unknown frame
 *
 * A frame of unknown type has been received from master.
 * Master will be informed about the error and connection will be reset.
 *
 * The following transitions in the state machine are implemented here:
 * - RESET_FAIL3
 * - SESSION_FAIL8
 * - CONN_FAIL11
 * - PARA_FAIL11
 * - DATA_FAIL6
 * See ETG.5100 ch. 8.5.[2-6].1.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received frame of unknown type
 */
static void fsoeslave_act_on_received_unknown_frame (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame)
{
   (void) Frame;

   FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "Received frame of unknown type: 0x%02"PRIx8"\n",
         (uint8_t)fsoeframe_command (Frame));

   fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_UNKNOWN_CMD);
}

/**
 * \brief Act on frame received event.
 *
 * This event is triggered by the slave stack when it has received
 * a frame from master.
 *
 * Depending on what type of frame (called "command" in ETG.5100) was
 * received, different actions will be taken.
 * See ETG.5100 ch. 8.5.[2-6].1.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     Frame          Received frame
 * \param[in]     SafeInputs     Data to (possibly) send to master
 */
static void fsoeslave_act_on_frame_received_event (
   fsoeslave_t * slave,
   const fsoeframe_t * Frame,
   const void * SafeInputs)
{
   const uint8_t command = fsoeframe_command (Frame);

   /* By default, set outputs in fail-safe state */
   slave->sync_status.is_process_data_received = false;

   /* By default, treat received frame as invalid until validated */
   slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_INVALID;

   /* Get data from received frame */
   fsoeframe_get_data (Frame, slave->SafeOutputs, slave->outputs_size);

   switch (command)
   {
   case FSOEFRAME_COMMAND_Reset:
      fsoeslave_act_on_received_reset_frame (slave, Frame);
      break;
   case FSOEFRAME_COMMAND_Session:
      fsoeslave_act_on_received_session_frame (slave, Frame);
      break;
   case FSOEFRAME_COMMAND_Connection:
      fsoeslave_act_on_received_connection_frame (
         slave, Frame, slave->SafeOutputs);
      break;
   case FSOEFRAME_COMMAND_Parameter:
      fsoeslave_act_on_received_parameter_frame (
         slave, Frame, slave->SafeOutputs);
      break;
   case FSOEFRAME_COMMAND_FailSafeData:
      /* Fall-through */
   case FSOEFRAME_COMMAND_ProcessData:
      fsoeslave_act_on_received_data_frame (
         slave, Frame, SafeInputs);
      break;
   default:
      fsoeslave_act_on_received_unknown_frame (slave, Frame);
      break;
   }
}

/**
 * \brief Act on Watchdog Expired event
 *
 * This event is triggered by the slave stack when it detects that
 * it has not received data (ProcessData or FailSafeData) in time.
 *
 * Master will be informed about the error and connection will be reset.
 *
 * The following transition in the state machine is implemented here:
 * - DATA_WD.
 * See ETG.5100 ch. 8.5.6.2.
 *
 * \pre Slave is in Data state
 *
 * \param[in,out] slave          Slave instance
 */
static void fsoeslave_act_on_watchdog_expired_event (fsoeslave_t * slave)
{
   /* Watchdog timer is only started upon entering Data state */
   FSOEPORT_ASSERT (slave->sync_status.current_state == FSOESLAVE_STATE_DATA);

   FSOELOG_ERROR (FSOE_LOG_SLAVE,
      "Watchdog timer expired. No ACK received within %"PRIu32"ms\n",
      fsoewatchdog_timeout_ms (&slave->watchdog));

   /* Set outputs in fail-safe state */
   slave->sync_status.is_process_data_received = false;

   fsoeslave_send_reset_request (slave, FSOEFRAME_ERROR_WD_EXPIRED);
}

/**
 * \brief Act on reset connection event
 *
 * This event is triggered by application when it has detected an
 * error requiring connection to be reset.
 *
 * Master will be informed about the error and connection will be reset.
 *
 * The following transitions in the state machine are implemented here:
 * - RESET_START
 * - SESSION_RESET2
 * - CONN_RESET3
 * - PARA_RESET3
 * - DATA_RESET3
 * See ETG.5100 ch. 8.5.[2-6].3.
 *
 * \param[in,out] slave          Slave instance
 */
static void fsoeslave_act_on_reset_connection_event (fsoeslave_t * slave)
{
   FSOELOG_WARNING (FSOE_LOG_SLAVE,
         "Application requested connection to be reset\n");

   fsoeslave_send_reset_request (
      slave, FSOEFRAME_ERROR_LOCAL_RESET_OR_RESET_REPLY);

   /* Set outputs in fail-safe state */
   slave->sync_status.is_process_data_received = false;

   slave->is_reset_requested = false;
}

/**
 * \brief Act on Set Data Command event
 *
 * This event is triggered by application when it wants to change what type
 * of data to be sent in Data state (ProcessData or FailSafeData).
 *
 * Note that calling this function is the only way to set the type to
 * ProcessData.
 *
 * The following transitions in the state machine is implemented here:
 * - RESET_STAY2
 * - SESSION_STAY3
 * - CONN_STAY2
 * - PARA_STAY2
 * - DATA_STAY
 * See ETG.5100 ch. 8.5.[2-6].4.
 *
 * \param[in,out] slave          Slave instance
 * \param[in]     DataCmd        Type of data (ProcessData or FailSafeData)
 */
static void fsoeslave_act_on_set_data_command_event (
   fsoeslave_t * slave, uint8_t DataCmd)
{
   slave->DataCommand = DataCmd;
}

/******************************************************************************
*
* Public API
*
* All non-static functions are defined here and documented in the public
* header file "fsoeslave.h".
*
*****************************************************************************/

/* \brief Get description of reset reason as a string literal
 *
 * See public header file for description
 */
const char * fsoeslave_reset_reason_description (
   uint8_t reset_reason)
{
   const char * description;

   description = fsoeframe_error_description (reset_reason);

   return description;
}

/* \brief Get description of state as a string literal
 *
 * See public header file for description
 */
const char * fsoeslave_state_description (fsoeslave_state_t state)
{
   const char * description;

   switch (state)
   {
   case FSOESLAVE_STATE_RESET:
      description = "Reset";
      break;
   case FSOESLAVE_STATE_SESSION:
      description = "Session";
      break;
   case FSOESLAVE_STATE_CONNECTION:
      description = "Connection";
      break;
   case FSOESLAVE_STATE_PARAMETER:
      description = "Parameter";
      break;
   case FSOESLAVE_STATE_DATA:
      description = "Data";
      break;
   default:
      description = "invalid";
      break;
   }

   return description;
}

/* \brief Update SRA CRC value
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_update_sra_crc (
   uint32_t * crc,
   const void * data,
   size_t size)
{
   fsoeslave_status_t status;

   if ((crc == NULL) || (data == NULL))
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to update SRA CRC, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      *crc = fsoesracrc_update (*crc, data, size);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Get current state
 *
 * See public header file for description
 */
 fsoeslave_status_t fsoeslave_get_state (
    const fsoeslave_t * slave,
    fsoeslave_state_t * state)
{
   fsoeslave_status_t status;

   if (state != NULL)
   {
      *state = FSOESLAVE_STATE_RESET;
   }

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the current state, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;

   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the current state, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (state == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the current state, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      *state = slave->sync_status.current_state;
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Get generated Slave Session ID
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_get_slave_session_id (
   const fsoeslave_t * slave,
   uint16_t * session_id)
{
   fsoeslave_status_t status;

   if (session_id != NULL)
   {
      *session_id = 0x0000;
   }

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Slave Session ID, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Slave Session ID, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (session_id == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Slave Session ID, but argument was NULL\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->sync_status.current_state == FSOESLAVE_STATE_RESET)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Slave Session ID, bus it had not yet been generated\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_WRONG_INSTANCE_STATE);
      status = FSOESLAVE_STATUS_ERROR;
      *session_id = 0x0000;
   }
   else
   {
      *session_id =  fsoeutil_from_le16 (slave->SessionId.little_endian);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Get received Master Session ID
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_get_master_session_id (
   const fsoeslave_t * slave,
   uint16_t * session_id)
{
   fsoeslave_status_t status;

   if (session_id != NULL)
   {
      *session_id = 0x0000;
   }

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Master Session ID, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Master Session ID, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (session_id == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Master Session ID, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if ((slave->sync_status.current_state == FSOESLAVE_STATE_RESET) ||
      (slave->sync_status.current_state == FSOESLAVE_STATE_SESSION))
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the Master Session ID, bus it had not yet been received\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_WRONG_INSTANCE_STATE);
      status = FSOESLAVE_STATUS_ERROR;
      *session_id = 0x0000;
   }
   else
   {
      *session_id = fsoeutil_from_le16 (slave->master_session_id.little_endian);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}
/* \brief Get time remaining until watchdog timer timeouts, in milliseconds
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_get_time_until_timeout_ms (
   const fsoeslave_t * slave,
   uint32_t * time_ms)
{
   fsoeslave_status_t status;

   if (time_ms != NULL)
   {
      *time_ms = UINT32_MAX;
   }

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the time remaining until timeout, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the time remaining until timeout, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (time_ms == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get the time remaining until timeout, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      *time_ms = fsoewatchdog_time_until_timeout_ms (&slave->watchdog);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Get flag indicating if sending process data to master is enabled
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_get_process_data_sending_enable_flag (
   const fsoeslave_t * slave,
   bool * is_enabled)
{
   fsoeslave_status_t status;

   if (is_enabled != NULL)
   {
      *is_enabled = false;
   }

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get process data send status, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get process data send status, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (is_enabled == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to get process data send status, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      *is_enabled = slave->DataCommand == FSOEFRAME_COMMAND_ProcessData;
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Clear flag indicating that sending process data to master is enabled
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_clear_process_data_sending_enable_flag (
   fsoeslave_t * slave)
{
   fsoeslave_status_t status;

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to disable sending process data, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to disable sending process data, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      fsoeslave_act_on_set_data_command_event (slave, FSOEFRAME_COMMAND_FailSafeData);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Set flag indicating that sending process data to master is enabled
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_set_process_data_sending_enable_flag (
   fsoeslave_t * slave)
{
   fsoeslave_status_t status;

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to enable sending process data, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to enable sending process data, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      fsoeslave_act_on_set_data_command_event (slave, FSOEFRAME_COMMAND_ProcessData);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Set reset request flag
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_set_reset_request_flag (fsoeslave_t * slave)
{
   fsoeslave_status_t status;

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to reset connection, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to reset connection, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      slave->is_reset_requested = true;
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Synchronise with master
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_sync_with_master (
   fsoeslave_t * slave,
   const void * inputs,
   void * outputs,
   fsoeslave_syncstatus_t * sync_status)
{
   fsoeslave_status_t status;

   if (sync_status != NULL)
   {
      fsoeport_memset (sync_status, 0, sizeof (*sync_status));
   }

   if (slave == NULL)
   {
	  FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to sync with master, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (slave->magic != FSOESLAVE_MAGIC)
   {
	  FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to sync with master, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if ((outputs == NULL) || (inputs == NULL) || (sync_status == NULL))
   {
	  FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to sync with master, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (slave->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
      if (outputs != NULL)
      {
         fsoeport_memset (outputs, 0, slave->outputs_size);
      }
   }
   else
   {
	  /* Clear connection reset event */
      slave->sync_status.reset_event = FSOESLAVE_RESETEVENT_NONE;
      slave->sync_status.reset_reason = FSOESLAVE_RESETREASON_LOCAL_RESET;

      /* Clear new process data event */
      slave->sync_status.received_frame_type = FSOESLAVE_FRAMETYPE_NONE;
      slave->sync_status.sent_frame_type = FSOESLAVE_FRAMETYPE_NONE;

      /* Check and act on events */
      if (slave->is_reset_requested)
      {
         fsoeslave_act_on_reset_connection_event (slave);
      }
      else if (fsoewatchdog_time_until_timeout_ms (&slave->watchdog) == 0)
      {
         fsoeslave_act_on_watchdog_expired_event (slave);
      }
      else
      {
         const fsoeframe_t * received_frame;

         received_frame = fsoechannel_receive_frame (&slave->channel);
         if (received_frame)
         {
            fsoeslave_act_on_frame_received_event (
               slave, received_frame, inputs);
         }
         else
         {
            /* No event */
         }
      }

      /* Copy outputs from state machine */
      if (slave->sync_status.is_process_data_received)
      {
         fsoeport_memcpy (outputs, slave->SafeOutputs, slave->outputs_size);
      }
      else
      {
         fsoeport_memset (outputs, 0, slave->outputs_size);
      }
      *sync_status = slave->sync_status;

      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}

/* \brief Initialise instance
 *
 * See public header file for description
 */
fsoeslave_status_t fsoeslave_init (
   fsoeslave_t * slave,
   const fsoeslave_cfg_t * cfg,
   void * app_ref)
{
   fsoeslave_status_t status;

   if (slave == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to initialise instance, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (cfg == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to initialise instance, but configuration was NULL\n");
      fsoeapp_handle_user_error (app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else if (!fsoeslave_is_configuration_valid (cfg))
   {
      FSOELOG_ERROR (FSOE_LOG_SLAVE,
         "User tried to initialise instance, but configuration was invalid\n");
      fsoeapp_handle_user_error (app_ref, FSOEAPP_USERERROR_BAD_CONFIGURATION);
      status = FSOESLAVE_STATUS_ERROR;
   }
   else
   {
      fsoeslave_initialise (slave, cfg, app_ref);
      status = FSOESLAVE_STATUS_OK;
   }

   return status;
}
