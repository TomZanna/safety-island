/**
 * @file safe_platform.c
 * @brief Aggancio alla piattaforma del ruolo safe.
 * @ingroup flow_safe
 *
 */
#include "safe_platform.h"

#include "flow_config.h"
#include "flow_mu18.h"
#include "flow_platform.h"

void safe_platform_init_mu18(flow_core_id_t self)
{
  switch (self) {
    case FLOW_CORE_SAFE_A:
      flow_mu18_bind(FLOW_MU18_B_BASE); /* M7_0 = lato B */
      break;
    case FLOW_CORE_SAFE_B:
      flow_mu18_bind(FLOW_MU18_A_BASE); /* M7_1 = lato A */
      break;
    default:
      flow_platform_fatal();
      break;
  }
}
