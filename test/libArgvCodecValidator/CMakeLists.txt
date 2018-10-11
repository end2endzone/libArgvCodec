add_library(libargvcodecvalidator
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  advancedsort.h
  dllmain.cpp
  libargvcodecvalidator.h
  ValidationRule.cpp
  ValidationRule.h
  ValidationRuleList.cpp
  ValidationRuleList.h
  ValidationRuleSorter.cpp
  ValidationRuleSorter.h
  Validator.cpp
  Validator.h
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libargvcodecvalidator PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

add_dependencies(libargvcodecvalidator libargvcodec)
target_link_libraries(libargvcodecvalidator PRIVATE rapidassist libargvcodec )

install(TARGETS libargvcodecvalidator
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
