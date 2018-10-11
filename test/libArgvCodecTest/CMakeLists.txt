add_executable(libargvcodec_unittest
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  ArgumentLister.cpp
  ArgumentLister.h
  gtesthelper.cpp
  gtesthelper.h
  main.cpp
  shellCommandLines.old.txt
  shellCommandLines.txt
  TestArgumentList.cpp
  TestArgumentList.h
  TestCmdPromptArgumentCodec.cpp
  TestCmdPromptArgumentCodec.h
  TestCreateProcessArgumentCodec.cpp
  TestCreateProcessArgumentCodec.h
  TestEncodeCommandLine001.txt
  TestEncodeCommandLine002.txt
  TestUtils.cpp
  TestUtils.h
  TestValidator.cpp
  TestValidator.h
)

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libargvcodec_unittest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(libargvcodec_unittest 
  PRIVATE
    ${CMAKE_SOURCE_DIR}/test/libArgvCodecValidator  # for Validator.h
    ${GTEST_INCLUDE_DIR}
)
add_dependencies(libargvcodec_unittest libargvcodec)
target_link_libraries(libargvcodec_unittest PRIVATE libargvcodecvalidator rapidassist libargvcodec ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

install(TARGETS libargvcodec_unittest
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
