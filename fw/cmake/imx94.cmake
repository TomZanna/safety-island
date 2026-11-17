# imx94_firmware(<target> CORE cm7_core0|cm7_core1|cm33_core1 ROLE safe_a|safe_b|fieldbus SOURCES <file>...)
#
# Crea l'eseguibile <target>.elf (+ .bin, .map) per un core secondario dell'i.MX94, con l'SDK MCUXpresso
# minimo di SDK_ROOT (ext/sdk/mcuxsdk, sincronizzato da west) e il codice comune di fw/common.
# Variabili (dal superbuild): SDK_ROOT, WAIT_DEBUGGER. Log/input via RTT condiviso (fw/common/rtt_shared.c).
# ROLE aggiunge il virtual safe PLC e linka il codice del ruolo: safe_a/safe_b -> fw/safe/ (dove il
# main.c dell'app chiama safe_main(FLOW_CORE_SAFE_A/B)), fieldbus -> fw/fieldbus/ (fieldbus_main()).
# Gli altri file del sottosistema stanno in fw/common/flow/.
include_guard(GLOBAL)
set(FW_COMMON "${CMAKE_CURRENT_LIST_DIR}/../common")

function(imx94_firmware target)
    cmake_parse_arguments(F "" "CORE;ROLE" "SOURCES" ${ARGN})
    if(NOT SDK_ROOT OR NOT EXISTS "${SDK_ROOT}/devices/i.MX/i.MX943")
        message(FATAL_ERROR "SDK_ROOT=${SDK_ROOT} non valido: 'make deps'")
    endif()
    set(dev "${SDK_ROOT}/devices/i.MX/i.MX943/MIMX94398")
    set(core "${F_CORE}")

    # --- per core: CPU, cache, memoria (indirizzi dei TCM visti dal core, come i .ld del SDK) ---------
    if(core MATCHES "^cm7_core[01]$")
        set(cpu -mcpu=cortex-m7 -mfpu=fpv5-d16)
        set(cache_dir "${SDK_ROOT}/drivers/cache/armv7-m7")
        set(LD_VECTORS 0x00000000)
        set(LD_TEXT    0x00002000)
    elseif(core STREQUAL "cm33_core1")
        set(cpu -mcpu=cortex-m33 -mfpu=fpv5-sp-d16)
        set(cache_dir "${SDK_ROOT}/drivers/cache/xcache")
        set(LD_VECTORS 0x0FFC0000)
        set(LD_TEXT    0x0FFC2000)
    else()
        message(FATAL_ERROR "imx94_firmware: CORE '${core}' non supportato")
    endif()
    set(LD_TEXT_SIZE 0x3E000)
    set(LD_DATA 0x20003000)
    set(LD_DATA_SIZE 0x2B000)
    set(LD_NCACHE 0x2002E000)
    set(LD_NCACHE_SIZE 0xB000)
    set(ld "${CMAKE_CURRENT_BINARY_DIR}/${target}.ld")
    configure_file("${FW_COMMON}/tcm.ld.in" "${ld}" @ONLY)

    # --- sorgenti: SDK minimo + comune + app -----------------------------------------------------------
    file(GLOB scmi_sources "${SDK_ROOT}/components/scmi/*.c")
    set(sdk_sources
        "${dev}/gcc/startup_MIMX94398_${core}.S"
        "${dev}/system_MIMX94398_${core}.c"
        "${dev}/drivers/fsl_clock.c"
        "${SDK_ROOT}/drivers/common/fsl_common.c"
        "${SDK_ROOT}/drivers/common/fsl_common_arm.c"
        "${SDK_ROOT}/drivers/mu1/fsl_mu.c"
        "${cache_dir}/fsl_cache.c"
        ${scmi_sources}
        "${SDK_ROOT}/components/smt/smt.c"
        "${SDK_ROOT}/components/imx_sm_crc/sm_crc.c"
        "${SDK_ROOT}/components/misc_utilities/fsl_syscall_stub.c"
        "${SDK_ROOT}/components/misc_utilities/fsl_sbrk.c")
    add_executable(${target} ${F_SOURCES} "${FW_COMMON}/board.c" "${FW_COMMON}/rtt_shared.c" ${sdk_sources})
    set_target_properties(${target} PROPERTIES SUFFIX ".elf")
    set(fw_includes
        "${FW_COMMON}"
        "${SDK_ROOT}/arch/arm/CMSIS/Core/Include"
        "${SDK_ROOT}/arch/arm/CMSIS/Core/Include/m-profile"
        "${dev}" "${dev}/drivers" "${dev}/drivers/${core}"
        "${SDK_ROOT}/devices/i.MX/i.MX943/periph"
        "${SDK_ROOT}/drivers/common" "${SDK_ROOT}/drivers/mu1" "${cache_dir}"
        "${SDK_ROOT}/components/scmi" "${SDK_ROOT}/components/smt" "${SDK_ROOT}/components/imx_sm_crc"
        # solo l'header: system_*.c del SDK lo include; con SDK_DEBUGCONSOLE=2 PRINTF e' una no-op
        "${SDK_ROOT}/components/debug_console_lite")
    target_include_directories(${target} PRIVATE ${fw_includes})
    target_compile_definitions(${target} PRIVATE
        CPU_MIMX94398AVKM_${core} MIMX94398_${core}_SERIES
        __STARTUP_CLEAR_BSS MCUXPRESSO_SDK SDK_DEBUGCONSOLE=2
        $<$<CONFIG:Debug>:DEBUG>)

    set(arch -mthumb ${cpu} -mfloat-abi=hard)

    # --- virtual safe PLC (fw/safe, fw/fieldbus, fw/common/flow) ---------------------------------
    # ROLE: safe_a e safe_b linkano fw/safe/ (i due core del 1oo2), fieldbus linka fw/fieldbus/.
    # Il ruolo lo passa il main.c dell'app (safe_main(FLOW_CORE_SAFE_A/B), fieldbus_main()):
    # qui si scelgono solo i sorgenti, senza define di ruolo.
    if(F_ROLE)
        set(flow_dir "${FW_COMMON}/flow")
        set(fsoe_dir "${FW_COMMON}/fsoe")
        set(fw_root "${CMAKE_CURRENT_LIST_DIR}/..")

        if(F_ROLE MATCHES "^safe_(a|b)$")
            set(role_dir "${fw_root}/safe")
            set(role_sources
                "${role_dir}/safe_main.c"
                "${role_dir}/safe_platform.c"
                "${role_dir}/selftest_safe.c"
                "${role_dir}/flow_safe.c"
                "${role_dir}/flow_safe_logic.c"
                "${role_dir}/fsoe_app.c")
        elseif(F_ROLE STREQUAL "fieldbus")
            set(role_dir "${fw_root}/fieldbus")
            set(role_sources
                "${role_dir}/fieldbus_main.c"
                "${role_dir}/flow_field.c"
                "${role_dir}/flow_a55.c"
                "${role_dir}/fsoe_app.c")
        else()
            message(FATAL_ERROR "imx94_firmware: ROLE '${F_ROLE}' non previsto (safe_a|safe_b|fieldbus)")
        endif()

        # Il ruolo deve combaciare con il core: un errore qui non arriva in board.
        if((F_ROLE STREQUAL "safe_a" AND NOT core STREQUAL "cm7_core0") OR
           (F_ROLE STREQUAL "safe_b" AND NOT core STREQUAL "cm7_core1") OR
           (F_ROLE STREQUAL "fieldbus" AND NOT core STREQUAL "cm33_core1"))
            message(FATAL_ERROR "imx94_firmware: ROLE '${F_ROLE}' incoerente con CORE '${core}'")
        endif()

        # Stack FSoE di rt-labs come archivio statico separato: ogni file compilato a parte, link
        # finale con l'app. FSoEappl/ e' l'esempio NXP e non si compila; il port e' flow/port.
        add_library(fsoe-${target} STATIC
            "${fsoe_dir}/src/fsoemaster.c"
            "${fsoe_dir}/src/fsoeslave.c"
            "${fsoe_dir}/src/fsoeframe.c"
            "${fsoe_dir}/src/fsoechannel.c"
            "${fsoe_dir}/src/fsoewatchdog.c"
            "${fsoe_dir}/src/fsoesracrc.c"
            "${fsoe_dir}/src/fsoeutil.c"
            "${fsoe_dir}/src/fsoeoptions.c"
            "${fsoe_dir}/src/fsoeapp.c"
            "${flow_dir}/port/fsoeport.c")
        target_include_directories(fsoe-${target} PRIVATE
            "${fsoe_dir}/include" "${fsoe_dir}/src" "${flow_dir}" "${flow_dir}/port" ${fw_includes})
        target_compile_definitions(fsoe-${target} PRIVATE FSOE_STATIC_DEFINE)
        target_compile_options(fsoe-${target} PRIVATE ${arch} -include "${FW_COMMON}/mcux_config.h"
            -ffunction-sections -fdata-sections -fno-common -fno-builtin -w)

        target_sources(${target} PRIVATE
            "${flow_dir}/flow_mem.c"
            "${flow_dir}/flow_crc.c"
            "${flow_dir}/flow_shm.c"
            "${flow_dir}/flow_app_parameters.c"
            "${flow_dir}/flow_mu.c"
            "${flow_dir}/flow_icc.c"
            "${flow_dir}/flow_mu18.c"
            "${flow_dir}/flow_selftest.c"
            "${flow_dir}/flow_init.c"
            "${FW_COMMON}/flow_platform_imx94.c"
            ${role_sources})
        target_include_directories(${target} PRIVATE "${flow_dir}" "${flow_dir}/port" "${role_dir}"
            "${fsoe_dir}/include" "${fsoe_dir}/src")
        target_compile_definitions(${target} PRIVATE FSOE_STATIC_DEFINE)
        target_link_libraries(${target} PRIVATE fsoe-${target})
    endif()

    target_compile_options(${target} PRIVATE ${arch}
        -include "${FW_COMMON}/mcux_config.h"
        -ffunction-sections -fdata-sections -fno-common -fno-builtin
        $<$<COMPILE_LANGUAGE:C>:-std=gnu11 -Wall -Wextra -Wno-unused-parameter>
        $<$<COMPILE_LANGUAGE:ASM>:-x assembler-with-cpp>)
    # Solo board.c: cambiare WAIT_DEBUGGER (task "Run" / "Run (attesa debugger)") ricompila un file
    set_property(SOURCE "${FW_COMMON}/board.c" APPEND PROPERTY COMPILE_DEFINITIONS
        "WAIT_DEBUGGER=$<BOOL:${WAIT_DEBUGGER}>")
    # Il SDK non e' "nostro": niente warning dai suoi file
    set_source_files_properties(${sdk_sources} PROPERTIES COMPILE_OPTIONS "-w")
    target_link_options(${target} PRIVATE ${arch}
        -T "${ld}" --specs=nano.specs --specs=nosys.specs
        -Wl,--gc-sections -Wl,--no-warn-rwx-segments -Wl,--print-memory-usage
        "-Wl,-Map=$<TARGET_FILE_DIR:${target}>/${target}.map")
    set_property(TARGET ${target} APPEND PROPERTY LINK_DEPENDS "${ld}")
    add_custom_command(TARGET ${target} POST_BUILD
        COMMAND "${CMAKE_OBJCOPY}" -O binary "$<TARGET_FILE:${target}>" "$<TARGET_FILE_DIR:${target}>/${target}.bin"
        COMMAND "${CMAKE_SIZE}" "$<TARGET_FILE:${target}>"
        VERBATIM)
endfunction()
