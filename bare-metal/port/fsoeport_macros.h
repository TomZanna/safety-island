/*
 * fsoeport_macros.h
 *
 * Bare metal port of the macros required by the rt-labs FSoE stack.
 * This file is found through -Iport, before the library's own port/linux.
 */

#ifndef FSOEPORT_MACROS_H
#define FSOEPORT_MACROS_H

#include "flow_platform.h"

#define FSOEPORT_FORMAT(str, arg) __attribute__ ((format (printf, str, arg)))

/* M7, M33 and A55 are little endian. */
#define FSOEPORT_IS_BIG_ENDIAN 0

/* An assertion failure means the FSoE stack detected an invalid state.  The
 * only safe reaction is to stop the outputs and wait for the external
 * watchdog.  Never use assert()/printf in the production safety build. */
#define FSOEPORT_ASSERT(expression) \
   do \
   { \
      if (!(expression)) \
      { \
         flow_platform_fatal (); \
      } \
   } while (0)

#endif /* FSOEPORT_MACROS_H */
