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
 * \brief Utility functions
 */

#include "fsoeutil.h"
#include "fsoeport.h"

#ifndef FSOEPORT_IS_BIG_ENDIAN
#error "Endianess not specified in fsoeport_macros.h"
#elif (FSOEPORT_IS_BIG_ENDIAN != 1 && FSOEPORT_IS_BIG_ENDIAN != 0)
#error "Invalid endianess specified in fsoeport_macros.h"
#endif

/**
 * \brief Convert 16 bit value from native to little endian byte order
 *
 * See header file for description.
 */
uint16_t fsoeutil_to_le16 (uint16_t native)
{
   uint16_t little;

#if FSOEPORT_IS_BIG_ENDIAN == 1
   little = (native >> 8u) | (uint16_t)(native << 8u);
#else
   little = native;
#endif

   return little;
}

/**
 * \brief Convert 16 bit value from little endian to native byte order
 *
 * See header file for description.
 */
uint16_t fsoeutil_from_le16 (uint16_t little)
{
   uint16_t native;

#if FSOEPORT_IS_BIG_ENDIAN == 1
   native = (little >> 8u) | (uint16_t)(little << 8u);
#else
   native = little;
#endif

   return native;
}
