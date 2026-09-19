# cmake/AppleBuild.cmake:jeff
#
# This file needs to be included before calling `project`.

if(APPLE)
  # This variable influences the system header files version we build against,
  # which in turn determines the **minimum** version of OS X this build will
  # be binary compatible with (run on).
  #
  # Internally, this influences the search paths for system header files and
  # also instructs CMake to pass -mmacosx-version-min to the compiler.
  #
  # To modify this variable's resulting value, you should pass
  # -DCMAKE_OSX_DEPLOYMENT_TARGET=<ver> to CMake at the time of project files
  # generation -- 'cmake ..' from your out-of-source build directory.
  #
  # NOTE: CMAKE_OSX_SYSROOT must be set **before** the project command is
  # called.
  if(NOT CMAKE_OSX_SYSROOT)
    execute_process(
        COMMAND xcrun --show-sdk-path
        OUTPUT_VARIABLE DETECTED_SDK_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    execute_process(
        COMMAND xcrun --show-sdk-version
        OUTPUT_VARIABLE DETECTED_SDK_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    set(CMAKE_OSX_SYSROOT ${DETECTED_SDK_PATH} CACHE PATH "macOS SDK Path" FORCE)
    set(NOM_MACOSX_SDK_VERSION ${DETECTED_SDK_VERSION} CACHE PATH "macOS SDK Version" FORCE)

    message(STATUS "Using macOS SDK at ${CMAKE_OSX_SYSROOT}")
    message(STATUS "Using macOS SDK version ${NOM_MACOSX_SDK_VERSION}")
  endif(NOT CMAKE_OSX_SYSROOT)

  # Support older macOS versions.
  set(CMAKE_OSX_DEPLOYMENT_TARGET 10.15 CACHE STRING "Minimum OS X deployment version")

  if(APPLE AND "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    # ARCHS_STANDARD is set by Xcode's compiler environment
    if(NOT CMAKE_OSX_ARCHITECTURES)
      set(CMAKE_OSX_ARCHITECTURES "${ARCHS_STANDARD}" CACHE INTERNAL "OS X architecture")
    endif(NOT CMAKE_OSX_ARCHITECTURES)
  endif()
endif()
