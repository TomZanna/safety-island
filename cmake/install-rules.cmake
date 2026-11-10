install(
    TARGETS lest_exe
    RUNTIME COMPONENT lest_Runtime
)

if(PROJECT_IS_TOP_LEVEL)
  include(CPack)
endif()
