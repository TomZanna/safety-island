/*
 * flow_log.h
 *
 * Small, thread safe, timestamped logger.
 *
 * All runtime messages are prefixed with the elapsed monotonic time and with
 * the name of the component that generated them ("M7", "M33", "A55", "MU",
 * ...) so that the interleaving of the three threads stays readable.
 */

#ifndef FLOW_LOG_H
#define FLOW_LOG_H

typedef enum flow_log_level
{
   FLOW_LOG_LEVEL_QUIET = 0,  /* only the final report                     */
   FLOW_LOG_LEVEL_ERROR = 1,  /* errors and fail-safe events               */
   FLOW_LOG_LEVEL_WARN  = 2,  /* recoverable anomalies                     */
   FLOW_LOG_LEVEL_INFO  = 3,  /* state changes, session id, faults (default) */
   FLOW_LOG_LEVEL_DETAIL= 4,  /* one line per safe cycle / bus cycle       */
   FLOW_LOG_LEVEL_TRACE = 5,  /* one line per FSoE frame                   */
} flow_log_level_t;

void flow_log_set_level (flow_log_level_t level);
flow_log_level_t flow_log_level (void);
int flow_log_is_enabled (flow_log_level_t level);

void flow_log_printf (flow_log_level_t level,
                      const char * tag,
                      const char * fmt,
                      ...) __attribute__ ((format (printf, 3, 4)));

#define LOG_ERROR(tag, ...)  flow_log_printf (FLOW_LOG_LEVEL_ERROR,  tag, __VA_ARGS__)
#define LOG_WARN(tag, ...)   flow_log_printf (FLOW_LOG_LEVEL_WARN,   tag, __VA_ARGS__)
#define LOG_INFO(tag, ...)   flow_log_printf (FLOW_LOG_LEVEL_INFO,   tag, __VA_ARGS__)
#define LOG_DETAIL(tag, ...) flow_log_printf (FLOW_LOG_LEVEL_DETAIL, tag, __VA_ARGS__)
#define LOG_TRACE(tag, ...)  flow_log_printf (FLOW_LOG_LEVEL_TRACE,  tag, __VA_ARGS__)

/* Tag strings used by the components. Kept short so that columns line up. */
#define FLOW_TAG_MAIN  "MAIN"
#define FLOW_TAG_M7    "M7  "
#define FLOW_TAG_M33   "M33 "
#define FLOW_TAG_A55   "A55 "
#define FLOW_TAG_SHM   "SHM "
#define FLOW_TAG_MU    "MU  "
#define FLOW_TAG_FSOE  "FSOE"
#define FLOW_TAG_SAFE  "SAFE"

#endif /* FLOW_LOG_H */
