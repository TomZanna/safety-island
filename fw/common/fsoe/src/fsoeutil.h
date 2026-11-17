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
 * \brief Utilities
 */

#ifndef FSOEUTIL_H
#define FSOEUTIL_H

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \brief Return the minimum of two numbers
 *
 * \param[in]     a           First number.
 * \param[in]     b           Second number.
 * \return                    The minimum value of \a a and \a b.
 */
#define FSOEUTIL_MIN(a, b) (((a) < (b)) ? (a) : (b))

/**
 * \brief Return the maximum of two numbers
 *
 * \param[in]     a           First number.
 * \param[in]     b           Second number.
 * \return                    The maximum value of \a a and \a b.
 */
#define FSOEUTIL_MAX(a, b) (((a) > (b)) ? (a) : (b))

/**
 * \brief Return number of elements in array
 *
 * Note that returned value is computed at compile-time
 * using the sizeof() operator.
 *
 * \param[in]     a           An array. This needs to be an actual array,
 *                            not a pointer to the first element in an
 *                            array.
 * \return                    Number of elements in array.
 */
#define FSOEUTIL_NELEMENTS(a) (sizeof(a)/sizeof((a)[0]))

/**
 * \brief Convert 16 bit value from native to little endian byte order
 *
 * \param[in]     native      16-bit value in native byte order.
 * \return                    The value converted to little endian byte order.
 */
uint16_t fsoeutil_to_le16 (uint16_t native);

/**
 * \brief Convert 16 bit value from little endian to native byte order
 *
 * \param[in]     little      16-bit value in little endian byte order.
 * \return                    The value converted to native byte order.
 */
uint16_t fsoeutil_from_le16 (uint16_t little);

#ifdef __cplusplus
}
#endif

#endif /* FSOEUTIL_H */
