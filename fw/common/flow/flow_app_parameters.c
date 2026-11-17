#include "flow_app_parameters.h"

#include <fsoeapp.h>

flow_u8 flow_app_parameters_verify(flow_u16 timeout_ms, const void *parameters, size_t size)
{
    const flow_u8 *bytes = (const flow_u8 *)parameters;
    flow_u16 param1;
    flow_u16 param2;

    (void)timeout_ms;

    if ((bytes == NULL) || (size != FLOW_APP_PARAMETERS_SIZE)) {
        return FSOEAPP_STATUS_BAD_APP_PARAMETER;
    }

    param1 = (flow_u16)(((flow_u16)bytes[1] << 8u) | bytes[0]);
    param2 = (flow_u16)(((flow_u16)bytes[3] << 8u) | bytes[2]);

    if ((param1 != FLOW_APP_PARAMETER_1) || (param2 != FLOW_APP_PARAMETER_2)) {
        return FSOEAPP_STATUS_BAD_APP_PARAMETER;
    }

    return FSOEAPP_STATUS_OK;
}
