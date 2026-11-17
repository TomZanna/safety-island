# Dipendenze del firmware, scaricate e pinnate:
#   - toolchain Arm GNU (gcc + gdb) per l'host corrente, sha256 verificato (o ARMGCC_DIR gia' installato)
#   - SDK MCUXpresso minimo via west (deps/west.yml) in ext/sdk/
# Esporta: SI_ARMGCC_DIR, SI_GDB, SI_TOOLCHAIN_TARGET, SI_SDK_ROOT, target "deps".

# --- toolchain -------------------------------------------------------------------------------------
set(ARMGCC_VERSION "15.2.rel1")
set(ARMGCC_DIR "" CACHE PATH "Toolchain arm-none-eabi gia' installato (vuoto = scarica la ${ARMGCC_VERSION})")
if(ARMGCC_DIR)
    if(NOT EXISTS "${ARMGCC_DIR}/bin/arm-none-eabi-gcc")
        message(FATAL_ERROR "ARMGCC_DIR=${ARMGCC_DIR}: manca bin/arm-none-eabi-gcc")
    endif()
    set(armgcc "${ARMGCC_DIR}")
    set(tc_target "")
    message(STATUS "toolchain: ${armgcc}")
else()
    si_host(host)
    set(tc_host_linux-x86_64 x86_64)
    set(tc_host_linux-aarch64 aarch64)
    set(tc_host_darwin-arm64 darwin-arm64)
    set(tch "${tc_host_${host}}")
    if(NOT tch)
        message(FATAL_ERROR "toolchain Arm: host ${host} non previsto, imposta ARMGCC_DIR")
    endif()
    set(tc_name "arm-gnu-toolchain-${ARMGCC_VERSION}-${tch}-arm-none-eabi")
    set(base "https://developer.arm.com/-/media/Files/downloads/gnu/${ARMGCC_VERSION}/binrel")
    si_tool(SI_ARMGCC_GCC NAME arm-gnu-toolchain VERSION ${ARMGCC_VERSION} NO_PATH KEEP_TREE
        EXE "${tc_name}/bin/arm-none-eabi-gcc"
        linux-x86_64  "${base}/arm-gnu-toolchain-${ARMGCC_VERSION}-x86_64-arm-none-eabi.tar.xz"
                      597893282ac8c6ab1a4073977f2362990184599643b4c5ee34870a8215783a16
        linux-aarch64 "${base}/arm-gnu-toolchain-${ARMGCC_VERSION}-aarch64-arm-none-eabi.tar.xz"
                      d061559d814b205ed30c5b7c577c03317ec447ca51cd5a159d26b12a5bbeb20c
        darwin-arm64  "${base}/arm-gnu-toolchain-${ARMGCC_VERSION}-darwin-arm64-arm-none-eabi.tar.xz"
                      1938a84b7105c192e3fb4fa5e893ba25f425f7ddab40515ae608cd40f68669a8)
    get_filename_component(armgcc "${SI_ARMGCC_GCC}" DIRECTORY)
    get_filename_component(armgcc "${armgcc}" DIRECTORY)
    set(tc_target ${SI_ARMGCC_GCC_TARGET})
endif()
# Link stabile per strumenti esterni (VS Code, gdb a mano): ext/toolchain -> toolchain in uso
file(MAKE_DIRECTORY "${PROJECT_SOURCE_DIR}/ext")
# Ricreare il link a ogni configure deve essere idempotente: CREATE_LINK fallisce se la
# destinazione esiste (e il target cambia quando cambia la versione della toolchain). Si rimuove
# solo se e' un symlink: un file o una directory vere restano e l'errore lo dice.
if(IS_SYMLINK "${PROJECT_SOURCE_DIR}/ext/toolchain")
    file(REMOVE "${PROJECT_SOURCE_DIR}/ext/toolchain")
endif()
file(CREATE_LINK "${armgcc}" "${PROJECT_SOURCE_DIR}/ext/toolchain" SYMBOLIC)
set(SI_ARMGCC_DIR "${armgcc}" PARENT_SCOPE)
set(SI_GDB "${armgcc}/bin/arm-none-eabi-gdb" PARENT_SCOPE)
set(SI_TOOLCHAIN_TARGET "${tc_target}" PARENT_SCOPE)

# --- venv per west e pylink ------------------------------------------------------------------------
# Target a parte: "make rtt" ha bisogno del venv (pylink) ma non dell'SDK, quindi non deve dipendere
# da deps-sdk (che fa west update).
add_custom_target(py-deps
    COMMAND "${SI_SCRIPTS}/py-deps.sh"
    COMMENT "venv ext/venv (west, pylink)"
    USES_TERMINAL VERBATIM)

# --- SDK via west ----------------------------------------------------------------------------------
set(manifest "${CMAKE_CURRENT_SOURCE_DIR}/west.yml")
file(SHA256 "${manifest}" manifest_sha)
string(SUBSTRING "${manifest_sha}" 0 16 manifest_sha)
set(stamp "${PROJECT_SOURCE_DIR}/ext/sdk/.synced-${manifest_sha}")
add_custom_command(
    OUTPUT "${stamp}"
    COMMAND "${CMAKE_COMMAND}" -E make_directory "${PROJECT_SOURCE_DIR}/ext/sdk"
    COMMAND "${SI_SCRIPTS}/deps.sh" "${stamp}"
    DEPENDS "${manifest}" "${SI_SCRIPTS}/deps.sh"
    COMMENT "SDK MCUXpresso: west update (deps/west.yml)"
    USES_TERMINAL VERBATIM)
add_custom_target(deps-sdk DEPENDS "${stamp}")
add_dependencies(deps-sdk py-deps)
# "make deps" forza comunque un west update (utile se qualcuno ha toccato ext/sdk a mano)
add_custom_target(deps
    COMMAND "${SI_SCRIPTS}/deps.sh" "${stamp}"
    COMMENT "SDK MCUXpresso: west update" USES_TERMINAL VERBATIM)
add_dependencies(deps py-deps)
if(tc_target)
    add_dependencies(deps ${tc_target})
endif()
set(SI_SDK_ROOT "${PROJECT_SOURCE_DIR}/ext/sdk/mcuxsdk" PARENT_SCOPE)
