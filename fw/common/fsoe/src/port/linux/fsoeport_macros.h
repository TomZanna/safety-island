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
 * \brief Port-specific macros
 *
 * Implements macros used by the FSoE stack.
 */

#ifndef FSOEPORT_MACROS_H
#define FSOEPORT_MACROS_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <assert.h>

/**
 * \brief Function attribute specifying printf-like format string
 *
 * This function attribute is used to enable compiler to emit warnings if
 * types of supplied arguments do not match supplied format string.
 * If compiler does not have such function attribute then this can
 * be defined as an empty macro.
 *
 * Example (usage):
 * \code
 * void fsoeport_log (uint8_t type, const char * fmt, ...) FSOEPORT_FORMAT (2,3);
 * \endcode
 *
 * \param[in]     str         Position of the format string within the argument
 *                            list. Argument positions start at 1.
 * \param[in]     arg         Position of first argument of the variable
 *                            number of arguments.
 *                            Argument positions start at 1.
 */
#define FSOEPORT_FORMAT(str, arg)   __attribute__((format (printf, str, arg)))

/**
 * \brief Specify if this is a big endian platform
 *
 * If set to 1 then this is a big endian platform. The most significant
 * 8 bits of a word are stored at the lowest byte address.
 * If set to 0 then this is a little endian platform. The least
 * significant 8 bits of a word are stored at the lowest byte address.
 */
#if BYTE_ORDER == LITTLE_ENDIAN
#define FSOEPORT_IS_BIG_ENDIAN      0
#else
#define FSOEPORT_IS_BIG_ENDIAN      1
#endif

/**
 * \brief Assert that expression is true
 *
 * Assertions should never fail. An assertion failure indicates that a fatal
 * event has occurred, likely some kind of memory corruption. Program is no
 * longer in a defined state and should be terminated as to avoid safety
 * violations. Termination may be accomplished by means of a watchdog reset
 * or some other mechanism.
 *
 * \param[in]     exp         Expression which should always be true
 */
#define FSOEPORT_ASSERT(exp)        fsoeport_assert (exp)

#ifdef NDEBUG
/**
 * \brief Assert that expression is true. Release build
 *
 * \param[in]     exp         Expression which should always be true
 */
static inline void fsoeport_assert (int exp)
{
   if (!exp)
   {
      /* Wait for watchdog reset */
      for (;;);                     // LCOV_EXCL_LINE
   }
}
#else
/**
 * \brief Assert that expression is true. Debug build
 *
 * \param[in]     exp         Expression which should always be true
 */
static inline void fsoeport_assert (int exp)
{
   assert (exp);                    // LCOV_EXCL_LINE
}
#endif /* NDEBUG */

#ifdef __cplusplus
}
#endif

#endif /* FSOEPORT_MACROS_H */
