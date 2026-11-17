/*
 * Callback richieste dallo stack FSoE, lato master.
 *
 * fw/safe/ e fw/fieldbus/ implementano le stesse cinque funzioni, ma ogni app ne
 * linka una sola: non c'e' nessun dispatch sul ruolo. Il riferimento consegnato
 * dallo stack e' un safe_fsoe_ref_t (fw/safe/flow_safe.h).
 */
#include "flow_app_parameters.h"
#include "flow_fsoe_app.h"
#include "flow_platform.h"
#include "flow_safe.h"

#include <fsoeapp.h>

static safe_fsoe_ref_t *safe_ref(void *app_ref)
{
    safe_fsoe_ref_t *ref = (safe_fsoe_ref_t *)app_ref;

    if ((ref == NULL) || (ref->magic != FLOW_FSOE_REF_MAGIC) || (ref->safe == NULL)) {
        flow_platform_fatal();
    }

    return ref;
}

void fsoeapp_send(void *app_ref, const void *buffer, size_t size)
{
    safe_fsoe_ref_t *ref = safe_ref(app_ref);

    flow_safe_on_egress(ref->safe, ref->k, buffer, size);
}

size_t fsoeapp_recv(void *app_ref, void *buffer, size_t size)
{
    safe_fsoe_ref_t *ref = safe_ref(app_ref);

    return flow_safe_on_recv(ref->safe, ref->k, buffer, size);
}

uint16_t fsoeapp_generate_session_id(void *app_ref)
{
    safe_fsoe_ref_t *ref = safe_ref(app_ref);

    return (uint16_t)flow_safe_on_session(ref->safe, ref->k);
}

uint8_t fsoeapp_verify_parameters(void *app_ref, uint16_t timeout_ms, const void *app_parameters, size_t size)
{
    (void)app_ref;

    return flow_app_parameters_verify((flow_u16)timeout_ms, app_parameters, size);
}

void fsoeapp_handle_user_error(void *app_ref, fsoeapp_usererror_t user_error)
{
    flow_platform_log("fsoe", "errore d'uso dell'API (app_ref=%p): %s", app_ref,
        fsoeapp_user_error_description(user_error));

    flow_platform_fatal();
}
