/**
 * @file flow_mu18.c
 * @brief MU18: l'unica MU fra due core secondari dell'i.MX94 EVK (config SM
 * mx94evk).
 * @ingroup flow_mu18
 *
 */
#include "flow_mu18.h"

#include <stddef.h>

#include "flow_platform.h"
#include "fsl_device_registers.h"

static flow_mu18_stats_t s_stats;
static MU_Type* s_mu;

void flow_mu18_bind(flow_u32 base)
{
  s_mu = (base != 0u) ? (MU_Type*)base : NULL;
}

bool flow_mu18_available(void)
{
  return s_mu != NULL;
}

bool flow_mu18_send(flow_u32 word0, flow_u32 word1, flow_u32 timeout_us)
{
  MU_Type* mu = s_mu;
  const flow_u32 start = flow_platform_time_us();
  const flow_u32 empty = MU_TSR_TE0_MASK | MU_TSR_TE1_MASK;

  if (mu == NULL) {
    return false;
  }

  /* TR0/TR1 liberi prima di scrivere, poi la MU trasferisce da sola. */
  while ((mu->TSR & empty) != empty) {
    if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
      s_stats.tx_timeouts++;
      return false;
    }
  }

  mu->TR[0] = word0;
  mu->TR[1] = word1;
  s_stats.sent++;
  return true;
}

bool flow_mu18_recv(flow_u32* word0, flow_u32* word1, flow_u32 timeout_us)
{
  const flow_u32 start = flow_platform_time_us();

  if (s_mu == NULL) {
    return false;
  }

  for (;;) {
    if (flow_mu18_poll(word0, word1)) {
      return true;
    }
    if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
      s_stats.rx_timeouts++;
      return false;
    }
  }
}

bool flow_mu18_poll(flow_u32* word0, flow_u32* word1)
{
  MU_Type* mu = s_mu;
  const flow_u32 full = MU_RSR_RF0_MASK | MU_RSR_RF1_MASK;

  if ((mu == NULL) || (mu->RSR & full) != full) {
    return false;
  }

  /* La lettura di RR0/RR1 azzera RF0/RF1. */
  *word0 = mu->RR[0];
  *word1 = mu->RR[1];
  s_stats.received++;
  return true;
}

bool flow_mu18_ping(flow_u32 value, flow_u32 timeout_us, flow_u32* peer_value)
{
  flow_u32 word0 = 0u;
  flow_u32 word1 = 0u;

  if (!flow_mu18_send(value, ~value, timeout_us)) {
    return false;
  }

  if (!flow_mu18_recv(&word0, &word1, timeout_us)) {
    return false;
  }

  if (peer_value != NULL) {
    *peer_value = word0;
  }

  /* Vale solo l'eco del proprio valore: un ping incrociato con il peer viene
   * scartato (il chiamante riprova). */
  return (word0 == value) && (word1 == ~value);
}

const flow_mu18_stats_t* flow_mu18_get_stats(void)
{
  return &s_stats;
}
