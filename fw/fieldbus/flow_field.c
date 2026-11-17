/**
 * @file flow_field.c
 * @brief Ruolo FIELDBUS: lo slave FSoE (il "campo") e il generatore di
 * ingressi.
 * @ingroup flow_field
 *
 * Il campo simulato e' deterministico (xorshift con seed costante): la stessa
 * sequenza si riproduce a ogni boot, utile per confrontare due run. Lo stato
 * cambia ogni ~500 ms con un bias verso "macchina in marcia"; quando le
 * condizioni di sicurezza sono soddisfatte parte un impulso di reset di ~120
 * ms, cosi' il 1oo2 arriva ad abilitare le uscite.
 *
 * Il ruolo gira a 2 ms per tenere vivo il watchdog FSoE del master (100 ms) e
 * pubblica la risposta dello slave una volta per ciclo safe. drop_frames e
 * inject_discrepancy sono gli strumenti di prova comandati da RTT: scartano
 * frame o fanno divergere il canale B di proposito.
 */
#include "flow_field.h"

#include "flow_mem.h"
#include "flow_platform.h"

/* Unico ruolo che esegue il campo: il tag non dipende dal build. */
#define FIELD_TAG "fld"

/** @brief Prossimo valore dello xorshift deterministico del campo.
 *
 * @param[in,out] field stato del ruolo (avanza il seed).
 * @return nuovo valore pseudo-casuale.
 */
static flow_u32 field_next_random(flow_field_t* field)
{
  flow_u32 x = field->rng;

  x ^= x << 13;
  x ^= x >> 17;
  x ^= x << 5;
  field->rng = x;

  return x;
}

void flow_field_init(flow_field_t* field, flow_u16 slave_address)
{
  flow_u32 k;

  flow_memset(field, 0, (flow_u32)sizeof(*field));

  /* Seed costante non nullo: stesso valore dei build precedenti (ruolo FIELDBUS
   * = 2). */
  field->rng = 0x1A2B3C4Du ^ (flow_u32)FLOW_CORE_FIELDBUS;

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    const fsoeslave_cfg_t cfg = {
        .slave_address = (uint16_t)(slave_address + k),
        .application_parameters_size = FLOW_APP_PARAMETERS_SIZE,
        .inputs_size = FLOW_SAFE_DATA_SIZE,
        .outputs_size = FLOW_SAFE_DATA_SIZE,
    };

    field->ref[k].magic = FLOW_FSOE_REF_MAGIC;
    field->ref[k].k = k;
    field->ref[k].field = field;

    if (fsoeslave_init(&field->slave[k], &cfg, &field->ref[k])
        != FSOESLAVE_STATUS_OK)
    {
      flow_platform_log(FIELD_TAG, "k=%u fsoeslave_init fallita", (unsigned)k);
      flow_platform_fatal();
    }
  }

  /* Stato iniziale: macchina in sicurezza (E-stop premuto, porta chiusa). */
  field->estop_ok = false;
  field->door_closed = true;

  flow_platform_log(FIELD_TAG,
                    "slave FSoE pronto (addr 0x%04x, %u byte di dati)",
                    (unsigned)slave_address,
                    (unsigned)FLOW_SAFE_DATA_SIZE);
}

/** @brief Genera il processo di campo simulato e gli ingressi di sicurezza.
 *
 * Cambia stato ogni ~500 ms con un bias verso "macchina in marcia" e da' un
 * impulso di reset quando le condizioni di sicurezza sono soddisfatte (cosi' il
 * 1oo2 arriva a ENABLE). Con `inject_discrepancy` il canale B diverge di
 * proposito.
 *
 * @param[in,out] field stato del ruolo.
 */
static void field_generate(flow_field_t* field)
{
  field->bus_cycles++;

  if ((field->bus_cycles % 250u) == 0u) {
    const flow_u32 x = field_next_random(field);
    field->estop_ok = ((x % 10u) != 0u); /* 90% E-stop rilasciato */
    field->door_closed = (((x >> 8) % 10u) < 8u); /* 80% porta chiusa       */
  }

  if (!field->estop_ok || !field->door_closed) {
    field->reset_given = false;
    field->reset_window = 0u;
  } else if (!field->reset_given) {
    field->reset_given = true;
    field->reset_window = 60u; /* ~120 ms di impulso */
  } else {
    /* Reset gia' in corso: la finestra scorre sotto, niente da fare. */
  }

  if (field->reset_window > 0u) {
    field->reset = true;
    field->reset_window--;
  } else {
    field->reset = false;
  }

  for (flow_u32 k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    flow_u16 bits = 0u;

    if (field->estop_ok) {
      bits |= FLOW_SAFE_IN_ESTOP_OK;
    }
    if (field->door_closed) {
      bits |= FLOW_SAFE_IN_DOOR_CLOSED;
    }
    if (field->reset) {
      bits |= FLOW_SAFE_IN_RESET;
    }

    field->field_inputs[k].ch_a = bits;
    field->field_inputs[k].ch_b = bits;

    if (field->inject_discrepancy) {
      field->field_inputs[k].ch_b ^= FLOW_SAFE_IN_DOOR_CLOSED;
    }
  }
}

void flow_field_provide_rx(flow_field_t* field,
                           const flow_u8* pdu,
                           flow_u32 len)
{
  if ((pdu == NULL) || (len > FLOW_PDU_MAX)) {
    return;
  }

  flow_memcpy(field->rx_pdu, pdu, len);
  field->rx_len = len;
  field->rx_ready = true;
}

void flow_field_step(flow_field_t* field)
{
  flow_u32 k;

  field_generate(field);

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    const fsoeslave_status_t status =
        fsoeslave_sync_with_master(&field->slave[k],
                                   &field->field_inputs[k],
                                   &field->field_outputs[k],
                                   &field->sync[k]);

    if (status != FSOESLAVE_STATUS_OK) {
      flow_platform_log(FIELD_TAG, "k=%u fsoeslave_sync fallita", (unsigned)k);
      continue;
    }

    /* In Parameter lo slave deve dichiarare di poter mandare i dati di
     * processo, altrimenti il master non arriva a DATA. */
    if (field->sync[k].current_state == FSOESLAVE_STATE_PARAMETER) {
      (void)fsoeslave_set_process_data_sending_enable_flag(&field->slave[k]);
    }

    if (field->sync[k].is_process_data_received) {
      field->outputs_valid = true;
      field->frames_ok++;
      if (field->field_outputs[k].enable == FLOW_SAFE_OUTPUT_ENABLED) {
        field->enabled_seen = true;
      }
    }
  }
}

bool flow_field_take_tx(flow_field_t* field, flow_u8* pdu, flow_u32* len)
{
  if (!field->tx_ready) {
    return false;
  }

  flow_memcpy(pdu, field->tx_pdu, field->tx_len);
  *len = field->tx_len;
  field->tx_ready = false;

  return true;
}

void flow_field_drop_frames(flow_field_t* field, flow_u32 frames)
{
  field->drop_frames = frames;
}

void flow_field_on_egress(flow_field_t* field,
                          flow_u32 k,
                          const void* buffer,
                          size_t size)
{
  (void)k;

  if (size > FLOW_PDU_MAX) {
    return;
  }

  flow_memcpy(field->tx_pdu, buffer, (flow_u32)size);
  field->tx_len = (flow_u32)size;
  field->tx_ready = true;
}

size_t flow_field_on_recv(flow_field_t* field,
                          flow_u32 k,
                          void* buffer,
                          size_t size)
{
  (void)k;

  if (!field->rx_ready || (field->rx_len != (flow_u32)size)) {
    return 0u;
  }

  field->rx_ready = false;

  if (field->drop_frames > 0u) {
    /* Frame scartato di proposito: il master deve andare in watchdog. */
    field->drop_frames--;
    field->frames_dropped++;
    return 0u;
  }

  flow_memcpy(buffer, field->rx_pdu, field->rx_len);

  return size;
}

flow_u16 flow_field_on_session(flow_field_t* field, flow_u32 k)
{
  (void)k;

  /* Session ID dello slave: e' suo, non fa parte della negoziazione 1oo2. */
  return (flow_u16)field_next_random(field);
}
