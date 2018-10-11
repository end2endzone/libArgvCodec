set(LIBARGVCODEC_HEADER_FILES ""
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/ArgumentList.h
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/CmdPromptArgumentCodec.h
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/CreateProcessArgumentCodec.h
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/IArgumentDecoder.h
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/IArgumentEncoder.h
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/libargvcodec.h
  ${LIBARGVCODEC_INCLUDE_DIR}/libargvcodec/Macros.h
)

add_library(libargvcodec
  ${LIBARGVCODEC_HEADER_FILES}
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  ArgumentList.cpp
  CmdPromptArgumentCodec.cpp
  CreateProcessArgumentCodec.cpp
  dllmain.cpp
  os.cpp
  os.h
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libargvcodec PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

# Define include directories for exported code.
target_include_directories(libargvcodec
  PUBLIC
    $<INSTALL_INTERFACE:${LIBARGVCODEC_INSTALL_INCLUDE_DIR}>  # for clients using the installed library.
)
target_link_libraries(libargvcodec PUBLIC rapidassist)

install(TARGETS libargvcodec
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
