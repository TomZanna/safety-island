/*
 * Implementazione unica delle cinque callback richieste dallo stack FSoE.
 *
 * In un binario esiste un solo tipo di istanza (master sui due core safe, slave
 * sul fieldbus), quindi la scelta e' a compile time: la stessa sorgente gira sui
 * tre core senza duplicare codice.
 */
#include "flow_field.h"
#include "flow_platform.h"
#include "flow_safe.h"

#include <fsoeapp.h>

static flow_fsoe_app_ref_t *app_ref_check(void *app_ref)
{
    flow_fsoe_app_ref_t *ref = (flow_fsoe_app_ref_t *)app_ref;

    if ((ref == NULL) || (ref->magic != FLOW_FSOE_REF_MAGIC) || (ref->owner == NULL)) {
        flow_platform_fatal();
    }

    return ref;
}

void fsoeapp_send(void *app_ref, const void *buffer, size_t size)
{
    flow_fsoe_app_ref_t *ref = app_ref_check(app_ref);

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    if (ref->kind != FLOW_FSOE_KIND_SLAVE) {
        flow_platform_fatal();
    }
    flow_field_on_egress((flow_field_t *)ref->owner, ref->k, buffer, size);
#else
    if (ref->kind != FLOW_FSOE_KIND_MASTER) {
        flow_platform_fatal();
    }
    flow_safe_on_egress((flow_safe_t *)ref->owner, ref->k, buffer, size);
#endif
}

size_t fsoeapp_recv(void *app_ref, void *buffer, size_t size)
{
    flow_fsoe_app_ref_t *ref = app_ref_check(app_ref);

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    return flow_field_on_recv((flow_field_t *)ref->owner, ref->k, buffer, size);
#else
    return flow_safe_on_recv((flow_safe_t *)ref->owner, ref->k, buffer, size);
#endif
}

uint16_t fsoeapp_generate_session_id(void *app_ref)
{
    flow_fsoe_app_ref_t *ref = app_ref_check(app_ref);

#if (FLOW_CORE_ID == FLOW_CORE_FIELDBUS)
    return (uint16_t)flow_field_on_session((flow_field_t *)ref->owner, ref->k);
#else
    return (uint16_t)flow_safe_on_session((flow_safe_t *)ref->owner, ref->k);
#endif
}

uint8_t fsoeapp_verify_parameters(void *app_ref, uint16_t timeout_ms, const void *app_parameters, size_t size)
{
    (void)app_ref;

    return flow_safe_validate_parameters(timeout_ms, app_parameters, size);
}

void fsoeapp_handle_user_error(void *app_ref, fsoeapp_usererror_t user_error)
{
    flow_platform_log("fsoe", "errore d'uso dell'API (app_ref=%p): %s", app_ref,
        fsoeapp_user_error_description(user_error));

    flow_platform_fatal();
}
