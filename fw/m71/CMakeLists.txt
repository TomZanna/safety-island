# App M7_1. Compilata dal superbuild (fw/CMakeLists.txt) con il toolchain e l'SDK di deps/.
cmake_minimum_required(VERSION 3.25)
project(m71 LANGUAGES C ASM)
include(../cmake/imx94.cmake)

imx94_firmware(m71 CORE cm7_core1 FLOW_ROLE master SOURCES main.c)
