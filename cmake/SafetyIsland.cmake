# Funzioni comuni ai moduli di safety-island.

set(SI_CMAKE_DIR "${CMAKE_CURRENT_LIST_DIR}")
set(SI_SCRIPTS "${PROJECT_SOURCE_DIR}/scripts")

# Strumenti scaricati: condivisi tra i preset, fuori da build/ (distclean non li riscarica).
set(SI_TOOLS_DIR "${PROJECT_SOURCE_DIR}/ext/tools" CACHE PATH "Dove scaricare toolchain e strumenti")

# si_host(<VAR>): host corrente come linux-x86_64 | linux-aarch64 | darwin-arm64 | darwin-x86_64 | windows-x86_64
function(si_host var)
    string(TOLOWER "${CMAKE_HOST_SYSTEM_NAME}" os)
    string(TOLOWER "${CMAKE_HOST_SYSTEM_PROCESSOR}" cpu)
    if(cpu MATCHES "^(x86_64|amd64)$")
        set(cpu x86_64)
    elseif(cpu MATCHES "^(aarch64|arm64)$")
        if(os STREQUAL "darwin")
            set(cpu arm64)
        else()
            set(cpu aarch64)
        endif()
    endif()
    set(${var} "${os}-${cpu}" PARENT_SCOPE)
endfunction()

# si_tool(<VAR> NAME <programma> VERSION <v> EXE <percorso> <host> <url> <sha256> [<host> <url> <sha256>...]
#         [RAW] [KEEP_TREE] [NO_PATH])
#
# Dichiara che un modulo ha bisogno di <programma>:
#   - se e' nel PATH (e non NO_PATH): <VAR> = quel percorso, <VAR>_DEPENDS vuoto
#   - altrimenti: <VAR> = ${SI_TOOLS_DIR}/<NAME>-<VERSION>/<EXE>, scaricato per l'host corrente
#     (sha256 verificato) al primo build che ne ha bisogno. <VAR>_DEPENDS va nei DEPENDS dei comandi che
#     lo usano; <VAR>_TARGET e' un target da usare con add_dependencies.
#   RAW: l'URL e' l'eseguibile. KEEP_TREE: l'archivio si estrae intero (toolchain), EXE relativo.
# Host: vedi si_host(); "any" vale per tutti. Va chiamata nella directory dei target che lo usano.
function(si_tool var)
    cmake_parse_arguments(T "RAW;KEEP_TREE;NO_PATH" "NAME;VERSION;EXE" "" ${ARGN})
    if(NOT T_NO_PATH)
        get_filename_component(exe_name "${T_EXE}" NAME)
        find_program(${var}_IN_PATH ${exe_name})
        if(${var}_IN_PATH)
            message(STATUS "${T_NAME}: ${${var}_IN_PATH}")
            set(${var} "${${var}_IN_PATH}" PARENT_SCOPE)
            set(${var}_DEPENDS "" PARENT_SCOPE)
            set(${var}_TARGET "" PARENT_SCOPE)
            return()
        endif()
    endif()
    si_host(host)
    set(url "")
    set(table ${T_UNPARSED_ARGUMENTS})
    while(table)
        list(POP_FRONT table h u h256)
        if(h STREQUAL host OR h STREQUAL "any")
            set(url "${u}")
            set(sha "${h256}")
            break()
        endif()
    endwhile()
    if(NOT url)
        message(FATAL_ERROR "${T_NAME} ${T_VERSION}: nessun download per l'host ${host}: installalo e mettilo nel PATH")
    endif()
    set(dir "${SI_TOOLS_DIR}/${T_NAME}-${T_VERSION}")
    set(exe "${dir}/${T_EXE}")
    set(stamp "${dir}/.sha256-${sha}")
    if(NOT TARGET si-tool-${T_NAME})
        add_custom_command(
            OUTPUT "${stamp}"
            COMMAND "${CMAKE_COMMAND}" -E rm -rf "${dir}"
            COMMAND "${CMAKE_COMMAND}" -E make_directory "${dir}"
            COMMAND "${CMAKE_COMMAND}" "-DURL=${url}" "-DSHA256=${sha}" "-DDEST=${dir}" "-DEXE=${T_EXE}"
                    "-DRAW=${T_RAW}" "-DKEEP_TREE=${T_KEEP_TREE}" -P "${SI_CMAKE_DIR}/DownloadTool.cmake"
            COMMAND "${CMAKE_COMMAND}" -E touch "${stamp}"
            COMMENT "${T_NAME} ${T_VERSION} (${host}): download"
            VERBATIM)
        add_custom_target(si-tool-${T_NAME} DEPENDS "${stamp}")
    endif()
    if(EXISTS "${stamp}")
        message(STATUS "${T_NAME}: ${exe}")
    else()
        message(STATUS "${T_NAME}: la ${T_VERSION} verra' scaricata al primo uso in ${dir}")
    endif()
    set(${var} "${exe}" PARENT_SCOPE)
    set(${var}_DEPENDS "${stamp}" PARENT_SCOPE)
    set(${var}_TARGET si-tool-${T_NAME} PARENT_SCOPE)
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
