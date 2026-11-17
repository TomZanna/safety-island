/*
 * Ruolo FIELDBUS: lo slave FSoE (il "campo") e il generatore di ingressi.
 *
 * Non e' un componente di sicurezza: genera ingressi casuali, risponde ai master
 * e pubblica la risposta nella mailbox. Serve a far girare il 1oo2 e a non far
 * scattare il watchdog FSoE (gira ogni 2 ms).
 *
 * Il generatore e' deterministico (xorshift con seed costante): utile per
 * riprodurre una sequenza con gli stessi semi.
 */
#ifndef SI_FLOW_FIELD_H
#define SI_FLOW_FIELD_H

#include "flow_config.h"
#include "flow_fsoe_app.h"
#include "flow_safe_io.h"
#include "flow_types.h"

#include <fsoeslave.h>

typedef struct flow_field flow_field_t;

/* Riferimento passato a fsoeslave_init(): tipo proprio del ruolo fieldbus (in
 * questo binario non esiste il master), quindi nessun dispatch a runtime. */
typedef struct field_fsoe_ref
{
    flow_u32 magic;
    flow_u32 k;
    flow_field_t *field;
} field_fsoe_ref_t;

typedef struct flow_field
{
    field_fsoe_ref_t ref[FLOW_NUM_CONNECTIONS];
    fsoeslave_t slave[FLOW_NUM_CONNECTIONS];
    fsoeslave_syncstatus_t sync[FLOW_NUM_CONNECTIONS];

    flow_safe_inputs_t field_inputs[FLOW_NUM_CONNECTIONS];
    flow_safe_outputs_t field_outputs[FLOW_NUM_CONNECTIONS];

    /* Frame ricevuto dal master (assemblato dal ruolo fieldbus). */
    flow_u8 rx_pdu[FLOW_PDU_MAX];
    flow_u32 rx_len;
    bool rx_ready;

    /* Risposta dello slave, pronta per la mailbox. */
    flow_u8 tx_pdu[FLOW_PDU_MAX];
    flow_u32 tx_len;
    bool tx_ready;

    flow_u32 bus_cycles;
    flow_u32 rng;

    /* Stato del processo di campo simulato. */
    bool estop_ok;
    bool door_closed;
    bool reset;
    bool reset_given;
    flow_u32 reset_window;
    bool inject_discrepancy;

    flow_u32 drop_frames; /* frame da scartare (test del watchdog FSoE) */

    bool outputs_valid;
    bool enabled_seen;
    flow_u32 frames_ok;
    flow_u32 frames_dropped;
} flow_field_t;

void flow_field_init(flow_field_t *field, flow_u16 slave_address);

/* Frame del master da consegnare allo slave (uno per ciclo safe). */
void flow_field_provide_rx(flow_field_t *field, const flow_u8 *pdu, flow_u32 len);

/* Un passo di campo + slave (2 ms). */
void flow_field_step(flow_field_t *field);

/* Prende la risposta dello slave da pubblicare in mailbox. */
bool flow_field_take_tx(flow_field_t *field, flow_u8 *pdu, flow_u32 *len);

/* Comandi da RTT. */
void flow_field_set_discrepancy(flow_field_t *field, bool on);
void flow_field_drop_frames(flow_field_t *field, flow_u32 frames);

/* Callback dal glue FSoE. */
void flow_field_on_egress(flow_field_t *field, flow_u32 k, const void *buffer, size_t size);
size_t flow_field_on_recv(flow_field_t *field, flow_u32 k, void *buffer, size_t size);
flow_u16 flow_field_on_session(flow_field_t *field, flow_u32 k);

#endif /* SI_FLOW_FIELD_H */
