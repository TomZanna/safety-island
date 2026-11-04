/*
 * flow_platform_host.h
 *
 * Configuration of the POSIX platform used only by the host validation test.
 */

#ifndef FLOW_PLATFORM_HOST_H
#define FLOW_PLATFORM_HOST_H

#include "flow_platform.h"

/* Must be called before the first flow_platform_init(). */
void flow_platform_host_configure (flow_u32 seed, bool verbose);

#endif /* FLOW_PLATFORM_HOST_H */
