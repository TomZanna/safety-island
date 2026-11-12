# Azioni sulla board: wrapper agli script (USB, uuu.exe da WSL, seriale, J-Link).
set(SM_UART "/dev/ttyUSB3" CACHE STRING "UART del debug monitor SM")
set(JLINK_CORE "M33_0" CACHE STRING "Core per il J-Link: M33_0 | M33_1 | M7_0 | M7_1")
set_property(CACHE JLINK_CORE PROPERTY STRINGS M33_0 M33_1 M7_0 M7_1)
set(JLINK_PORT "2331" CACHE STRING "Porta del J-Link GDB server")
set(GDB_ELF "" CACHE FILEPATH "ELF per gdb (vuoto = build/<preset>/sm-docker/sm_<SM_CONFIG>.elf se esiste)")
set(ARM_TOOLS "" CACHE PATH "Directory con arm-gnu-toolchain-*-arm-none-eabi, per gdb (vuoto = cerca in /opt/tools e /)")

si_script_env(env "SM_UART=${SM_UART}" "SM_CONFIG=${SM_CONFIG}" "JLINK_CORE=${JLINK_CORE}"
    "JLINK_PORT=${JLINK_PORT}" "ELF=${GDB_ELF}" "TOOLS=${ARM_TOOLS}")

add_custom_target(flash
    COMMAND ${env} "${SI_SCRIPTS}/flash.sh" "${SI_FLASH_BIN}"
    COMMENT "Caricamento in RAM via uuu" USES_TERMINAL VERBATIM)
add_dependencies(flash image)

add_custom_target(run)
add_dependencies(run flash)

add_custom_target(reset COMMAND ${env} "${SI_SCRIPTS}/reset.sh" USES_TERMINAL VERBATIM)
add_custom_target(uart  COMMAND ${env} "${SI_SCRIPTS}/uart.sh"  USES_TERMINAL VERBATIM)
add_custom_target(debug COMMAND ${env} "${SI_SCRIPTS}/debug.sh" USES_TERMINAL VERBATIM)
add_custom_target(debug-flash COMMAND ${env} "${SI_SCRIPTS}/debug.sh" USES_TERMINAL VERBATIM)
add_dependencies(debug-flash flash)

add_custom_target(inspect
    COMMAND "${Python3_EXECUTABLE}" -I "${SI_SCRIPTS}/ahab_walk.py" "${SI_FLASH_BIN}" VERBATIM)
add_dependencies(inspect image)
