add_executable(showargs
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  ShowArgs.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(showargs PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(showargs PRIVATE rapidassist libargvcodec)
add_dependencies(showargs libargvcodec)
target_link_libraries(showargs PRIVATE rapidassist libargvcodec )

install(TARGETS showargs
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
