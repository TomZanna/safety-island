# Documentazione typst. typst e' una dipendenza dichiarata: se non e' nel PATH viene scaricata
# (versione pinnata, sha256 verificato) al primo build di docs.
set(TYPST_VERSION "0.13.1" CACHE STRING "Versione typst scaricata se non e' nel PATH")
set(TYPST_SHA256 "7d214bfeffc2e585dc422d1a09d2b144969421281e8c7f5d784b65fc69b5673f"
    CACHE STRING "sha256 di typst-x86_64-unknown-linux-musl.tar.xz")
set(DOCS_VIEWER "" CACHE STRING "Visualizzatore per docs-live (es. wslview); vuoto = non apre nulla")

si_tool(TYPST NAME typst VERSION ${TYPST_VERSION}
    URL "https://github.com/typst/typst/releases/download/v${TYPST_VERSION}/typst-x86_64-unknown-linux-musl.tar.xz"
    SHA256 ${TYPST_SHA256} EXE typst)

file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/*.typ")
set(pdf "${CMAKE_BINARY_DIR}/docs/safety-island.pdf")

add_custom_command(
    OUTPUT "${pdf}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${CMAKE_BINARY_DIR}/docs"
    COMMAND "${TYPST}" compile --root "${PROJECT_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/main.typ" "${pdf}"
    DEPENDS ${sources} ${TYPST_DEPENDS}
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
    COMMAND "${TYPST}" watch --root "${PROJECT_SOURCE_DIR}" ${open} "${CMAKE_CURRENT_SOURCE_DIR}/main.typ" "${pdf}"
    DEPENDS ${TYPST_DEPENDS}
    USES_TERMINAL VERBATIM)
