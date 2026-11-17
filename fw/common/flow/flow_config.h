/**
 * @file flow_config.h
 * @brief Configurazione del virtual safe PLC: taglie, indirizzi e tempi, in un
 * posto solo (niente magic number sparsi).
 * @ingroup flow_config
 *
 * Non c'e' un "ruolo di compilazione": il ruolo lo sceglie il main.c dell'app
 * (fw/m70, fw/m71, fw/m33s) e il build linka solo i sorgenti di quel ruolo
 * (fw/safe/ oppure fw/fieldbus/).
 *
 * Rispetto a imx-flow/bare-metal/include/flow_config.h cambiano tre cose: gli
 * indirizzi della mailbox vengono dall'OCRAM condivisa (patch SM 0002) e non
 * dalla DDR, i ruoli si chiamano SAFE_A/SAFE_B/FIELDBUS invece di M7/M33, e
 * tempo e RNG arrivano dal target i.MX94 (flow_platform_imx94.c).
 */
#ifndef SI_FLOW_CONFIG_H
#define SI_FLOW_CONFIG_H

#include "flow_types.h"

/** Numero di connessioni FSoE indipendenti gestite da ogni master. */
#ifndef FLOW_NUM_CONNECTIONS
#  define FLOW_NUM_CONNECTIONS 1u
#endif

/** Process data di sicurezza in byte (pari, oppure 1): 4 byte sono due uint16
 * per gli ingressi e due per le uscite, la taglia con cui sono costruiti i
 * frame FSoE. */
#define FLOW_SAFE_DATA_SIZE 4u

/** Dimensione della PDU FSoE (ETG.5100 ch. 8.1.1): 11 byte con 4 di dati. */
/* Parentesi esplicite su ogni operando: MISRA 12.1 (precedenza resa
 * esplicita) e nessuna sorpresa quando la macro viene usata in un'altra
 * espressione. */
#define FLOW_FSOE_PDU_SIZE(data_size) \
  ((((data_size) == 1u)) ? (6u) : (((2u * (data_size)) + 3u)))
#define FLOW_PDU_MAX FLOW_FSOE_PDU_SIZE(FLOW_SAFE_DATA_SIZE) /**< 11 byte */
#define FLOW_PDU_MIN 6u /**< frame con un solo byte di dati */

/** Parametri della connessione FSoE: indirizzo dello slave, Connection ID e
 * watchdog di 100 ms (dieci cicli safe). */
#define FLOW_SLAVE_ADDRESS 0xFFFEu
#define FLOW_CONNECTION_ID 0x0003u
#define FLOW_WATCHDOG_MS 100u

/** Parametri applicativi scambiati nello stato Parameter (2 x uint16): master
 * e slave devono dichiarare gli stessi valori o la connessione e' rifiutata. */
#define FLOW_APP_PARAMETERS_SIZE 4u
#define FLOW_APP_PARAMETER_1 0x1234u
#define FLOW_APP_PARAMETER_2 0x5678u

/** Mailbox: 3 banchi. Il banco scelto da MIN(count_A, count_B) non e' mai
 * quello che un core sta scrivendo, neanche con un core uno o due pubblicazioni
 * indietro. */
#define FLOW_SHM_BANKS 3u

/** Memoria condivisa (patch SM 0002): la finestra OCRAM da 16 KB visibile a
 * M33S/M7_0/M7_1 e' divisa cosi':
 *   0x204D4000  flow_shm_t  mailbox frame FSoE (4 KB riservati)
 *   0x204D5000  flow_icc_t  cross-check 1oo2 + barrier di ciclo (1 KB)
 * Oggi flow_shm_t occupa 328 B; il resto dei 4 KB e i ~11 KB dopo l'ICC sono
 * riserva per crescita e guardia, non spazio gia' usato. */
#define FLOW_SHM_BASE_ADDR 0x0204D4000u
#define FLOW_ICC_BASE_ADDR 0x0204D5000u

/** Versione dell'interfaccia: 2 = ruoli SAFE_A/SAFE_B/FIELDBUS e heartbeat[3].
 * Il driver Linux di imx-flow (se riusato) va allineato a questi valori;
 * layout_hash cambia quando cambia la disposizione delle struct. */
#define FLOW_SHM_VERSION 2u /**< versione dell'ABI della mailbox */
#define FLOW_SHM_LAYOUT_HASH 0x00000002u /**< impronta del layout atteso */

/** Tempo. Il tick dei core safe viene da SysTick (board_millis); il tempo per
 * il watchdog FSoE da DWT->CYCCNT, per non avere una causa comune con il tick.
 */
#define FLOW_SAFE_CYCLE_MS 10u
#define FLOW_FIELD_CYCLE_MS 2u

/** Attesa massima del peer prima di dichiarare fallito il cross-check (bounded,
 * mai un loop infinito: si va in fail-safe). */
#ifndef FLOW_MU_TIMEOUT_US
#  define FLOW_MU_TIMEOUT_US 5000u
#endif
/** Attesa massima al rendezvous di ciclo (a 3: SAFE_A, SAFE_B, FIELDBUS). Il
 * ciclo lo apre il FIELDBUS dopo che i due core safe hanno finito: l'attesa
 * vale al massimo un periodo safe (10 ms) piu' il tempo che il fieldbus passa
 * a servire il proprio ciclo (2 ms). 30 ms sono tre periodi safe, un margine
 * comodo e comunque sotto il watchdog FSoE di 100 ms; allo scadere il ciclo si
 * apre lo stesso e chi e' vivo lo rileva dal cross-check. */
#ifndef FLOW_BARRIER_TIMEOUT_US
#  define FLOW_BARRIER_TIMEOUT_US 30000u
#endif

/** Logica safe: cicli consecutivi di discrepanza prima del fault latchato e
 * pattern di uscita abilitata (due bit per canale, atteso dallo slave). */
#define FLOW_DISCREPANCY_CYCLES 3u
#define FLOW_SAFE_OUTPUT_ENABLED 0x00C3u

/** MU18: unica MU fra due core secondari (lato A = M7_1, lato B = M7_0). Ha 4
 * registri TR e 4 RR da 32 bit: 16 byte per direzione, quindi porta solo il
 * doorbell {seq, tag, channel} + il CRC-32 del payload; il payload sta in
 * flow_icc (OCRAM). Gli indirizzi sono gli alias non-secure, uguali per tutti i
 * core: ogni core raggiunge fisicamente solo il proprio lato. */
#define FLOW_MU18_A_BASE 0x42B70000u /**< lato A: M7_1 (SAFE_B) */
#define FLOW_MU18_B_BASE 0x42B80000u /**< lato B: M7_0 (SAFE_A) */

/** Debug: con 0 il codice di log e il fault injection non vengono linkati. */
#ifndef FLOW_DEBUG
#  define FLOW_DEBUG 1
#endif

/** Log per ciclo: a 10 ms sarebbero 100 righe/s per core e il buffer RTT (4 KB,
 * no-block-skip) le perde. Di default solo eventi (stati, anomalie,
 * statistiche). */
#ifndef FLOW_LOG_VERBOSE
#  define FLOW_LOG_VERBOSE 0
#endif

/** RNG di banco per il Session ID (DWT + SysTick + core id). NON e' un TRNG:
 * con l'opzione a 0 flow_platform_random_u16() chiama flow_platform_fatal(),
 * cioe' log e board_fault() (breakpoint): nel build safety non c'e' ripiego. */
#ifndef FLOW_BENCH_RNG
#  define FLOW_BENCH_RNG 1
#endif

/** Build freestanding: false per le app, che linkano newlib-nano (memcpy ecc.).
 */
#ifndef FLOW_FREESTANDING
#  define FLOW_FREESTANDING 0
#endif

/** Ruolo che pubblica la meta' CRC del frame: lo definisce flow_shm_api.h
 * (FLOW_SHM_CRC_NODE = nodo SAFE_A, l'altro pubblica la meta' dati). */

#endif /* SI_FLOW_CONFIG_H */
