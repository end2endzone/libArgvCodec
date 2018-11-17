add_executable(libargvcodec_unittest
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  ArgumentLister.cpp
  ArgumentLister.h
  main.cpp
  TestShellCommandLines.txt
  TestArgumentList.cpp
  TestArgumentList.h
  TestCmdPromptArgumentCodec.cpp
  TestCmdPromptArgumentCodec.h
  TestCreateProcessArgumentCodec.cpp
  TestCreateProcessArgumentCodec.h
  TestEncodeCommandLine001.txt
  TestEncodeCommandLine002.txt
  TestTerminalArgumentCodec.cpp
  TestTerminalArgumentCodec.h
  Test.CommandLines.Linux.txt
  TestUtils.cpp
  TestUtils.h
)

# Unit test projects requires to link with pthread if also linking with gtest
if(NOT WIN32)
  set(PTHREAD_LIBRARIES -pthread)
endif()

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libargvcodec_unittest PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(libargvcodec_unittest 
  PRIVATE
    ${GTEST_INCLUDE_DIR}
)
add_dependencies(libargvcodec_unittest libargvcodec)
target_link_libraries(libargvcodec_unittest PRIVATE rapidassist libargvcodec ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )

# Copy test files to $(ProjectDir) and $(OutDir)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TestShellCommandLines.txt         ${CMAKE_CURRENT_BINARY_DIR}/TestShellCommandLines.txt         COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TestEncodeCommandLine001.txt  ${CMAKE_CURRENT_BINARY_DIR}/TestEncodeCommandLine001.txt  COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TestEncodeCommandLine002.txt  ${CMAKE_CURRENT_BINARY_DIR}/TestEncodeCommandLine002.txt  COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Test.CommandLines.Linux.txt  ${CMAKE_CURRENT_BINARY_DIR}/Test.CommandLines.Linux.txt  COPYONLY)
set(TEST_FILES_BINARY_DIR ${EXECUTABLE_OUTPUT_PATH})
if(WIN32)
  set(TEST_FILES_BINARY_DIR ${TEST_FILES_BINARY_DIR}/${CMAKE_CFG_INTDIR})
endif()
add_custom_command(
  TARGET libargvcodec_unittest POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/test/libArgvCodecTest/TestShellCommandLines.txt         ${TEST_FILES_BINARY_DIR}/TestShellCommandLines.txt
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/test/libArgvCodecTest/TestEncodeCommandLine001.txt  ${TEST_FILES_BINARY_DIR}/TestEncodeCommandLine001.txt
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/test/libArgvCodecTest/TestEncodeCommandLine002.txt  ${TEST_FILES_BINARY_DIR}/TestEncodeCommandLine002.txt
  COMMAND ${CMAKE_COMMAND} -E copy ${PROJECT_SOURCE_DIR}/test/libArgvCodecTest/Test.CommandLines.Linux.txt  ${TEST_FILES_BINARY_DIR}/Test.CommandLines.Linux.txt
)

install(TARGETS libargvcodec_unittest
        EXPORT libargvcodec-targets
        ARCHIVE DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        LIBRARY DESTINATION ${LIBARGVCODEC_INSTALL_LIB_DIR}
        RUNTIME DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/TestShellCommandLines.txt       
              ${CMAKE_CURRENT_SOURCE_DIR}/TestEncodeCommandLine001.txt
              ${CMAKE_CURRENT_SOURCE_DIR}/TestEncodeCommandLine002.txt
              ${CMAKE_CURRENT_SOURCE_DIR}/Test.CommandLines.Linux.txt
        DESTINATION ${LIBARGVCODEC_INSTALL_BIN_DIR}
)
