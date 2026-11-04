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
 * \brief FSoE PDU frame object
 *
 * Implementation of an FSoE frame.
 *
 * Provides the following functionality:
 * - Construct frames, including CRC.
 * - Check the contents of frames, including CRC.
 *
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 */

#ifndef FSOEFRAME_H
#define FSOEFRAME_H

#include <fsoetypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Communication error codes
 *
 * This is the second byte in Reset PDU frames.
 * See ETG.5100 ch. 8.3. table 28: "FSoE communication error codes".
 */
#define FSOEFRAME_ERROR_LOCAL_RESET_OR_RESET_REPLY (0) /**< Local reset or
                                                    * reply to Reset frame
                                                    */
#define FSOEFRAME_ERROR_INVALID_CMD          (1)   /**< Invalid command */
#define FSOEFRAME_ERROR_UNKNOWN_CMD          (2)   /**< Unknown command */
#define FSOEFRAME_ERROR_INVALID_CONNID       (3)   /**< Invalid Connection ID */
#define FSOEFRAME_ERROR_INVALID_CRC          (4)   /**< Invalid CRC */
#define FSOEFRAME_ERROR_WD_EXPIRED           (5)   /**< Watchdog expired */
#define FSOEFRAME_ERROR_INVALID_ADDRESS      (6)   /**< Invalid slave address */
#define FSOEFRAME_ERROR_INVALID_DATA         (7)   /**< Invalid data*/
#define FSOEFRAME_ERROR_INVALID_COMPARALEN   (8)   /**< Invalid size of Communication
                                                    * parameters
                                                    */
#define FSOEFRAME_ERROR_INVALID_COMPARA      (9)   /**< Invalid Communication parameters */
#define FSOEFRAME_ERROR_INVALID_USERPARALEN  (10)  /**< Invalid length of Application
                                                    * parameters
                                                    */
#define FSOEFRAME_ERROR_INVALID_USERPARA     (11)  /**< Invalid Application parameter */

/**
 * \brief PDU command
 *
 * This is the first byte in each PDU frame.
 * See ETG.5100 ch. 8.1.2 table 5: "Safety PDU command".
 */
#define FSOEFRAME_COMMAND_ProcessData  (0x36)  /**< Process data*/
#define FSOEFRAME_COMMAND_Reset        (0x2A)  /**< Reset */
#define FSOEFRAME_COMMAND_Session      (0x4E)  /**< Session */
#define FSOEFRAME_COMMAND_Connection   (0x64)  /**< Connection */
#define FSOEFRAME_COMMAND_Parameter    (0x52)  /**< Parameter */
#define FSOEFRAME_COMMAND_FailSafeData (0x08) /**< Fail-safe data */

/**
 * \brief Get description of error as a string
 *
 * See ETG.5100 ch. 8.3 table 28: "FSoE communication error codes".
 *
 * \param[in]     error          Error code
 * \return                       String constant describing the error
 */
const char * fsoeframe_error_description (uint8_t error);

/**
 * \brief Get error code field in Reset frame
 *
 * The error code is the first data byte of a Reset frame.
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * \pre The frame is of type Reset.
 *
 * \param[in]     frame          Reset frame (FSoE PDU)
 * \return                       Error code field in frame.
 */
uint8_t fsoeframe_error_code (const fsoeframe_t * frame);

/**
 * \brief Get name of command as a string
 *
 * See ETG.5100 ch. 8.1.2 table 5: "Safety PDU command".
 *
 * \param[in]     command        Command (i.e. type of frame)
 * \return                       Command name as constant string if
 *                               \a command is valid, "unknown" otherwise.
 */
const char * fsoeframe_command_name (uint8_t command);

/**
 * \brief Get command field in frame
 *
 * The command (the type of the frame) is the first byte of a frame.
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       Command field in frame,
 *                               i.e. the type of frame
 */
uint8_t fsoeframe_command (const fsoeframe_t * frame);

/**
 * \brief Set command field in frame
 *
 * The command (the type of the frame) is the first byte of a frame.
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * \param[out]     frame         Frame (FSoE PDU)
 * \param[in]      command       Command, i.e. the type of frame
 */
void fsoeframe_set_command (fsoeframe_t * frame, uint8_t command);

/**
 * \brief Get CRC_0 field in frame
 *
 * The CRC_0 is placed after the first data in frame.
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       CRC_0 field in frame
 */
uint16_t fsoeframe_crc0 (const fsoeframe_t * frame);

/**
 * \brief Get Connection ID field in frame
 *
 * The Connection ID field is placed at the end of frame.
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       Connection ID field in frame
 */
uint16_t fsoeframe_connection_id (const fsoeframe_t * frame);

/**
 * \brief Set Connection ID field in frame
 *
 * The Connection ID field is placed at the end of frame.
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * \param[in,out] frame          Frame (FSoE PDU)
 * \param[in]     connection_id  Connection ID
 */
void fsoeframe_set_connection_id (
   fsoeframe_t * frame,
   uint16_t connection_id);

/**
 * \brief Get data in frame
 *
 * Data is not stored as an contiguous array in the frame. Instead, it is
 * split in multiple 16-bit fields (except if its size is 1 or 2 bytes).
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * If requested \a data_size is larger than what is contained in the frame,
 * remaining bytes in \a data buffer will be left untouched.
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \param[out]    data           Buffer to store data
 * \param[in]     data_size      Size in bytes of buffer
 */
void fsoeframe_get_data (
   const fsoeframe_t * frame,
   void * data,
   size_t data_size);

/**
 * \brief Set data in frame
 *
 * Data is not stored as an contiguous array in the frame. Instead, it is
 * split in multiple 16-bit fields with 16-bit CRC fields inbetween (except
 * if its size is 1 or 2 bytes).
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
 *
 * If requested \a data_size is larger than what fits in the frame,
 * excess bytes will not be stored.
 *
 * \param[in,out] frame          Frame (FSoE PDU)
 * \param[in]     data           Buffer containing data
 * \param[in]     data_size      Size in bytes of buffer
 */
void fsoeframe_set_data (
   fsoeframe_t * frame,
   const void * data,
   size_t data_size);

/**
 * \brief Get pointer to raw frame buffer
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       Pointer to frame's internal buffer
 */
uint8_t * fsoeframe_buffer (fsoeframe_t * frame);

/**
 * \brief Get pointer to raw frame buffer (read-only)
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       Pointer to frame's internal buffer
 */
const uint8_t * fsoeframe_readonly_buffer (const fsoeframe_t * frame);

/**
 * \brief Get size of frame in bytes
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       Size of frame in bytes
 */
size_t fsoeframe_size (const fsoeframe_t * frame);

/**
 * \brief Check if a buffer overflow condition has been detected
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \return                       true if buffer overflow is detected,
 *                               false otherwise.
 */
bool fsoeframe_is_overflowed (const fsoeframe_t * frame);

/**
 * \brief Check if two frame are identical
 *
 * \pre The frames have the same size
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \param[in]     other          Other frame (FSoE PDU) to compare against
 * \return                       true if they are equal,
 *                               false otherwise.
 */
bool fsoeframe_equals (const fsoeframe_t * frame, const fsoeframe_t * other);

/**
 * \brief Check if data contained in received frame is identical with corresponding
 * part of sent data
 *
 * See ETG.5100 ch. 8.4.1.4 "Master Macros", table 33.
 *
 * \param[in]     frame          Received frame (FSoE PDU)
 * \param[in]     max_data_size  Max number of data bytes in one frame
 * \param[in]     expectedData   Data sent
 * \param[in]     bytesSent      Total number of data bytes sent
 * \return                       True if received data is the same as
 *                               relevant part of expected data.
 *                               False if any data byte is incorrect.
 */
bool fsoeframe_IS_SAFEDATA_CORRECT (
   const fsoeframe_t * frame,
   size_t max_data_size,
   const void * expectedData,
   size_t bytesSent);

/**
 * \brief Check if all CRC words in received frame are correct
 *
 * Also get the CRC_0 of this frame and the expected sequence
 * number of next received frame.
 *
 * This function is similar to the macro IS_CRC_CORRECT().
 * See ETG.5100 ch. 8.4.1.4 table 33: "Macros in the FSoe Master state table".
 * and ETG.5100 ch. 8.5.1.4 table 38: "Macros in the FSoe Slave state table".
 *
 * \param[in]     frame          Received frame (FSoE PDU).
 * \param[in]     lastCrc        CRC_0 in last sent frame.
 * \param[in,out] seqNo          [in]:
 *                                  Expected sequence number for frame.
 *                                  Actual sequence number may be different
 *                                  in case \a bNew is true.
 *                               [out]:
 *                                  Expected sequence number for next received
 *                                  frame. This will be the actual sequence
 *                                  number used for this frame, incremented
 *                                  (or set to 1 if used value was 65535).
 * \param[in,out] oldCrc         [in]:
 *                                  CRC_0 in last received frame. Only used
 *                                  if \a bNew is true.
 *                               [out]:
 *                                  CRC_0 of frame.
 * \param[in]     bNew           true:
 *                                  The calculated CRC_0 should be different
 *                                  from CRC_0 of previously received frame.
 *                                  This is accomplished by incrementing
 *                                  the used sequence number (or setting to
 *                                  1 if previous value was 65535).
 *                               false:
 *                                  The calculated CRC_0 may be the same as
 *                                  CRC_0 of previously received frame.
 * \return                       true:
 *                                  All CRC words are correct.
 *                               false:
 *                                  At least one CRC word is incorrect.
 */
bool fsoeframe_IS_CRC_CORRECT (
   const fsoeframe_t * frame,
   uint16_t lastCrc,
   uint16_t * seqNo,
   uint16_t * oldCrc,
   bool bNew);

/**
 * \brief Add CRC words to frame to be sent
 *
 * Also get CRC_0 for this frame and the expected sequence number
 * for next sent frame.
 *
 * \param[in,out] frame          [in]:
 *                                  A partially constructed frame (FSoE PDU)
 *                                  containing Command, data and Connection ID.
 *                               [out]:
 *                                  Same frame, with CRC words inserted at the
 *                                  correct places. Ready to be sent.
 * \param[in]     lastCrc        CRC_0 in last received frame.
 * \param[in,out] seqNo          [in]:
 *                                  Sequence number to be used in calculated
 *                                  CRCs, unless \a bNew is true and the
 *                                  generated CRC_0 is the same as the
 *                                  previously sent CRC_0, in which case
 *                                  it will be first incremented (or set to
 *                                  1 if previously equal to 65535).
 *                               [out]:
 *                                  Expected sequence number for next sent
 *                                  frame. This will be the actual sequence
 *                                  number used for this frame, incremented
 *                                  (or set to 1 if used value was 65535).
 * \param[in,out] oldCrc         [in]:
 *                                  CRC_0 in last sent frame. Only used
 *                                  if \a bNew is true.
 *                               [out]:
 *                                  CRC_0 of frame.
 * \param[in]     bNew           true:
 *                                  The calculated CRC_0 should be different
 *                                  from CRC_0 of previously sent frame.
 *                                  This is accomplished by incrementing
 *                                  the used sequence number (or setting to
 *                                  1 if previous value was 65535).
 *                               false:
 *                                  The calculated CRC_0 may be the same as
 *                                  CRC_0 of previously sent frame.
 */
void fsoeframe_add_crc_words (
   fsoeframe_t * frame,
   uint16_t lastCrc,
   uint16_t * seqNo,
   uint16_t * oldCrc,
   bool bNew);

/**
 * \brief Copy contents of frame to other frame
 *
 * \pre The frames have the same size
 *
 * \param[in]     frame          Frame (FSoE PDU)
 * \param[in,out] to             Destination frame buffer
 */
void fsoeframe_copy (const fsoeframe_t * frame, fsoeframe_t * to);

/**
 * \brief Set contents of frame buffer to zero
 *
 * \param[in,out] frame          Frame (FSoE PDU)
 */
void fsoeframe_zeroise (fsoeframe_t * frame);

/**
 * \brief Initialise frame
 *
 * The frame will be initialised with given \a data_size.
 * Triggers assertion error if arguments are invalid.
 *
 * \param[out]    frame          Frame (FSoE PDU)
 * \param[in]     data_size      Size of data payload in bytes
 */
void fsoeframe_init (fsoeframe_t * frame, size_t data_size);

#ifdef __cplusplus
}
#endif

#endif /* FSOEFRAME_H */
