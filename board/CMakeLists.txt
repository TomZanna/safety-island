# Azioni sulla board: wrapper agli script (USB/usbip, uuu, seriale, J-Link).
# Seriali per by-id: stabili anche se l'ordine di attach dei ttyUSB cambia.
set(SM_UART "/dev/serial/by-id/usb-FTDI_Quad_RS232-HS-if03-port0" CACHE STRING "UART del debug monitor SM (LPUART2)")
set(JLINK_CORE "M33_0" CACHE STRING "Core per il J-Link: M33_0 | M33_1 | M7_0 | M7_1")
set_property(CACHE JLINK_CORE PROPERTY STRINGS M33_0 M33_1 M7_0 M7_1)
set(JLINK_PORT "2331" CACHE STRING "Porta del J-Link GDB server")
set(GDB_ELF "" CACHE FILEPATH "ELF per gdb (vuoto = SM per M33_0, ELF dell'app per gli altri)")

# uuu (mfgtools) nativo
set(UUU_VERSION "1.5.243")
set(uuu_url "https://github.com/nxp-imx/mfgtools/releases/download/uuu_${UUU_VERSION}")
si_tool(SI_UUU NAME uuu VERSION ${UUU_VERSION} EXE uuu RAW
    linux-x86_64  "${uuu_url}/uuu"         7ed83eba9dce63cd4edc3a28bcc0cf4341de046d6316bde4885018773087ea24
    linux-aarch64 "${uuu_url}/uuu_aarch64" d57389225fced3b5307fb003e19d5eb6c252b51c9b92e8cc68b95b1b173b604d
    darwin-arm64  "${uuu_url}/uuu_mac_arm" 7f2ce10d5e738af635b339696709e2c3df4984801cc6c3764cd1c207c7aac9b0)
add_custom_target(uuu-tool DEPENDS ${SI_UUU_DEPENDS})

# Reset della board (scripts/reset.sh): hardware via J-Link per default, BCU come alternativa,
# monitor SM come ultima risorsa
set(RESET_METHOD "auto" CACHE STRING "auto | jlink | bcu | monitor")
set_property(CACHE RESET_METHOD PROPERTY STRINGS auto jlink bcu monitor)
set(BCU_BOARD "imx943evk19b1" CACHE STRING "Profilo BCU: imx943evk19a0 (proto1, SW7-1 ON) | imx943evk19b1 (proto2 REV B1, SW7-1 OFF); vuoto = BCU non usato")
set(BCU_ID "" CACHE STRING "location_id dell'FTDI della EVK per bcu ('bcu lsftdi'); vuoto = ricavato da sysfs (0403:6011)")
set(BCU_VERSION "1.1.128")
set(bcu_url "https://github.com/nxp-imx/bcu/releases/download/bcu_${BCU_VERSION}")
si_tool(SI_BCU NAME bcu VERSION ${BCU_VERSION} EXE bcu RAW
    linux-x86_64 "${bcu_url}/bcu"     b93799531def776109e5f9416e177755df00c36a83f315fbd1451e0580eb6aa0
    darwin-arm64 "${bcu_url}/bcu_mac" efeffad3e2c6fff29d181f4a38c18d30b889ec07c1681b86ef371ee7d2ba9755)
add_custom_target(bcu-tool DEPENDS ${SI_BCU_DEPENDS})

set(common_env "SM_UART=${SM_UART}" "SM_CONFIG=${SM_CONFIG}" "JLINK_PORT=${JLINK_PORT}"
    "ELF=${GDB_ELF}" "GDB=${SI_GDB}" "APPS_OUT=${SI_APPS_OUT}" "UUU=${SI_UUU}"
    "RESET_METHOD=${RESET_METHOD}" "BCU=${SI_BCU}" "BCU_BOARD=${BCU_BOARD}" "BCU_ID=${BCU_ID}")
si_script_env(env ${common_env} "JLINK_CORE=${JLINK_CORE}")

add_custom_target(flash
    COMMAND ${env} "${SI_SCRIPTS}/flash.sh" "${SI_FLASH_BIN}"
    COMMENT "Caricamento in RAM via uuu" USES_TERMINAL VERBATIM)
add_dependencies(flash image uuu-tool)
if(BCU_BOARD)
    add_dependencies(flash bcu-tool)
endif()

add_custom_target(run)
add_dependencies(run flash)

add_custom_target(reset COMMAND ${env} "${SI_SCRIPTS}/reset.sh" USES_TERMINAL VERBATIM)
if(BCU_BOARD)
    add_dependencies(reset bcu-tool)
endif()
add_custom_target(uart  COMMAND ${env} "${SI_SCRIPTS}/uart.sh"  USES_TERMINAL VERBATIM)
add_custom_target(debug COMMAND ${env} "${SI_SCRIPTS}/debug.sh" USES_TERMINAL VERBATIM)
add_custom_target(debug-flash COMMAND ${env} "${SI_SCRIPTS}/debug.sh" USES_TERMINAL VERBATIM)
add_dependencies(debug-flash flash)

# Per core secondario: debug-<app> (debug.sh) con JLINK_CORE fissato
foreach(pair "m33s;M33_1" "m70;M7_0" "m71;M7_1")
    list(GET pair 0 short)
    list(GET pair 1 core)
    si_script_env(core_env ${common_env} "JLINK_CORE=${core}")
    add_custom_target(debug-${short} COMMAND ${core_env} "${SI_SCRIPTS}/debug.sh" USES_TERMINAL VERBATIM)
endforeach()

# USB in WSL: setup una tantum (sudo) e stato
add_custom_target(usb-setup COMMAND ${env} "${SI_SCRIPTS}/usb-setup.sh" USES_TERMINAL VERBATIM)
add_dependencies(usb-setup uuu-tool bcu-tool)
add_custom_target(usb-status COMMAND ${env} "${SI_SCRIPTS}/usb-status.sh" USES_TERMINAL VERBATIM)

# Console RTT da terminale (un canale per core, control block condiviso): rtt = tutti i core,
# rtt-<app> = solo quel core (input diretto). In VS Code l'RTT lo legge Cortex-Debug (rttConfig).
add_custom_target(rtt COMMAND ${env} "${SI_SCRIPTS}/rtt.sh" USES_TERMINAL VERBATIM)
foreach(app m33s m70 m71)
    si_script_env(rtt_env ${common_env} "RTT_CHANNEL=${app}")
    add_custom_target(rtt-${app} COMMAND ${rtt_env} "${SI_SCRIPTS}/rtt.sh" USES_TERMINAL VERBATIM)
endforeach()

add_custom_target(inspect
    COMMAND "${Python3_EXECUTABLE}" -I "${SI_SCRIPTS}/ahab_walk.py" "${SI_FLASH_BIN}" VERBATIM)
add_dependencies(inspect image)
