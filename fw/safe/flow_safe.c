/**
 * @file flow_safe.c
 * @brief Un core safe (m70 = SAFE_A, m71 = SAFE_B): stessa sorgente per
 * entrambi; il ruolo lo passa il main.c dell'app (safe_main), non una macro di
 * build. Ogni core ha la sua istanza dello stack FSoE master, il suo stato di
 * sequenza MU e i suoi canali TX nella mailbox.
 * @ingroup flow_safe
 *
 * Un ciclo (flow_safe_step) esegue quattro passi nell'ordine: latch dei frame
 * RX dalla mailbox, sync con lo stack FSoE (il cross-check del PDU in uscita
 * avviene dentro fsoeapp_send), cross-check delle SafeInputs, logica safe e
 * cross-check delle SafeOutputs. Ogni divergenza o timeout chiama
 * safe_note_anomaly(): il ciclo finisce in fail-safe, le uscite vanno a zero e
 * si chiede il reset, senza aspettare il ciclo successivo.
 *
 * I tre payload di cross-check (safe_session_message_t, safe_inputs_message_t,
 * safe_logic_message_t) portano solo cio' che i due core devono confrontare e
 * stanno nei 48 byte disponibili (static assert). L'egress porta anche il PDU
 * generato, perche' due master che producono frame diversi non sono un 1oo2.
 */
#include "flow_safe.h"

#include <fsoemaster.h>

#include "flow_crc.h"
#include "flow_mem.h"
#include "flow_platform.h"
#include "flow_shm.h"

/** @brief Tag di log del ruolo ("m70/A" o "m71/B").
 *
 * @param[in] safe stato del core.
 * @return stringa statica del ruolo.
 */
static const char* safe_tag(const flow_safe_t* safe)
{
  return flow_role_log_tag(safe->id);
}

FLOW_STATIC_ASSERT(FLOW_PDU_MAX
                       == (flow_u32)FSOEMASTER_FRAME_SIZE(FLOW_SAFE_DATA_SIZE),
                   "FLOW_PDU_MAX does not match the FSoE configuration");

/* ------------------------------------------------------------- payload MU */

typedef struct FLOW_PACKED safe_session_message
{
  flow_u16 k;
  flow_u16 random;
} safe_session_message_t;

typedef struct FLOW_PACKED safe_egress_message
{
  flow_u16 k;
  flow_u16 len;
  flow_u32 rx_fc;
  flow_u32 anomalies;
  flow_u8 pdu[FLOW_PDU_MAX];
} safe_egress_message_t;

typedef struct FLOW_PACKED safe_inputs_message
{
  flow_u16 k;
  flow_u16 flags;
  flow_u8 inputs[FLOW_SAFE_DATA_SIZE];
  flow_u8 state;
  flow_u8 sent_frame_type;
  flow_u8 recv_frame_type;
  flow_u8 reserved;
  flow_u32 tx_index;
  flow_u32 rx_index;
  flow_u32 rx_fc;
  flow_u32 anomalies;
} safe_inputs_message_t;

#define SAFE_INPUTS_PROCESS_DATA 0x0001u
#define SAFE_INPUTS_STATUS_OK 0x0002u
#define SAFE_INPUTS_SENDING 0x0004u

typedef struct FLOW_PACKED safe_logic_message
{
  flow_u16 k;
  flow_u16 logic_state;
  flow_u8 outputs[FLOW_SAFE_DATA_SIZE];
  flow_u32 anomalies;
} safe_logic_message_t;

FLOW_STATIC_ASSERT(sizeof(safe_egress_message_t) <= FLOW_MU_PAYLOAD_MAX,
                   "egress message too big");
FLOW_STATIC_ASSERT(sizeof(safe_inputs_message_t) <= FLOW_MU_PAYLOAD_MAX,
                   "inputs message too big");
FLOW_STATIC_ASSERT(sizeof(safe_logic_message_t) <= FLOW_MU_PAYLOAD_MAX,
                   "logic message too big");

/* ---------------------------------------------------------------- helpers */

/** @brief Minimo fra due contatori.
 *
 * @param[in] a primo valore;
 * @param[in] b secondo valore.
 * @return il minore dei due.
 */
static flow_u32 safe_min(flow_u32 a, flow_u32 b)
{
  return (a < b) ? a : b;
}

/** @brief Registra un'anomalia e porta il ciclo in fail-safe.
 *
 * @param[in,out] safe stato del core (incrementa `anomalies`, alza
 *                     `cycle_fault`);
 * @param[in]     what descrizione breve dell'anomalia (log RTT).
 */
static void safe_note_anomaly(flow_safe_t* safe, const char* what)
{
  safe->anomalies++;
  safe->cycle_fault = true;
  flow_platform_log(safe_tag(safe),
                    "cycle %u: ANOMALIA %s -> fail-safe",
                    (unsigned)safe->cycle,
                    what);
}

/** @brief Porta il contatore delle anomalie al massimo fra i due core.
 *
 * @param[in,out] safe stato del core;
 * @param[in]     peer contatore comunicato dal peer.
 * @note Serve a tenere i due core allineati sul numero di anomalie senza
 *       riportare all'indietro un contatore gia' avanzato.
 */
static void safe_merge_anomalies(flow_safe_t* safe, flow_u32 peer)
{
  if (peer > safe->anomalies) {
    safe->anomalies = peer;
  }
}

/** @brief Chiede allo stack FSoE il reset della connessione.
 *
 * @param[in,out] safe stato del core;
 * @param[in]     k    indice della connessione.
 */
static void safe_request_reset(flow_safe_t* safe, flow_u32 k)
{
  if (fsoemaster_set_reset_request_flag(&safe->master[k])
      == FSOEMASTER_STATUS_OK)
  {
    safe->stats.reset_requests++;
    flow_platform_log(safe_tag(safe), "k=%u reset richiesto", (unsigned)k);
  }
}

/** @brief Abilita o disabilita l'invio dei dati di processo, se cambia.
 *
 * @param[in,out] safe   stato del core;
 * @param[in]     k      indice della connessione;
 * @param[in]     enable nuovo stato richiesto.
 * @note Il flag locale si aggiorna solo se lo stack accetta la richiesta:
 *       cosi' il prossimo ciclo riprova.
 */
static void safe_set_sending(flow_safe_t* safe, flow_u32 k, bool enable)
{
  fsoemaster_status_t status;

  if (enable == safe->send_enabled[k]) {
    return;
  }

  if (enable) {
    status = fsoemaster_set_process_data_sending_enable_flag(&safe->master[k]);
  } else {
    status =
        fsoemaster_clear_process_data_sending_enable_flag(&safe->master[k]);
  }

  if (status == FSOEMASTER_STATUS_OK) {
    safe->send_enabled[k] = enable;
  }
}

/** @brief Bit di stato della logica da cross-checkare con il peer.
 *
 * @param[in] logic stato della logica di una connessione.
 * @return bit 0 = abilitato, bit 1 = fault latchato, bit 2 = discrepanza in
 *         corso.
 */
static flow_u16 safe_logic_state_bits(const flow_safe_logic_t* logic)
{
  flow_u16 bits = 0u;

  if (logic->enabled != 0u) {
    bits |= 0x0001u;
  }
  if (logic->latched_fault != 0u) {
    bits |= 0x0002u;
  }
  if (logic->discrepancy_count != 0u) {
    bits |= 0x0004u;
  }

  return bits;
}

/** @brief Il fault injection richiesto vale per questo ciclo?
 *
 * Attivo solo nel ciclo richiesto e solo con FLOW_DEBUG. Il fault di SESSION si
 * arma per la prossima negoziazione: quella avviene solo quando la connessione
 * riparte, non in un ciclo qualsiasi.
 *
 * @param[in,out] safe stato del core (disarma il fault quando scatta);
 * @param[in]     type tipo di fault da verificare.
 * @return true se il fault va iniettato in questo ciclo.
 */
static bool safe_fault_active(flow_safe_t* safe, flow_test_fault_t type)
{
#if FLOW_DEBUG
  if (safe->fault != type) {
    return false;
  }

  if (type == FLOW_TEST_FAULT_SESSION) {
    safe->fault = FLOW_TEST_FAULT_NONE;
    safe->fault_cycle = 0u;
    flow_platform_log(safe_tag(safe), "fault injection attivo (session)");
    return true;
  }

  if ((safe->fault_cycle == 0u) || (safe->cycle != safe->fault_cycle)) {
    return false;
  }

  safe->fault = FLOW_TEST_FAULT_NONE;
  safe->fault_cycle = 0u;
  flow_platform_log(
      safe_tag(safe), "fault injection attivo (tipo %u)", (unsigned)type);
  return true;
#else
  (void)safe;
  (void)type;
  return false;
#endif
}

void flow_safe_set_fault(flow_safe_t* safe,
                         flow_test_fault_t fault,
                         flow_u32 fault_cycle)
{
  safe->fault = fault;
  safe->fault_cycle = fault_cycle;
}

/* ------------------------------------------------------ passo 1: latch RX */

/** @brief Latcha l'ultimo frame RX pubblicato dal FIELDBUS.
 *
 * Copia il frame nella RAM locale solo se il contatore del canale e' cambiato;
 * uno slot non valido incrementa `rx_latch_failures` e lascia il precedente.
 *
 * @param[in,out] safe stato del core.
 */
static void safe_latch_rx(flow_safe_t* safe)
{
  flow_u32 k;

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    const flow_shm_channel_t* channel = &safe->shm->rx[k];
    flow_shm_slot_t slot;
    const flow_u32 published = flow_shm_count(channel);

    if ((published == 0u) || (published == safe->rx_count[k])) {
      continue; /* nessun frame nuovo dal black channel */
    }

    if (!flow_shm_read_index(channel, published, &slot)) {
      safe->stats.rx_latch_failures++;
      flow_platform_log(safe_tag(safe),
                        "k=%u slot RX %u non valido",
                        (unsigned)k,
                        (unsigned)published);
      continue;
    }

    safe->rx_slot[k].len = slot.len;
    safe->rx_slot[k].fc = slot.fc;
    flow_memcpy(safe->rx_slot[k].pdu, slot.pdu, slot.len);
    safe->rx_count[k] = published;
    safe->rx_ready[k] = true;
  }
}

/* ----------------------------------------------- passo 2: sync con lo stack */

/** @brief Un passo dello stack FSoE master.
 *
 * Azzera gli ingressi grezzi, esegue `fsoemaster_sync_with_slave` e logga i
 * cambi di stato e i reset. Un errore di sync lascia gli ingressi a zero: il
 * ciclo successivo li tratta come non validi (fail-safe).
 *
 * @param[in,out] safe stato del core.
 */
static void safe_sync(flow_safe_t* safe)
{
  flow_u32 k;

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    const fsoemaster_state_t previous_state = safe->sync[k].current_state;
    fsoemaster_status_t status;

    flow_memset(&safe->raw_inputs[k], 0, sizeof(safe->raw_inputs[k]));
    flow_memset(&safe->sync[k], 0, sizeof(safe->sync[k]));

    status = fsoemaster_sync_with_slave(&safe->master[k],
                                        &safe->safe_outputs[k],
                                        &safe->raw_inputs[k],
                                        &safe->sync[k]);

    if (status != FSOEMASTER_STATUS_OK) {
      flow_platform_log(
          safe_tag(safe), "k=%u fsoemaster_sync failed", (unsigned)k);
      continue;
    }

    if (safe->sync[k].current_state != previous_state) {
      flow_platform_log(
          safe_tag(safe),
          "k=%u stato %s -> %s",
          (unsigned)k,
          fsoemaster_state_description(previous_state),
          fsoemaster_state_description(safe->sync[k].current_state));
    }

    if (safe->sync[k].reset_event != FSOEMASTER_RESETEVENT_NONE) {
      safe->send_enabled[k] = false;
      flow_platform_log(
          safe_tag(safe),
          "k=%u reset da %s: %s",
          (unsigned)k,
          (safe->sync[k].reset_event == FSOEMASTER_RESETEVENT_BY_MASTER)
              ? "master"
              : "slave",
          fsoemaster_reset_reason_description(safe->sync[k].reset_reason));
    }
  }
}

/* ------------------------ passo 3: cross-check delle SafeInputs decodificate
 */

/** @brief Riallinea i contatori TX/RX al MIN con il peer.
 *
 * Serve dopo un fault: i core non devono rigiocare frame vecchi, si
 * risincronizzano sul punto comune piu' basso.
 *
 * @param[in,out] safe stato del core;
 * @param[in]     k    indice della connessione;
 * @param[in]     peer messaggio del peer (contatori).
 */
static void safe_realign_counters(flow_safe_t* safe,
                                  flow_u32 k,
                                  const safe_inputs_message_t* peer)
{
  const flow_u32 tx_common = safe_min(safe->tx_count[k], peer->tx_index);
  const flow_u32 rx_common = safe_min(safe->rx_count[k], peer->rx_index);

  if (tx_common != safe->tx_count[k]) {
    flow_shm_commit(&safe->shm->tx[k][safe->id], tx_common);
    safe->tx_count[k] = tx_common;
  }

  if (rx_common != safe->rx_count[k]) {
    safe->rx_count[k] = rx_common;
  }
}

/** @brief Cross-check delle SafeInputs decodificate dai due core.
 *
 * Confronta ingressi, stato FSoE, contatori di mailbox e anomalie. Una
 * divergenza svuota gli ingressi sicuri, disabilita l'invio e richiede il
 * reset: il core non prosegue con dati non concordati.
 *
 * @param[in,out] safe stato del core.
 */
static void safe_crosscheck_inputs(flow_safe_t* safe)
{
  flow_u32 k;

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    safe_inputs_message_t message;
    safe_inputs_message_t peer_message;
    flow_mu_message_t peer;
    bool match;

    flow_memset(&message, 0, sizeof(message));

    message.k = (flow_u16)k;
    message.state = (flow_u8)safe->sync[k].current_state;
    message.sent_frame_type = (flow_u8)safe->sync[k].sent_frame_type;
    message.recv_frame_type = (flow_u8)safe->sync[k].received_frame_type;
    message.tx_index = safe->tx_count[k];
    message.rx_index = safe->rx_count[k];
    message.rx_fc = safe->rx_slot[k].fc;
    message.anomalies = safe->anomalies;
    flow_memcpy(message.inputs, &safe->raw_inputs[k], FLOW_SAFE_DATA_SIZE);

    if (safe->sync[k].is_process_data_received) {
      message.flags |= SAFE_INPUTS_PROCESS_DATA;
    }
    if (safe->sync[k].reset_event == FSOEMASTER_RESETEVENT_NONE) {
      message.flags |= SAFE_INPUTS_STATUS_OK;
    }
    if (safe->send_enabled[k]) {
      message.flags |= SAFE_INPUTS_SENDING;
    }

    if (safe_fault_active(safe, FLOW_TEST_FAULT_INPUTS)) {
      message.inputs[0] ^= 0x01u;
    }

    if (!flow_mu_exchange(&safe->mu,
                          FLOW_MU_CHAN_INPUTS,
                          (flow_u8)k,
                          safe->cycle,
                          &message,
                          (flow_u16)sizeof(message),
                          &peer,
                          FLOW_MU_TIMEOUT_US)
        || (peer.length != sizeof(message)))
    {
      safe->stats.inputs_mismatches++;
      flow_memset(&safe->safe_inputs[k], 0, sizeof(safe->safe_inputs[k]));
      safe_set_sending(safe, k, false);
      safe_note_anomaly(safe, "timeout cross-check SafeInputs");
      safe_request_reset(safe, k);
      continue;
    }

    /* Copia nel tipo del messaggio invece di un cast del buffer: niente
     * conversione fra puntatori a oggetti diversi (11.3). */
    flow_memcpy(&peer_message, peer.payload, (flow_u32)sizeof(peer_message));
    safe_merge_anomalies(safe, peer_message.anomalies);

    if ((message.tx_index != peer_message.tx_index)
        || (message.rx_index != peer_message.rx_index)
        || (message.rx_fc != peer_message.rx_fc))
    {
      safe->stats.inputs_mismatches++;
      flow_memset(&safe->safe_inputs[k], 0, sizeof(safe->safe_inputs[k]));
      safe_set_sending(safe, k, false);
      safe_note_anomaly(safe, "contatori SHMEM / frame RX divergenti");
      safe_request_reset(safe, k);
      safe_realign_counters(safe, k, &peer_message);
      continue;
    }

    match =
        (flow_memcmp(message.inputs, peer_message.inputs, FLOW_SAFE_DATA_SIZE)
         == 0)
        && (message.flags == peer_message.flags)
        && (message.state == peer_message.state)
        && (message.anomalies == peer_message.anomalies);

    if (!match) {
      safe->stats.inputs_mismatches++;
      flow_memset(&safe->safe_inputs[k], 0, sizeof(safe->safe_inputs[k]));
      safe_set_sending(safe, k, false);
      safe_note_anomaly(safe, "SafeInputs divergenti");
      continue;
    }

    if ((message.flags & SAFE_INPUTS_PROCESS_DATA) == 0u) {
      flow_memset(&safe->safe_inputs[k], 0, sizeof(safe->safe_inputs[k]));
      if (safe->sync[k].current_state == FSOEMASTER_STATE_DATA) {
        safe_set_sending(safe, k, false);
      }
    } else {
      safe->safe_inputs[k] = safe->raw_inputs[k];
    }
  }
}

/* ------------------- passo 4: logica safe e cross-check delle SafeOutputs */

/** @brief Esegue la logica safe e ne fa il cross-check con il peer.
 *
 * Confronta lo stato della logica e le SafeOutputs dei due core; se coincidono
 * e il ciclo non e' in fault le pubblica, altrimenti le azzera e disabilita
 * l'invio.
 *
 * @param[in,out] safe stato del core.
 */
static void safe_run_logic(flow_safe_t* safe)
{
  flow_u32 k;

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    flow_safe_outputs_t next_outputs;
    safe_logic_message_t message;
    safe_logic_message_t peer_message;
    flow_mu_message_t peer;
    bool match;

    flow_memset(&next_outputs, 0, sizeof(next_outputs));
    flow_memset(&message, 0, sizeof(message));

    flow_safe_logic_step(&safe->safe_inputs[k], &safe->logic[k], &next_outputs);

    message.k = (flow_u16)k;
    message.logic_state = safe_logic_state_bits(&safe->logic[k]);
    flow_memcpy(message.outputs, &next_outputs, FLOW_SAFE_DATA_SIZE);
    message.anomalies = safe->anomalies;

    if (safe_fault_active(safe, FLOW_TEST_FAULT_LOGIC)) {
      message.outputs[0] ^= 0x01u;
    }

    if (!flow_mu_exchange(&safe->mu,
                          FLOW_MU_CHAN_LOGIC,
                          (flow_u8)k,
                          safe->cycle,
                          &message,
                          (flow_u16)sizeof(message),
                          &peer,
                          FLOW_MU_TIMEOUT_US)
        || (peer.length != sizeof(message)))
    {
      safe->stats.logic_mismatches++;
      flow_memset(&safe->safe_outputs[k], 0, sizeof(safe->safe_outputs[k]));
      safe_set_sending(safe, k, false);
      safe_note_anomaly(safe, "timeout cross-check SafeOutputs");
      safe_request_reset(safe, k);
      continue;
    }

    flow_memcpy(&peer_message, peer.payload, (flow_u32)sizeof(peer_message));
    safe_merge_anomalies(safe, peer_message.anomalies);

    match = (peer_message.logic_state == message.logic_state)
        && (flow_memcmp(
                peer_message.outputs, message.outputs, FLOW_SAFE_DATA_SIZE)
            == 0)
        && (peer_message.anomalies == message.anomalies);

    if (!match) {
      safe->stats.logic_mismatches++;
      flow_memset(&safe->safe_outputs[k], 0, sizeof(safe->safe_outputs[k]));
      safe_set_sending(safe, k, false);
      safe_note_anomaly(safe, "SafeOutputs divergenti");
      safe_request_reset(safe, k);
      continue;
    }

    if (safe->cycle_fault) {
      flow_memset(&safe->safe_outputs[k], 0, sizeof(safe->safe_outputs[k]));
      safe_set_sending(safe, k, false);
    } else {
      safe->safe_outputs[k] = next_outputs;
      safe_set_sending(safe, k, true);
    }
  }
}

/* ------------------------------------------------------------ cycle driver */

void flow_safe_step(flow_safe_t* safe, flow_u32 cycle)
{
  safe->cycle = cycle;
  safe->stats.cycles++;
  safe->cycle_fault = false;

  /* 1. latch dei frame ricevuti dalla mailbox */
  safe_latch_rx(safe);

  /* 2. sync dello stack; il cross-check del PDU in uscita e la pubblicazione
   *    della propria meta' avvengono dentro fsoeapp_send() */
  safe_sync(safe);

  /* 3. cross-check delle SafeInputs e dello stato della connessione */
  safe_crosscheck_inputs(safe);

  /* 4. logica safe e cross-check delle SafeOutputs */
  safe_run_logic(safe);

  flow_shm_update_heartbeat(safe->shm, safe->id, cycle);

  if ((FLOW_NUM_CONNECTIONS > 0u)
      && (safe->sync[0].current_state == FSOEMASTER_STATE_DATA)
      && safe->sync[0].is_process_data_received)
  {
    safe->stats.data_cycles++;
  }
}

/* --------------------------------------------- negoziazione del Session ID */

flow_u16 flow_safe_on_session(flow_safe_t* safe, flow_u32 k)
{
  safe_session_message_t message;
  safe_session_message_t peer_message;
  flow_mu_message_t peer;
  flow_u16 random;
  flow_u16 session;

  flow_memset(&message, 0, sizeof(message));

  random = flow_platform_random_u16();
  message.k = (flow_u16)k;
  message.random = random;

  if (safe_fault_active(safe, FLOW_TEST_FAULT_SESSION)) {
    message.random ^= 0x0100u;
  }

  safe->stats.session_requests++;

  if (!flow_mu_exchange(&safe->mu,
                        FLOW_MU_CHAN_SESSION,
                        0u,
                        safe->cycle,
                        &message,
                        (flow_u16)sizeof(message),
                        &peer,
                        FLOW_MU_TIMEOUT_US)
      || (peer.length != sizeof(message)))
  {
    safe_note_anomaly(safe, "negoziazione Session ID fallita");
    safe_request_reset(safe, 0u);
    return 0u;
  }

  flow_memcpy(&peer_message, peer.payload, (flow_u32)sizeof(peer_message));

  /* Session ID = somma dei due numeri casuali: i due core devono arrivare allo
   * stesso valore (requisito 1oo2). */
  session = (flow_u16)(random + peer_message.random);
  safe->session_id = session;

  flow_platform_log(safe_tag(safe),
                    "SESSION ID: 0x%04x + 0x%04x = 0x%04x",
                    (unsigned)random,
                    (unsigned)peer_message.random,
                    (unsigned)session);

  return session;
}

/* ----------------------------------------- passo 2b: cross-check del PDU out
 */

void flow_safe_on_egress(flow_safe_t* safe,
                         flow_u32 k,
                         const void* buffer,
                         size_t size)
{
  flow_u8 frame[FLOW_PDU_MAX];
  safe_egress_message_t message;
  safe_egress_message_t peer_message;
  flow_mu_message_t peer;
  flow_u8 part[FLOW_PDU_MAX];
  flow_shm_slot_t stored;
  const flow_u32 len = (flow_u32)size;
  const bool crc_part = (safe->id == FLOW_SHM_CRC_NODE);
  flow_u32 index;

  if ((k >= FLOW_NUM_CONNECTIONS) || (buffer == NULL) || (len == 0u)
      || (len > FLOW_PDU_MAX))
  {
    safe_note_anomaly(safe, "frame in uscita non valido");
    return;
  }

  /* Copia locale del frame: il buffer arriva come void* dallo stack FSoE, cosi'
   * si evita la conversione void* -> flow_u8* (11.5) e si ha una copia stabile
   * per il cross-check e per lo split. */
  flow_memcpy(frame, buffer, len);

  flow_memset(&message, 0, sizeof(message));
  message.k = (flow_u16)k;
  message.len = (flow_u16)len;
  message.rx_fc = safe->rx_slot[k].fc;
  message.anomalies = safe->anomalies;
  flow_memcpy(message.pdu, frame, len);

  if (safe_fault_active(safe, FLOW_TEST_FAULT_PDU) && (len > 1u)) {
    message.pdu[1] ^= 0xFFu;
  }

  if (!flow_mu_exchange(&safe->mu,
                        FLOW_MU_CHAN_EGRESS,
                        (flow_u8)k,
                        safe->cycle,
                        &message,
                        (flow_u16)sizeof(message),
                        &peer,
                        FLOW_MU_TIMEOUT_US)
      || (peer.length != sizeof(message)))
  {
    safe->stats.pdu_mismatches++;
    safe_note_anomaly(safe, "timeout cross-check del PDU in uscita");
    safe_request_reset(safe, k);
    return;
  }

  flow_memcpy(&peer_message, peer.payload, (flow_u32)sizeof(peer_message));
  safe_merge_anomalies(safe, peer_message.anomalies);

  if ((peer_message.len != message.len) || (peer_message.rx_fc != message.rx_fc)
      || (peer_message.anomalies != message.anomalies)
      || (flow_memcmp(peer_message.pdu, message.pdu, len) != 0))
  {
    safe->stats.pdu_mismatches++;
    safe_note_anomaly(safe, "i due core hanno generato PDU diversi");
    safe_request_reset(safe, k);
    return;
  }

  /* Si pubblica solo la propria meta': nessuno dei due core ha da solo un frame
   * FSoE valido, lo assembla il fieldbus. */
  flow_shm_split(part, frame, len, crc_part);

  index = safe->tx_count[k] + 1u;

  if (!flow_shm_stage(
          &safe->shm->tx[k][safe->id], index, len, safe->cycle, 0u, part))
  {
    safe_note_anomaly(safe, "scrittura della meta' TX in SHMEM fallita");
    safe_request_reset(safe, k);
    return;
  }

  if (!flow_shm_read_index(&safe->shm->tx[k][safe->id], index, &stored)
      || !flow_shm_part_matches(stored.pdu, frame, len, crc_part))
  {
    safe_note_anomaly(safe, "meta' TX corrotta in SHMEM (read-back)");
    safe_request_reset(safe, k);
    return;
  }

  safe->tx_count[k]++;
  flow_shm_commit(&safe->shm->tx[k][safe->id], safe->tx_count[k]);
  safe->stats.tx_published++;
}

size_t flow_safe_on_recv(flow_safe_t* safe,
                         flow_u32 k,
                         void* buffer,
                         size_t size)
{
  if ((k >= FLOW_NUM_CONNECTIONS) || (buffer == NULL)) {
    return 0u;
  }

  if (!safe->rx_ready[k]) {
    return 0u;
  }

  safe->rx_ready[k] = false;

  if (safe->rx_slot[k].len != (flow_u32)size) {
    safe->stats.rx_latch_failures++;
    return 0u;
  }

  flow_memcpy(buffer, safe->rx_slot[k].pdu, (flow_u32)size);

  return size;
}

void flow_safe_init(flow_safe_t* safe, flow_core_id_t id, flow_shm_t* shm)
{
  /* Parametri applicativi della connessione: costanti, usati solo qui (8.9).
   * Hanno durata statica, quindi il puntatore passato allo stack resta valido.
   */
  static const flow_u16 app_parameters[2] = {(flow_u16)FLOW_APP_PARAMETER_1,
                                             (flow_u16)FLOW_APP_PARAMETER_2};
  flow_u32 k;

  flow_memset(safe, 0, (flow_u32)sizeof(*safe));

  safe->id = id;
  safe->shm = shm;
  safe->fault = FLOW_TEST_FAULT_NONE;
  flow_mu_init(&safe->mu, id);

  for (k = 0u; k < FLOW_NUM_CONNECTIONS; k++) {
    const fsoemaster_cfg_t cfg = {
        .slave_address = (uint16_t)(FLOW_SLAVE_ADDRESS + k),
        .connection_id = (uint16_t)(FLOW_CONNECTION_ID + k),
        .watchdog_timeout_ms = (uint16_t)FLOW_WATCHDOG_MS,
        .application_parameters = app_parameters,
        .application_parameters_size = FLOW_APP_PARAMETERS_SIZE,
        .outputs_size = FLOW_SAFE_DATA_SIZE,
        .inputs_size = FLOW_SAFE_DATA_SIZE,
    };

    safe->app_ref[k].magic = FLOW_FSOE_REF_MAGIC;
    safe->app_ref[k].k = k;
    safe->app_ref[k].safe = safe;

    flow_safe_logic_init(&safe->logic[k]);

    if (fsoemaster_init(&safe->master[k], &cfg, &safe->app_ref[k])
        != FSOEMASTER_STATUS_OK)
    {
      flow_platform_log(
          safe_tag(safe), "k=%u fsoemaster_init fallita", (unsigned)k);
      flow_platform_fatal();
    }
  }

  flow_platform_log(safe_tag(safe),
                    "master FSoE pronto (%u connessioni, watchdog %u ms)",
                    (unsigned)FLOW_NUM_CONNECTIONS,
                    (unsigned)FLOW_WATCHDOG_MS);
}
