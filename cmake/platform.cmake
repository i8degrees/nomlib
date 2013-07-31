# Platform detection script for CMake

if ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

  set ( PLATFORM_OSX true )
  message ( STATUS "Platform: Darwin (OS X)" )

elseif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" )

  set ( PLATFORM_LINUX true )
  message ( STATUS "Platform: Linux" )

else () # Not Linux nor OSX

  set ( PLATFORM_UNKNOWN true )
  message ( STATUS "Platform: Unknown" )

endif ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
