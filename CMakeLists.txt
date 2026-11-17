cmake_minimum_required(VERSION 3.25)

# Superbuild: nessun linguaggio qui. I moduli orchestrano tool esterni (mkimage, doxygen, uuu, J-Link)
# e, in futuro, le app dei core come sottoprogetti con il loro toolchain.
project(safety_island
    DESCRIPTION "i.MX94 EVK: System Manager e core secondari, build e run"
    LANGUAGES NONE)

if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "Build nella directory dei sorgenti non supportata: usa 'cmake --preset mx94evk'")
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
include(SafetyIsland)

find_package(Python3 REQUIRED COMPONENTS Interpreter)

# Ordine: deps (toolchain, SDK), poi ele, sm e fw producono gli input di image; board usa l'immagine.
add_subdirectory(deps)    # toolchain Arm + SDK MCUXpresso minimo (west, deps/west.yml)
add_subdirectory(ele)     # firmware ELE NXP (scaricato, EULA)
add_subdirectory(sm)      # System Manager: binari committati + build riproducibile in Docker
add_subdirectory(fw)      # app dei core secondari (fw/m33s, fw/m70, fw/m71)
add_subdirectory(image)   # mkimage_imx8 + flash.bin
add_subdirectory(board)   # flash, run, reset, uart, debug (USB/WSL)
add_subdirectory(docs)    # doxygen
add_subdirectory(quality) # clang-format/clang-tidy/cppcheck (opzionali)

add_custom_target(si-help
    COMMAND "${CMAKE_COMMAND}" -E echo "Target (cmake --build --preset <preset> --target <t>, o make <t>):"
    COMMAND "${CMAKE_COMMAND}" -E echo "  image        build/<preset>/flash.bin: ELE + SM [+ M33S/M70/M71]"
    COMMAND "${CMAKE_COMMAND}" -E echo "  flash, run   immagine aggiornata e caricata in RAM via uuu (reset automatico)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  reset        board in serial download (comando reset del monitor SM)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  uart         console del debug monitor SM"
    COMMAND "${CMAKE_COMMAND}" -E echo "  fw           app di fw/ (m33s, m70, m71); fw-<app> per una sola"
    COMMAND "${CMAKE_COMMAND}" -E echo "  deps         west update dell'SDK (deps/west.yml) + toolchain"
    COMMAND "${CMAKE_COMMAND}" -E echo "  usb-setup    setup una tantum di usbip/uuu/servizio di attach in WSL (sudo);  usb-status"
    COMMAND "${CMAKE_COMMAND}" -E echo "  debug        J-Link GDB server + gdb sul core JLINK_CORE;  debug-flash: flash poi debug"
    COMMAND "${CMAKE_COMMAND}" -E echo "  debug-m33s, debug-m70, debug-m71   debug su M33_1, M7_0, M7_1 con l'ELF dell'app"
    COMMAND "${CMAKE_COMMAND}" -E echo "  uart-m33s, uart-m70, uart-m71      console dell'app (picocom/screen)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  inspect      container AHAB di flash.bin"
    COMMAND "${CMAKE_COMMAND}" -E echo "  ele          scarica il firmware ELE NXP (chiede la EULA)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  sm-verify    ricostruisce l'SM nel Docker e confronta con sm/ (sm-update: sovrascrive)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  docs         build/<preset>/docs/html/index.html;  docs-serve: server locale"
    COMMAND "${CMAKE_COMMAND}" -E echo "  format       clang-format sui sorgenti fw/ (format-check: solo verifica)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  lint         clang-tidy su compile_commands.json (lint-fix: applica i fix)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  misra        cppcheck MISRA non certificato (misra-check: fallisce sulle violazioni)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  check        format-check + lint + report MISRA; check-misra: anche il gate MISRA"
    COMMAND "${CMAKE_COMMAND}" -E echo "  docker-image il target image dentro un container"
    COMMAND "${CMAKE_COMMAND}" -E echo "  clean, distclean, clean-all   pulizia (solo make): build del preset / tutti i build / anche ext e .west"
    VERBATIM)
