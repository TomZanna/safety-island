/*
 * fsoe_app.c
 *
 * The five application callbacks required by the rt-labs FSoE stack.
 *
 * On the target each core has its own binary, so app_ref always points to a
 * flow_fsoe_app_ref_t of kind MASTER; no dispatch between master and slave is
 * needed.  The host test replaces this file with tests/fsoe_app_host.c to add
 * the emulated slave, keeping this production file free of test code.
 */

#include "flow_safe.h"
#include "flow_platform.h"

#include <fsoeapp.h>

static flow_safe_t * safe_from_ref (void * app_ref)
{
   flow_fsoe_app_ref_t * ref = (flow_fsoe_app_ref_t *)app_ref;

   if ((ref == NULL) || (ref->magic != FLOW_FSOE_REF_MAGIC) ||
       (ref->kind != FLOW_FSOE_KIND_MASTER) || (ref->owner == NULL))
   {
      flow_platform_fatal ();
   }

   return (flow_safe_t *)ref->owner;
}

void fsoeapp_send (void * app_ref, const void * buffer, size_t size)
{
   flow_fsoe_app_ref_t * ref = (flow_fsoe_app_ref_t *)app_ref;
   flow_safe_t * safe = safe_from_ref (app_ref);

   flow_safe_on_egress (safe, ref->k, buffer, size);
}

size_t fsoeapp_recv (void * app_ref, void * buffer, size_t size)
{
   flow_fsoe_app_ref_t * ref = (flow_fsoe_app_ref_t *)app_ref;
   flow_safe_t * safe = safe_from_ref (app_ref);

   return flow_safe_on_recv (safe, ref->k, buffer, size);
}

uint16_t fsoeapp_generate_session_id (void * app_ref)
{
   flow_fsoe_app_ref_t * ref = (flow_fsoe_app_ref_t *)app_ref;
   flow_safe_t * safe = safe_from_ref (app_ref);

   return (uint16_t)flow_safe_on_session (safe, ref->k);
}

uint8_t fsoeapp_verify_parameters (
   void * app_ref,
   uint16_t timeout_ms,
   const void * app_parameters,
   size_t size)
{
   (void)app_ref;

   return flow_safe_validate_parameters (timeout_ms, app_parameters, size);
}

void fsoeapp_handle_user_error (
   void * app_ref,
   fsoeapp_usererror_t user_error)
{
   (void)app_ref;
   (void)user_error;

   flow_platform_fatal ();
}
