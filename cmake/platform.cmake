# cmake/platform.cmake:jeff
#
# Platform detection script for use in CMake & C preprocessor macros defined
# elsewhere

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
  # TODO: Rename to NOM_PLATFORM_OSX
  set(PLATFORM_OSX TRUE)
  set(NOM_PLATFORM_OSX TRUE)
  set(NOM_PLATFORM_POSIX TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
  set(PLATFORM_LINUX TRUE)
  set(NOM_PLATFORM_POSIX TRUE)
  set(PLATFORM_POSIX TRUE)
endif()

if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
  set(PLATFORM_WINDOWS TRUE)
  set(PLATFORM_WIN TRUE)
endif()
