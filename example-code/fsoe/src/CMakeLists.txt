#********************************************************************
#        _       _         _
#  _ __ | |_  _ | |  __ _ | |__   ___
# | '__|| __|(_)| | / _` || '_ \ / __|
# | |   | |_  _ | || (_| || |_) |\__ \
# |_|    \__|(_)|_| \__,_||_.__/ |___/
#
# www.rt-labs.com
# Copyright 2019 rt-labs AB, Sweden.
# See LICENSE file in the project root for full license information.
#*******************************************************************/

include(GenerateExportHeader)
include(pc-lint)

message (STATUS "Building for ${CMAKE_SYSTEM_NAME}")

# NOTE: add headers to make them show up in an IDE
set (FSOE_SOURCES
  ${FSOE_SOURCE_DIR}/include/fsoemaster.h
  ${FSOE_SOURCE_DIR}/include/fsoeslave.h
  ${FSOE_SOURCE_DIR}/include/fsoeapp.h
  ${FSOE_SOURCE_DIR}/include/fsoetypes.h
  fsoemaster.c
  fsoeslave.c
  fsoeapp.c
  fsoeframe.c
  fsoeframe.h
  fsoewatchdog.c
  fsoewatchdog.h
  fsoechannel.c
  fsoechannel.h
  fsoesracrc.c
  fsoesracrc.h
  fsoeutil.c
  fsoeutil.h
  fsoeoptions.c
  fsoelog.h
  fsoeport.h
  )

# Create library
add_library (fsoe
  ${FSOE_SOURCES}
  ${PORT_SOURCES}
  )
generate_export_header(fsoe
  BASE_NAME fsoe
  EXPORT_FILE_NAME ${FSOE_BINARY_DIR}/include/fsoeexport.h
  )
include_directories(${PORT_INCLUDES})
target_link_libraries(fsoe ${PORT_LIBS})

install (TARGETS fsoe DESTINATION bin)
install (FILES
  ${FSOE_SOURCE_DIR}/include/fsoemaster.h
  ${FSOE_SOURCE_DIR}/include/fsoeslave.h
  ${FSOE_BINARY_DIR}/include/fsoeexport.h
  ${FSOE_BINARY_DIR}/include/fsoeoptions.h
  DESTINATION include)

# Doxygen configuration
cmake_policy(SET CMP0057 NEW)
find_package(Doxygen)
if (DOXYGEN_FOUND)
  set(DOXYGEN_OPTIMIZE_OUTPUT_FOR_C YES)
  set(DOXYGEN_TYPEDEF_HIDES_STRUCT YES)
  set(DOXYGEN_EXTRACT_STATIC YES)
  set(DOXYGEN_STRIP_CODE_COMMENTS NO)
  set(DOXYGEN_MACRO_EXPANSION YES)
  set(DOXYGEN_EXPAND_ONLY_PREDEF YES)
  set(DOXYGEN_PREDEFINED FSOE_EXPORT)
  set(DOXYGEN_EXPAND_AS_DEFINED FSOE_EXPORT)
  set(DOXYGEN_COLLABORATION_GRAPH NO)
  set(DOXYGEN_INCLUDE_GRAPH NO)
  set(DOXYGEN_INCLUDED_BY_GRAPH NO)
  set(DOXYGEN_RECURSIVE NO)
  set(DOXYGEN_USE_MDFILE_AS_MAINPAGE README.md)
  doxygen_add_docs(docs
    ${FSOE_SOURCE_DIR}/README.md
    ${FSOE_SOURCE_DIR}/include
    ${FSOE_SOURCE_DIR}/src
    )
endif (DOXYGEN_FOUND)

# PC-lint configuration
if(COMMAND add_pc_lint)
  add_pc_lint(fsoe ${FSOE_SOURCES})
endif(COMMAND add_pc_lint)
