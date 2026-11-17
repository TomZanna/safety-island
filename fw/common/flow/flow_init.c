#include "flow_init.h"

#include "flow_config.h"
#include "flow_platform.h"
#include "flow_shm.h"

void flow_init(flow_core_id_t self, flow_u32 cycle_ms)
{
    flow_platform_set_core(self);
    flow_platform_init(cycle_ms);

    flow_platform_log(flow_role_log_tag(self), "flow: PDU %u B, mailbox @0x%08x (%u B), ICC @0x%08x",
        (unsigned)FLOW_PDU_MAX, (unsigned)FLOW_SHM_BASE_ADDR, (unsigned)sizeof(flow_shm_t),
        (unsigned)FLOW_ICC_BASE_ADDR);
}
