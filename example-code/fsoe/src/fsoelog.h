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
 * \brief Logging utilities
 */

#ifndef FSOELOG_H
#define FSOELOG_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "fsoeport.h"
#include "options.h"
#include <inttypes.h>

/** \brief Log levels */
#define FSOELOG_LEVEL_DEBUG   0x00u  /**< Debug messages and higher */
#define FSOELOG_LEVEL_INFO    0x01u  /**< Informational messages and higher */
#define FSOELOG_LEVEL_WARNING 0x02u  /**< Warning messages and higher */
#define FSOELOG_LEVEL_ERROR   0x03u  /**< Error messages only */
#define FSOELOG_LEVEL_MASK    0x03u  /**< Bit mask for log level */
#define FSOELOG_LEVEL_GET(t)  ((t) & FSOELOG_LEVEL_MASK) /**< Get log level */

/**
 * \brief Log states
 */
#define FSOELOG_STATE_ON      0x80u  /**< Logging is enabled */
#define FSOELOG_STATE_OFF     0x00u  /**< Logging is disabled */

#ifdef FSOE_LOG_ENABLE
/**
 * \brief Log message
 */
#define FSOELOG(type, ...)                               \
do                                                       \
{                                                        \
   if ((FSOELOG_LEVEL_GET (type) >= FSOE_LOG_LEVEL) &&   \
       ((type & FSOELOG_STATE_ON) == FSOELOG_STATE_ON))  \
      {                                                  \
         fsoeport_log (type, __VA_ARGS__);               \
      }                                                  \
} while(0)
#else
/** Discard message */
#define FSOELOG(type, ...)
#endif

/**
 * \brief Log debug message
 *
 * A debug message is similar to an informational message, but may be
 * logged more frequently as well as contain more text.
 *
 * Note that the message will be discarded in case
 * - global debug level is higher than FSOELOG_LEVEL_DEBUG,
 * - logging is disabled for this message type or
 * - logging is globally disabled.
 *
 * Example:
 * \code
 * #define TYPE1 FSOELOG_STATE_ON
 * #define TYPE2 FSOELOG_STATE_OFF
 * FSOELOG_DEBUG (TYPE1, "logged if debug messages are enabled");
 * FSOELOG_DEBUG (TYPE2, "discarded unconditionally");
 * \endcode
 *
 * \param[in]     type        Type of message
 * \param[in]     ...         Format string (like printf) and optional
 *                            arguments
 */
#define FSOELOG_DEBUG(type, ...)                    \
   FSOELOG ((FSOELOG_LEVEL_DEBUG | type), __VA_ARGS__)

/**
 * \brief Log informational message
 *
 * An information message may be logged during normal operation.
 * Care should be taken to ensure that system is not overloaded
 * with informational messages. For very frequent messages or for
 * messages where lots of text is printed, use FSOELOG_DEBUG() instead.
 *
 * Note that the message will be discarded in case
 * - global debug level is higher than FSOELOG_LEVEL_INFO,
 * - logging is disable for this type or
 * - logging is globally disabled.
 *
 * Example:
 * \code
 * #define TYPE1 FSOELOG_STATE_ON
 * #define TYPE2 FSOELOG_STATE_OFF
 * FSOELOG_INFO (TYPE1, "logged if informational messages are enabled");
 * FSOELOG_INFO (TYPE2, "discarded unconditionally");
 * \endcode
 *
 * \param[in]     type        Type of message
 * \param[in]     ...         Format string (like printf) and optional
 *                            arguments
 */
#define FSOELOG_INFO(type, ...) \
   FSOELOG ((FSOELOG_LEVEL_INFO | type), __VA_ARGS__)

/**
 * \brief Log warning message
 *
 * Warnings messages may be logged if a condition is detected that may
 * or may not signify an error condition, such as remote peer
 * terminating the connection.
 *
 * Example:
 * \code
 * #define TYPE1 FSOELOG_STATE_ON
 * #define TYPE2 FSOELOG_STATE_OFF
 * FSOELOG_WARNING (TYPE1, "logged if warning messages are enabled");
 * FSOELOG_WARNING (TYPE2, "discarded unconditionally");
 * \endcode
 *
 * Note that the message will be discarded in case
 * - global debug level is higher than FSOELOG_LEVEL_WARNING,
 * - logging is disable for this type or
 * - logging is globally disabled.
 *
 * \param[in]     type        Type of message
 * \param[in]     ...         Format string (like printf) and optional
 *                            arguments
 */
#define FSOELOG_WARNING(type, ...) \
   FSOELOG ((FSOELOG_LEVEL_WARNING | type), __VA_ARGS__)

/**
 * \brief Log error message
 *
 * An error message may be logged only when an error condition is
 * detected, such as receiving a corrupt frame.
 *
 * Note that the message will be discarded in case
 * - logging is disable for this type or
 * - logging is globally disabled.
 *
 * Example:
 * \code
 * #define TYPE1 FSOELOG_STATE_ON
 * #define TYPE2 FSOELOG_STATE_OFF
 * FSOELOG_ERROR (TYPE1, "logged if logging is enabled");
 * FSOELOG_ERROR (TYPE2, "discarded unconditionally");
 * \endcode
 *
 * \param[in]     type        Type of message
 * \param[in]     ...         Format string (like printf) and optional
 *                            arguments
 */
#define FSOELOG_ERROR(type, ...) \
   FSOELOG ((FSOELOG_LEVEL_ERROR | type), __VA_ARGS__)

#ifdef __cplusplus
}
#endif

#endif /* FSOELOG_H */
