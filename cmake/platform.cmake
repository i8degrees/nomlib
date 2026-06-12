# Platform detection script for CMake

# Always use SDL2's enhanced assert facilities.
#
# See also: include/nomlib/config.hpp.in
set( NOM_USE_SDL2_ASSERT true )

#
## Platform architecture optimizations and targets
#

option(ARCH_32 "Build target for 32-bit CPU" OFF)
option(ARCH_64 "Build target for 64-bit CPU" ON)
option(AARCH_32 "Build target ARM CPU" OFF)
option(AARCH_64 "Build target ARM64 CPU" OFF)

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  # TODO: Rename to NOM_PLATFORM_OSX
  set( PLATFORM_OSX true )
  set( NOM_PLATFORM_POSIX true )

  # Platform-specific options; the following options are applicable when the
  # platform is Darwin (PLATFORM_OSX).
  # TODO: Rename to BUILD_FRAMEWORK
  option(FRAMEWORK "Build OSX Framework instead of dylib" ON)
  option(UNIVERSAL "Build as an OSX Universal Library" OFF) # multi-arch

  # FIXME(JEFF): Refactor platform architectures section; universal binaries
  # are NOT a MacOSX specific feature!
  if(UNIVERSAL)
    set(CMAKE_OSX_ARCHITECTURES i386; x86_64)
    set(PLATFORM_ARCH "x86; x64") # Reserved for future use
  else()
    set(CMAKE_OSX_ARCHITECTURES x86_64)
    set(PLATFORM_ARCH "x64") # Reserved for future use
  endif(UNIVERSAL)

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
  message( STATUS "Using SDK: ${CMAKE_OSX_SYSROOT}" )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

  message ( STATUS "Platform: Darwin (Mac OS X)" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" ) # Tested on Ubuntu v12.04-LTS
  set( PLATFORM_LINUX true )
  # DEPRECATED(JEFF): Use `PLATFORM_POSIX` instead as `NOM_PLATFORM_POSIX`
  # does not fit with the other platform variables we define. We must update
  # our codebase to not use the following before we can remove it!
  set( NOM_PLATFORM_POSIX true )
  set( PLATFORM_POSIX true )

  # TODO(JEFF): Create build time option to override compilation tooling
  # between clang and probably even GNU GCC, once we find the right C++
  # variant for it...
  #
  # Default to clang based tooling when found...
  if ( CMAKE_CXX_COMPILER MATCHES "clang" )
    message ( STATUS "Using clang based platform to build..." )
    #set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14" )
    # nomlib began its life under c++14 on Intel Darwin Mac OSX
    set ( CMAKE_CXX_STANDARD 14 )
    # >> Modern GTest build requires a c++17 minimum
    set ( CMAKE_CXX_STANDARD 14 )
    set ( CMAKE_CXX_STANDARD_REQUIRED ON )
    set ( CMAKE_CXX_EXTENSIONS OFF)

    # libc++ requires OSX v10.7+
    #set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++" )
  elseif ( CMAKE_C_COMPILER MATCHES "gcc" )
    message ( STATUS "Using gcc based platform to build..." )
    message ( FATAL_ERROR "nomlib only supports building with clang." )

    # !! GoogleTest unit testing framework v1.10.x requires a minimum C++ level 11
    set( CMAKE_CXX_STANDARD 11 )
    # !! Our engine is based on a c++ level of 14
    set( CMAKE_CXX_STANDARD 14 )
    # NOTE(JEFF): This should only be set when GNU GCC is enabled?
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )
  endif( CMAKE_CXX_COMPILER MATCHES "clang" )

  message ( STATUS "Platform: Linux" )
  message ( STATUS "Build platform: ${CMAKE_CXX_COMPILER}" )
  message ( STATUS "Compiler C++ level: ${CMAKE_CXX_STANDARD}" )
  message ( STATUS "Compiler flags: ${CMAKE_CXX_FLAGS}" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( PLATFORM_WINDOWS true )

  message ( STATUS "Platform: Windows" )
else () # Not Linux nor OSX

  set ( PLATFORM_UNKNOWN true )
  message ( STATUS "Platform: Unknown" )

endif ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

# FIXME(JEFF): Refactor platform architectures
if ( PLATFORM_WINDOWS AND ARCH_32 )
  set ( PLATFORM_ARCH "x86" )
elseif ( PLATFORM_WINDOWS AND ARCH_64 )
  set ( PLATFORM_ARCH "x64" )
endif ( PLATFORM_WINDOWS AND ARCH_32 )

message(STATUS "Target architectures: ${PLATFORM_ARCH}")
message(STATUS "Generating CMake project files with ${CMAKE_GENERATOR}")

