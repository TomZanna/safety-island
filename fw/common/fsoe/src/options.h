#ifndef OPTIONS_H
#define OPTIONS_H

#define OPTIONS_H

/* #undef FSOE_LOG_ENABLE */
//#define FSOE_LOG_ENABLE

#ifndef FSOE_LOG_LEVEL
#define FSOE_LOG_LEVEL          (FSOELOG_LEVEL_ERROR)
#endif

#ifndef FSOE_LOG_MASTER
#define FSOE_LOG_MASTER     (FSOELOG_STATE_ON)
#endif

#ifndef FSOE_LOG_SLAVE
#define FSOE_LOG_SLAVE       (FSOELOG_STATE_ON)
#endif

#ifndef FSOE_LOG_FRAME
#define FSOE_LOG_FRAME       (FSOELOG_STATE_ON)
#endif

#ifndef FSOE_LOG_CHANNEL
#define FSOE_LOG_CHANNEL     (FSOELOG_STATE_ON)
#endif

#endif  /* OPTIONS_H */
