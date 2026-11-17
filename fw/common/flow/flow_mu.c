/**
 * @file flow_mu.c
 * @brief Messaggi di cross-check fra i due core safe (1oo2).
 * @ingroup flow_mu
 *
 * Il CRC copre header e payload con il campo `crc` azzerato, cosi' il valore
 * trasmesso e quello ricalcolato dal ricevente partono dagli stessi byte. La
 * sequence e' indipendente da quella FSoE e il confronto e' wrap-aware: un
 * messaggio con sequence uguale o troppo vecchia (differenza >= 0x8000) viene
 * scartato, cosi' un frame ritardato non viene preso per nuovo.
 *
 * flow_mu_exchange() fa due verifiche, non una: pubblica e attende il peer, poi
 * riverifica il messaggio ricevuto con flow_mu_verify(). Il timeout lo applica
 * il trasporto, e un solo messaggio invalido basta a far fallire lo scambio:
 * meglio un ciclo in fail-safe che un cross-check su dati dubbi.
 */
#include "flow_mu.h"

#include "flow_crc.h"
#include "flow_mem.h"
#include "flow_platform.h"

#define MU_SEQUENCE_HALF 0x8000u

/** @brief CRC-32 del messaggio MU, con il campo crc azzerato.
 *
 * @param[in] message messaggio da proteggere/verificare.
 * @return CRC-32 su header + payload (crc escluso).
 */
static flow_u32 mu_message_crc(const flow_mu_message_t* message)
{
  flow_mu_message_t copy = *message;

  copy.crc = 0u;

  return flow_crc32(&copy, (flow_u32)sizeof(copy));
}

/** @brief Il peer di un core safe. Nel 1oo2 i ruoli sono SAFE_A e SAFE_B.
 *
 * @param[in] self SAFE_A o SAFE_B.
 * @return l'altro ruolo safe.
 */
static flow_core_id_t mu_peer_of(flow_core_id_t self)
{
  return (self == FLOW_CORE_SAFE_A) ? FLOW_CORE_SAFE_B : FLOW_CORE_SAFE_A;
}

void flow_mu_init(flow_mu_state_t* state, flow_core_id_t self)
{
  if (state == NULL) {
    return;
  }

  flow_memset(state, 0, (flow_u32)sizeof(*state));
  state->self = self;
}

bool flow_mu_verify(const flow_mu_message_t* message,
                    flow_core_id_t self,
                    flow_u8 channel,
                    flow_u8 tag,
                    flow_u32 cycle)
{
  if (message == NULL) {
    return false;
  }

  if (message->magic != FLOW_MU_MAGIC) {
    return false;
  }
  if (message->version != FLOW_MU_VERSION) {
    return false;
  }
  if (message->crc != mu_message_crc(message)) {
    return false;
  }
  if ((message->source != mu_peer_of(self)) || (message->destination != self)
      || (message->channel != channel) || (message->tag != tag)
      || (message->cycle != cycle))
  {
    return false;
  }
  if (message->length > FLOW_MU_PAYLOAD_MAX) {
    return false;
  }

  return true;
}

bool flow_mu_exchange(flow_mu_state_t* state,
                      flow_u8 channel,
                      flow_u8 tag,
                      flow_u32 cycle,
                      const void* payload,
                      flow_u16 length,
                      flow_mu_message_t* peer,
                      flow_u32 timeout_us)
{
  flow_mu_message_t message;
  flow_mu_message_t received;
  flow_u16 difference;

  if ((state == NULL) || (peer == NULL) || (channel >= FLOW_MU_CHAN_COUNT)
      || (tag >= FLOW_MU_TAG_MAX) || (length > FLOW_MU_PAYLOAD_MAX)
      || ((payload == NULL) && (length != 0u)))
  {
    return false;
  }

  flow_memset(&message, 0, (flow_u32)sizeof(message));

  message.magic = FLOW_MU_MAGIC;
  message.version = FLOW_MU_VERSION;
  message.channel = channel;
  message.source = state->self;
  message.destination = mu_peer_of(state->self);
  message.tag = tag;
  message.cycle = cycle;
  state->tx_sequence[channel][tag]++;
  message.sequence = state->tx_sequence[channel][tag];
  message.length = length;

  if (length != 0u) {
    flow_memcpy(message.payload, payload, length);
  }

  message.crc = mu_message_crc(&message);

  if (!flow_platform_mu_post(state->self,
                             channel,
                             tag,
                             cycle,
                             &message,
                             (flow_u32)sizeof(message)))
  {
    return false;
  }

  if (!flow_platform_mu_wait(state->self,
                             channel,
                             tag,
                             cycle,
                             &received,
                             (flow_u32)sizeof(received),
                             timeout_us))
  {
    return false;
  }

  if (!flow_mu_verify(&received, state->self, channel, tag, cycle)) {
    return false;
  }

  /* Sequence indipendente da FSoE: deve essere piu' recente dell'ultimo visto
   * su questo canale/tag. Confronto wrap-aware. */
  difference = (flow_u16)(received.sequence - state->rx_sequence[channel][tag]);

  if ((difference == 0u) || (difference >= MU_SEQUENCE_HALF)) {
    return false;
  }

  state->rx_sequence[channel][tag] = received.sequence;
  *peer = received;

  return true;
}
