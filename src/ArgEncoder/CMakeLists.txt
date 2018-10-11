add_executable(argencoder
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  ArgEncoder.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(argencoder PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

add_dependencies(argencoder libargvcodec)
target_link_libraries(argencoder PRIVATE rapidassist libargvcodec )

install(TARGETS argencoder
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
