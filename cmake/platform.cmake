# Platform detection script for CMake

# Always use SDL2's enhanced assert facilities.
#
# See also: include/nomlib/config.hpp.in
set( NOM_USE_SDL2_ASSERT true )

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  # TODO: Rename to NOM_PLATFORM_OSX
  set( PLATFORM_OSX true )
  set( NOM_PLATFORM_POSIX true )

  # TODO: Rename to BUILD_FRAMEWORK
  option ( FRAMEWORK "Build OSX Framework instead of dylib" on )
  option ( UNIVERSAL "Build as an OSX Universal Library" off )

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
  # TODO: Rename to NOM_PLATFORM_LINUX
  set( PLATFORM_LINUX true )
  set( NOM_PLATFORM_POSIX true )

  # Clang is not supported on Linux due to libc++ not being distributed by
  # default yet
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )

  # ARCH_32 and ARCH_64 are not presently used here, but are reserved for future
  # consistency with the other supported platforms.
  option ( ARCH_32 "Compile ${PROJECT_NAME} as a 32-bit library" off )
  option ( ARCH_64 "Compile ${PROJECT_NAME} as a 64-bit library" on )

  message ( STATUS "Platform: Linux" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  # TODO: Rename to NOM_PLATFORM_WINDOWS
  set( PLATFORM_WINDOWS true )

  option ( ARCH_32 "Compile ${PROJECT_NAME} as a 32-bit library" off )
  option ( ARCH_64 "Compile ${PROJECT_NAME} as a 64-bit library" on )

  message ( STATUS "Platform: Windows" )
else () # Not Linux nor OSX

  set ( PLATFORM_UNKNOWN true )
  message ( STATUS "Platform: Unknown" )

endif ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

message ( STATUS "Generating build files for: ${CMAKE_GENERATOR}" )

# I was able to shave off close to 50% time compiling nomlib by leaving
# the UNIVERSAL option off by default.
#
# PPC is not officially supported because I have no means whatsoever of
# testing such a package.
#
# TODO; we might be able to put these platform checks shown below in the
# CMAKE_SYSTEM_NAME checks above.
if ( UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  set ( PLATFORM_ARCH "x86; x64" ) # Reserved for future use

else ( NOT UNIVERSAL )
  set ( CMAKE_OSX_ARCHITECTURES x86_64 )
  set ( PLATFORM_ARCH "x64" ) # Reserved for future use

endif ( UNIVERSAL )

if ( PLATFORM_WINDOWS AND ARCH_32 )
  set ( PLATFORM_ARCH "x86" )
elseif ( PLATFORM_WINDOWS AND ARCH_64 )
  set ( PLATFORM_ARCH "x64" )
endif ( PLATFORM_WINDOWS AND ARCH_32 )

message ( STATUS "Platform Architecture: ${PLATFORM_ARCH}" )
