# Documentazione typst. typst e' una dipendenza dichiarata: se non e' nel PATH viene scaricata
# (versione pinnata, sha256 verificato) al primo build di docs.
# 0.15.1: richiesta dai package pinnati in docs/lib/packages.typ (cetz 0.5.x e codly 2.x vogliono >= 0.15).
set(TYPST_VERSION "0.15.1" CACHE STRING "Versione typst scaricata se non e' nel PATH")
set(TYPST_SHA256 "a6d077d0a95eed5a2eba715b2dae06be954f624ccbf85758a03f389ded33118c"
    CACHE STRING "sha256 di typst-x86_64-unknown-linux-musl.tar.xz")
set(DOCS_VIEWER "" CACHE STRING "Visualizzatore per docs-live (es. wslview); vuoto = non apre nulla")
set(DOCS_PACKAGE_PATH "" CACHE PATH
    "Package typst locali (build offline): vuoto = registry + ~/.cache/typst")

si_tool(TYPST NAME typst VERSION ${TYPST_VERSION} EXE typst
    linux-x86_64 "https://github.com/typst/typst/releases/download/v${TYPST_VERSION}/typst-x86_64-unknown-linux-musl.tar.xz"
    ${TYPST_SHA256})

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.typ")
# Sorgenti citati dai listati (codly.new(path(...))): se cambia il codice, il PDF si rigenera.
set(quoted_sources
    "${PROJECT_SOURCE_DIR}/fw/common/flow/flow_config.h"
    "${PROJECT_SOURCE_DIR}/fw/common/flow/flow_shm_api.h"
    "${PROJECT_SOURCE_DIR}/fw/common/flow/flow_mu.h"
    "${PROJECT_SOURCE_DIR}/fw/common/flow/flow_icc.h"
    "${PROJECT_SOURCE_DIR}/fw/common/flow/flow_safe.c"
    "${PROJECT_SOURCE_DIR}/fw/cmake/imx94.cmake")
set(pdf "${CMAKE_BINARY_DIR}/docs/safety-island.pdf")

if(DOCS_PACKAGE_PATH)
    set(package_path --package-path "${DOCS_PACKAGE_PATH}")
endif()

add_custom_command(
    OUTPUT "${pdf}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/docs"
    COMMAND "${TYPST}" compile --root "${PROJECT_SOURCE_DIR}" ${package_path}
            "${CMAKE_CURRENT_SOURCE_DIR}/main.typ" "${pdf}"
    DEPENDS ${sources} ${quoted_sources} ${TYPST_DEPENDS}
    COMMENT "typst: docs/main.typ -> ${pdf}"
    VERBATIM)
add_custom_target(docs DEPENDS "${pdf}")

set(open "")
if(DOCS_VIEWER)
    set(open --open "${DOCS_VIEWER}")
endif()
add_custom_target(docs-live
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/docs"
    COMMAND "${CMAKE_COMMAND}" -E echo "typst watch: salva docs/*.typ e ${pdf} si rigenera (Ctrl-C per uscire)"
    COMMAND "${TYPST}" watch --root "${PROJECT_SOURCE_DIR}" ${package_path} ${open}
            "${CMAKE_CURRENT_SOURCE_DIR}/main.typ" "${pdf}"
    DEPENDS ${TYPST_DEPENDS}
    USES_TERMINAL VERBATIM)
