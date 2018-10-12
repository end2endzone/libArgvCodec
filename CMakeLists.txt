# CMakeLists.txt
cmake_minimum_required(VERSION 3.4.3 FATAL_ERROR)
project(libArgvCodec)

##############################################################################################################################################
# Standard CMake variables
##############################################################################################################################################

# BUILD_SHARED_LIBS is a standard CMake variable, but we declare it here to
# make it prominent in the GUI.
option(BUILD_SHARED_LIBS "Build shared libraries (DLLs)." OFF)

# Set a default build type if none was specified.
# See https://blog.kitware.com/cmake-and-the-default-build-type/
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  mark_as_advanced(CMAKE_BUILD_TYPE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# Export no symbols by default (if the compiler supports it). 
# This makes e.g. GCC's "visibility behavior" consistent with MSVC's.  
# On Windows/MSVC this is a noop. 
if (BUILD_SHARED_LIBS)
  set(CMAKE_C_VISIBILITY_PRESET hidden) 
  set(CMAKE_CXX_VISIBILITY_PRESET hidden) 
endif()

# Set the output folder where your program will be created
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(   LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)

##############################################################################################################################################
# CMake properties
##############################################################################################################################################
MESSAGE( STATUS "PROJECT_NAME:             " ${PROJECT_NAME} )
MESSAGE( STATUS "CMAKE_BINARY_DIR:         " ${CMAKE_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_BINARY_DIR: " ${CMAKE_CURRENT_BINARY_DIR} )
MESSAGE( STATUS "CMAKE_SOURCE_DIR:         " ${CMAKE_SOURCE_DIR} )
MESSAGE( STATUS "CMAKE_CURRENT_SOURCE_DIR: " ${CMAKE_CURRENT_SOURCE_DIR} )
MESSAGE( STATUS "PROJECT_BINARY_DIR:       " ${PROJECT_BINARY_DIR} )
MESSAGE( STATUS "PROJECT_SOURCE_DIR:       " ${PROJECT_SOURCE_DIR} )
MESSAGE( STATUS "EXECUTABLE_OUTPUT_PATH:   " ${EXECUTABLE_OUTPUT_PATH} )
MESSAGE( STATUS "LIBRARY_OUTPUT_PATH:      " ${LIBRARY_OUTPUT_PATH} )
MESSAGE( STATUS "CMAKE_MODULE_PATH:        " ${CMAKE_MODULE_PATH} )
MESSAGE( STATUS "CMAKE_COMMAND:            " ${CMAKE_COMMAND} )
MESSAGE( STATUS "CMAKE_ROOT:               " ${CMAKE_ROOT} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_FILE:  " ${CMAKE_CURRENT_LIST_FILE} )
MESSAGE( STATUS "CMAKE_CURRENT_LIST_LINE:  " ${CMAKE_CURRENT_LIST_LINE} )
MESSAGE( STATUS "CMAKE_INCLUDE_PATH:       " ${CMAKE_INCLUDE_PATH} )
MESSAGE( STATUS "CMAKE_LIBRARY_PATH:       " ${CMAKE_LIBRARY_PATH} )
MESSAGE( STATUS "CMAKE_SYSTEM:             " ${CMAKE_SYSTEM} )
MESSAGE( STATUS "CMAKE_SYSTEM_NAME:        " ${CMAKE_SYSTEM_NAME} )
MESSAGE( STATUS "CMAKE_SYSTEM_VERSION:     " ${CMAKE_SYSTEM_VERSION} )
MESSAGE( STATUS "CMAKE_SYSTEM_PROCESSOR:   " ${CMAKE_SYSTEM_PROCESSOR} )

##############################################################################################################################################
# Global settings
##############################################################################################################################################

# Product version according to Semantic Versioning v2.0.0 https://semver.org/
SET(LIBARGVCODEC_VERSION_MAJOR 1)
SET(LIBARGVCODEC_VERSION_MINOR 1)
SET(LIBARGVCODEC_VERSION_PATCH 0)
set(LIBARGVCODEC_VERSION ${LIBARGVCODEC_VERSION_MAJOR}.${LIBARGVCODEC_VERSION_MINOR}.${LIBARGVCODEC_VERSION_PATCH})

# read license file
file(READ ${CMAKE_SOURCE_DIR}/LICENSE.h LICENSE)

# version.h file
set(LIBARGVCODEC_VERSION_HEADER ${CMAKE_BINARY_DIR}/include/libargvcodec/version.h)
message("Generating ${LIBARGVCODEC_VERSION_HEADER}...")
configure_file( ${CMAKE_SOURCE_DIR}/src/libargvcodec/version.h.in ${LIBARGVCODEC_VERSION_HEADER} )

# config.h file
set(LIBARGVCODEC_CONFIG_HEADER ${CMAKE_BINARY_DIR}/include/libargvcodec/config.h)
message("Generating ${LIBARGVCODEC_CONFIG_HEADER}...")
if (BUILD_SHARED_LIBS)
  set(LIBARGVCODEC_BUILD_TYPE_CPP_DEFINE "#define LIBARGVCODEC_BUILT_AS_SHARED")
else()
  set(LIBARGVCODEC_BUILD_TYPE_CPP_DEFINE "#define LIBARGVCODEC_BUILT_AS_STATIC")
endif()
configure_file( ${CMAKE_SOURCE_DIR}/src/libargvcodec/config.h.in ${LIBARGVCODEC_CONFIG_HEADER} )
set(LIBARGVCODEC_BUILD_TYPE_CPP_DEFINE)

# Define installation directories
set(LIBARGVCODEC_INSTALL_BIN_DIR      "bin")
set(LIBARGVCODEC_INSTALL_LIB_DIR      "lib/libargvcodec-${LIBARGVCODEC_VERSION}")
set(LIBARGVCODEC_INSTALL_INCLUDE_DIR  "include/libargvcodec-${LIBARGVCODEC_VERSION}")
set(LIBARGVCODEC_INSTALL_CMAKE_DIR    ${LIBARGVCODEC_INSTALL_LIB_DIR}) # CMake files (*.cmake) should have the same destination as the library files. Some also prefers to use "cmake".

##############################################################################################################################################
# Project settings
##############################################################################################################################################

# Build options
option(LIBARGVCODEC_BUILD_TEST "Build all libArgvCodec's unit tests" OFF)
option(LIBARGVCODEC_BUILD_SAMPLES "Build all samples projects" OFF)
option(LIBARGVCODEC_BUILD_DOC "Build documentation" OFF)

# Force a debug postfix if none specified.
# This allows publishing both release and debug binaries to the same location
# and it helps to prevent linking with the wrong library on Windows.
if(NOT CMAKE_DEBUG_POSTFIX)
  set(CMAKE_DEBUG_POSTFIX "-d")
endif()

# Prevents annoying warnings on MSVC
if (WIN32)
  add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

# Define include directories for source code.
# The specified values will not be exported.
set( LIBARGVCODEC_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
include_directories(${LIBARGVCODEC_INCLUDE_DIR}   # public header files, for source code.
                    ${CMAKE_BINARY_DIR}/include   # for ${LIBARGVCODEC_VERSION_HEADER} and ${LIBARGVCODEC_CONFIG_HEADER} generated files.
)

##############################################################################################################################################
# Dependencies
##############################################################################################################################################
find_package(GTest REQUIRED) #rapidassist requires GTest
find_package(rapidassist 0.5.0 REQUIRED)

##############################################################################################################################################
# Subprojects
##############################################################################################################################################

if (BUILD_SHARED_LIBS)
  set(LIBARGVCODEC_EXPORT_HEADER_FILENAME "export.h")
  set(LIBARGVCODEC_EXPORT_HEADER ${CMAKE_BINARY_DIR}/include/libargvcodec/${LIBARGVCODEC_EXPORT_HEADER_FILENAME})
endif()

add_subdirectory(src/libArgvCodec)
add_subdirectory(src/ArgEncoder)
add_subdirectory(src/ShowArgs)

# unit tests
if(LIBARGVCODEC_BUILD_TEST)
  add_subdirectory(test/libArgvCodecValidator)
  add_subdirectory(test/libArgvCodecTest)
endif()

##############################################################################################################################################
# Support for static and shared library
##############################################################################################################################################

if (BUILD_SHARED_LIBS)
  message("Generating ${LIBARGVCODEC_EXPORT_HEADER_FILENAME} for shared library...")
  include (GenerateExportHeader) 
  GENERATE_EXPORT_HEADER(libargvcodec 
               BASE_NAME libargvcodec 
               EXPORT_MACRO_NAME LIBARGVCODEC_EXPORT 
               EXPORT_FILE_NAME ${LIBARGVCODEC_EXPORT_HEADER} 
               STATIC_DEFINE LIBARGVCODEC_BUILT_AS_STATIC
  )
endif()

##############################################################################################################################################
# Generate doxygen documentation
# See https://vicrucann.github.io/tutorials/quick-cmake-doxygen/
##############################################################################################################################################
if (LIBARGVCODEC_BUILD_DOC)
  # check if Doxygen is installed
  find_package(Doxygen)
  if (DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
 
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")
 
    # note the option ALL which allows to build the docs together with the application
    add_custom_target( libArgvCodec_doc ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()

##############################################################################################################################################
# Install
##############################################################################################################################################

# Install locations:   See https://unix.stackexchange.com/a/36874
#   On UNIX, installs to "/usr/local".
#   On Windows, installs to "C:\Program Files (x86)\${PROJECT_NAME}" or to "C:\Program Files\${PROJECT_NAME}" for 64 bit binaries

# Target config version verification file
configure_file(${CMAKE_SOURCE_DIR}/cmake/libargvcodec-config-version.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/libargvcodec-config-version.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/libargvcodec-config-version.cmake DESTINATION ${LIBARGVCODEC_INSTALL_CMAKE_DIR})

# Target config file
configure_file(${CMAKE_SOURCE_DIR}/cmake/libargvcodec-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/cmake/libargvcodec-config.cmake @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/cmake/libargvcodec-config.cmake DESTINATION ${LIBARGVCODEC_INSTALL_CMAKE_DIR})

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION ${LIBARGVCODEC_INSTALL_INCLUDE_DIR})
install(FILES ${LIBARGVCODEC_EXPORT_HEADER}
              ${LIBARGVCODEC_VERSION_HEADER}
              ${LIBARGVCODEC_CONFIG_HEADER}
              DESTINATION ${LIBARGVCODEC_INSTALL_INCLUDE_DIR}/libargvcodec)
install(EXPORT libargvcodec-targets DESTINATION ${LIBARGVCODEC_INSTALL_CMAKE_DIR})

##############################################################################################################################################
# Packaging
##############################################################################################################################################

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${LIBARGVCODEC_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR "${LIBARGVCODEC_VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${LIBARGVCODEC_VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${LIBARGVCODEC_VERSION_PATCH}")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "libArgvCodec - libArgvCodec is a windows/linux implementation of many arduino functions to allow an arduino library developer to unit test code outside of the arduino platform.")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")

# we don't want to split our program up into several things
set(CPACK_MONOLITHIC_INSTALL 1)

# This must be last
include(CPack)
