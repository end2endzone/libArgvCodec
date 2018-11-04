add_executable(libargvcodec_samples
  ${LIBARGVCODEC_EXPORT_HEADER}
  ${LIBARGVCODEC_VERSION_HEADER}
  ${LIBARGVCODEC_CONFIG_HEADER}
  samples.cpp
)

# Force CMAKE_DEBUG_POSTFIX for executables
set_target_properties(libargvcodec_samples PROPERTIES DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_include_directories(libargvcodec_samples PRIVATE rapidassist)
target_include_directories(libargvcodec_samples PRIVATE ${GTEST_INCLUDE_DIR})
add_dependencies(libargvcodec_samples libargvcodec)
target_link_libraries(libargvcodec_samples PUBLIC rapidassist libargvcodec ${PTHREAD_LIBRARIES} ${GTEST_LIBRARIES} )
