# System Manager: i binari committati qui sono la sorgente di verita' per 'image'.
# sm-verify / sm-update li ricostruiscono da sorgenti nel Docker riproducibile (sm/Dockerfile).
set(SM_CONFIG "mx94evk" CACHE STRING "Config SM: mx94evk (Linux + M7) | mx94alt (demo SDK)")
set_property(CACHE SM_CONFIG PROPERTY STRINGS mx94evk mx94alt)
set(SM_BIN "" CACHE FILEPATH "SM alternativo da provare (vuoto = sm/sm_<SM_CONFIG>.bin); niente verifica sha256")
set(SM_TAG "lf-6.12.20-2.0.0" CACHE STRING "Tag imx-sm per sm-verify/sm-update (deve combaciare con ELE 2.0.2)")
set(SM_REPO "https://github.com/nxp-imx/imx-sm.git" CACHE STRING "Repo imx-sm")
set(SM_CONFIGS "mx94evk;mx94alt" CACHE STRING "Config compilate da sm-verify/sm-update")

if(SM_BIN)
    file(REAL_PATH "${SM_BIN}" img BASE_DIRECTORY "${PROJECT_SOURCE_DIR}")
    set(sums "")
else()
    set(img "${CMAKE_CURRENT_SOURCE_DIR}/sm_${SM_CONFIG}.bin")
    set(sums "${CMAKE_CURRENT_SOURCE_DIR}/SHA256SUMS")
    if(NOT EXISTS "${img}")
        message(FATAL_ERROR "Nessun SM per SM_CONFIG=${SM_CONFIG} in sm/ (config disponibili: ${SM_CONFIGS})")
    endif()
endif()
set(SI_SM_IMAGE "${img}" PARENT_SCOPE)
set(SI_SM_SUMS "${sums}" PARENT_SCOPE)

string(REPLACE ";" " " configs "${SM_CONFIGS}")
si_script_env(env "SM_TAG=${SM_TAG}" "SM_REPO=${SM_REPO}" "SM_CONFIGS=${configs}")
foreach(mode verify update)
    add_custom_target(sm-${mode}
        COMMAND ${env} "${SI_SCRIPTS}/sm-docker.sh" ${mode}
        COMMENT "SM ${SM_TAG}: build da sorgenti nel Docker (${mode})"
        USES_TERMINAL VERBATIM)
endforeach()
