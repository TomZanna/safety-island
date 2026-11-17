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
 * \brief Porting layer
 *
 * The functions in this header file need to be implemented when porting
 * FSoE to a new platform.
 *
 * In addition, the following macros need to be implemented in the header file
 * fsoeport_macros.h:
 * - FSOEPORT_FORMAT(),
 * - FSOEPORT_IS_BIG_ENDIAN,
 * - FSOEPORT_ASSERT().
 */

#ifndef FSOEPORT_H
#define FSOEPORT_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "fsoeport_macros.h"
#include <stddef.h>
#include <stdint.h>

/**
 * \brief Write a log message with printf-like format
 *
 * Logging is an optional feature. If logging is not enabled, then this
 * function does not need to be implemented.
 *
 * Example implementation:
 * \code
 * #include <stdio.h>
 * #include <stdarg.h>
 * void fsoeport_log (uint8_t type, const char * fmt, ...)
 * {
 *    va_list list;
 *    switch (FSOELOG_LEVEL_GET (type))
 *    {
 *    case FSOELOG_LEVEL_DEBUG:   printf ("[DEBUG] "); break;
 *    case FSOELOG_LEVEL_INFO:    printf ("[INFO ] "); break;
 *    case FSOELOG_LEVEL_WARNING: printf ("[WARN ] "); break;
 *    case FSOELOG_LEVEL_ERROR:   printf ("[ERROR] "); break;
 *    default: break;
 *    }
 *    va_start (list, fmt);
 *    vprintf (fmt, list);
 *    va_end (list);
 * }
 * \endcode
 *
 * \param[in]     type        Type of log message.
 * \param[in]     fmt         Printf-like format string.
 * \param[in]     ...         Additional parameters as specified.
 *                            by format string.
 */
void fsoeport_log (uint8_t type, const char * fmt, ...) FSOEPORT_FORMAT (2,3);

/**
 * \brief Get current microsecond time
 *
 * Time should be monotonically increasing (except for wrap-around).
 * When time is close to 0xffffffff it should wrap around to a value
 * close to zero.
 *
 * Time will be used for determining whether the watchdog timer has
 * expired or not. It may also be used for logging purposes.
 *
 * \return                    Current time, in microseconds
 */
uint32_t fsoeport_current_time_us (void);

/**
 * \brief Copy block of memory
 *
 * Copies the values of \a num bytes from the location pointed to by
 * source directly to the memory block pointed to by destination.
 * The blocks may not overlap.
 *
 * Example implementation:
 * \code
 * #include <string.h>
 * void fsoeport_memcpy (void * dest, const void * src, size_t num)
 * {
 *    memcpy (dest, src, num);
 * }
 * \endcode
 *
 * \param[out]    dest        Pointer to the destination array
 *                            where the content is to be copied.
 * \param[in]     src         Pointer to the source of data to be
 *                            copied.
 * \param[in]     num         Number of bytes to copy.
 */
void fsoeport_memcpy (void * dest, const void * src, size_t num);

/**
 * \brief Compare two blocks of memory
 *
 * Compares the first \a num bytes of the block of memory pointed by
 * \a ptr1 to the first \a num bytes pointed by \a ptr2, returning zero if
 * they all match or a value different from zero otherwise.
 *
 * Example implementation:
 * \code
 * #include <string.h>
 * int32_t fsoeport_memcmp (const void * ptr1, const void * ptr2, size_t num)
 * {
 *    return memcmp (ptr1, ptr2, num);
 * }
 * \endcode
 *
 * \param[in]     ptr1        Pointer to block of memory.
 * \param[in]     ptr2        Pointer to block of memory.
 * \param[in]     num         Number of bytes to compare.
 * \return                    Result of comparison:
 *                            - Negative if the first byte that does not match
 *                              in both memory blocks has a lower
 *                              value in \a ptr1 than in \a ptr2 (if
 *                              evaluated as unsigned char values).
 *                            - Zero if the contents of both memory blocks
 *                              are equal.
 *                            - Positive if the first byte that does not match
 *                              in both memory blocks has a greater
 *                              value in \a ptr1 than in \a ptr2 (if
 *                              evaluated as unsigned char values).
 */
int32_t fsoeport_memcmp (const void * ptr1, const void * ptr2, size_t num);

/**
 * \brief Fill block of memory
 *
 * Sets the first \a num bytes of the block of memory pointed by \a ptr
 * to the specified value.
 *
 * Example implementation:
 * \code
 * #include <string.h>
 * void fsoeport_memset (void * ptr, uint8_t value, size_t num)
 * {
 *    memset (ptr, value, num);
 * }
 * \endcode
 *
 * \param[out]    ptr         Pointer to the block of memory to fill.
 * \param[in]     value       Value to be set.
 * \param[in]     num         Number of bytes to be set to the value.
 */
void fsoeport_memset (void * ptr, uint8_t value, size_t num);

#ifdef __cplusplus
}
#endif

#endif /* FSOEPORT_H */
