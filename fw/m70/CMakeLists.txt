# App M7_0. Compilata dal superbuild (fw/CMakeLists.txt) con il toolchain e l'SDK di deps/.
cmake_minimum_required(VERSION 3.25)
project(m70 LANGUAGES C ASM)
include(../cmake/imx94.cmake)

imx94_firmware(m70 CORE cm7_core0 SOURCES main.c)
