/**
 * @file fsoe_app.c
 * @brief Callback richieste dallo stack FSoE, lato master.
 * @ingroup flow_safe
 *
 * fw/safe/ e fw/fieldbus/ implementano le stesse cinque funzioni, ma ogni app
 * ne linka una sola: non c'e' nessun dispatch sul ruolo. Il riferimento
 * consegnato dallo stack e' un safe_fsoe_ref_t (fw/safe/flow_safe.h).
 */

/**
 * @page fsoe_states Stati dello stack FSoE
 *
 * La progressione e' quella standard di ETG.5100 ed e' identica sui due master.
 * Lo stack e' compilato come libreria statica per app (`libfsoe-<app>.a`) con
 * un port minimo in `fw/common/flow/port/fsoeport.c`.
 *
 * \mermaid
 * stateDiagram-v2
 *     [*] --> Reset
 *     Reset --> Session: richiesta di connessione
 *     Session --> Connection: Session ID
 *     Connection --> Parameter: Connection ID
 *     Parameter --> Data: watchdog + parametri
 *     Data --> Reset: watchdog scaduto / errore
 * \endmermaid
 *
 * Il **Session ID** e' l'unico valore che i due core devono coordinare invece
 * di calcolare: ciascuno genera un numero casuale con il proprio RNG, lo
 * scambia, e il Session ID finale e' la somma dei due. Cosi' i frame che i due
 * master generano sono identici e il cross-check del PDU in uscita ha senso.
 *
 * Il **watchdog** FSoE e' a 100 ms, dieci volte il ciclo safe: se lo slave
 * smette di rispondere (o i frame vengono scartati) il master va in
 * `WD_EXPIRED` e riparte da `Reset`. Il comportamento e' verificato scartando
 * 60 frame di proposito (`d` sul canale RTT del fieldbus).
 *
 * Le callback di questo file sono il lato master; `fw/fieldbus/fsoe_app.c` ha
 * le stesse funzioni per il lato slave.
 */
#include <fsoeapp.h>

#include "flow_app_parameters.h"
#include "flow_fsoe_app.h"
#include "flow_platform.h"
#include "flow_safe.h"

static safe_fsoe_ref_t* safe_ref(void* app_ref)
{
  safe_fsoe_ref_t* ref = (safe_fsoe_ref_t*)app_ref;

  if ((ref == NULL) || (ref->magic != FLOW_FSOE_REF_MAGIC)
      || (ref->safe == NULL))
  {
    flow_platform_fatal();
  }

  return ref;
}

void fsoeapp_send(void* app_ref, const void* buffer, size_t size)
{
  safe_fsoe_ref_t* ref = safe_ref(app_ref);

  flow_safe_on_egress(ref->safe, ref->k, buffer, size);
}

size_t fsoeapp_recv(void* app_ref, void* buffer, size_t size)
{
  safe_fsoe_ref_t* ref = safe_ref(app_ref);

  return flow_safe_on_recv(ref->safe, ref->k, buffer, size);
}

uint16_t fsoeapp_generate_session_id(void* app_ref)
{
  safe_fsoe_ref_t* ref = safe_ref(app_ref);

  return (uint16_t)flow_safe_on_session(ref->safe, ref->k);
}

uint8_t fsoeapp_verify_parameters(void* app_ref,
                                  uint16_t timeout_ms,
                                  const void* app_parameters,
                                  size_t size)
{
  (void)app_ref;

  return flow_app_parameters_verify((flow_u16)timeout_ms, app_parameters, size);
}

void fsoeapp_handle_user_error(void* app_ref, fsoeapp_usererror_t user_error)
{
  flow_platform_log("fsoe",
                    "errore d'uso dell'API (app_ref=%p): %s",
                    app_ref,
                    fsoeapp_user_error_description(user_error));

  flow_platform_fatal();
}
