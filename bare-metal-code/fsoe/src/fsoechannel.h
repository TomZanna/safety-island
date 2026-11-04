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
 * \brief FSoE black channel
 *
 * Internal API for sending and receiving FSoE frames.
 */

#ifndef FSOECHANNEL_H
#define FSOECHANNEL_H

#include <fsoetypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Try to receive a frame over black channel
 *
 * If frame is received it will be stored in internal buffer.
 *
 * \param[in,out] channel        Black channel instance. Contains an internal
 *                               buffer for storing received frame.
 * \return                       NULL if nothing was received or if received
 *                               frame were identical with last received one.
 *                               Read-only pointer to received frame otherwise.
 *                               The frame is owned by the channel
 */
const fsoeframe_t * fsoechannel_receive_frame (fsoechannel_t * channel);

/**
 * \brief Construct frame and send it over black channel
 *
 * Arguments used for CRC calculation will be updated.
 *
 * The interface of this function is defined in the standard as "SendFrame",
 * though some non-standard parameters were added. See ETG.5100
 * ch. 8.4.1.3 table 31 "Functions in the FSoE Master state table" and
 * ch. 8.5.1.3 table 36 "Functions in the FSoE Slave state table".
 *
 * \param[in,out] channel        Black channel instance. Non-standard
 * \param[in]     cmd            Command (type of frame) to send
 * \param[in]     safeData       Data to send
 * \param[in]     safeData_size  Size of data to send in bytes. Non-standard.
 * \param[in]     lastCrc        CRC_0 of last received frame
 * \param[in]     connId         Connection ID to send
 * \param[in,out] seqNo          Frame sequence number. Sequence number of next
 *                               sent frame is returned.
 * \param[in,out] oldCrc         CRC_0 of last sent frame. CRC_0 of this frame
 *                               is returned.
 * \param[in]     bNew           if true:
 *                                  Calculated CRC_0 needs to be different from
 *                                  oldCrc (by repeating the CRC calculation
 *                                  with incremented seqNo until calculated
 *                                  CRC_0 is different from oldCrc.
 *                                  See ETG.5100 ch. 8.1.3.4.)
 *                               if false:
 *                                  No effect. Value of oldCrc is ignored
 * \return                       Read-only pointer to sent frame.
 *                               The frame is owned by the channel
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
   bool bNew);

/**
 * \brief Initialise FSoE black channel instance
 *
 * This will configure the channel according to supplied arguments.
 * If supplied arguments are invalid, an assertion error will be
 * triggered and program execution will halt.
 *
 * \param[out]    channel        Black channel instance.
 * \param[in]     received_data_size Size in bytes of payload data in
 *                               received frame.
 * \param[in]     sent_data_size Size in bytes of payload data in
 *                               sent frame.
 * \param[in]     app_ref        Application reference. This will be passed
 *                               as first argument to callback functions
 *                               implemented by application. The stack
 *                               does not interpret this value in any way.
 */
void fsoechannel_init (
   fsoechannel_t * channel,
   size_t received_data_size,
   size_t sent_data_size,
   void * app_ref);

#ifdef __cplusplus
}
#endif

#endif /* FSOECHANNEL_H */
