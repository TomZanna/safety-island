# Firmware ELE NXP: proprietario, non committato. Lo scarica scripts/fetch-ele.sh.
set(ELE_URL "https://www.nxp.com/lgfiles/NMG/MAD/YOCTO/firmware-ele-imx-2.0.2-89161a8.bin"
    CACHE STRING "Pacchetto firmware-ele-imx (2.0.2: contiene mx943a0, la 1.3.0 no)")
option(ACCEPT_ELE_EULA "Ho letto e accetto la EULA NXP del firmware ELE (serve senza terminale, es. Docker)" OFF)
set(ELE_IMAGE "" CACHE FILEPATH "Container ELE gia' disponibile (vuoto = scarica ELE_URL); niente verifica sha256")

if(ELE_IMAGE)
    add_custom_target(ele)
    set(img "${ELE_IMAGE}")
    set(sums "")
else()
    set(img "${CMAKE_BINARY_DIR}/ele/mx943a0-ahab-container.img")
    set(sums "${CMAKE_CURRENT_SOURCE_DIR}/ele.sha256")
    si_script_env(env "ELE_URL=${ELE_URL}" "ACCEPT_ELE_EULA=$<IF:$<BOOL:${ACCEPT_ELE_EULA}>,1,0>")
    add_custom_command(
        OUTPUT "${img}"
        COMMAND ${env} "${SI_SCRIPTS}/fetch-ele.sh"
        DEPENDS "${sums}"
        COMMENT "Firmware ELE: download ed estrazione (EULA NXP)"
        USES_TERMINAL VERBATIM)
    add_custom_target(ele DEPENDS "${img}")
endif()

set(SI_ELE_IMAGE "${img}" PARENT_SCOPE)
set(SI_ELE_SUMS "${sums}" PARENT_SCOPE)
