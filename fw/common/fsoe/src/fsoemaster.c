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
 * \brief FSoE master state machine implementation
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
 * - RESET_OK:       Received Reset reply.
 * - RESET_START:    Application requested connection to be reset.
 * - RESET_STAY1:    Received invalid frame.
 * - RESET_STAY2:    Application set new data command.
 * - RESET_WD        Watchdog timer expired.
 * - SESSION_FAIL1:  Received second Session reply with bad CRC.
 * - SESSION_FAIL3*: Received invalid frame.
 * - SESSION_FAIL4:  Received unknown frame.
 * - SESSION_OK:     Received final Session reply.
 * - SESSION_RESET1: Received Reset request.
 * - SESSION_RESET2: Application requested connection to be reset.
 * - SESSION_STAY1:  Received non-final Session reply.
 * - SESSION_STAY2+: Received non-second Session reply with bad CRC.
 * - SESSION_STAY2+: Application requested new data command.
 * - SESSION_WD      Watchdog timer expired.
 * - CONN_FAIL1:     Received Connection reply with bad CRC.
 * - CONN_FAIL2:     Received Connection reply with bad data.
 * - CONN_FAIL3:     Received Connection reply with bad Connection ID.
 * - CONN_FAIL4:     Received invalid frame.
 * - CONN_FAIL5:     Received unknown frame.
 * - CONN_OK:        Received final Connection reply.
 * - CONN_RESET1:    Received Reset request.
 * - CONN_RESET2:    Application requested connection to be reset.
 * - CONN_STAY1:     Received non-final Connection reply.
 * - CONN_STAY2:     Application requested new data command.
 * - CONN_WD         Watchdog timer expired.
 * - PARA_FAIL1:     Received Parameter reply with bad CRC.
 * - PARA_FAIL2:     Received Parameter reply with bad data.
 * - PARA_FAIL3:     Received Parameter reply with bad Connection ID.
 * - PARA_FAIL4:     Received invalid frame.
 * - PARA_FAIL5:     Received unknown frame.
 * - PARA_OK:        Received final Parameter reply.
 * - PARA_RESET1:    Received Reset request.
 * - PARA_RESET2:    Application requested connection to be reset.
 * - PARA_STAY1:     Received non-final Parameter reply.
 * - PARA_STAY2:     Application requested new data command.
 * - PARA_WD         Watchdog timer expired.
 * - DATA_FAIL1:     Received ProcessData/FailSafeData with bad CRC.
 * - DATA_FAIL2:     Received ProcessData/FailSafeData with bad Connection ID.
 * - DATA_FAIL3:     Received invalid frame.
 * - DATA_FAIL4:     Received unknown frame.
 * - DATA_OK1:       Received ProcessData.
 * - DATA_OK2:       Received FailSafeData.
 * - DATA_RESET1:    Received Reset request.
 * - DATA_RESET2:    Application requested connection to be reset.
 * - DATA_STAY:      Application requested new data command.
 * - DATA_WD:        Watchdog timer expired.
 * *) Note that there are no state with the name SESSION_FAIL2.
 * +) Note that there are two states with the same name SESSION_STAY2.
 *    This is a defect in the ETG.5100 specification.
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
 * unit test suite for master.
 */
#ifdef UNIT_TEST
#define fsoeframe_IS_CRC_CORRECT mock_fsoeframe_is_crc_correct
#endif

#include <fsoemaster.h>
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
 * All master instances will have their first field set to this value.
 * API functions will check that the field has this value as to ensure
 * the instance was initialised using fsoemaster_init().
 * It also serves as a check for memory corruption.
 */
#define FSOEMASTER_MAGIC (0xF50E1234u)

/******************************************************************************
 *
 * Miscellaneous functions
 *
 *****************************************************************************/

/**
 * \brief Create a new Session ID and set variable
 *
 * The variable SecondSessionFrameSent is also set to false
 *
 * See ETG.5100 ch. 8.4.1.4 "Master Macros", table 33.
 *
 * \param[in,out] master         Master instance
 * \return                       New Session ID
 */
static fsoeframe_uint16_t fsoemaster_CREATE_SESSION_ID (fsoemaster_t * master)
{
   fsoeframe_uint16_t session_id;

   master->SecondSessionFrameSent = false;
   session_id.little_endian =
         fsoeutil_to_le16 (fsoeapp_generate_session_id (master->app_ref));
   return session_id;
}

/**
 * \brief Return how many bytes are still to be sent before state is changed
 *
 * Used when establishing connection as data sent may not fit in a
 * single frame.
 *
 * This function is called UPDATE_BYTES_TO_BE_SENT() in the standard.
 * See ETG.5100 ch. 8.4.1.4 "Master Macros", table 33.
 *
 * \param[in,out] master         Master instance
 * \param[in]     bytesSent      Number of bytes yet to be sent
 *                               before sending frame
 * \return                       Number of bytes yet to send
 *                               after sending frame
 */
static size_t fsoemaster_UPDATE_BYTES_TO_BE_SENT (
   const fsoemaster_t * master, size_t bytesSent)
{
   size_t remaining_bytes_to_be_sent;
   size_t max_bytes_to_send =
      FSOEUTIL_MIN (master->inputs_size, master->outputs_size);

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
 * \param[in]     cfg            Configuration of FSoE master
 * \return                       true if configuration is valid,
 *                               false if any field is invalid
 */
static bool fsoemaster_is_configuration_valid (const fsoemaster_cfg_t * cfg)
{
   bool is_valid = false;

   if (cfg->connection_id == 0)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER, "Connection ID is 0\n");
   }
   else if (cfg->watchdog_timeout_ms == 0)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER, "Watchdog timeout is 0\n");
   }
   else if ((cfg->application_parameters == NULL) &&
      (cfg->application_parameters_size != 0))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Application parameters are NULL while size is non-zero\n");
   }
   else if (cfg->application_parameters_size >
      FSOE_APPLICATION_PARAMETERS_MAX_SIZE)
   {
       FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Application parameters are too large (%zu > %zu)\n",
          cfg->application_parameters_size,
          FSOE_APPLICATION_PARAMETERS_MAX_SIZE);
   }
   else if (cfg->inputs_size == 0)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER, "Size of inputs is 0\n");
   }
   else if (cfg->inputs_size > FSOE_PROCESS_DATA_MAX_SIZE)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Size of inputs is too large (%zu > %zu)\n",
          cfg->inputs_size, FSOE_PROCESS_DATA_MAX_SIZE);
   }
   else if ((cfg->inputs_size % 2 == 1) && (cfg->inputs_size != 1))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Size of inputs is odd (%zu) and is not 1\n",
          cfg->inputs_size);
   }
   else if (cfg->outputs_size == 0)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER, "Size of outputs is 0\n");
   }
   else if (cfg->outputs_size > FSOE_PROCESS_DATA_MAX_SIZE)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Size of outputs is too large (%zu > %zu)\n",
          cfg->outputs_size, FSOE_PROCESS_DATA_MAX_SIZE);
   }
   else if ((cfg->outputs_size % 2 == 1) && (cfg->outputs_size != 1))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Size of outputs is odd (%zu) and is not 1\n",
          cfg->outputs_size);
   }
   else
   {
      FSOELOG_INFO (FSOE_LOG_MASTER, "Configuration is valid\n");
      is_valid = true;
   }

   return is_valid;
}

/**
 * \brief Initialise variables in master instance
 *
 * No memory allocation is done in this function.
 *
 * \param[out]    master         Master instance
 * \param[in]     cfg            Configuration
 * \param[in]     app_ref        Application reference
 */
static void fsoemaster_initialise (
   fsoemaster_t * master,
   const fsoemaster_cfg_t * cfg,
   void * app_ref)
{
   fsoeport_memset (master, 0, sizeof (*master));

   /* Initialise standard variables. See ETG.5100 ch. 8.4.1.3 table 32 */
   master->LastCrc = 0;
   master->OldMasterCrc = 0;
   master->OldSlaveCrc = 0;
   master->MasterSeqNo = 0;
   master->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   master->ConnData.members.ConnId.little_endian =
      fsoeutil_to_le16 (cfg->connection_id);
   master->ConnData.members.SlaveAddress.little_endian =
      fsoeutil_to_le16 (cfg->slave_address);
   master->SafeParaSize =
      offsetof (fsoeframe_encoded_safepara_t, app_parameters) +
      cfg->application_parameters_size;
   master->SafePara.members.watchdog_size.little_endian =
      fsoeutil_to_le16 (sizeof (master->SafePara.members.watchdog));
   master->SafePara.members.watchdog.little_endian =
      fsoeutil_to_le16 (cfg->watchdog_timeout_ms);
   master->SafePara.members.app_parameters_size.little_endian =
      fsoeutil_to_le16 ((uint16_t)cfg->application_parameters_size);
   if (cfg->application_parameters_size > 0)
   {
      fsoeport_memcpy (master->SafePara.members.app_parameters,
         cfg->application_parameters,
         cfg->application_parameters_size);
   }
   fsoeport_memset (master->SafeInputs, 0, cfg->inputs_size);

   /* Initialise other variables */
   fsoewatchdog_init (&master->watchdog);
   fsoewatchdog_set_timeout_ms (&master->watchdog, cfg->watchdog_timeout_ms);
   fsoechannel_init (
      &master->channel, cfg->inputs_size, cfg->outputs_size, app_ref);
   master->is_reset_requested = true;
   master->sync_status.reset_event = FSOEMASTER_RESETEVENT_NONE;
   master->sync_status.reset_reason = FSOEMASTER_RESETREASON_LOCAL_RESET;
   master->sync_status.current_state = FSOEMASTER_STATE_RESET;
   master->sync_status.is_process_data_received = false;
   master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_NONE;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_NONE;

   /* Initialise other constants */
   master->magic = FSOEMASTER_MAGIC;
   master->connection_id = cfg->connection_id;
   master->slave_address = cfg->slave_address;
   master->inputs_size = cfg->inputs_size;
   master->outputs_size = cfg->outputs_size;
   master->app_ref = app_ref;
}

/******************************************************************************
 *
 * State machine Actions
 *
 * For each state machine transition, an Action will be performed.
 * An Action corresponds to a function below.
 * See ETG.5100 ch. 8.4.1 "FSoE Master state machine".
 *
 *****************************************************************************/

/**
 * \brief Resend Reset request
 *
 * Reset CRC generation, disable process-data outputs,
 * resend Reset request to slave (but with CommFaultReason set to 0)
 * and continue waiting for reply.
 * Note that watchdog timer is not reset.
 *
 * \pre Master is in Reset state
 *
 * \param[in,out] master         Master instance
 */
static void fsoemaster_resend_reset_request (fsoemaster_t * master)
{
   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_RESET);

   master->LastCrc = 0;
   master->OldMasterCrc = 0;
   master->OldSlaveCrc = 0;
   master->MasterSeqNo = 1;
   master->SlaveSeqno = 1;
   master->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   master->CommFaultReason = 0;
   (void)fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Reset,
      &master->CommFaultReason,
      sizeof (master->CommFaultReason),
      master->LastCrc,
      0,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      false
   );
   master->MasterSeqNo = 1;
   master->sync_status.current_state = FSOEMASTER_STATE_RESET;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_RESET;

   /* Report information about connection reset event to user */
   master->sync_status.reset_event = FSOEMASTER_RESETEVENT_BY_MASTER;
   master->sync_status.reset_reason = 0;

   FSOELOG_INFO (FSOE_LOG_MASTER,
      "Sent repeated Reset request\n");
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "1 of 1 byte sent. Expecting ACK within %"PRIu32"ms\n",
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send Reset request
 *
 * Reset CRC generation, disable process-data outputs,
 * send Reset request to slave and start waiting for reply.
 *
 * \param[in,out] master          Master instance
 * \param[in]     CommFaultReason Reason for resetting connection
 */
static void fsoemaster_send_reset_request (
   fsoemaster_t * master,
   uint8_t CommFaultReason)
{
   master->LastCrc = 0;
   master->OldMasterCrc = 0;
   master->OldSlaveCrc = 0;
   master->MasterSeqNo = 1;
   master->SlaveSeqno = 1;
   master->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   master->CommFaultReason = CommFaultReason;
   (void)fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Reset,
      &master->CommFaultReason,
      sizeof (master->CommFaultReason),
      master->LastCrc,
      0,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      false
   );
   master->MasterSeqNo = 1;
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_RESET;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_RESET;

   /* Report information about connection reset event to user */
   master->sync_status.reset_event = FSOEMASTER_RESETEVENT_BY_MASTER;
   master->sync_status.reset_reason = CommFaultReason;

   FSOELOG_INFO (FSOE_LOG_MASTER,
      "Sent Reset request. Cause: %s (0x%02"PRIx8")\n",
      fsoeframe_error_description (CommFaultReason),
      CommFaultReason);
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "1 of 1 byte sent. Expecting ACK within %"PRIu32"ms\n",
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send initial Session start request
 *
 * Create Session ID, send it to slave and start waiting for reply.
 * Note that multiple requests may be needed.
 *
 * \pre Master is in Reset state
 *
 * \param[in,out] master         Master instance
 */
static void fsoemaster_send_initial_session_start_request (
   fsoemaster_t * master)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_RESET);

   master->SessionId = fsoemaster_CREATE_SESSION_ID (master);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Session,
      &master->SessionId,
      FSOEUTIL_MIN (sizeof (master->SessionId), master->inputs_size),
      master->LastCrc,
      0,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      false
   );
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent = fsoemaster_UPDATE_BYTES_TO_BE_SENT (master, 2);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_SESSION;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_SESSION;

   FSOELOG_INFO (FSOE_LOG_MASTER,
      "Sent initial Session request with Session ID 0x%04"PRIx16".\n",
      fsoeutil_from_le16 (master->SessionId.little_endian));
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of 2 bytes sent. Expecting ACK within %"PRIu32"ms\n",
      sizeof (master->SessionId) - master->BytesToBeSent,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send initial Session restart request
 *
 * Reset CRC generation, disable process-data outputs, create new Session ID,
 * send it to slave and start waiting for reply.
 * Note that multiple requests may be needed.
 *
 * \pre Master is not in Reset state
 *
 * \param[in,out] master         Master instance
 */
static void fsoemaster_send_initial_session_restart_request (
   fsoemaster_t * master)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state != FSOEMASTER_STATE_RESET);

   master->LastCrc = 0;
   master->OldMasterCrc = 0;
   master->OldSlaveCrc = 0;
   master->MasterSeqNo = 1;
   master->SlaveSeqno = 1;
   master->DataCommand = FSOEFRAME_COMMAND_FailSafeData;
   master->SessionId = fsoemaster_CREATE_SESSION_ID (master);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Session,
      &master->SessionId,
      FSOEUTIL_MIN (sizeof (master->SessionId), master->inputs_size),
      master->LastCrc,
      0,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      false
   );
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent = fsoemaster_UPDATE_BYTES_TO_BE_SENT (master, 2);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_SESSION;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_SESSION;

   FSOELOG_INFO (FSOE_LOG_MASTER,
      "Sent initial Session request with new Session ID 0x%04"PRIx16".\n",
      fsoeutil_from_le16 (master->SessionId.little_endian));
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of 2 bytes sent. Expecting ACK within %"PRIu32"ms\n",
      sizeof (master->SessionId) - master->BytesToBeSent,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send next Session request
 *
 * Send remaining part of the Session ID an a second Session request to slave
 * and start waiting for reply. This is not needed if the Session ID can
 * fit in a single sent frame and a single received frame.
 *
 * \pre Master is in Session state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 */
static void fsoemaster_send_next_session_request (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_SESSION);
   FSOEPORT_ASSERT (master->BytesToBeSent <= 2u);

   master->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Session,
      &master->SessionId.bytes[2u - master->BytesToBeSent],
      FSOEUTIL_MIN (master->BytesToBeSent, master->inputs_size),
      fsoeframe_crc0 (Frame),
      0,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      true
   );
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent = fsoemaster_UPDATE_BYTES_TO_BE_SENT (
      master, master->BytesToBeSent);
   master->SecondSessionFrameSent = true;
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_SESSION;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_SESSION;

   FSOELOG_INFO (FSOE_LOG_MASTER, "Sent next Session request\n");
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of 2 bytes sent. Expecting ACK within %"PRIu32"ms\n",
      sizeof (master->SessionId) - master->BytesToBeSent,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send initial Connection request
 *
 * Start sending connection data to slave and wait for reply.
 * The connection data is the Connection ID and the Slave Address.
 *
 * Note that multiple frames may be required to send all.
 *
 * \pre Master is in Session state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 */
static void fsoemaster_send_initial_connection_request (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_SESSION);

   master->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Connection,
      &master->ConnData,
      FSOEUTIL_MIN (sizeof (master->ConnData), master->inputs_size),
      master->LastCrc,
      master->connection_id,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      true);
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent = fsoemaster_UPDATE_BYTES_TO_BE_SENT (master, 4);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_CONNECTION;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_CONNECTION;

   FSOELOG_INFO (FSOE_LOG_MASTER,
      "Sent initial Connection request. "
      "ConnID: %"PRIu16". Slave Address %"PRIu16"\n",
      master->connection_id,
      master->slave_address);
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of 4 bytes sent. Expecting ACK within %"PRIu32"ms\n",
      sizeof (master->ConnData) - master->BytesToBeSent,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send next Connection request
 *
 * Continue sending connection data to slave and wait for reply.
 * The connection data is the Connection ID and the Slave Address.
 * This is not needed if the connection data can
 * fit in a single sent frame and a single received frame.
 *
 * \pre Master is in Connection state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 */
static void fsoemaster_send_next_connection_request (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_CONNECTION);
   FSOEPORT_ASSERT (master->BytesToBeSent <= 4u);

   master->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Connection,
      &master->ConnData.bytes[4u - master->BytesToBeSent],
      FSOEUTIL_MIN (4u - master->BytesToBeSent, master->inputs_size),
      fsoeframe_crc0 (Frame),
      master->connection_id,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      true);
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent = fsoemaster_UPDATE_BYTES_TO_BE_SENT (
      master, master->BytesToBeSent);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_CONNECTION;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_CONNECTION;

   FSOELOG_INFO (FSOE_LOG_MASTER, "Sent next Connection request\n");
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of 4 bytes sent. Expecting ACK within %"PRIu32"ms\n",
      sizeof (master->ConnData) - master->BytesToBeSent,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send initial Parameter request
 *
 * Start sending parameters to slave and wait for reply.
 * The parameters include both the watchdog timeout and the
 * device-specific application parameters.
 * Note that multiple frames may be required to send them all.
 *
 * \pre Master is in Connection state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 */
static void fsoemaster_send_initial_parameter_request (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_CONNECTION);

   master->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Parameter,
      &master->SafePara,
      FSOEUTIL_MIN (master->inputs_size, master->SafeParaSize),
      fsoeframe_crc0 (Frame),
      master->connection_id,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      true);
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent =
      fsoemaster_UPDATE_BYTES_TO_BE_SENT (master, master->SafeParaSize);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_PARAMETER;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_PARAMETER;

   FSOELOG_INFO (FSOE_LOG_MASTER, "Sent initial Parameter request\n");
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of %"PRIu16" bytes sent. Expecting ACK within %"PRIu32"ms\n",
      master->SafeParaSize - master->BytesToBeSent,
      master->SafeParaSize,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send next Parameter request
 *
 * Send more parameters to slave and start waiting for reply
 * This is not needed if the connection data can
 * fit in a single sent frame and a single received frame.
 *
 * \pre Master is in Parameter state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 */
static void fsoemaster_send_next_parameter_request (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   const fsoeframe_t * SendFrame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_PARAMETER);
   FSOEPORT_ASSERT (master->BytesToBeSent <= master->SafeParaSize);

   master->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      FSOEFRAME_COMMAND_Parameter,
      &master->SafePara.bytes[master->SafeParaSize - master->BytesToBeSent],
      FSOEUTIL_MIN (master->BytesToBeSent, master->inputs_size),
      fsoeframe_crc0 (Frame),
      master->connection_id,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      true);
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   master->BytesToBeSent =
      fsoemaster_UPDATE_BYTES_TO_BE_SENT (master, master->BytesToBeSent);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_PARAMETER;
   master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_PARAMETER;

   FSOELOG_INFO (FSOE_LOG_MASTER, "Sent next Parameter request\n");
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of %"PRIu16" bytes sent. Expecting ACK within %"PRIu32"ms\n",
      master->SafeParaSize - master->BytesToBeSent,
      master->SafeParaSize,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
}

/**
 * \brief Send data frame (ProcessData or FailSafeData)
 *
 * Send outputs to slave and start waiting for ACK (i.e. inputs)
 *
 * \pre Master is in Data state or is entering Data state from Parameter state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 * \param[in]     SafeOutputs    Data to (possibly) send
 */
static void fsoemaster_send_data_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame,
   const void * SafeOutputs)
{
   const fsoeframe_t * SendFrame;

   master->LastCrc = fsoeframe_crc0 (Frame);
   SendFrame = fsoechannel_SendFrame (
      &master->channel,
      master->DataCommand,
      SafeOutputs,
      master->outputs_size,
      fsoeframe_crc0 (Frame),
      master->connection_id,
      &master->MasterSeqNo,
      &master->OldMasterCrc,
      true);
   master->LastCrc = fsoeframe_crc0 (SendFrame);
   fsoewatchdog_START_WD (&master->watchdog);
   master->sync_status.current_state = FSOEMASTER_STATE_DATA;
   if (master->DataCommand == FSOEFRAME_COMMAND_ProcessData)
   {
      master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_PROCESS_DATA;
   }
   else
   {
      master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_FAILSAFE_DATA;
   }

   FSOELOG_INFO (FSOE_LOG_MASTER, "Sent %s frame\n",
      fsoeframe_command_name (fsoeframe_command (SendFrame)));
   FSOELOG_DEBUG (FSOE_LOG_MASTER,
      "%"PRIu16" of %"PRIu16" bytes sent. Expecting ACK within %"PRIu32"ms\n",
      (uint16_t)master->outputs_size,
      (uint16_t)master->outputs_size,
      fsoewatchdog_time_until_timeout_ms (&master->watchdog));
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
 * See ETG.5100 table 30: "Events in the FSoE Master state table".
 *
 *****************************************************************************/

/**
 * \brief Act on received Reset reply in Reset state
 *
 * A Reset reply was received from slave as an ACK to previously sent
 * Reset request. A Session request with a new Session ID will be sent
 * to slave.
 *
 * The following transition in the state machine is implemented here:
 * - RESET_OK
 * See ETG.5100 ch. 8.4.2.1.
 *
 * \pre Master is in Reset state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Reset reply frame
 */
static void fsoemaster_act_on_received_reset_reply (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   (void) Frame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_RESET);

   FSOELOG_INFO (FSOE_LOG_MASTER, "Received Reset reply\n");

   /* Report that a new valid frame was received */
   master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_RESET;

   fsoemaster_send_initial_session_start_request (master);
}

/**
 * \brief Act on received error while not in Reset state
 *
 * A Reset request was received from slave after it detected some error.
 * Connection will be reset and a Session request with a new Session ID
 * will be sent to slave.
 *
 * The following transitions in the state machine are implemented here:
 * - SESSION_RESET1
 * - CONN_RESET1
 * - PARA_RESET1
 * - DATA_RESET1
 * See ETG.5100 ch. 8.4.[3-6].1.
 *
 * \pre Master is not in Reset state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Reset request frame
 */
static void fsoemaster_act_on_received_reset_request (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   uint8_t error_code;

   FSOEPORT_ASSERT (master->sync_status.current_state != FSOEMASTER_STATE_RESET);

   /* Report information about connection reset event to user */
   error_code = fsoeframe_error_code (Frame);
   master->sync_status.reset_event = FSOEMASTER_RESETEVENT_BY_SLAVE;
   master->sync_status.reset_reason = error_code;

   /* Report that a new valid frame was received */
   master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_RESET;

   FSOELOG_WARNING (FSOE_LOG_MASTER,
          "Received Reset request. Cause: %s (0x%02"PRIx8")\n",
         fsoeframe_error_description (error_code),
         error_code);

   fsoemaster_send_initial_session_restart_request (master);
}

/**
 * \brief Act on received Session reply in Session state
 *
 * Master was waiting for a reply to the Session request it sent to
 * slave and receives it.
 * If all Session ID data has been transferred without error, master will send
 * a Connection request to slave. If more Session ID data remains to be sent,
 * the next Session request will be sent to slave (assuming no error
 * is detected). If the received Session request is incorrect and it is the
 * second reply, then the connection will be reset and slave will be informed
 * of the error. If the received Session request is incorrect and it is
 * the non-second reply, the watchdog timer will be restarted.
 *
 * See ETG.5100 ch. 8.4.3.1.
 *
 * \pre Master is in Session state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Session reply frame
 */
static void fsoemaster_act_on_received_session_reply (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_SESSION);

   if (!fsoeframe_IS_CRC_CORRECT (
      Frame, master->LastCrc, &master->SlaveSeqno, &master->OldSlaveCrc, true))
   {
      if (master->SecondSessionFrameSent)
      {
         /* Transition SESSION_FAIL1 */
         FSOELOG_ERROR (FSOE_LOG_MASTER,
            "Received second Session reply with bad CRC (0x%04"PRIx16")\n",
            fsoeframe_crc0 (Frame));
         fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CRC);
      }
      else
      {
         /* Transition SESSION_STAY2 */
         FSOELOG_WARNING (FSOE_LOG_MASTER,
            "Received non-second Session reply with bad CRC (0x%04"PRIx16")\n",
            fsoeframe_crc0 (Frame));
         fsoewatchdog_START_WD (&master->watchdog);
      }
   }
   else if (master->BytesToBeSent != 0)
   {
      /* Save least significant byte of received Slave Session ID */
      fsoeframe_get_data (Frame, &master->slave_session_id.bytes[0], 1);

      /* Report that a new valid frame was received */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_SESSION;

      /* Transition SESSION_STAY1 */
      FSOELOG_INFO (FSOE_LOG_MASTER, "Received initial Session reply\n");
      fsoemaster_send_next_session_request (master, Frame);
   }
   else
   {
      if (master->SecondSessionFrameSent)
      {
         /* Save most significant byte of received Slave Session ID */
         fsoeframe_get_data (Frame, &master->slave_session_id.bytes[1], 1);
      }
      else
      {
         /* Save received Slave Session ID */
         fsoeframe_get_data (
            Frame,
            &master->slave_session_id,
            sizeof (master->slave_session_id));
      }

      /* Report that a new valid frame was received */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_SESSION;

      /* Transition SESSION_OK */
      FSOELOG_INFO (FSOE_LOG_MASTER, "Received Session ID 0x%04"PRIx16"\n",
         fsoeutil_from_le16 (master->slave_session_id.little_endian));
      fsoemaster_send_initial_connection_request (master, Frame);
   }
}

/**
 * \brief Act on received Connection reply in Connection state
 *
 * Master was waiting for a reply to the Connection request it sent to
 * slave and receives it.
 * The connection data is the Connection ID and the Slave Address.
 * If all Connection data has been transferred without error, master will send
 * a Parameter request to slave. If more Connection data remains to be sent,
 * the next Connection request will be sent to slave (assuming no error
 * is detected). Otherwise, the connection will be reset and slave will
 * be informed of the error.
 *
 * See ETG.5100 ch. 8.4.4.1.
 *
 * \pre Master is in Connection state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Connection reply frame
 */
static void fsoemaster_act_on_received_connection_reply (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_CONNECTION);

   if (FrameConnId != master->connection_id)
   {
      /* Transition CONN_FAIL3 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received Connection reply with bad ConnID (%"PRIu16"), expected %"PRIu16"\n",
         FrameConnId,
         master->connection_id);
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (!fsoeframe_IS_SAFEDATA_CORRECT (
      Frame,
      FSOEUTIL_MIN (master->inputs_size, master->outputs_size),
      &master->ConnData,
      4u - master->BytesToBeSent))
   {
      /* Transition CONN_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received Connection reply with bad data\n");
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_DATA);
   }
   else if (!fsoeframe_IS_CRC_CORRECT (
      Frame, master->LastCrc, &master->SlaveSeqno, &master->OldSlaveCrc, true))
   {
      /* Transition CONN_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received Connection reply with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CRC);
   }
   else if (master->BytesToBeSent != 0)
   {
      /* Report that a new valid frame was received */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_CONNECTION;

      /* Transition CONN_STAY1 */
      FSOELOG_INFO (FSOE_LOG_MASTER, "Received next Connection reply\n");
      fsoemaster_send_next_connection_request (master, Frame);
   }
   else
   {
      /* Report that a new valid frame was received */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_CONNECTION;

      /* Transition CONN_OK */
      FSOELOG_INFO (FSOE_LOG_MASTER, "Received final Connection reply\n");
      fsoemaster_send_initial_parameter_request (master, Frame);
   }
}

/**
 * \brief Act on received Parameter reply in Parameter state
 *
 * Master was waiting for a reply to the Parameter request it sent to
 * slave and receives it.
 * The parameter data include both the watchdog timeout and the
 * device-specific application parameters.
 * If all Parameter data has been transferred without error, master will start
 * sending data frames to slave. If more Parameter data remains to be sent,
 * the next Parameter request will be sent to slave (assuming no error
 * is detected). Otherwise, the connection will be reset and slave will
 * be informed of the error.
 *
 * See ETG.5100 ch. 8.4.5.1.
 *
 * \pre Master is in Parameter state
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Parameter reply frame
 * \param[in]     SafeOutputs    Data to (possibly) send
 */
static void fsoemaster_act_on_received_parameter_reply (
   fsoemaster_t * master,
   const fsoeframe_t * Frame,
   const void * SafeOutputs)
{
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_PARAMETER);

   if (FrameConnId != master->connection_id)
   {
      /* Transition PARA_FAIL3 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received Parameter reply with bad ConnID (%"PRIu16"), expected %"PRIu16"\n",
         FrameConnId,
         master->connection_id);
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (!fsoeframe_IS_SAFEDATA_CORRECT (
      Frame,
      FSOEUTIL_MIN (master->inputs_size, master->outputs_size),
      &master->SafePara,
      master->SafeParaSize - master->BytesToBeSent))
   {
      /* Transition PARA_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received Parameter reply with bad data\n");
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_DATA);
   }
   else if (!fsoeframe_IS_CRC_CORRECT (
      Frame, master->LastCrc, &master->SlaveSeqno, &master->OldSlaveCrc, true))
   {
      /* Transition PARA_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received Parameter reply with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_crc0 (Frame));
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CRC);
   }
   else if (master->BytesToBeSent != 0)
   {
      /* Report that a new valid frame was received */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_PARAMETER;

      /* Transition PARA_STAY1 */
      FSOELOG_INFO (FSOE_LOG_MASTER, "Received next Parameter reply\n");
      fsoemaster_send_next_parameter_request (master, Frame);
   }
   else
   {
      /* Report that a new valid frame was received */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_PARAMETER;

      /* Transition PARA_OK */
      FSOELOG_INFO (FSOE_LOG_MASTER, "Received final Parameter reply\n");
      fsoemaster_send_data_frame (master, Frame, SafeOutputs);
   }
}

/**
 * \brief Act on received data frame (ProcessData or FailSafeData) in Data state
 *
 * Master was waiting for a data frame from slave and received it.
 * If the received frame is correct, the received data will be stored
 * and a data frame will be sent to slave. Otherwise, connection is
 * reset and slave is informed of the error.
 *
 * \pre Master is in Data state

 * See ETG.5100 ch. 8.4.6.1.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received ProcessData or FailSafeData frame
 * \param[in]     SafeOutputs    Data to (possibly) send
 */
static void fsoemaster_act_on_received_data (
   fsoemaster_t * master,
   const fsoeframe_t * Frame,
   const void * SafeOutputs)
{
   const uint8_t FrameCommand = fsoeframe_command (Frame);
   const uint16_t FrameConnId = fsoeframe_connection_id (Frame);

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_DATA);

   if (FrameConnId != master->connection_id)
   {
      /* Transition DATA_FAIL2 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received %s frame with bad ConnID (%"PRIu16"), expected %"PRIu16"\n",
         fsoeframe_command_name (FrameCommand),
         FrameConnId,
         master->connection_id);
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CONNID);
   }
   else if (!fsoeframe_IS_CRC_CORRECT (
      Frame, master->LastCrc, &master->SlaveSeqno, &master->OldSlaveCrc, true))
   {
      /* Transition DATA_FAIL1 */
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received %s frame with bad CRC (0x%04"PRIx16")\n",
         fsoeframe_command_name (FrameCommand),
         fsoeframe_crc0 (Frame));
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CRC);
   }
   else
   {
      if (FrameCommand == FSOEFRAME_COMMAND_ProcessData)
      {
         /* Report that a new valid frame was received */
         master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_PROCESS_DATA;

         /* Transition DATA_OK1 */
         FSOELOG_INFO (FSOE_LOG_MASTER,
            "Received ProcessData frame (%"PRIu32" bytes SafeInputs)\n",
            (uint32_t)master->inputs_size);
         master->sync_status.is_process_data_received = true;
      }
      else
      {
         /* Report that a new valid frame was received */
         master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_FAILSAFE_DATA;

         /* Transition DATA_OK2 */
         FSOELOG_INFO (FSOE_LOG_MASTER,
            "Received FailSafeData frame (%"PRIu32" bytes SafeInputs)\n",
            (uint32_t)master->inputs_size);
      }
      fsoemaster_send_data_frame (master, Frame, SafeOutputs);
   }
}

/**
 * \brief Act on received invalid frame in Reset state
 *
 * An unexpected non-Reset frame has been received from slave.
 * The previously sent Reset frame will be sent again but without
 * restarting the watchdog timer.
 *
 * The following transition in the state machine is implemented here:
 * - RESET_STAY1
 * See ETG.5100 ch. 8.4.2.1.
 *
 * \pre Master is in Reset state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received non-Reset frame
 */
static void fsoemaster_act_on_received_invalid_frame_in_reset_state (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   (void) Frame;

   FSOEPORT_ASSERT (master->sync_status.current_state == FSOEMASTER_STATE_RESET);

   FSOELOG_ERROR (FSOE_LOG_MASTER,
      "Received %s frame (0x%02"PRIx8") in Reset state\n",
      fsoeframe_command_name (fsoeframe_command (Frame)),
      (uint8_t)fsoeframe_command (Frame));

   fsoemaster_resend_reset_request (master);
}

/**
 * \brief Act on received invalid frame while not in Reset state
 *
 * A frame of a type which is not valid for the current state
 * has been received from slave.
 * Connection will be reset and slave will be informed about the error.
 *
 * The following transition in the state machine is implemented here:
 * - SESSION_FAIL3
 * - CONN_FAIL4
 * - PARA_FAIL4
 * - DATA_FAIL3
 * See ETG.5100 ch. 8.4.[3-6].1.
 *
 * \pre Master is not in Reset state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame of type which is not
 *                               valid for current state
 */
static void fsoemaster_act_on_received_invalid_frame_in_non_reset_state (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   (void) Frame;

   FSOEPORT_ASSERT (master->sync_status.current_state != FSOEMASTER_STATE_RESET);

   FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received %s frame in %s state\n",
         fsoeframe_command_name (fsoeframe_command (Frame)),
         fsoemaster_state_description (master->sync_status.current_state));

   fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_INVALID_CMD);
}

/**
 * \brief Act on received unknown frame in non-Reset state
 *
 * A frame of unknown type has been received from slave.
 *
 * The following transitions in the state machine are implemented here:
 * - SESSION_FAIL4
 * - CONN_FAIL5
 * - PARA_FAIL5
 * - DATA_FAIL4
 * See ETG.5100 ch. 8.4.[3-6].1.
 *
 * \pre Master is not in Reset state.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame of unknown type
 */
static void fsoemaster_act_on_received_unknown_frame_in_non_reset_state (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   (void) Frame;

   FSOEPORT_ASSERT (master->sync_status.current_state != FSOEMASTER_STATE_RESET);

   FSOELOG_ERROR (FSOE_LOG_MASTER,
         "Received frame of unknown type: 0x%02"PRIx8"\n",
         (uint8_t)fsoeframe_command (Frame));

   fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_UNKNOWN_CMD);
}

/**
 * \brief Act on received Reset frame
 *
 * A frame of type Reset has been received from slave.
 * A new Session ID will be generated and sent to slave in
 * a Session request.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Reset frame
 */
static void fsoemaster_act_on_received_reset_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_act_on_received_reset_reply (master, Frame);
      break;
   default:
      fsoemaster_act_on_received_reset_request (master, Frame);
      break;
   }
}

/**
 * \brief Act on received Session frame
 *
 * A frame of type Session has been received from slave.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Session frame
 */
static void fsoemaster_act_on_received_session_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_act_on_received_invalid_frame_in_reset_state (master, Frame);
      break;
   case FSOEMASTER_STATE_SESSION:
      fsoemaster_act_on_received_session_reply (master, Frame);
      break;
   default:
      fsoemaster_act_on_received_invalid_frame_in_non_reset_state (master, Frame);
      break;
   }
}

/**
 * \brief Act on received Connection frame
 *
 * A frame of type Connection has been received from slave.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Connection frame
 */
static void fsoemaster_act_on_received_connection_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_act_on_received_invalid_frame_in_reset_state (master, Frame);
      break;
   case FSOEMASTER_STATE_CONNECTION:
      fsoemaster_act_on_received_connection_reply (master, Frame);
      break;
   default:
      fsoemaster_act_on_received_invalid_frame_in_non_reset_state (master, Frame);
      break;
   }
}

/**
 * \brief Act on received Parameter frame
 *
 * A frame of type Parameter has been received from slave.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received Parameter frame
 * \param[in]     SafeOutputs    Data to (possibly) send to slave
 */
static void fsoemaster_act_on_received_parameter_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame,
   const void * SafeOutputs)
{
   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_act_on_received_invalid_frame_in_reset_state (master, Frame);
      break;
   case FSOEMASTER_STATE_PARAMETER:
      fsoemaster_act_on_received_parameter_reply (master, Frame, SafeOutputs);
      break;
   default:
      fsoemaster_act_on_received_invalid_frame_in_non_reset_state (master, Frame);
      break;
   }
}

/**
 * \brief Act on received ProcessData or FailSafeData frame
 *
 * A frame of type ProcessData or FailSafeData has been received from slave.
 * If this was expected and no errors are detected, the received inputs data
 * will be stored and outputs data will be sent back to slave. In any other case,
 * slave will be informed of the error and connection will be reset.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received ProcessData or FailSafeData frame
 * \param[in]     SafeOutputs    Data to (possibly) send to slave
 */
static void fsoemaster_act_on_received_data_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame,
   const void * SafeOutputs)
{
   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_act_on_received_invalid_frame_in_reset_state (master, Frame);
      break;
   case FSOEMASTER_STATE_DATA:
      fsoemaster_act_on_received_data (master, Frame, SafeOutputs);
      break;
   default:
      fsoemaster_act_on_received_invalid_frame_in_non_reset_state (master, Frame);
      break;
   }
}

/**
 * \brief Act on received unknown frame
 *
 * A frame of unknown type has been received from slave.
 * A Reset request will be sent to slave.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame of unknown type
 */
static void fsoemaster_act_on_received_unknown_frame (
   fsoemaster_t * master,
   const fsoeframe_t * Frame)
{
   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_act_on_received_invalid_frame_in_reset_state (master, Frame);
      break;
   default:
      fsoemaster_act_on_received_unknown_frame_in_non_reset_state (master, Frame);
      break;
   }
}

/**
 * \brief Act on frame received event
 *
 * This event is triggered by the master stack when it has received
 * a frame from slave.
 *
 * Depending on what type of frame (called "command" in ETG.5100) was
 * received, different actions will be taken.
 * See ETG.5100 ch. 8.4.[2-6].1.
 *
 * \param[in,out] master         Master instance
 * \param[in]     Frame          Received frame
 * \param[in]     SafeOutputs    Data to (possibly) send to slave
 */
static void fsoemaster_act_on_frame_received_event (
   fsoemaster_t * master,
   const fsoeframe_t * Frame,
   const void * SafeOutputs)
{
   const uint8_t FrameCommand = fsoeframe_command (Frame);

   /* By default, inputs are set to their fail-safe value */
   master->sync_status.is_process_data_received = false;

   /* By default, treat received frame as invalid until validated */
   master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_INVALID;

   /* Get data from received frame */
   fsoeframe_get_data (Frame, master->SafeInputs, master->inputs_size);

   switch (FrameCommand)
   {
   case FSOEFRAME_COMMAND_Reset:
      fsoemaster_act_on_received_reset_frame (master, Frame);
      break;
   case FSOEFRAME_COMMAND_Session:
      fsoemaster_act_on_received_session_frame (master, Frame);
      break;
   case FSOEFRAME_COMMAND_Connection:
      fsoemaster_act_on_received_connection_frame (master, Frame);
      break;
   case FSOEFRAME_COMMAND_Parameter:
      fsoemaster_act_on_received_parameter_frame (
         master, Frame, SafeOutputs);
      break;
   case FSOEFRAME_COMMAND_FailSafeData:
      /* Fall-through */
   case FSOEFRAME_COMMAND_ProcessData:
      fsoemaster_act_on_received_data_frame (
         master, Frame, SafeOutputs);
      break;
   default:
      fsoemaster_act_on_received_unknown_frame (master, Frame);
      break;
   }
}

/**
 * \brief Act on Watchdog Expired event
 *
 * This event is triggered by the master stack when it detects that
 * it has not received a frame in time.
 *
 * Slave will be informed about the error and connection will be reset.
 *
 * The following transition in the state machine is implemented here:
 * - RESET_WD.
 * - SESSION_WD.
 * - CONN_WD.
 * - PARA_WD.
 * - DATA_WD.
 * See ETG.5100 ch. 8.4.[2-6].2.
 *
 * \param[in,out] master           Master instance
 */
static void fsoemaster_act_on_watchdog_expired_event (fsoemaster_t * master)
{
   FSOELOG_ERROR (FSOE_LOG_MASTER,
      "Watchdog timer expired. No ACK received within %"PRIu32"ms\n",
      fsoewatchdog_timeout_ms (&master->watchdog));

   /* Set inputs in fail-safe state */
   master->sync_status.is_process_data_received = false;

   switch (master->sync_status.current_state)
   {
   case FSOEMASTER_STATE_RESET:
      fsoemaster_send_initial_session_start_request (master);
      break;
   default:
      fsoemaster_send_reset_request (master, FSOEFRAME_ERROR_WD_EXPIRED);
      break;
   }
}

/**
 * \brief Act on reset connection event
 *
 * This event is triggered by application when it has detected an
 * error requiring connection to be reset.
 *
 * Slave will be informed about the error and connection will be reset.
 *
 * The following transitions in the state machine are implemented here:
 * - RESET_START
 * - SESSION_RESET2
 * - CONN_RESET2
 * - PARA_RESET2
 * - DATA_RESET2
 * See ETG.5100 ch. 8.4.[2-6].3.
 *
 * \param[in,out] master         Master instance
 */

static void fsoemaster_act_on_reset_connection_event (fsoemaster_t * master)
{
   FSOELOG_WARNING (FSOE_LOG_MASTER,
         "Application requested connection to be reset\n");

   fsoemaster_send_reset_request (
      master, FSOEFRAME_ERROR_LOCAL_RESET_OR_RESET_REPLY);

   /* Set inputs in fail-safe state */
   master->sync_status.is_process_data_received = false;

   master->is_reset_requested = false;
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
 * - SESSION_STAY2
 * - CONN_STAY2
 * - PARA_STAY2
 * - DATA_STAY
 * See ETG.5100 ch. 8.4.[2-6].4.
 *
 * \param[in,out] master         Master instance
 * \param[in]     DataCmd        Type of data (ProcessData or FailSafeData)
 */

static void fsoemaster_act_on_set_data_command_event (
   fsoemaster_t * master, uint8_t DataCmd)
{
   master->DataCommand = DataCmd;
}

/******************************************************************************
*
* Public API
*
* All non-static functions are defined here and documented in the public
* header file "fsoemaster.h".
*
*****************************************************************************/

/* \brief Get description of reset reason as a string literal
 *
 * See public header file for description
 */
const char * fsoemaster_reset_reason_description (
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
const char * fsoemaster_state_description (fsoemaster_state_t state)
{
   const char * description;

   switch (state)
   {
   case FSOEMASTER_STATE_RESET:
      description = "Reset";
      break;
   case FSOEMASTER_STATE_SESSION:
      description = "Session";
      break;
   case FSOEMASTER_STATE_CONNECTION:
      description = "Connection";
      break;
   case FSOEMASTER_STATE_PARAMETER:
      description = "Parameter";
      break;
   case FSOEMASTER_STATE_DATA:
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
fsoemaster_status_t fsoemaster_update_sra_crc (
   uint32_t * crc,
   const void * data,
   size_t size)
{
   fsoemaster_status_t status;

   if ((crc == NULL) || (data == NULL))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to update SRA CRC, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      *crc = fsoesracrc_update (*crc, data, size);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Get current state
 *
 * See public header file for description
 */
 fsoemaster_status_t fsoemaster_get_state (
    const fsoemaster_t * master,
    fsoemaster_state_t * state)
{
   fsoemaster_status_t status;

   if (state != NULL)
   {
      *state = FSOEMASTER_STATE_RESET;
   }

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the current state, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;

   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the current state, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (state == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the current state, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      *state = master->sync_status.current_state;
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Get generated Master Session ID
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_get_master_session_id (
   const fsoemaster_t * master,
   uint16_t * session_id)
{
   fsoemaster_status_t status;

   if (session_id != NULL)
   {
      *session_id = 0x0000;
   }

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Master Session ID, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Master Session ID, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (session_id == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Master Session ID, but argument was NULL\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->sync_status.current_state == FSOEMASTER_STATE_RESET)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Master Session ID, bus it had not yet been generated\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_WRONG_INSTANCE_STATE);
      status = FSOEMASTER_STATUS_ERROR;
      *session_id = 0x0000;
   }
   else
   {
      *session_id =  fsoeutil_from_le16 (master->SessionId.little_endian);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Get received Slave Session ID
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_get_slave_session_id (
   const fsoemaster_t * master,
   uint16_t * session_id)
{
   fsoemaster_status_t status;

   if (session_id != NULL)
   {
      *session_id = 0x0000;
   }

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Slave Session ID, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Slave Session ID, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (session_id == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Slave Session ID, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if ((master->sync_status.current_state == FSOEMASTER_STATE_RESET) ||
      (master->sync_status.current_state == FSOEMASTER_STATE_SESSION))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the Slave Session ID, bus it had not yet been received\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_WRONG_INSTANCE_STATE);
      status = FSOEMASTER_STATUS_ERROR;
      *session_id = 0x0000;
   }
   else
   {
      *session_id = fsoeutil_from_le16 (master->slave_session_id.little_endian);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Get time remaining until watchdog timer timeouts, in milliseconds
 *
 * See public header file for description
 */
 fsoemaster_status_t fsoemaster_get_time_until_timeout_ms (
    const fsoemaster_t * master,
    uint32_t * time_ms)
{
   fsoemaster_status_t status;

   if (time_ms != NULL)
   {
      *time_ms = UINT32_MAX;
   }

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the time remaining until timeout, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the time remaining until timeout, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (time_ms == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get the time remaining until timeout, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      *time_ms = fsoewatchdog_time_until_timeout_ms (&master->watchdog);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Get flag indicating if sending process data to slave is enabled
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_get_process_data_sending_enable_flag (
   const fsoemaster_t * master,
   bool * is_enabled)
{
   fsoemaster_status_t status;

   if (is_enabled != NULL)
   {
      *is_enabled = false;
   }

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get process data send status, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get process data send status, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (is_enabled == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to get process data send status, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      *is_enabled = master->DataCommand == FSOEFRAME_COMMAND_ProcessData;
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Clear flag indicating that sending process data to slave is enabled
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_clear_process_data_sending_enable_flag (
   fsoemaster_t * master)
{
   fsoemaster_status_t status;

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to disable sending process data, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to disable sending process data, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      fsoemaster_act_on_set_data_command_event (
         master, FSOEFRAME_COMMAND_FailSafeData);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Set flag indicating that sending process data to slave is enabled
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_set_process_data_sending_enable_flag (
   fsoemaster_t * master)
{
   fsoemaster_status_t status;

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to enable sending process data, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to enable sending process data, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      fsoemaster_act_on_set_data_command_event (
         master, FSOEFRAME_COMMAND_ProcessData);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Set reset request flag
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_set_reset_request_flag (fsoemaster_t * master)
{
   fsoemaster_status_t status;

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to reset connection, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to reset connection, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      master->is_reset_requested = true;
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Synchronise with slave
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_sync_with_slave (
   fsoemaster_t * master,
   const void * outputs,
   void * inputs,
   fsoemaster_syncstatus_t * sync_status)
{
   fsoemaster_status_t status;

   if (sync_status != NULL)
   {
      fsoeport_memset (sync_status, 0, sizeof (*sync_status));
   }

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to sync with slave, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (master->magic != FSOEMASTER_MAGIC)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to sync with slave, but instance had not been initialised\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_UNINITIALISED_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if ((outputs == NULL) || (inputs == NULL) || (sync_status == NULL))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to sync with slave, but passed a NULL pointer\n");
      fsoeapp_handle_user_error (master->app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
      if (inputs != NULL)
      {
         fsoeport_memset (inputs, 0, master->inputs_size);
      }
   }
   else
   {
      /* Clear connection reset event */
      master->sync_status.reset_event = FSOEMASTER_RESETEVENT_NONE;
      master->sync_status.reset_reason = FSOEMASTER_RESETREASON_LOCAL_RESET;

      /* Clear frame received and sent events */
      master->sync_status.received_frame_type = FSOEMASTER_FRAMETYPE_NONE;
      master->sync_status.sent_frame_type = FSOEMASTER_FRAMETYPE_NONE;

      /* Check and act on events */
      if (master->is_reset_requested)
      {
         fsoemaster_act_on_reset_connection_event (master);
      }
      else if (fsoewatchdog_time_until_timeout_ms (&master->watchdog) == 0)
      {
         fsoemaster_act_on_watchdog_expired_event (master);
      }
      else
      {
         const fsoeframe_t * received_frame;

         received_frame = fsoechannel_receive_frame (&master->channel);
         if (received_frame)
         {
            fsoemaster_act_on_frame_received_event (
               master, received_frame, outputs);
         }
         else
         {
            /* No event */
         }
      }

      /* Copy inputs from state machine */
      if (master->sync_status.is_process_data_received)
      {
         fsoeport_memcpy (inputs, master->SafeInputs, master->inputs_size);
      }
      else
      {
         fsoeport_memset (inputs, 0, master->inputs_size);
      }
      *sync_status = master->sync_status;

      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}

/* \brief Initialise instance
 *
 * See public header file for description
 */
fsoemaster_status_t fsoemaster_init (
   fsoemaster_t * master,
   const fsoemaster_cfg_t * cfg,
   void * app_ref)
{
   fsoemaster_status_t status;

   if (master == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to initialise instance, but instance was NULL\n");
      fsoeapp_handle_user_error (NULL, FSOEAPP_USERERROR_NULL_INSTANCE);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (cfg == NULL)
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to initialise instance, but configuration was NULL\n");
      fsoeapp_handle_user_error (app_ref, FSOEAPP_USERERROR_NULL_ARGUMENT);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else if (!fsoemaster_is_configuration_valid (cfg))
   {
      FSOELOG_ERROR (FSOE_LOG_MASTER,
         "User tried to initialise instance, but configuration was invalid\n");
      fsoeapp_handle_user_error (app_ref, FSOEAPP_USERERROR_BAD_CONFIGURATION);
      status = FSOEMASTER_STATUS_ERROR;
   }
   else
   {
      fsoemaster_initialise (master, cfg, app_ref);
      status = FSOEMASTER_STATUS_OK;
   }

   return status;
}
