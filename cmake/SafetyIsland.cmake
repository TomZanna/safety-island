# Funzioni comuni ai moduli di safety-island.

set(SI_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(SI_SCRIPTS "${PROJECT_SOURCE_DIR}/scripts")

# si_tool(<VAR> NAME <programma> VERSION <v> URL <url> SHA256 <hash> EXE <nome nell'archivio>)
#
# Dichiara che un modulo ha bisogno di <programma>:
#   - se e' nel PATH: <VAR> = quel percorso, <VAR>_DEPENDS vuoto
#   - altrimenti: <VAR> = percorso in build/_tools/, scaricato (sha256 verificato) al primo build
#     che ne ha bisogno. <VAR>_DEPENDS va messo nei DEPENDS dei comandi che lo usano.
# Va chiamata nella stessa directory dei target che usano il programma (regola delle custom command).
function(si_tool var)
    cmake_parse_arguments(T "" "NAME;VERSION;URL;SHA256;EXE" "" ${ARGN})
    find_program(${var}_IN_PATH ${T_NAME})
    if(${var}_IN_PATH)
        message(STATUS "${T_NAME}: ${${var}_IN_PATH}")
        set(${var} "${${var}_IN_PATH}" PARENT_SCOPE)
        set(${var}_DEPENDS "" PARENT_SCOPE)
        return()
    endif()
    set(dir "${CMAKE_BINARY_DIR}/_tools/${T_NAME}-${T_VERSION}")
    set(exe "${dir}/${T_EXE}")
    add_custom_command(
        OUTPUT "${exe}"
        COMMAND "${CMAKE_COMMAND}" "-DURL=${T_URL}" "-DSHA256=${T_SHA256}"
                "-DDEST=${dir}" "-DEXE=${T_EXE}" -P "${SI_CMAKE_DIR}/DownloadTool.cmake"
        COMMENT "${T_NAME} non trovato nel PATH: scarico la ${T_VERSION}"
        VERBATIM)
    message(STATUS "${T_NAME}: non nel PATH, la ${T_VERSION} verra' scaricata al primo uso")
    set(${var} "${exe}" PARENT_SCOPE)
    set(${var}_DEPENDS "${exe}" PARENT_SCOPE)
endfunction()

# si_verify_sha256(<OUT_LIST> FILE <file> SUMS <file di checksum stile sha256sum>)
# Restituisce in <OUT_LIST> il COMMAND da mettere in una custom command per verificare <file>
# al momento del build (non alla configurazione: il file puo' cambiare dopo).
function(si_verify_sha256 out)
    cmake_parse_arguments(V "" "FILE;SUMS" "" ${ARGN})
    set(${out} COMMAND "${CMAKE_COMMAND}" "-DFILE=${V_FILE}" "-DSUMS=${V_SUMS}"
               -P "${SI_CMAKE_DIR}/VerifySha256.cmake" PARENT_SCOPE)
endfunction()

# Ambiente passato agli script (scripts/*.sh): build dir e configurazione dalla cache CMake.
macro(si_script_env out)
    set(${out} "${CMAKE_COMMAND}" -E env "SI_BUILD_DIR=${CMAKE_BINARY_DIR}" ${ARGN})
endmacro()
