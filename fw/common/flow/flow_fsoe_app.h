/**
 * @file flow_fsoe_app.h
 * @brief Identita' del riferimento passato allo stack FSoE.
 * @ingroup flow
 *
 * L'API di rt-labs consegna le callback un `void *app_ref`: ogni ruolo
 * definisce il proprio tipo concreto (safe_fsoe_ref_t in fw/safe/flow_safe.h,
 * field_fsoe_ref_t in fw/fieldbus/flow_field.h). In un binario c'e' un solo
 * ruolo, quindi non serve nessun dispatch a runtime; resta il magic come
 * guardia contro un app_ref sbagliato.
 */
#ifndef SI_FLOW_FSOE_APP_H
#define SI_FLOW_FSOE_APP_H

#define FLOW_FSOE_REF_MAGIC 0x46534F45u /**< "FSOE" */

#endif /* SI_FLOW_FSOE_APP_H */
