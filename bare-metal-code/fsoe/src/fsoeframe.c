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
 * \brief FSoE PDU frame object implementation
 *
 * See public header file for description.
 * All comments below are implementation details only.
 *
 * See ETG.5100 ch. 8.1.1 "Safety PDU structure".
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

#include "fsoeframe.h"
#include "fsoeport.h"
#include "fsoelog.h"
#include "fsoeutil.h"
#include <string.h>

/** \brief Minimum size of a frame (FSoE PDU) in bytes */
#define FSOEFRAME_MIN_SIZE     6

/** \brief Sentinel byte used for detecting buffer overflow */
#define FSOEFRAME_SENTINEL 0xCC

/* \brief Constants used by CalcCrc().
 *
 * See ETG.5100 Annex A.1 "Hash function calculation".
 */
#define OFFS_COMMAND 0  /**< Offset to command byte in frame */
#define OFFS_DATA 1     /**< Offset to first data byte in frame */
#define OFFS_CRC_HI 1   /**< Offset to high byte in little endian CRC word */
#define OFFS_CRC_LO 0   /**< Offset to low byte in little endian CRC word */
#ifndef FSOEPORT_IS_BIG_ENDIAN
#error "Endianess not specified in fsoeport_macros.h"
#elif (FSOEPORT_IS_BIG_ENDIAN == 1)
#define HI_BYTE 0       /**< Offset to high byte in 16-bit word */
#define LO_BYTE 1       /**< Offset to low byte in 16-bit word */
#elif (FSOEPORT_IS_BIG_ENDIAN == 0)
#define HI_BYTE 1       /**< Offset to high byte in 16-bit word */
#define LO_BYTE 0       /**< Offset to low byte in 16-bit word */
#else
#error "Invalid endianess specified in fsoeport_macros.h"
#endif

/** \brief PDU used by CalcCrc().
 *
 * See ETG.5100 Annex A.1 "Hash function calculation".
 */
typedef struct fsoeframe SAFETY_PDU;

/** \brief CRC table used by CalcCrc().
 *
 * See ETG.5100 Annex A.1 "Hash function calculation".
 */
static const uint16_t aCrcTab1[256] =
{
   0x0000, 0x39B7, 0x736E, 0x4AD9, 0xE6DC, 0xDF6B, 0x95B2, 0xAC05, 0xF40F, 0xCDB8,
   0x8761, 0xBED6, 0x12D3, 0x2B64, 0x61BD, 0x580A, 0xD1A9, 0xE81E, 0xA2C7, 0x9B70,
   0x3775, 0x0EC2, 0x441B, 0x7DAC, 0x25A6, 0x1C11, 0x56C8, 0x6F7F, 0xC37A, 0xFACD,
   0xB014, 0x89A3, 0x9AE5, 0xA352, 0xE98B, 0xD03C, 0x7C39, 0x458E, 0x0F57, 0x36E0,
   0x6EEA, 0x575D, 0x1D84, 0x2433, 0x8836, 0xB181, 0xFB58, 0xC2EF, 0x4B4C, 0x72FB,
   0x3822, 0x0195, 0xAD90, 0x9427, 0xDEFE, 0xE749, 0xBF43, 0x86F4, 0xCC2D, 0xF59A,
   0x599F, 0x6028, 0x2AF1, 0x1346, 0x0C7D, 0x35CA, 0x7F13, 0x46A4, 0xEAA1, 0xD316,
   0x99CF, 0xA078, 0xF872, 0xC1C5, 0x8B1C, 0xB2AB, 0x1EAE, 0x2719, 0x6DC0, 0x5477,
   0xDDD4, 0xE463, 0xAEBA, 0x970D, 0x3B08, 0x02BF, 0x4866, 0x71D1, 0x29DB, 0x106C,
   0x5AB5, 0x6302, 0xCF07, 0xF6B0, 0xBC69, 0x85DE, 0x9698, 0xAF2F, 0xE5F6, 0xDC41,
   0x7044, 0x49F3, 0x032A, 0x3A9D, 0x6297, 0x5B20, 0x11F9, 0x284E, 0x844B, 0xBDFC,
   0xF725, 0xCE92, 0x4731, 0x7E86, 0x345F, 0x0DE8, 0xA1ED, 0x985A, 0xD283, 0xEB34,
   0xB33E, 0x8A89, 0xC050, 0xF9E7, 0x55E2, 0x6C55, 0x268C, 0x1F3B, 0x18FA, 0x214D,
   0x6B94, 0x5223, 0xFE26, 0xC791, 0x8D48, 0xB4FF, 0xECF5, 0xD542, 0x9F9B, 0xA62C,
   0x0A29, 0x339E, 0x7947, 0x40F0, 0xC953, 0xF0E4, 0xBA3D, 0x838A, 0x2F8F, 0x1638,
   0x5CE1, 0x6556, 0x3D5C, 0x04EB, 0x4E32, 0x7785, 0xDB80, 0xE237, 0xA8EE, 0x9159,
   0x821F, 0xBBA8, 0xF171, 0xC8C6, 0x64C3, 0x5D74, 0x17AD, 0x2E1A, 0x7610, 0x4FA7,
   0x057E, 0x3CC9, 0x90CC, 0xA97B, 0xE3A2, 0xDA15, 0x53B6, 0x6A01, 0x20D8, 0x196F,
   0xB56A, 0x8CDD, 0xC604, 0xFFB3, 0xA7B9, 0x9E0E, 0xD4D7, 0xED60, 0x4165, 0x78D2,
   0x320B, 0x0BBC, 0x1487, 0x2D30, 0x67E9, 0x5E5E, 0xF25B, 0xCBEC, 0x8135, 0xB882,
   0xE088, 0xD93F, 0x93E6, 0xAA51, 0x0654, 0x3FE3, 0x753A, 0x4C8D, 0xC52E, 0xFC99,
   0xB640, 0x8FF7, 0x23F2, 0x1A45, 0x509C, 0x692B, 0x3121, 0x0896, 0x424F, 0x7BF8,
   0xD7FD, 0xEE4A, 0xA493, 0x9D24, 0x8E62, 0xB7D5, 0xFD0C, 0xC4BB, 0x68BE, 0x5109,
   0x1BD0, 0x2267, 0x7A6D, 0x43DA, 0x0903, 0x30B4, 0x9CB1, 0xA506, 0xEFDF, 0xD668,
   0x5FCB, 0x667C, 0x2CA5, 0x1512, 0xB917, 0x80A0, 0xCA79, 0xF3CE, 0xABC4, 0x9273,
   0xD8AA, 0xE11D, 0x4D18, 0x74AF, 0x3E76, 0x07C1
};

/** \brief CRC table used by CalcCrc().
 *
 * See ETG.5100 Annex A.1 "Hash function calculation".
 */
static const uint16_t  aCrcTab2[256] =
{
   0x0000, 0x7648, 0xEC90, 0x9AD8, 0xE097, 0x96DF, 0x0C07, 0x7A4F, 0xF899, 0x8ED1,
   0x1409, 0x6241, 0x180E, 0x6E46, 0xF49E, 0x82D6, 0xC885, 0xBECD, 0x2415, 0x525D,
   0x2812, 0x5E5A, 0xC482, 0xB2CA, 0x301C, 0x4654, 0xDC8C, 0xAAC4, 0xD08B, 0xA6C3,
   0x3C1B, 0x4A53, 0xA8BD, 0xDEF5, 0x442D, 0x3265, 0x482A, 0x3E62, 0xA4BA, 0xD2F2,
   0x5024, 0x266C, 0xBCB4, 0xCAFC, 0xB0B3, 0xC6FB, 0x5C23, 0x2A6B, 0x6038, 0x1670,
   0x8CA8, 0xFAE0, 0x80AF, 0xF6E7, 0x6C3F, 0x1A77, 0x98A1, 0xEEE9, 0x7431, 0x0279,
   0x7836, 0x0E7E, 0x94A6, 0xE2EE, 0x68CD, 0x1E85, 0x845D, 0xF215, 0x885A, 0xFE12,
   0x64CA, 0x1282, 0x9054, 0xE61C, 0x7CC4, 0x0A8C, 0x70C3, 0x068B, 0x9C53, 0xEA1B,
   0xA048, 0xD600, 0x4CD8, 0x3A90, 0x40DF, 0x3697, 0xAC4F, 0xDA07, 0x58D1, 0x2E99,
   0xB441, 0xC209, 0xB846, 0xCE0E, 0x54D6, 0x229E, 0xC070, 0xB638, 0x2CE0, 0x5AA8,
   0x20E7, 0x56AF, 0xCC77, 0xBA3F, 0x38E9, 0x4EA1, 0xD479, 0xA231, 0xD87E, 0xAE36,
   0x34EE, 0x42A6, 0x08F5, 0x7EBD, 0xE465, 0x922D, 0xE862, 0x9E2A, 0x04F2, 0x72BA,
   0xF06C, 0x8624, 0x1CFC, 0x6AB4, 0x10FB, 0x66B3, 0xFC6B, 0x8A23, 0xD19A, 0xA7D2,
   0x3D0A, 0x4B42, 0x310D, 0x4745, 0xDD9D, 0xABD5, 0x2903, 0x5F4B, 0xC593, 0xB3DB,
   0xC994, 0xBFDC, 0x2504, 0x534C, 0x191F, 0x6F57, 0xF58F, 0x83C7, 0xF988, 0x8FC0,
   0x1518, 0x6350, 0xE186, 0x97CE, 0x0D16, 0x7B5E, 0x0111, 0x7759, 0xED81, 0x9BC9,
   0x7927, 0x0F6F, 0x95B7, 0xE3FF, 0x99B0, 0xEFF8, 0x7520, 0x0368, 0x81BE, 0xF7F6,
   0x6D2E, 0x1B66, 0x6129, 0x1761, 0x8DB9, 0xFBF1, 0xB1A2, 0xC7EA, 0x5D32, 0x2B7A,
   0x5135, 0x277D, 0xBDA5, 0xCBED, 0x493B, 0x3F73, 0xA5AB, 0xD3E3, 0xA9AC, 0xDFE4,
   0x453C, 0x3374, 0xB957, 0xCF1F, 0x55C7, 0x238F, 0x59C0, 0x2F88, 0xB550, 0xC318,
   0x41CE, 0x3786, 0xAD5E, 0xDB16, 0xA159, 0xD711, 0x4DC9, 0x3B81, 0x71D2, 0x079A,
   0x9D42, 0xEB0A, 0x9145, 0xE70D, 0x7DD5, 0x0B9D, 0x894B, 0xFF03, 0x65DB, 0x1393,
   0x69DC, 0x1F94, 0x854C, 0xF304, 0x11EA, 0x67A2, 0xFD7A, 0x8B32, 0xF17D, 0x8735,
   0x1DED, 0x6BA5, 0xE973, 0x9F3B, 0x05E3, 0x73AB, 0x09E4, 0x7FAC, 0xE574, 0x933C,
   0xD96F, 0xAF27, 0x35FF, 0x43B7, 0x39F8, 0x4FB0, 0xD568, 0xA320, 0x21F6, 0x57BE,
   0xCD66, 0xBB2E, 0xC161, 0xB729, 0x2DF1, 0x5BB9
};

#ifdef FSOE_LOG_ENABLE
/** \brief Get description of boolean value as a string
 *
 * \param[in]     boolean        Boolean value (true or false)
 * \return                       String constant "TRUE" or "FALSE"
 */
static const char * fsoeframe_boolean_as_string (bool boolean)
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

/**
 * \brief CalcCrc()
 *
 * This function does two different things depending on
 * the value of \a bRcvDir:
 *
 * - If this is a received frame (\a bRcvDir == true) this will verify
 *   that all contained CRCs in the frame are correct and update
 *   the sequence number. The frame will not be modified.
 *
 * - If this is a frame to be sent (\a bRcvDir == false) this will
 *   calculate all CRC words and place them at the correct places in
 *   the frame. The sequence number will also be updated.
 *
 * Note that this function is copied from the specification.
 * It has not be refactored for readability as that would make
 * it different from the code described in the specification.
 * See ETG.5100 Annex A.1 "Hash function calculation"
 *
 * Parameters and return values when filling in CRCs in frame to SEND:
 * \param[in,out] psPacket [in]:
 *                            A frame containing command, data and
 *                            connection ID.
 *                            Note that its type is a typedef of fsoeframe_t.
 *                         [out]:
 *                            Same frame, with CRC words inserted at the
 *                            correct places.
 * \param[in]     startCrc CRC_0 in last received frame, little endian encoded.
 * \param[in,out] seqNo    [in]:
 *                            Sequence number to be used in calculated CRCs,
 *                            unless \a bNew is true and the
 *                            generated CRC_0 is the same as the
 *                            previously sent one, in which case
 *                            it will be first incremented.
 *                         [out]:
 *                            Expected sequence number for next sent
 *                            frame. This is 1 if used sequence number for
 *                            this frame was 65535, otherwise it is its
 *                            incremented value.
 * \param[in]     oldCrc   CRC_0 in the last sent frame
 * \param[in]     bRcvDir  false
 * \param[in]     size     Size in bytes of the frame. Max 255 bytes.
 *                         Min 6 bytes. If larger than 6, it needs to
 *                         3 + 4*n bytes, where n is an integer.
 * \param[in]     bNew     true:
 *                            The calculated CRC_0 should be different from
 *                            CRC_0 of previously sent frame.
 *                            This is accomplished by incrementing
 *                            the used sequence number.
 *                         false:
 *                            The calculated CRC_0 may be the same as
 *                            CRC_0 of previously sent frame.
 * \return                 Always false.
 *
 * Parameters and return value when checking CRCs of RECEIVED frame:
 * \param[in]     psPacket A complete read-only frame containing CRC words
 *                         to be verified.
 *                         Note that its type is a typedef of fsoeframe_t.
 * \param[in]     startCrc CRC_0 in last sent frame, little endian encoded.
 * \param[in,out] seqNo    [in]:
 *                            Expected sequence number,
 *                            unless \a bNew is true and the
 *                            calculated CRC_0 is the same as the
 *                            previously received one, in which case
 *                            it will be first incremented.
 *                         [out]:
 *                            Expected sequence number for next received
 *                            frame. This is 1 if used sequence number for
 *                            this frame was 65535, otherwise it is its
 *                            incremented value.
 * \param[in]     oldCrc   CRC_0 in the last received frame.
 * \param[in]     bRcvDir  true
 * \param[in]     size     Size in bytes of the frame. Max 255 bytes.
 *                         Min 6 bytes. If larger than 6, it needs to
 *                         3 + 4*n bytes, where n is an integer.
 * \param[in]     bNew     true:
 *                            The calculated CRC_0 should be different from
 *                            CRC_0 of previously received frame.
 *                            This is accomplished by incrementing
 *                            the used sequence number.
 *                         false:
 *                            The CRC_0 of the frame may be the same as
 *                            CRC_0 of previously received frame.
 * \return                 true:
 *                            All CRCs are correct.
 *                         false:
 *                            At least one CRC was incorrect.
 */
static uint8_t fsoeframe_CalcCrc (
   SAFETY_PDU * const psPacket,  // Safety-over-EtherCat Safety PDU
   const uint16_t startCrc,      // Startvalue of CRC Calculation
   uint16_t * const seqNo,       // SeqNo
   const uint16_t oldCrc,        // CRC_0 of the last received/sent Safety Slave PDU
   const uint8_t bRcvDir,        // bRcvDir = true: calc of CRC of the received frame
                                 // bRcvDir = false: calc of the CRCs for the send-frame
   uint8_t size,
   const uint8_t bNew)
{
   uint8_t bSuccess = false;
   uint16_t w1, w2;              // Temporary values
   uint16_t crc;

   uint16_t crc_common;          // Common part of the CRC calculation, includes CRC_0,
                                 // Conn-ID, Sequence-No, Cmd
   uint8_t *pCrc = &psPacket->au8Data[2]; // Pointer to the CRC Low-Byte
   const uint8_t *pSafeData;     // Pointer to SafeData Low-Byte

   // 2 or multiple of 2 safety data?
   if (size > 6)
   {
      // Crc0 LowByte at byte offset 3 instead of 2
      pCrc++;
   }

   /* Calculate CRC_0 and update sequence number */
   do
   {
      // Reset CRC
      crc = 0;

      // Sequence for calculation:
      // old CRC-Lo, old CRC-Hi, ConnId-Lo, ConnId-Hi, SeqNo-Lo, SeqNo-Hi,
      // Command, (Index,) Data

      // CRC-Lo
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];  // look at the CRC-table
      w2 = aCrcTab2[((uint8_t *) &startCrc)[0]];   // look at the CRC-table
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // CRC-Hi
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];  // look at the CRC-table
      w2 = aCrcTab2[((uint8_t *) &startCrc)[1]];   // look at the CRC-table
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // ConnId-Lo
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
      w2 = aCrcTab2[psPacket->au8Data[size - 2]];
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // ConnId-Hi
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
      w2 = aCrcTab2[psPacket->au8Data[size - 1]];
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // SeqNo-Lo
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
      w2 = aCrcTab2[((uint8_t *) seqNo)[LO_BYTE]];
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // SeqNo-Hi
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
      w2 = aCrcTab2[((uint8_t *) seqNo)[HI_BYTE]];
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // Command
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
      w2 = aCrcTab2[psPacket->au8Data[OFFS_COMMAND]];
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // CRC part that is common for all other crc-calculations is saved
      crc_common = crc;

      // Data [0]
      w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
      w2 = aCrcTab2[psPacket->au8Data[OFFS_DATA]];
      w1 = w1 ^ w2;
      ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
      ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

      // if 2 byte safety data -> calculate next byte into the CRC
      if (size > 6)
      {
         // Data [1]
         w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
         w2 = aCrcTab2[psPacket->au8Data[OFFS_DATA + 1]];
         w1 = w1 ^ w2;
         ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
         ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];
      }

      // Update sequence number
      seqNo[0]++;
      if (seqNo[0] == 0)
      {
         seqNo[0]++;
      }

   } while (crc == oldCrc && bNew);

   // receive direction?
   if (bRcvDir)
   {
      if (((uint8_t *) &crc)[HI_BYTE] == pCrc[OFFS_CRC_HI - OFFS_CRC_LO]
         && ((uint8_t *) &crc)[LO_BYTE] == pCrc[0])
      {
         // CRC is correct
         bSuccess = true;
      }
   }
   else // for send direction
   {
      // insert checksum
      pCrc[OFFS_CRC_HI - OFFS_CRC_LO] = ((uint8_t *) &crc)[HI_BYTE];
      pCrc[0] = ((uint8_t *) &crc)[LO_BYTE];
   }

   // if more than 2 byte safety data are transferred,
   // CRC_1 and so forth must be calculated
   if (size > 10)
   {
      uint16_t i = 1;
      pSafeData = pCrc + 2;   //set pSafeData to SafeData Low-byte
                              // of the next part = safeData[2]
      pCrc += 4;              // set pCrc to CRC_i Low-byte
      size -= 7;              // substract first part of the frame

                              // As long as other parts follow
      while (size >= 4)
      {
         // Start-CRC
         crc = crc_common; // this part is already calculated above

         // i (bit 0 - 7)
         w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
         w2 = aCrcTab2[((uint8_t *) &i)[LO_BYTE]];
         w1 = w1 ^ w2;
         ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
         ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

         // i (bit 8 - 15)
         w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
         w2 = aCrcTab2[((uint8_t *) &i)[HI_BYTE]];
         w1 = w1 ^ w2;
         ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
         ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

         // Data 2 * i
         w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
         w2 = aCrcTab2[pSafeData[0]];
         w1 = w1 ^ w2;
         ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
         ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

         // Data 2 * i + 1
         w1 = aCrcTab1[((uint8_t *) &crc)[HI_BYTE]];
         w2 = aCrcTab2[pSafeData[1]];
         w1 = w1 ^ w2;
         ((uint8_t *) &crc)[HI_BYTE] = ((uint8_t *) &w1)[HI_BYTE] ^ ((uint8_t *) &crc)[LO_BYTE];
         ((uint8_t *) &crc)[LO_BYTE] = ((uint8_t *) &w1)[LO_BYTE];

         if (((uint8_t *) &crc)[HI_BYTE] == pCrc[1]
            && ((uint8_t *) &crc)[LO_BYTE] == pCrc[0])
         {
            // CRC is correct
         }
         else
         {
            bSuccess = false;
            if (bRcvDir == 0) // for send direction
            {
               pCrc[1] = ((uint8_t *) &crc)[HI_BYTE];
               pCrc[0] = ((uint8_t *) &crc)[LO_BYTE];
            }
         }

         size      -= 4;   // Substract this part of the frame
         pSafeData += 4;   // Set to next SafeData Low-byte
         pCrc      += 4;   // Set to next CRC_i Low-byte
         i++;
      }
   }

   return bSuccess;
}

/******************************************************************************
 *
 * Public API
 *
 * All non-static functions are defined here and documented in the public
 * header file.
 *
 *****************************************************************************/

/* \brief Get command field in frame
 *
 * See public header file for description
 */
uint8_t fsoeframe_command (const fsoeframe_t * frame)
{
   return frame->au8Data[0];
}

/* \brief Set command field in frame
 *
 * See public header file for description
 */
void fsoeframe_set_command (fsoeframe_t * frame, uint8_t command)
{
   frame->au8Data[0] = command;
}

/* \brief Get error code field in Reset frame
 *
 * See public header file for description
 */
uint8_t fsoeframe_error_code (const fsoeframe_t * frame)
{
   FSOEPORT_ASSERT (frame->au8Data[0] == (uint8_t)FSOEFRAME_COMMAND_Reset);

   return frame->au8Data[1];
}

/* \brief Get CRC_0 field in frame
 *
 * See public header file for description
 */
uint16_t fsoeframe_crc0 (const fsoeframe_t * frame)
{
   uint16_t crc0;

   /* See ETG.5100 ch. 8.1.1 "Safety PDU structure" table 3 and 4 */
   if (frame->size == FSOEFRAME_MIN_SIZE)
   {
      /* See table 4 */
      crc0 =
         ((uint16_t) frame->au8Data[2] << 0) +
         ((uint16_t) frame->au8Data[3] << 8);
   }
   else
   {
      /* See table 3 */
      crc0 =
         ((uint16_t) frame->au8Data[3] << 0) +
         ((uint16_t) frame->au8Data[4] << 8);
   }
   return crc0;
}

/* \brief Get Connection ID field in frame
 *
 * See public header file for description
 */
uint16_t fsoeframe_connection_id (const fsoeframe_t * frame)
{
   uint16_t connection_id =
      ((uint16_t)frame->au8Data[frame->size - 2] << 0) +
      ((uint16_t)frame->au8Data[frame->size - 1] << 8);
   return connection_id;
}

/* \brief Set Connection ID field in frame
 *
 * See public header file for description
 */
void fsoeframe_set_connection_id (
   fsoeframe_t * frame,
   uint16_t connection_id)
{
   frame->au8Data[frame->size - 2] = (connection_id >> 0) & 0xFFu;
   frame->au8Data[frame->size - 1] = (connection_id >> 8) & 0xFFu;
}

/* \brief Get data in frame
 *
 * See public header file for description
 */
void fsoeframe_get_data (
   const fsoeframe_t * frame,
   void * data,
   size_t data_size)
{
   uint8_t * typed_data = (uint8_t *) data;

   FSOEPORT_ASSERT (data_size > 0);
   FSOEPORT_ASSERT (frame->size >= FSOEFRAME_MIN_SIZE);

   if (frame->size == FSOEFRAME_MIN_SIZE)
   {
      /* Short PDU:
       * [0]: Command
       * [1]: Data[0]
       * [2]: CRC_0.Lo
       * [3]: CRC_0.Hi
       * See ETG.5100 ch. 8.1.1 table 4
       */
      typed_data[0] = frame->au8Data[1];
   }
   else
   {
      size_t i;
      const size_t frame_data_size = (frame->size - 3) / 2;
      const size_t used_data_size = FSOEUTIL_MIN (data_size, frame_data_size);

      /* General PDU:
       * [0]: Command
       * [1]: Data[0]
       * [2]: Data[1]
       * [3]: CRC_0.Lo
       * [4]: CRC_0.Hi
       * [5]: Data[2]
       * [6]: Data[3]
       * [7]: CRC_1.Lo
       * [8]: CRC_1.Hi
       * ...
       * See ETG.5100 ch. 8.1.1 table 3
       */
      FSOEPORT_ASSERT (frame_data_size % 2 == 0);
      for (i = 0; i < used_data_size; i += 2)
      {
         typed_data[i + 0] = frame->au8Data[2 * i + 1];
         if (i + 1 < used_data_size)
         {
            typed_data[i + 1] = frame->au8Data[2 * i + 2];
         }
      }
   }
}

/* \brief Set data in frame
 *
 * See public header file for description
 */
void fsoeframe_set_data (
   fsoeframe_t * frame,
   const void * data,
   size_t data_size)
{
   const uint8_t * typed_data = (const uint8_t *) data;

   FSOEPORT_ASSERT (data_size > 0);
   FSOEPORT_ASSERT (frame->size >= FSOEFRAME_MIN_SIZE);

   if (frame->size == FSOEFRAME_MIN_SIZE)
   {
      /* Short PDU:
       * [0]: Command
       * [1]: Data[0]
       * [2]: CRC_0.Lo
       * [3]: CRC_0.Hi
       * See ETG.5100 ch. 8.1.1 table 4
       */
      frame->au8Data[1] = typed_data[0];
   }
   else
   {
      size_t i;
      const size_t frame_data_size = (frame->size - 3) / 2;
      const size_t used_data_size = FSOEUTIL_MIN (data_size, frame_data_size);

      /* General PDU:
       * [0]: Command
       * [1]: Data[0]
       * [2]: Data[1]
       * [3]: CRC_0.Lo
       * [4]: CRC_0.Hi
       * [5]: Data[2]
       * [6]: Data[3]
       * [7]: CRC_1.Lo
       * [8]: CRC_1.Hi
       * ...
       * See ETG.5100 ch. 8.1.1 table 3
       */
      FSOEPORT_ASSERT (frame_data_size % 2 == 0);
      for (i = 0; i < used_data_size; i += 2)
      {
         frame->au8Data[2 * i + 1] = typed_data[i];
         if (i + 1 < used_data_size)
         {
            frame->au8Data[2 * i + 2] = typed_data[i + 1];
         }
      }
   }
}

/* \brief Get description of error as a string
 *
 * See public header file for description
 */
const char * fsoeframe_error_description (uint8_t error)
{
   const char * error_string;

   switch (error)
   {
   case FSOEFRAME_ERROR_LOCAL_RESET_OR_RESET_REPLY:
      error_string = "local reset";
      break;
   case FSOEFRAME_ERROR_INVALID_CMD:
      error_string = "INVALID_CMD";
      break;
   case FSOEFRAME_ERROR_UNKNOWN_CMD:
      error_string = "UNKNOWN_CMD";
      break;
   case FSOEFRAME_ERROR_INVALID_CONNID:
      error_string = "INVALID_CONNID";
      break;
   case FSOEFRAME_ERROR_INVALID_CRC:
      error_string = "INVALID_CRC";
      break;
   case FSOEFRAME_ERROR_WD_EXPIRED:
      error_string = "WD_EXPIRED";
      break;
   case FSOEFRAME_ERROR_INVALID_ADDRESS:
      error_string = "INVALID_ADDRESS";
      break;
   case FSOEFRAME_ERROR_INVALID_DATA:
      error_string = "INVALID_DATA";
      break;
   case FSOEFRAME_ERROR_INVALID_COMPARALEN:
      error_string = "INVALID_COMPARALEN";
      break;
   case FSOEFRAME_ERROR_INVALID_COMPARA:
      error_string = "INVALID_COMPARA";
      break;
   case FSOEFRAME_ERROR_INVALID_USERPARALEN:
      error_string = "INVALID_USERPARALEN";
      break;
   case FSOEFRAME_ERROR_INVALID_USERPARA:
      error_string = "INVALID_USERPARA";
      break;
   default:
      if (error >= 0x80)
      {
         error_string = "device-specific";
      }
      else
      {
         error_string = "invalid error code";
      }
      break;
   }

   return error_string;
}

/* \brief Get name of command as a string
 *
 * See public header file for description
 */
const char * fsoeframe_command_name (uint8_t command)
{
   const char * name;

   switch (command)
   {
   case FSOEFRAME_COMMAND_ProcessData:
      name = "ProcessData";
      break;
   case FSOEFRAME_COMMAND_Reset:
      name = "Reset";
      break;
   case FSOEFRAME_COMMAND_Session:
      name = "Session";
      break;
   case FSOEFRAME_COMMAND_Connection:
      name = "Connection";
      break;
   case FSOEFRAME_COMMAND_Parameter:
      name = "Parameter";
      break;
   case FSOEFRAME_COMMAND_FailSafeData:
      name = "FailSafeData";
      break;
   default:
      name = "unknown";
      break;
   }

   return name;
}

/* \brief Check if two frame are identical
 *
 * See public header file for description
 */
bool fsoeframe_equals (const fsoeframe_t * frame, const fsoeframe_t * other)
{
   FSOEPORT_ASSERT (frame->size == other->size);

   return fsoeport_memcmp (frame->au8Data, other->au8Data, frame->size) == 0;
}

/* \brief Get pointer to raw frame buffer
 *
 * See public header file for description
 */
uint8_t * fsoeframe_buffer (fsoeframe_t * frame)
{
   return frame->au8Data;
}

/* \brief Get pointer to raw frame buffer (read-only)
 *
 * See public header file for description
 */
const uint8_t * fsoeframe_readonly_buffer (const fsoeframe_t * frame)
{
   return frame->au8Data;
}

/* \brief Get size of frame in bytes
 *
 * See public header file for description
 */
size_t fsoeframe_size (const fsoeframe_t * frame)
{
   return frame->size;
}

/* \brief Check if a buffer overflow condition has been detected
 *
 * See public header file for description
 */
bool fsoeframe_is_overflowed (const fsoeframe_t * frame)
{
   return frame->au8Data[frame->size] != FSOEFRAME_SENTINEL;
}

/* \brief Check if data contained in received frame is identical with corresponding
 * part of sent data
 *
 * See public header file for description
 */
bool fsoeframe_IS_SAFEDATA_CORRECT (
   const fsoeframe_t * frame,
   size_t max_data_size,
   const void * expectedData,
   size_t bytesSent)
{
   size_t data_size_in_frame;
   const uint8_t * sent_data;
   size_t index_in_frame;
   size_t i;
   bool is_correct;

   FSOEPORT_ASSERT (bytesSent > 0);
   FSOEPORT_ASSERT (max_data_size > 0);
   FSOEPORT_ASSERT (frame->size >= FSOEFRAME_MIN_SIZE);

   /* Calculate how much valid data the frame contains.
    * In case the data is larger than what fits in one
    * frame, multiple frames are needed to send the data.
    * Any previous frame will contain the maximum amount of
    * data that fits in one frame, while this last frame
    * may contain less.
    */
   data_size_in_frame = bytesSent % max_data_size;
   if (data_size_in_frame == 0)
   {
      /* Frame contains max size data */
      data_size_in_frame = max_data_size;
   }

   /* A minimum size frame may only contain a single data byte.
    * See ETG.5100 ch. 8.1.1.
    */
   if (frame->size == FSOEFRAME_MIN_SIZE)
   {
      FSOEPORT_ASSERT (data_size_in_frame == 1);
   }

   /* Calculate current position of sent data */
   sent_data = expectedData;
   sent_data = &sent_data[bytesSent - data_size_in_frame];

   /* Assume data to be correct unless proven otherwise */
   is_correct = true;

   /* Iterate over each data byte in frame */
   index_in_frame = 1;
   for (i = 0; i < data_size_in_frame; i++)
   {
      /* The last data in a frame is followed by 2 CRC bytes
       * and 2 Connection ID bytes.
       * See ETG.5100 ch. 8.1.1. table 3 and 4.
       */
      FSOEPORT_ASSERT (index_in_frame < frame->size - 4);

      if (sent_data[i] != frame->au8Data[index_in_frame])
      {
         /* Found unexpected byte value */
         is_correct = false;
         break;
      }
      index_in_frame++;
      if (index_in_frame % 2 == 1)
      {
         /* Skip over 16-bit CRC */
         index_in_frame += 2;
      }
   }

   return is_correct;
}

/* \brief Check if all CRC words in received frame are correct
 *
 * See public header file for description
 */
bool fsoeframe_IS_CRC_CORRECT (
   const fsoeframe_t * frame,
   uint16_t lastCrc,
   uint16_t * seqNo,
   uint16_t * oldCrc,
   bool bNew)
{
   uint8_t is_correct;

   FSOELOG_DEBUG (FSOE_LOG_FRAME,
      "IS_CRC_CORRECT(lastCrc=0x%04"PRIx16", seqNo=%"PRIu16
      ", oldCrc=0x%04"PRIx16", bNew=%s)\n",
      lastCrc,
      *seqNo,
      *oldCrc,
      fsoeframe_boolean_as_string (bNew));

   /* Check all CRC words in frame and get expected sequence number
   * for next received frame.
   *
   * Note that const-qualification is removed. This is actually safe
   * as CalcCrc() will not modify the frame buffer when bRcvDir is set
   * to true (received frame is to be checked).
   */
   is_correct = fsoeframe_CalcCrc (
      (SAFETY_PDU *)frame, /*lint !e9005 */
      fsoeutil_to_le16 (lastCrc),/* Last sent CRC_0 (little endian) */
      seqNo,               /* Expected sequence number. Will be updated */
      *oldCrc,             /* Last received CRC_0 */
      1,                   /* Is receiving? */
      (uint8_t)frame->size,
      bNew);               /* CRC_0 must be different from last received CRC_0? */

   /* Get CRC_0 in this frame */
   *oldCrc = fsoeframe_crc0 (frame);

   FSOELOG_DEBUG (FSOE_LOG_FRAME,
      "result = %s (new seqNo=%"PRIu16", new OldCrc=0x%04"PRIx16")\n",
      fsoeframe_boolean_as_string (is_correct),
      *seqNo,
      *oldCrc);

   return is_correct;
}

/* \brief Add CRC words to frame to be sent.
 *
 * See public header file for description
 */
void fsoeframe_add_crc_words (
   fsoeframe_t * frame,
   uint16_t lastCrc,
   uint16_t * seqNo,
   uint16_t * oldCrc,
   bool bNew)
{
   /* Write all CRC words in frame and get expected sequence number
    * for next sent frame.
    */
   (void) fsoeframe_CalcCrc (
      frame,
      fsoeutil_to_le16 (lastCrc),/* Last received CRC_0 (little endian) */
      seqNo,               /* Expected sequence number. Will be updated */
      *oldCrc,             /* Last sent CRC_0 */
      0,                   /* Is receiving? */
      (uint8_t)frame->size,
      bNew);               /* CRC_0 must be different from last sent CRC_0? */

   /* Get CRC_0 in this frame */
   *oldCrc = fsoeframe_crc0 (frame);
}

/* \brief Copy contents of frame to another frame
 *
 * See public header file for description
 */
void fsoeframe_copy (const fsoeframe_t * frame, fsoeframe_t * to)
{
   FSOEPORT_ASSERT (frame->size == to->size);

   fsoeport_memcpy (to->au8Data, frame->au8Data, frame->size);
}

/* \brief Initialise frame to zero
 *
 * See public header file for description
 */
void fsoeframe_zeroise (fsoeframe_t * frame)
{
   fsoeport_memset (frame->au8Data, 0, frame->size);
}

/* \brief Initialise frame
 *
 * See public header file for description
 */
void fsoeframe_init (fsoeframe_t * frame, size_t data_size)
{
   size_t size;

   FSOEPORT_ASSERT (frame != NULL);

   size = FSOEFRAME_SIZE (data_size);
   frame->size = size;
   frame->au8Data[size] = FSOEFRAME_SENTINEL;
}
