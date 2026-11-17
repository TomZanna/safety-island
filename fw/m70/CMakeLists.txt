# App M7_0. Compilata dal superbuild (fw/CMakeLists.txt) con il toolchain e l'SDK di deps/.
# Core cm7_core0 + ruolo SAFE_A: main.c chiama safe_main(FLOW_CORE_SAFE_A) in fw/safe/.
cmake_minimum_required(VERSION 3.25)
project(m70 LANGUAGES C ASM)
include(../cmake/imx94.cmake)

imx94_firmware(m70 CORE cm7_core0 ROLE safe_a SOURCES main.c)
