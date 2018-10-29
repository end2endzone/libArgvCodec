# Install #

The library does not provides pre-build binaries or provides an installation package. To install the binary files on the system, the source code must be compiled and copied to the appropriate directory.

The following steps show how to install the library:

1) Download the source code from an existing [tags](http://github.com/end2endzone/libArgvCodec/tags) and extract the content to a local directory (for example `c:\projects\libArgvCodec` or `~/dev/libArgvCodec`).

2) Build the source code according to the [Build Steps](#build-steps) instructions specified in this document.

3) Navigate to the `build` directory and execute the 'install' command for your platform:
   1) On Windows, enter `cmake --build . --config Release --target INSTALL`.
   2) On Linux, enter `sudo make install`.

# Build #

This section explains how to compile and build the software and how to get a development environment ready.




## Prerequisites ##


### Software Requirements ###
The following software must be installed on the system for compiling source code:

* [Google C++ Testing Framework v1.8.0](https://github.com/google/googletest/tree/release-1.8.0)
* [RapidAssist v0.5.0](https://github.com/end2endzone/RapidAssist/tree/0.5.0)
* [CMake](http://www.cmake.org/) v3.4.3 (or newer)



### Linux Requirements ###

These are the base requirements to build and use libArgvCodec:

  * GNU-compatible Make or gmake
  * POSIX-standard shell
  * A C++98-standard-compliant compiler



### Windows Requirements ###

* Microsoft Visual C++ 2010 or newer




## Build steps ##

The libArgvCodec library uses the CMake build system to generate a platform-specific build environment. CMake reads the CMakeLists.txt files that you'll find throughout the directories, checks for installed dependencies and then generates files for the selected build system.

To build the software, execute the following steps:

1) Download the source code from an existing [tags](http://github.com/end2endzone/libArgvCodec/tags) and extract the content to a local directory (for example `c:\projects\libArgvCodec` or `~/dev/libArgvCodec`).

2) Generate the project files for your build system using the following commands:
```
mkdir build
cd build
cmake ..
```

If you do want to specify a specific location, where libargvcodec should be installed, you can provide an extra option to cmake at this step `-DCMAKE_INSTALL_PREFIX=c:\projects\install\libargvcodec`.

If you do not specify this, cmake will install that library into its default location, which differs from system to system. On Windows it is `C:\Program Files (x86)\${PROJECT_NAME}` which expands to `C:\Program Files (x86)\libArgvCodec` and for this admin rights are needed. The same applies for Linux. CMake default installation folder is `/usr/local` which requires admin rights (sudo).

If you choose to install your libraries into a separate folder, cmake doesn't know where to search for libargvcodec, when you try to build another project, [win32Arduino](https://github.com/end2endzone/win32Arduino) for example.
So do not forget, to tell cmake where to search for this with the environment variable on windows for example `set CMAKE_PREFIX_PATH=c:\projects\install`. CMake will then look at this location for already installed libraries.

3) Build the source code:
   1) On Windows, run `cmake --build . --config Release` or open `libArgvCodec.sln` with Visual Studio.
   2) On Linux, run `make` command.


## Build options ##

The following table shows the available build option supported:

| Name | Type | Default | Usage |
|------|------|:-------:|-------|
| CMAKE_INSTALL_PREFIX    | STRING | See CMake documentation | Defines the installation folder of the library.           |
| BUILD_SHARED_LIBS       | BOOL   | OFF                     | Enable/disable the generation of shared library makefiles |
| LIBARGVCODEC_BUILD_TEST | BOOL   | OFF                     | Enable/disable the generation of unit tests target. |
| LIBARGVCODEC_BUILD_DOC  | BOOL   | OFF                     | Enable/disable the generation of API documentation target. |

To enable a build option, run the following command at the cmake configuration time:
```cmake
cmake -D<BUILD-OPTION-NAME>=ON ..
```



# Testing #
libArgvCodec comes with unit tests which help maintaining the product stability and level of quality.

Test are build using the Google Test v1.8.0 framework. For more information on how googletest is working, see the [google test documentation primer](https://github.com/google/googletest/blob/release-1.8.0/googletest/docs/V1_6_Primer.md).  

Test are disabled by default and must be manually enabled. See the [Build Options](#build-options) for details on activating unit tests.

Test are automatically build when building the solution.

To run tests, navigate to the `build/bin` folder and run `libargvcodec_unittest` executable. For Windows users, the executable is located in `build\bin\Release`.

Test results are saved in junit format in file `libargvcodec_unittest.x86.debug.xml` or `libargvcodec_unittest.x86.release.xml` depending on the selected configuration.

The latest test results are available at the beginning of the [README.md](README.md) file.



## Test samples ##

Please refer to file [Tests.Windows.md](Tests.Windows.md) for all the command line samples that were used as a baseline for creating unit tests for the Windows platform.
