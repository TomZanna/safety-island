#include "flow_mu18.h"

#include "flow_platform.h"
#include "fsl_device_registers.h"

static flow_mu18_stats_t s_stats;

/* Il proprio lato della MU18: dipende dal ruolo, e' un vincolo hardware. Gli
 * altri ruoli (M33S) non hanno nessun lato MU18 raggiungibile. */
#if (FLOW_CORE_ID == FLOW_CORE_SAFE_A)
#define MU18_SELF ((MU_Type *)FLOW_MU18_B_BASE) /* M7_0 = lato B */
#define MU18_PRESENT 1
#elif (FLOW_CORE_ID == FLOW_CORE_SAFE_B)
#define MU18_SELF ((MU_Type *)FLOW_MU18_A_BASE) /* M7_1 = lato A */
#define MU18_PRESENT 1
#else
#define MU18_PRESENT 0
#endif

bool flow_mu18_available(void)
{
#if MU18_PRESENT
    return true;
#else
    return false;
#endif
}

bool flow_mu18_send(flow_u32 word0, flow_u32 word1, flow_u32 timeout_us)
{
#if MU18_PRESENT
    MU_Type *mu = MU18_SELF;
    const flow_u32 start = flow_platform_time_us();
    const flow_u32 empty = MU_TSR_TE0_MASK | MU_TSR_TE1_MASK;

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
#else
    (void)word0;
    (void)word1;
    (void)timeout_us;
    return false;
#endif
}

bool flow_mu18_recv(flow_u32 *word0, flow_u32 *word1, flow_u32 timeout_us)
{
#if MU18_PRESENT
    const flow_u32 start = flow_platform_time_us();

    for (;;) {
        if (flow_mu18_poll(word0, word1)) {
            return true;
        }
        if ((flow_u32)(flow_platform_time_us() - start) >= timeout_us) {
            s_stats.rx_timeouts++;
            return false;
        }
    }
#else
    (void)word0;
    (void)word1;
    (void)timeout_us;
    return false;
#endif
}

bool flow_mu18_poll(flow_u32 *word0, flow_u32 *word1)
{
#if MU18_PRESENT
    MU_Type *mu = MU18_SELF;
    const flow_u32 full = MU_RSR_RF0_MASK | MU_RSR_RF1_MASK;

    if ((mu->RSR & full) != full) {
        return false;
    }

    /* La lettura di RR0/RR1 azzera RF0/RF1. */
    *word0 = mu->RR[0];
    *word1 = mu->RR[1];
    s_stats.received++;
    return true;
#else
    (void)word0;
    (void)word1;
    return false;
#endif
}

bool flow_mu18_ping(flow_u32 value, flow_u32 timeout_us, flow_u32 *peer_value)
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

const flow_mu18_stats_t *flow_mu18_get_stats(void)
{
    return &s_stats;
}
