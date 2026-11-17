# App M33_1. Compilata dal superbuild (fw/CMakeLists.txt) con il toolchain e l'SDK di deps/.
cmake_minimum_required(VERSION 3.25)
project(m33s LANGUAGES C ASM)
include(../cmake/imx94.cmake)

imx94_firmware(m33s CORE cm33_core1 ROLE fieldbus SOURCES main.c)
