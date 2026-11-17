/**
 * @file mcux_config.h
 * @brief Sostituto del mcux_config.h che il build west/Kconfig del SDK genera: solo i CONFIG_* letti dai driver e componenti che usiamo (valori uguali a quelli del build SDK di riferimento). Incluso in ogni file con -include (fw/cmake/imx94.cmake).
 * @ingroup flow
 *
 */
#ifndef SI_MCUX_CONFIG_H
#define SI_MCUX_CONFIG_H
#define CONFIG_CPU_CORTEX_M 1
#define CONFIG_UART_RETRY_TIMES 0
#define CONFIG_MU1_BUSY_POLL_COUNT 0
#endif
