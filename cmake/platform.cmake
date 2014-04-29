# Platform detection script for CMake

# Always use SDL2's enhanced assert facilities.
#
# See also: include/nomlib/config.hpp, include/nomlib/version.hpp.in
set( NOM_USE_SDL2_ASSERT true )

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  set ( PLATFORM_OSX true )

  option ( FRAMEWORK "Build OSX Framework instead of dylib" on )
  option ( UNIVERSAL "Build as an OSX Universal Library" off )

  # Setup the SDK selection for backwards compatibility
  set ( SDKVER "10.7" )
  set ( DEVROOT "/Applications/Developer/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer" )
  set ( SDKROOT "${DEVROOT}/SDKs/MacOSX${SDKVER}.sdk" )

  if ( EXISTS ${SDKROOT} )
    set ( CMAKE_OSX_SYSROOT "${SDKROOT}" )
    set ( CMAKE_OSX_DEPLOYMENT_TARGET "${SDKVER}" )
    set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mmacosx-version-min=${SDKVER}" )
  else () # Mac OS X v10.5 SDK not found -- that's OK
    message ( WARNING "Warning, Mac OS X ${SDKVER} SDK path not found: ${SDKROOT}" )
  endif ( EXISTS ${SDKROOT} )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL "Xcode" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
  endif ( CMAKE_GENERATOR STREQUAL "Xcode" )

  message ( STATUS "Platform: Darwin (Mac OS X)" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" ) # Tested on Ubuntu v12.04-LTS
  set ( PLATFORM_LINUX true )

  # Clang is not supported on Linux due to libc++ not being distributed by
  # default yet
  set ( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x" )

  # ARCH_32 and ARCH_64 are not presently used here, but are reserved for future
  # consistency with the other supported platforms.
  option ( ARCH_32 "Compile ${PROJECT_NAME} as a 32-bit library" off )
  option ( ARCH_64 "Compile ${PROJECT_NAME} as a 64-bit library" on )

  message ( STATUS "Platform: Linux" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set ( PLATFORM_WINDOWS true )

  set ( CMAKE_CONFIGURATION_TYPES "${CMAKE_BUILD_TYPE}" )

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
