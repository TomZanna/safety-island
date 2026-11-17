/**
 * @file flow_field.h
 * @brief Ruolo FIELDBUS: lo slave FSoE (il "campo") e il generatore di
 * ingressi.
 * @ingroup flow_field
 *
 * Non e' un componente di sicurezza: genera ingressi casuali, risponde ai
 * master e pubblica la risposta nella mailbox. Serve a far girare il 1oo2 e a
 * non far scattare il watchdog FSoE (gira ogni 2 ms).
 *
 * Il generatore e' deterministico (xorshift con seed costante): utile per
 * riprodurre una sequenza con gli stessi semi.
 */
#ifndef SI_FLOW_FIELD_H
#define SI_FLOW_FIELD_H

#include <fsoeslave.h>

#include "flow_config.h"
#include "flow_fsoe_app.h"
#include "flow_safe_io.h"
#include "flow_types.h"

typedef struct flow_field flow_field_t;

/** Riferimento passato a fsoeslave_init(): tipo proprio del ruolo fieldbus (in
 * questo binario non esiste il master), quindi nessun dispatch a runtime. */
typedef struct field_fsoe_ref
{
  flow_u32 magic; /**< FLOW_FSOE_REF_MAGIC: guardia sull'app_ref */
  flow_u32 k; /**< indice della connessione */
  flow_field_t* field; /**< stato del ruolo (mai NULL) */
} field_fsoe_ref_t;

/** @brief Stato del ruolo fieldbus (slave, campo simulato, contatori). */
struct flow_field
{
  field_fsoe_ref_t ref[FLOW_NUM_CONNECTIONS]; /**< app_ref dello stack */
  fsoeslave_t slave[FLOW_NUM_CONNECTIONS]; /**< istanze dello slave */
  fsoeslave_syncstatus_t sync[FLOW_NUM_CONNECTIONS]; /**< stato FSoE */

  flow_safe_inputs_t field_inputs[FLOW_NUM_CONNECTIONS]; /**< generati */
  flow_safe_outputs_t field_outputs[FLOW_NUM_CONNECTIONS]; /**< ricevuti */

  /** Frame ricevuto dal master (assemblato dal ruolo fieldbus). */
  flow_u8 rx_pdu[FLOW_PDU_MAX];
  flow_u32 rx_len;
  bool rx_ready;

  /** Risposta dello slave, pronta per la mailbox. */
  flow_u8 tx_pdu[FLOW_PDU_MAX];
  flow_u32 tx_len;
  bool tx_ready;

  flow_u32 bus_cycles; /**< cicli da 2 ms dall'avvio (base del processo) */
  flow_u32 rng; /**< seed dello xorshift deterministico */

  /** Stato del processo di campo simulato. */
  bool estop_ok; /**< E-stop rilasciato */
  bool door_closed; /**< porta chiusa */
  bool reset; /**< impulso di reset in corso */
  bool reset_given; /**< reset gia' dato in questa finestra di sicurezza */
  flow_u32 reset_window; /**< cicli residui dell'impulso (~120 ms) */
  bool inject_discrepancy; /**< canale B forzato diverso (comando RTT) */

  flow_u32 drop_frames; /**< frame da scartare (test del watchdog FSoE) */

  bool outputs_valid; /**< almeno una risposta valida ricevuta */
  bool enabled_seen; /**< il master ha inviato enable = 0x00C3 */
  flow_u32 frames_ok; /**< risposte di processo ricevute */
  flow_u32 frames_dropped; /**< frame scartati per drop_frames */
};

/** @brief Inizializza lo slave FSoE e il processo di campo simulato.
 *
 * @param[out] field         stato del ruolo fieldbus;
 * @param[in]  slave_address indirizzo dello slave (la connessione k usa
 *                           `slave_address + k`).
 * @note Se `fsoeslave_init` fallisce il ruolo va in stato fatale.
 */
void flow_field_init(flow_field_t* field, flow_u16 slave_address);

/** @brief Consegna allo slave il frame del master (uno per ciclo safe).
 *
 * @param[out] field stato del ruolo;
 * @param[in]  pdu   frame assemblato dal lato A55;
 * @param[in]  len   lunghezza del frame in byte.
 */
void flow_field_provide_rx(flow_field_t* field,
                           const flow_u8* pdu,
                           flow_u32 len);

/** @brief Un passo del processo di campo e dello slave FSoE.
 *
 * @param[in,out] field stato del ruolo (ingressi generati, uscite ricevute,
 *                      contatori).
 * @note Periodo 2 ms: mantiene vivo il watchdog FSoE del master.
 */
void flow_field_step(flow_field_t* field);

/** @brief Preleva la risposta dello slave da pubblicare in mailbox.
 *
 * @param[in,out] field stato del ruolo;
 * @param[out]    pdu   buffer di destinazione (FLOW_PDU_MAX byte);
 * @param[out]    len   lunghezza della risposta.
 * @return true se c'era una risposta pronta; false altrimenti.
 */
bool flow_field_take_tx(flow_field_t* field, flow_u8* pdu, flow_u32* len);

/** @brief Programma lo scarto di N frame (test del watchdog FSoE).
 *
 * @param[out] field  stato del ruolo;
 * @param[in]  frames numero di frame da scartare prima di consegnarli allo
 *                    slave.
 */
void flow_field_drop_frames(flow_field_t* field, flow_u32 frames);

/** @brief Callback di trasmissione: copia il frame generato dallo slave.
 *
 * @param[in,out] field  stato del ruolo;
 * @param[in]     k      indice della connessione;
 * @param[in]     buffer frame FSoE dello slave;
 * @param[in]     size   lunghezza del frame.
 */
void flow_field_on_egress(flow_field_t* field,
                          flow_u32 k,
                          const void* buffer,
                          size_t size);

/** @brief Callback di ricezione: consegna allo slave il frame del master.
 *
 * @param[in,out] field  stato del ruolo;
 * @param[in]     k      indice della connessione;
 * @param[out]    buffer buffer dello stack da riempire;
 * @param[in]     size   capacita' di `buffer`.
 * @return byte consegnati; 0 se non c'e' un frame o se va scartato (test del
 *         watchdog).
 */
size_t flow_field_on_recv(flow_field_t* field,
                          flow_u32 k,
                          void* buffer,
                          size_t size);

/** @brief Session ID dello slave (non fa parte della negoziazione 1oo2).
 *
 * @param[in,out] field stato del ruolo (avanza l'RNG deterministico);
 * @param[in]     k     indice della connessione.
 * @return Session ID generato.
 */
flow_u16 flow_field_on_session(flow_field_t* field, flow_u32 k);

#endif /* SI_FLOW_FIELD_H */
