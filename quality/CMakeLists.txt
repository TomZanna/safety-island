# Tool di qualita' opzionali: clang-format, clang-tidy e cppcheck.
#
# Non sono dipendenze di build: `make image`/`run`/`docs` funzionano senza.
# I target quality falliscono con l'istruzione di installazione se il tool
# manca; se la versione e' sotto la minima stampano un warning e proseguono
# (le distro cambiano build e sha256, quindi non si pinna un binario).
set(SI_CLANG_MIN_VERSION "19.1.7" CACHE STRING "Versione minima di clang-format/clang-tidy")
set(SI_CPPCHECK_MIN_VERSION "2.19.0" CACHE STRING "Versione minima di cppcheck")

find_program(SI_CLANG_FORMAT NAMES clang-format-19 clang-format)
find_program(SI_CLANG_TIDY NAMES clang-tidy-19 clang-tidy)
find_program(SI_CPPCHECK NAMES cppcheck)

# Estrae la prima versione x.y.z dall'output di --version.
function(si_quality_version exe out)
    if("${exe}" STREQUAL "")
        set(${out} "" PARENT_SCOPE)
        return()
    endif()
    execute_process(COMMAND "${exe}" --version
        OUTPUT_VARIABLE out_text ERROR_VARIABLE err_text RESULT_VARIABLE result)
    if(NOT result EQUAL 0)
        set(${out} "" PARENT_SCOPE)
        return()
    endif()
    string(REGEX MATCH "[0-9]+\\.[0-9]+\\.[0-9]+" version "${out_text} ${err_text}")
    set(${out} "${version}" PARENT_SCOPE)
endfunction()

si_quality_version("${SI_CLANG_FORMAT}" CLANG_FORMAT_VERSION)
si_quality_version("${SI_CLANG_TIDY}" CLANG_TIDY_VERSION)
si_quality_version("${SI_CPPCHECK}" CPPCHECK_FOUND_VERSION)

foreach(pair "clang-format;${SI_CLANG_FORMAT};${CLANG_FORMAT_VERSION};${SI_CLANG_MIN_VERSION}"
             "clang-tidy;${SI_CLANG_TIDY};${CLANG_TIDY_VERSION};${SI_CLANG_MIN_VERSION}"
             "cppcheck;${SI_CPPCHECK};${CPPCHECK_FOUND_VERSION};${SI_CPPCHECK_MIN_VERSION}")
    list(GET pair 0 tool)
    list(GET pair 1 path)
    list(GET pair 2 version)
    list(GET pair 3 minimum)
    if("${path}" STREQUAL "")
        message(STATUS "${tool}: assente (solo per i target quality; vedi README §Cosa serve)")
    elseif(version VERSION_LESS minimum)
        message(WARNING "${tool} ${version} < ${minimum}: i target quality proseguono, ma la resa puo' cambiare")
    else()
        message(STATUS "${tool}: ${path} (${version})")
    endif()
endforeach()

set(quality_scripts "${PROJECT_SOURCE_DIR}/scripts")
set(quality_env
    "${CMAKE_COMMAND}" -E env
    "SI_BUILD_DIR=${CMAKE_BINARY_DIR}"
    "SI_ARMGCC_DIR=${SI_ARMGCC_DIR}"
    "SI_CLANG_FORMAT=${SI_CLANG_FORMAT}"
    "SI_CLANG_TIDY=${SI_CLANG_TIDY}"
    "SI_CPPCHECK=${SI_CPPCHECK}"
    "SI_CLANG_MIN_VERSION=${SI_CLANG_MIN_VERSION}"
    "SI_CPPCHECK_MIN_VERSION=${SI_CPPCHECK_MIN_VERSION}")

add_custom_target(quality-tools
    COMMAND ${quality_env} "${quality_scripts}/quality-tools.sh" all
    COMMENT "quality: verifica tool e versioni"
    USES_TERMINAL VERBATIM)

add_custom_target(format
    COMMAND ${quality_env} "${quality_scripts}/format.sh" --fix
    COMMENT "clang-format: applica ai sorgenti fw/"
    USES_TERMINAL VERBATIM)

add_custom_target(format-check
    COMMAND ${quality_env} "${quality_scripts}/format.sh" --check
    COMMENT "clang-format: verifica i sorgenti fw/"
    USES_TERMINAL VERBATIM)

add_custom_target(lint
    COMMAND ${quality_env} "${quality_scripts}/lint.py"
    COMMENT "clang-tidy: analizza i sorgenti fw/ (compile_commands.json)"
    USES_TERMINAL VERBATIM)
if(TARGET fw)
    add_dependencies(lint fw)
endif()

add_custom_target(lint-fix
    COMMAND ${quality_env} "${quality_scripts}/lint.py" --fix
    COMMENT "clang-tidy: applica i fix (da rivedere)"
    USES_TERMINAL VERBATIM)
if(TARGET fw)
    add_dependencies(lint-fix fw)
endif()

add_custom_target(misra
    COMMAND ${quality_env} "${quality_scripts}/misra.sh"
    COMMENT "cppcheck MISRA (non certificato): report"
    USES_TERMINAL VERBATIM)
if(TARGET fw)
    add_dependencies(misra fw)
endif()

add_custom_target(misra-check
    COMMAND ${quality_env} "${quality_scripts}/misra.sh" --check
    COMMENT "cppcheck MISRA (non certificato): gate"
    USES_TERMINAL VERBATIM)
if(TARGET fw)
    add_dependencies(misra-check fw)
endif()

add_custom_target(check
    COMMAND ${quality_env} "${quality_scripts}/check.sh"
    COMMENT "quality: format-check + lint + report MISRA"
    USES_TERMINAL VERBATIM)
add_dependencies(check quality-tools)
if(TARGET fw)
    add_dependencies(check fw)
endif()

add_custom_target(check-misra
    COMMAND ${quality_env} "${quality_scripts}/check.sh" --strict
    COMMENT "quality: check + gate MISRA"
    USES_TERMINAL VERBATIM)
add_dependencies(check-misra quality-tools)
if(TARGET fw)
    add_dependencies(check-misra fw)
endif()
