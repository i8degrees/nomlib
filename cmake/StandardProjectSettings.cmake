# cmake/StandardProjectSettings.cmake:jeff
#

if(NOM_USE_CCACHE)
  find_program(CCACHE ccache)
  if(CCACHE)
    message(STATUS "Using ccache")
    set(CMAKE_CXX_COMPILER_LAUNCHER ${CCACHE})
  else(NOT CCACHE)
    message(STATUS "Ccache not found")
  endif()
endif()

# EXPERIMENTAL(JEFF): This is a WIP and not ready for general consumption.
if(NOM_USE_DISTCC)
  find_program(DISTCC distcc)
  if(DISTCC)
    message(STATUS "Using distcc")
    set(CMAKE_CXX_COMPILER_LAUNCHER "${CMAKE_CXX_COMPILER_LAUNCHER};${DISTCC}")
  else(NOT DISTCC)
    message(STATUS "distcc not found")
  endif(DISTCC)
endif()

# nomlib began its life using c++14 with stdlib libc++ on MacOSX v10.9
# (i386; x86_64) arch
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build." FORCE)

  # Set the possible values of build type for cmake-gui, ccmake
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "RelWithDebInfo" "MinSizeRel")
endif()

if(CMAKE_BUILD_TYPE MATCHES "Debug" OR CMAKE_BUILD_TYPE MATCHES "RelWithDebInfo")
  set(CMAKE_VERBOSE_MAKEFILE ON)
  add_compile_definitions("NOM_DEBUG")
  message(STATUS "[NOM_DEBUG] Debugging macros and logging activated.")
endif()

if(DEBUG_ASSERT)
  # Enable NOM_ASSERT macros
  add_compile_definitions("NOM_DEBUG_ASSERT")
  message(STATUS "[NOM_DEBUG_ASSERT] Run-time assertions are enabled.")
else(NOT DEBUG_ASSERT)
  # Disable NOM_ASSERT macros
  add_compile_definitions("NDEBUG")
  message(STATUS "[NDEBUG] Disabling run-time assertions")
endif()

# Generate compile_commands.json to make it easier to work with clang based
# tools
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# cmake option NOLOG - disable engine logging
option(NOLOG "Disable logging" OFF)
if(NOLOG)
  # TODO(JEFF): This needs to be implemented in config.hpp.in;
  # rewrap our logging macros
  add_compile_definitions("NOLOG")
endif()

# Support building nomlib as a DLL shared library on MS Windows; the
# following properties ensure that all library targets built will have the
# right symbol visibility in conjunction with the use of the internal
# NOM_EXPORT macros that wrap the necessary __declspec(dllexport) declarations.
if(WIN32 OR HIDE_SYMBOL_VISIBILITY)
  if(NOT DEFINED CMAKE_CXX_VISIBILITY_PRESET AND
    NOT DEFINED CMAKE_VISIBILITY_INLINES_HIDDEN)
    set(CMAKE_CXX_VISIBILITY_PRESET hidden)
    set(CMAKE_VISIBILITY_INLINES_HIDDEN YES)
  endif()
endif(WIN32 OR HIDE_SYMBOL_VISIBILITY)

message(STATUS "Generating CMake project files with ${CMAKE_GENERATOR}")
