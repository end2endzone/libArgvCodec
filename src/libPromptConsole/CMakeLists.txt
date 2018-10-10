add_library(libpromptconsole
  libPromptConsole.h
  libPromptConsole.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libpromptconsole PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

install(TARGETS libpromptconsole
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
