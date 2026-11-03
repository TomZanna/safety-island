/*
 * fsoe_app_glue.c
 *
 * Single implementation of the rt-labs FSoE application callbacks, dispatching
 * on the `app_ref` pointer. See fsoe_app_glue.h.
 */

#include "fsoe_app_glue.h"
#include "flow_config.h"
#include "flow_log.h"
#include "flow_rng.h"

#include <fsoeapp.h>
#include <fsoemaster.h>
#include <fsoeslave.h>

#include <stdio.h>
#include <string.h>

/* The PDU buffer of the mailbox must be able to hold any FSoE frame used by
 * this configuration. */
_Static_assert (
   FLOW_FSOE_PDU_MAX == FSOEMASTER_FRAME_SIZE (FLOW_SAFE_DATA_SIZE),
   "FLOW_FSOE_PDU_MAX does not match FSOEMASTER_FRAME_SIZE");

/* RNG used for the slave Session ID. Seeded by flow_app_glue_init(). */
static flow_rng_t slave_rng;

void flow_app_glue_init (uint32_t seed)
{
   flow_rng_seed (&slave_rng, seed ^ 0x5A5A5A5Au);
}

const char * flow_ctx_kind_name (flow_ctx_kind_t kind)
{
   switch (kind)
   {
   case FLOW_CTX_M7:    return "M7";
   case FLOW_CTX_M33:   return "M33";
   case FLOW_CTX_SLAVE: return "A55-slave";
   default:             return "?";
   }
}

void flow_app_ctx_init (
   flow_app_ctx_t * ctx,
   flow_ctx_kind_t kind,
   uint32_t k,
   void * stack,
   void * owner,
   flow_app_egress_fn on_egress,
   flow_app_session_fn on_session_id)
{
   memset (ctx, 0, sizeof (*ctx));
   ctx->magic = FLOW_CTX_MAGIC;
   ctx->kind = kind;
   ctx->k = k;
   ctx->stack = stack;
   ctx->owner = owner;
   ctx->on_egress = on_egress;
   ctx->on_session_id = on_session_id;
   flow_fsoe_slot_clear (&ctx->rx);
   flow_fsoe_slot_clear (&ctx->tx);
}

void flow_app_ctx_provide_rx (
   flow_app_ctx_t * ctx,
   const flow_fsoe_slot_t * slot,
   uint32_t published)
{
   if (ctx->rx_ready)
   {
      ctx->stats.recv_overwritten++;
      LOG_TRACE (FLOW_TAG_FSOE,
         "%s k=%u: previous RX frame not consumed, overwritten",
         flow_ctx_kind_name (ctx->kind),
         ctx->k);
   }

   ctx->rx = *slot;
   ctx->rx_published = published;
   ctx->rx_ready = true;
   ctx->stats.recv_provided++;
}

void flow_app_ctx_clear_rx (flow_app_ctx_t * ctx)
{
   flow_fsoe_slot_clear (&ctx->rx);
   ctx->rx_published = 0u;
   ctx->rx_ready = false;
}

bool flow_app_ctx_take_tx (flow_app_ctx_t * ctx, flow_fsoe_slot_t * slot)
{
   if (!ctx->tx_ready)
   {
      return false;
   }

   *slot = ctx->tx;
   ctx->tx_ready = false;

   return true;
}

/* ------------------------------------------------------------- callbacks */

static flow_app_ctx_t * flow_app_ctx_check (void * app_ref)
{
   flow_app_ctx_t * ctx = (flow_app_ctx_t *)app_ref;

   if ((ctx == NULL) || (ctx->magic != FLOW_CTX_MAGIC))
   {
      LOG_ERROR (FLOW_TAG_FSOE,
         "fsoeapp callback called with an invalid app_ref (%p)", app_ref);
      return NULL;
   }

   return ctx;
}

void fsoeapp_send (void * app_ref, const void * buffer, size_t size)
{
   flow_app_ctx_t * ctx = flow_app_ctx_check (app_ref);

   if (ctx == NULL)
   {
      return;
   }

   if (size > FLOW_FSOE_PDU_MAX)
   {
      LOG_ERROR (FLOW_TAG_FSOE,
         "%s k=%u: stack produced a frame of %zu bytes, truncated to %u",
         flow_ctx_kind_name (ctx->kind),
         ctx->k,
         size,
         (unsigned)FLOW_FSOE_PDU_MAX);
      size = FLOW_FSOE_PDU_MAX;
   }

   flow_fsoe_slot_set (&ctx->tx, buffer, (uint32_t)size, 0u);
   ctx->tx_ready = true;
   ctx->stats.sent++;

   LOG_TRACE (FLOW_TAG_FSOE,
      "%s k=%u: fsoeapp_send %s len=%u digest=0x%08x",
      flow_ctx_kind_name (ctx->kind),
      ctx->k,
      flow_fsoe_command_name (flow_fsoe_command (&ctx->tx)),
      ctx->tx.len,
      ctx->tx.digest);

   if (ctx->on_egress != NULL)
   {
      ctx->on_egress (ctx, &ctx->tx);
   }
}

size_t fsoeapp_recv (void * app_ref, void * buffer, size_t length)
{
   flow_app_ctx_t * ctx = flow_app_ctx_check (app_ref);
   size_t copied;

   if (ctx == NULL)
   {
      return 0u;
   }

   if (!ctx->rx_ready)
   {
      LOG_TRACE (FLOW_TAG_FSOE,
         "%s k=%u: fsoeapp_recv - no new frame",
         flow_ctx_kind_name (ctx->kind),
         ctx->k);
      return 0u;
   }

   copied = (ctx->rx.len < length) ? (size_t)ctx->rx.len : length;
   if (copied < length)
   {
      memset ((uint8_t *)buffer + copied, 0, length - copied);
   }
   memcpy (buffer, ctx->rx.pdu, copied);
   ctx->rx_ready = false;
   ctx->stats.recv_consumed++;

   LOG_TRACE (FLOW_TAG_FSOE,
      "%s k=%u: fsoeapp_recv %s len=%zu digest=0x%08x",
      flow_ctx_kind_name (ctx->kind),
      ctx->k,
      flow_fsoe_command_name (flow_fsoe_command (&ctx->rx)),
      copied,
      ctx->rx.digest);

   return copied;
}

uint16_t fsoeapp_generate_session_id (void * app_ref)
{
   flow_app_ctx_t * ctx = flow_app_ctx_check (app_ref);

   if (ctx == NULL)
   {
      return 0u;
   }

   if (ctx->kind == FLOW_CTX_SLAVE)
   {
      /* The slave generates its own Session ID: it is not part of the 1oo2
       * cross-check of the two safety cores. */
      ctx->session_rnd = flow_rng_next (&slave_rng);
      ctx->session_id = ctx->session_rnd & 0xFFFFu;
      ctx->session_count++;

      LOG_INFO (FLOW_TAG_FSOE,
         "A55-slave k=%u: generated Session ID 0x%04x",
         ctx->k,
         (unsigned)ctx->session_id);

      return (uint16_t)ctx->session_id;
   }

   /* For the safety cores the negotiation (sum of the two random numbers) is
    * performed by the owner, which has access to the messaging unit. */
   if (ctx->on_session_id != NULL)
   {
      ctx->session_id = ctx->on_session_id (ctx);
   }
   ctx->session_count++;

   LOG_TRACE (FLOW_TAG_FSOE,
      "%s k=%u: stack requested Session ID -> 0x%04x",
      flow_ctx_kind_name (ctx->kind),
      ctx->k,
      (unsigned)ctx->session_id);

   return (uint16_t)ctx->session_id;
}

uint8_t fsoeapp_verify_parameters (
   void * app_ref,
   uint16_t timeout_ms,
   const void * app_parameters,
   size_t size)
{
   const uint8_t * parameters = (const uint8_t *)app_parameters;
   uint16_t param1;
   uint16_t param2;

   (void)app_ref;

   if (size != FLOW_APP_PARAMETERS_SIZE)
   {
      LOG_ERROR (FLOW_TAG_FSOE,
         "A55-slave: received %zu application parameter bytes, expected %u",
         size,
         (unsigned)FLOW_APP_PARAMETERS_SIZE);
      return FSOEAPP_STATUS_BAD_APP_PARAMETER;
   }

   param1 = (uint16_t)((uint16_t)parameters[1] << 8 | parameters[0]);
   param2 = (uint16_t)((uint16_t)parameters[3] << 8 | parameters[2]);

   if ((param1 != FLOW_APP_PARAMETER_1) || (param2 != FLOW_APP_PARAMETER_2))
   {
      LOG_ERROR (FLOW_TAG_FSOE,
         "A55-slave: unexpected application parameters 0x%04x 0x%04x",
         (unsigned)param1,
         (unsigned)param2);
      return FSOEAPP_STATUS_BAD_APP_PARAMETER;
   }

   LOG_INFO (FLOW_TAG_FSOE,
      "A55-slave: parameters verified (watchdog=%u ms, app=0x%04x 0x%04x)",
      (unsigned)timeout_ms,
      (unsigned)param1,
      (unsigned)param2);

   return FSOEAPP_STATUS_OK;
}

void fsoeapp_handle_user_error (
   void * app_ref,
   fsoeapp_usererror_t user_error)
{
   LOG_ERROR (FLOW_TAG_FSOE,
      "API misuse (app_ref=%p): %s",
      app_ref,
      fsoeapp_user_error_description (user_error));
}

