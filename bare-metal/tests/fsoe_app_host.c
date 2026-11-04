/*
 * fsoe_app_host.c
 *
 * Test-only implementation of the five FSoE application callbacks.  It
 * dispatches on the app_ref kind: a master instance calls into the safe core,
 * a slave instance calls into the test field emulation.
 */

#include "host_field.h"
#include "flow_safe.h"
#include "flow_mem.h"
#include "flow_platform.h"

#include <fsoeapp.h>
#include <stdio.h>

static flow_fsoe_app_ref_t * host_ref (void * app_ref)
{
   flow_fsoe_app_ref_t * ref = (flow_fsoe_app_ref_t *)app_ref;

   if ((ref == NULL) || (ref->magic != FLOW_FSOE_REF_MAGIC))
   {
      flow_platform_fatal ();
   }

   return ref;
}

void fsoeapp_send (void * app_ref, const void * buffer, size_t size)
{
   flow_fsoe_app_ref_t * ref = host_ref (app_ref);

   if (ref->kind == FLOW_FSOE_KIND_MASTER)
   {
      flow_safe_on_egress ((flow_safe_t *)ref->owner, ref->k, buffer, size);
   }
   else
   {
      host_slave_t * host = (host_slave_t *)ref->owner;

      if (size <= FLOW_PDU_MAX)
      {
         flow_memcpy (host->tx_pdu, buffer, (flow_u32)size);
         host->tx_len = (flow_u32)size;
         host->tx_ready = true;
      }
   }
}

size_t fsoeapp_recv (void * app_ref, void * buffer, size_t size)
{
   flow_fsoe_app_ref_t * ref = host_ref (app_ref);

   if (ref->kind == FLOW_FSOE_KIND_MASTER)
   {
      return flow_safe_on_recv (
         (flow_safe_t *)ref->owner, ref->k, buffer, size);
   }
   else
   {
      host_slave_t * host = (host_slave_t *)ref->owner;

      if (!host->rx_ready || (host->rx_len != (flow_u32)size))
      {
         return 0u;
      }

      flow_memcpy (buffer, host->rx_pdu, host->rx_len);
      host->rx_ready = false;

      return size;
   }
}

uint16_t fsoeapp_generate_session_id (void * app_ref)
{
   flow_fsoe_app_ref_t * ref = host_ref (app_ref);

   if (ref->kind == FLOW_FSOE_KIND_MASTER)
   {
      return (uint16_t)flow_safe_on_session (
         (flow_safe_t *)ref->owner, ref->k);
   }
   else
   {
      host_slave_t * host = (host_slave_t *)ref->owner;

      host->session_counter++;

      return (uint16_t)(0x1000u + host->session_counter);
   }
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

   fprintf (stderr, "host: FSoE API misuse: %s\n",
      fsoeapp_user_error_description (user_error));

   flow_platform_fatal ();
}
