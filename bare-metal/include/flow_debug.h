/*
 * flow_debug.h
 *
 * Debug logging is completely removed from the production safety build.
 * All the call sites use FLOW_LOG(); with FLOW_DEBUG=0 the macro expands to
 * nothing and no printf/stdio code is linked.
 */

#ifndef FLOW_DEBUG_H
#define FLOW_DEBUG_H

#include "flow_config.h"

#if FLOW_DEBUG

#include "flow_platform.h"

#define FLOW_LOG(tag, ...) flow_platform_log ((tag), __VA_ARGS__)

#else

#define FLOW_LOG(tag, ...) ((void)0)

#endif

#endif /* FLOW_DEBUG_H */
