# Documentazione del progetto: Doxygen unico per API, protocolli e pagine Markdown.
#
# Doxygen e' una dipendenza del target: il binario e' pinnato e scaricato in
# ext/tools/doxygen-<versione>/ (sha256 verificato). NO_PATH evita di usare un
# doxygen di sistema, cosi' la resa non cambia da una macchina all'altra.
# Cambiare versione = aggiornare anche URL e sha256 nella tabella si_tool qui sotto.
set(DOXYGEN_VERSION "1.18.0" CACHE STRING "Doxygen pinnato scaricato in ext/tools")
set(DOCS_PORT "8000" CACHE STRING "Porta di docs-serve")
option(DOCS_AWESOME_CSS "Tema doxygen-awesome-css committato in docs/theme" ON)

si_host(host)
if(host STREQUAL "linux-x86_64" OR host STREQUAL "darwin-arm64" OR host STREQUAL "darwin-x86_64")
    si_tool(DOXYGEN NAME doxygen VERSION ${DOXYGEN_VERSION} NO_PATH EXE doxygen
        linux-x86_64
            "https://github.com/doxygen/doxygen/releases/download/Release_1_18_0/doxygen-1.18.0.linux.bin.tar.gz"
            14fa81bdc34171edb5f1f02b1d60e74802f0439b77fa44e592565d517d72df90
        darwin-arm64
            "https://github.com/doxygen/doxygen/releases/download/Release_1_18_0/doxygen-1.18.0-mac-arm.zip"
            31d1c74467a9f6f456b4e6a4eff0c03e93d1ffa92da5fdf2d53a2f14ebaabbd7
        darwin-x86_64
            "https://github.com/doxygen/doxygen/releases/download/Release_1_18_0/doxygen-1.18.0-mac-intel.zip"
            8045d72f6f900fd430042339c1493d6eefcc85a7f990361fb57685682f16a2f9)
    set(DOXYGEN_CMD "${DOXYGEN}")
    set(DOXYGEN_CMD_DEPENDS "${DOXYGEN_DEPENDS}")
    set(DOXYGEN_CMD_TARGET "${DOXYGEN_TARGET}")
else()
    # Host senza binario precompilato: si usa quello nel PATH e, se manca o e'
    # troppo vecchio, solo il target docs fallisce (make image non deve
    # richiedere doxygen).
    find_program(DOXYGEN_CMD doxygen)
    set(DOXYGEN_CMD_DEPENDS "")
    set(DOXYGEN_CMD_TARGET "")
    if(DOXYGEN_CMD)
        execute_process(COMMAND "${DOXYGEN_CMD}" --version
            OUTPUT_VARIABLE DOXYGEN_FOUND_VERSION
            OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET)
        string(REGEX MATCH "^[0-9]+\\.[0-9]+\\.[0-9]+" DOXYGEN_FOUND_VERSION "${DOXYGEN_FOUND_VERSION}")
        if(DOXYGEN_FOUND_VERSION VERSION_LESS "${DOXYGEN_VERSION}")
            message(STATUS "doxygen ${DOXYGEN_FOUND_VERSION}: troppo vecchio per docs (serve ${DOXYGEN_VERSION})")
            set(DOXYGEN_CMD "")
        endif()
    endif()
    if(NOT DOXYGEN_CMD)
        message(STATUS "doxygen: non disponibile per ${host}; il target docs spieghera' cosa fare")
    endif()
endif()

if(DOCS_AWESOME_CSS)
    set(DOCS_HTML_EXTRA_STYLESHEET
        "${PROJECT_SOURCE_DIR}/docs/theme/doxygen-awesome-css/doxygen-awesome.css")
    set(DOCS_HTML_COLORSTYLE "LIGHT")
else()
    set(DOCS_HTML_EXTRA_STYLESHEET "")
    set(DOCS_HTML_COLORSTYLE "AUTO_LIGHT")
endif()

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in" "${CMAKE_BINARY_DIR}/docs/Doxyfile" @ONLY)

# Sorgenti che, se cambiano, rigenerano la documentazione. La libreria FSoE
# vendored e' esclusa: non si modifica e non si documenta come nostra.
file(GLOB_RECURSE doc_sources CONFIGURE_DEPENDS
    "${PROJECT_SOURCE_DIR}/fw/*.c"
    "${PROJECT_SOURCE_DIR}/fw/*.h"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.md"
    "${CMAKE_CURRENT_SOURCE_DIR}/*.dox"
    "${CMAKE_CURRENT_SOURCE_DIR}/theme/*.css"
    "${CMAKE_CURRENT_SOURCE_DIR}/theme/*.js")
list(FILTER doc_sources EXCLUDE REGEX "/fw/common/fsoe/")

if(DOXYGEN_CMD)
    add_custom_command(
        OUTPUT "${CMAKE_BINARY_DIR}/docs/html/index.html"
        COMMAND "${CMAKE_COMMAND}" -E rm -rf "${CMAKE_BINARY_DIR}/docs/html"
        COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/docs"
        COMMAND "${DOXYGEN_CMD}" "${CMAKE_BINARY_DIR}/docs/Doxyfile"
        DEPENDS ${doc_sources} "${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in"
                "${CMAKE_BINARY_DIR}/docs/Doxyfile" ${DOXYGEN_CMD_DEPENDS}
        COMMENT "doxygen: sorgenti + docs/ -> ${CMAKE_BINARY_DIR}/docs/html"
        VERBATIM)
    add_custom_target(docs DEPENDS "${CMAKE_BINARY_DIR}/docs/html/index.html")
    if(DOXYGEN_CMD_TARGET)
        add_dependencies(docs ${DOXYGEN_CMD_TARGET})
    endif()
else()
    add_custom_target(docs
        COMMAND "${CMAKE_COMMAND}" -E echo
                "doxygen >= ${DOXYGEN_VERSION} non disponibile: aggiornalo nel PATH oppure usa un host con download pinnato"
        COMMAND "${CMAKE_COMMAND}" -E false
        VERBATIM)
endif()

add_custom_target(docs-serve
    COMMAND "${CMAKE_COMMAND}" -E echo "docs: http://127.0.0.1:${DOCS_PORT}/ (Ctrl-C per uscire)"
    COMMAND "${Python3_EXECUTABLE}" -m http.server "${DOCS_PORT}" --bind 127.0.0.1
            --directory "${CMAKE_BINARY_DIR}/docs/html"
    DEPENDS docs
    USES_TERMINAL VERBATIM)
