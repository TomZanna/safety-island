# flash.bin = container ELE (AHAB) + container con SM ed eventuali core secondari.
include(ExternalProject)

set(MKIMAGE_REPO "https://github.com/nxp-imx/imx-mkimage.git" CACHE STRING "Repo imx-mkimage")
set(MKIMAGE_TAG "lf-6.18.20-2.0.0" CACHE STRING "Tag imx-mkimage (commit 1b57785, quello usato nei test)")
set(MKIMAGE_EXE "" CACHE FILEPATH "mkimage_imx8 gia' compilato (vuoto = scarica e compila MKIMAGE_TAG)")
set(MSEL "0" CACHE STRING "mSel passato all'SM: sceglie il blocco 'MODE msel=N' del suo cfg")
set(M33S_BIN "" CACHE FILEPATH "Binario per M33_1 (M33S), opzionale")
set(M70_BIN "" CACHE FILEPATH "Binario per M7_0, opzionale")
set(M71_BIN "" CACHE FILEPATH "Binario per M7_1, opzionale")

if(MKIMAGE_EXE)
    set(mkimage "${MKIMAGE_EXE}")
    set(mkimage_target "")
else()
    find_program(SI_MAKE make REQUIRED)
    # Il Makefile NXP usa $(PWD) per il percorso di output: lo forziamo con MKIMG=
    ExternalProject_Add(mkimage
        GIT_REPOSITORY "${MKIMAGE_REPO}"
        GIT_TAG "${MKIMAGE_TAG}"
        GIT_SHALLOW TRUE
        GIT_PROGRESS FALSE
        UPDATE_DISCONNECTED TRUE
        PREFIX "${CMAKE_BINARY_DIR}/_deps/mkimage"
        BUILD_IN_SOURCE TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND "${SI_MAKE}" -s "MKIMG=<SOURCE_DIR>/mkimage_imx8" "CFLAGS=-O2 -Wall -std=c99" SOC=iMX94 bin
        INSTALL_COMMAND ""
        BUILD_BYPRODUCTS "<SOURCE_DIR>/mkimage_imx8")
    ExternalProject_Get_Property(mkimage SOURCE_DIR)
    set(mkimage "${SOURCE_DIR}/mkimage_imx8")
    set(mkimage_target mkimage)
endif()

# Indirizzi come iMX94/soc.mak di mkimage
set(core_bins "")
set(cores "")
if(M33S_BIN)
    file(REAL_PATH "${M33S_BIN}" b BASE_DIRECTORY "${PROJECT_SOURCE_DIR}")
    list(APPEND cores -m33 "${b}" 1 0x1FFC0000 0x309C0000)
    list(APPEND core_bins "${b}")
endif()
if(M70_BIN)
    file(REAL_PATH "${M70_BIN}" b BASE_DIRECTORY "${PROJECT_SOURCE_DIR}")
    list(APPEND cores -m7 "${b}" 0 0x0 0x303C0000)
    list(APPEND core_bins "${b}")
endif()
if(M71_BIN)
    file(REAL_PATH "${M71_BIN}" b BASE_DIRECTORY "${PROJECT_SOURCE_DIR}")
    list(APPEND cores -m7 "${b}" 1 0x0 0x302C0000)
    list(APPEND core_bins "${b}")
endif()

set(verify "")
if(SI_SM_SUMS)
    si_verify_sha256(v FILE "${SI_SM_IMAGE}" SUMS "${SI_SM_SUMS}")
    list(APPEND verify ${v})
endif()
if(SI_ELE_SUMS)
    si_verify_sha256(v FILE "${SI_ELE_IMAGE}" SUMS "${SI_ELE_SUMS}")
    list(APPEND verify ${v})
endif()

set(SI_FLASH_BIN "${CMAKE_BINARY_DIR}/flash.bin")
add_custom_command(
    OUTPUT "${SI_FLASH_BIN}"
    ${verify}
    COMMAND "${mkimage}" -soc IMX9 -cntr_version 2 -append "${SI_ELE_IMAGE}" -c -msel ${MSEL}
            -m33 "${SI_SM_IMAGE}" 0 0x1FFC0000 ${cores} -out "${SI_FLASH_BIN}"
    COMMAND "${Python3_EXECUTABLE}" -I "${SI_SCRIPTS}/ahab_walk.py" "${SI_FLASH_BIN}"
    DEPENDS "${SI_SM_IMAGE}" "${SI_ELE_IMAGE}" ${core_bins} "${mkimage}"
    COMMENT "flash.bin: SM ${SM_CONFIG}, msel=${MSEL}"
    VERBATIM)
add_custom_target(image ALL DEPENDS "${SI_FLASH_BIN}")
add_dependencies(image ele ${mkimage_target})
set(SI_FLASH_BIN "${SI_FLASH_BIN}" PARENT_SCOPE)

# image dentro un container (serve solo docker): riusa l'ELE gia' scaricato su questo host
file(RELATIVE_PATH ele_rel "${PROJECT_SOURCE_DIR}" "${SI_ELE_IMAGE}")
add_custom_target(docker-image
    COMMAND "${PROJECT_SOURCE_DIR}/docker/run.sh" image
            "-DELE_IMAGE=/work/${ele_rel}" "-DSM_CONFIG=${SM_CONFIG}" "-DMSEL=${MSEL}"
    COMMENT "image dentro docker (output in build/docker/)"
    USES_TERMINAL VERBATIM)
add_dependencies(docker-image ele)
