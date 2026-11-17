/**
 * @file selftest_safe.c
 * @brief Scambio ICC fra i due core safe (payload in OCRAM + doorbell MU18 +
 * CRC).
 * @ingroup flow_safe
 *
 * Il probe e' piccolo e autodescrittivo (ruolo, cicli, magic): viene pubblicato
 * sullo slot ICC e atteso dal peer. I 50 tentativi con 20 ms di timeout
 * assorbono il disallineamento di boot fra i due core (uno puo' arrivare al
 * selftest mentre l'altro sta ancora inizializzando la mailbox), senza
 * bloccare per sempre: se il peer non risponde il sistema non parte.
 */
#include "selftest_safe.h"

#include "flow_icc.h"
#include "flow_mem.h"
#include "flow_mu.h"
#include "flow_platform.h"
#include "flow_selftest.h"

bool safe_selftest_icc(flow_core_id_t self)
{
  typedef struct FLOW_PACKED icc_probe
  {
    flow_u32 role;
    flow_u32 cycles;
    flow_u32 magic;
  } icc_probe_t;

  const flow_core_id_t peer =
      (self == FLOW_CORE_SAFE_A) ? FLOW_CORE_SAFE_B : FLOW_CORE_SAFE_A;
  icc_probe_t out;
  bool ok = false;
  flow_u16 attempt;

  out.role = self;
  out.cycles = flow_platform_cycles();
  out.magic = 0x1CC1u;

  for (attempt = 0u; (attempt < 50u) && !ok; attempt++) {
    icc_probe_t in;
    flow_u16 length = 0u;
    const flow_u32 cycle = flow_icc_cycle_current();

    flow_memset(&in, 0, sizeof(in));

    if (!flow_icc_post(self,
                       peer,
                       FLOW_MU_CHAN_SESSION,
                       0u,
                       cycle,
                       &out,
                       (flow_u16)sizeof(out)))
    {
      continue;
    }

    if (!flow_icc_wait(self,
                       peer,
                       FLOW_MU_CHAN_SESSION,
                       0u,
                       cycle,
                       &in,
                       (flow_u16)sizeof(in),
                       &length,
                       20000u))
    {
      continue;
    }

    ok = (length == sizeof(in)) && (in.magic == 0x1CC1u) && (in.role == peer);
  }

  flow_selftest_check("icc exchange", ok);
  return ok;
}
