# Firmware dei core secondari: un progetto CMake per app (fw/<app>/), compilato come ExternalProject
# con il toolchain arm-none-eabi e l'SDK minimo di deps/. Sempre ricompilato (incrementale): basta
# modificare fw/<app>/*.c, fw/safe/*.c, fw/fieldbus/*.c o fw/common/*.c e rifare "make image"/"make run".
# Uscite: build/<preset>/fw/<app>/<app>.{elf,bin,map} e compile_commands.json per l'IntelliSense.
include(ExternalProject)

option(FW_APPS "Compila le app di fw/ e le mette in flash.bin" ON)
set(FW_BUILD_TYPE "Debug" CACHE STRING "CMAKE_BUILD_TYPE delle app (Debug = -O0 -g)")
set_property(CACHE FW_BUILD_TYPE PROPERTY STRINGS Debug Release RelWithDebInfo MinSizeRel)
# Attesa del debugger all'avvio (breakpoint nell'init raggiungibili con l'attach)
option(M33S_WAIT_DEBUGGER "M33S aspetta il J-Link prima dell'init" OFF)
option(M70_WAIT_DEBUGGER "M7_0 aspetta il J-Link prima dell'init" OFF)
option(M71_WAIT_DEBUGGER "M7_1 aspetta il J-Link prima dell'init" OFF)

set(SI_APPS_OUT "${CMAKE_BINARY_DIR}/fw" PARENT_SCOPE)
foreach(app M33S M70 M71)
    set(SI_APP_${app}_BIN "" PARENT_SCOPE)
endforeach()
if(NOT FW_APPS)
    message(STATUS "firmware: disattivato (FW_APPS=OFF)")
    return()
endif()

set(deps deps-sdk ${SI_TOOLCHAIN_TARGET})
foreach(app m33s m70 m71)
    string(TOUPPER "${app}" APP)
    set(bin_dir "${CMAKE_BINARY_DIR}/fw/${app}")
    ExternalProject_Add(fw-${app}
        SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${app}"
        BINARY_DIR "${bin_dir}"
        PREFIX "${CMAKE_BINARY_DIR}/fw/_ep/${app}"
        CMAKE_ARGS
            "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_CURRENT_SOURCE_DIR}/cmake/toolchain-arm.cmake"
            "-DARMGCC_DIR=${SI_ARMGCC_DIR}"
            "-DSDK_ROOT=${SI_SDK_ROOT}"
            "-DCMAKE_BUILD_TYPE=${FW_BUILD_TYPE}"
            "-DWAIT_DEBUGGER=${${APP}_WAIT_DEBUGGER}"
            "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
        BUILD_ALWAYS TRUE
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS "${bin_dir}/${app}.elf" "${bin_dir}/${app}.bin"
        USES_TERMINAL_BUILD TRUE
        DEPENDS ${deps})
    set(SI_APP_${APP}_BIN "${bin_dir}/${app}.bin" PARENT_SCOPE)
endforeach()
# compile_commands.json unico (le tre app) per IntelliSense/clangd: build/<preset>/compile_commands.json
add_custom_target(fw
    COMMAND "${CMAKE_COMMAND}" "-DOUT=${CMAKE_BINARY_DIR}/compile_commands.json"
            "-DINPUTS=${CMAKE_BINARY_DIR}/fw/m70/compile_commands.json;${CMAKE_BINARY_DIR}/fw/m71/compile_commands.json;${CMAKE_BINARY_DIR}/fw/m33s/compile_commands.json"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/MergeCompileCommands.cmake"
    VERBATIM)
add_dependencies(fw fw-m33s fw-m70 fw-m71)
