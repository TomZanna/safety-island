/**
 * @file mcux_config.h
 * @brief Sostituto del mcux_config.h che il build west/Kconfig del SDK genera:
 * solo i CONFIG_* letti dai driver e componenti che usiamo (valori uguali a
 * quelli del build SDK di riferimento). Incluso in ogni file con -include
 * (fw/cmake/imx94.cmake).
 * @ingroup flow
 */
#ifndef SI_MCUX_CONFIG_H
#define SI_MCUX_CONFIG_H
/** Gli header CMSIS/FSL scelgono i path Cortex-M. */
#define CONFIG_CPU_CORTEX_M 1
/** 0 = nessun retry nei driver UART: le app non usano UART. */
#define CONFIG_UART_RETRY_TIMES 0
/** 0 = fsl_mu.h non definisce MU1_BUSY_POLL_COUNT: le attese sulla MU sono
 * quelle del nostro driver (flow_mu18.c), non un timeout del SDK. */
#define CONFIG_MU1_BUSY_POLL_COUNT 0
#endif
