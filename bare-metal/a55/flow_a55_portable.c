/*
 * flow_a55_portable.c
 *
 * Kbuild wrapper: the SHMEM protocol and the A55 merge are shared with the
 * bare metal safety firmware, so they live in common/ and in a55/flow_a55.c.
 * Kbuild cannot easily compile sources outside the module directory, so this
 * single translation unit includes them.  There is no duplicated logic here.
 */

#include "../common/flow_mem.c"
#include "../common/flow_crc.c"
#include "../common/flow_shm.c"
#include "flow_a55.c"
