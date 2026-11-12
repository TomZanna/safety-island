cmake_minimum_required(VERSION 3.25)

# Superbuild: nessun linguaggio qui. I moduli orchestrano tool esterni (mkimage, typst, uuu, J-Link)
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

# Ordine: ele e sm producono gli input di image; board usa l'immagine.
add_subdirectory(ele)     # firmware ELE NXP (scaricato, EULA)
add_subdirectory(sm)      # System Manager: binari committati + build riproducibile in Docker
add_subdirectory(image)   # mkimage_imx8 + flash.bin
add_subdirectory(board)   # flash, run, reset, uart, debug (USB/WSL)
add_subdirectory(docs)    # typst

add_custom_target(si-help
    COMMAND "${CMAKE_COMMAND}" -E echo "Target (cmake --build --preset <preset> --target <t>, o make <t>):"
    COMMAND "${CMAKE_COMMAND}" -E echo "  image        build/<preset>/flash.bin: ELE + SM [+ M33S/M70/M71]"
    COMMAND "${CMAKE_COMMAND}" -E echo "  flash, run   immagine aggiornata e caricata in RAM via uuu (reset automatico)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  reset        board in serial download (comando reset del monitor SM)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  uart         console del debug monitor SM"
    COMMAND "${CMAKE_COMMAND}" -E echo "  debug        J-Link GDB server + gdb;  debug-flash: flash poi debug"
    COMMAND "${CMAKE_COMMAND}" -E echo "  inspect      container AHAB di flash.bin"
    COMMAND "${CMAKE_COMMAND}" -E echo "  ele          scarica il firmware ELE NXP (chiede la EULA)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  sm-verify    ricostruisce l'SM nel Docker e confronta con sm/ (sm-update: sovrascrive)"
    COMMAND "${CMAKE_COMMAND}" -E echo "  docs         build/<preset>/docs/safety-island.pdf;  docs-live: typst watch"
    COMMAND "${CMAKE_COMMAND}" -E echo "  docker-image il target image dentro un container"
    VERBATIM)
