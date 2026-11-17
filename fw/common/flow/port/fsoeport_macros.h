/**
 * @file fsoeport_macros.h
 * @brief Macro richieste dallo stack FSoE (vedi fsoe/src/fsoeport.h).
 * @ingroup flow
 *
 */
#ifndef FSOEPORT_MACROS_H
#define FSOEPORT_MACROS_H

#include "flow_platform.h"

/** Attributo di formato per le funzioni printf-like. */
#define FSOEPORT_FORMAT(str, arg) __attribute__((format(printf, str, arg)))

/** ARM Cortex-M7/M33 dell'i.MX94: little endian. */
#define FSOEPORT_IS_BIG_ENDIAN 0

/** Un'asserzione violata dallo stack e' una condizione fatale di sicurezza: le
 * uscite sono gia' a 0 dal chiamante, ci si ferma (breakpoint/watchdog). */
#define FSOEPORT_ASSERT(exp)  \
    do {                      \
        if (!(exp)) {         \
            flow_platform_fatal(); \
        }                     \
    } while (0)

#endif /* FSOEPORT_MACROS_H */
