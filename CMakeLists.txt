# CMakeLists.txt:jeff
#
# nomlib
#

# WARNING(JEFF): CMake v4.4.x is known to break MacOSX builds -- the
# framework bundles no longer accept our headers as they are specified. No
# known workaround.
cmake_minimum_required(VERSION 3.24...4.2.3)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")
list(APPEND CMAKE_TEMPLATE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates")

include("${CMAKE_SOURCE_DIR}/cmake/project_version.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/policy.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/functions.cmake")
include("${CMAKE_SOURCE_DIR}/cmake/macros.cmake")
include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake")

include("${CMAKE_SOURCE_DIR}/cmake/AppleBuild.cmake")
#
## vcpkg env
#

# !! this must be set *before* the project declaration
# defaults to a static library target
set(VCPKG_TARGET_TRIPLET "x64-linux" CACHE STRING "")

if(LINUX AND DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-linux-dynamic")
endif()

if(APPLE AND DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-osx-dynamic")
endif()

if(WIN32 AND DEFINED BUILD_SHARED_LIBS)
  set(VCPKG_TARGET_TRIPLET "x64-windows-dynamic")
endif()

##
# Project Options
##

#
## architecture
#

option(ARCH_32 "Build target for 32-bit CPU" OFF)
option(ARCH_64 "Build target for 64-bit CPU" ON)
# Reserved for future implementation
option(AARCH_32 "Build target ARM CPU" OFF)
option(AARCH_64 "Build target ARM64 CPU" OFF)

# Debugging
option(DEBUG_ASSERT "Build with run-time assertion enabled" OFF)
option(HIDE_SYMBOL_VISIBILITY "Enforce symbol visibility - see NOM_EXPORT macros" OFF)
# Engine build configuration
option(BUILD_SHARED_LIBS "Build dynamic library targets" OFF)
option(BUILD_EXAMPLES "Build usage examples" OFF)
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_DOCS "Build engine documentation" ON)
option(NOM_USE_CCACHE "Use ccache - a compiler cache for C/C++" OFF)
# EXPERIMENTAL(JEFF): This feature is a WIP and is not yet ready for general
# consumption. Your milage may vary.
option(NOM_USE_DISTCC "Use distcc - a free, fast distributed C/C++ compiler" OFF)

#
## DEPRECATED options handling
#

if(DEFINED UNIVERSAL)
  message(DEPRECATION "Option 'UNIVERSAL' is deprecated. Use 'CMAKE_OSX_ARCHITECTURES' instead.")
endif(DEFINED UNIVERSAL)

if(DEFINED DEBUG)
  message(DEPRECATION "Option 'DEBUG' is deprecated. Use 'CMAKE_BUILD_TYPE' instead.")
  set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE BOOL "Enable debugging features" FORCE)
endif(DEFINED DEBUG)

if(DEFINED EXAMPLES)
  message(DEPRECATION "Option 'EXAMPLES' is deprecated. Use 'BUILD_EXAMPLES' instead.")
  set(BUILD_EXAMPLES ${EXAMPLES} CACHE BOOL "Build usage examples" FORCE)
endif(DEFINED EXAMPLES)

if(DEFINED NOM_BUILD_TESTS)
  message(DEPRECATION "Option 'NOM_BUILD_TESTS' is deprecated. Use 'BUILD_TESTS' instead.")
  set(BUILD_TESTS ${NOM_BUILD_TESTS} CACHE BOOL "Build unit tests" FORCE)
endif(DEFINED NOM_BUILD_TESTS)

if(DEFINED NOM_BUILD_DOCS)
  message(DEPRECATION "Option 'NOM_BUILD_DOCS' is deprecated. Use 'BUILD_DOCS' instead.")
  set(BUILD_DOCS ${NOM_BUILD_DOCS} CACHE BOOL "Build engine documentation" FORCE)
endif(DEFINED NOM_BUILD_DOCS)

#
## Library units
#
option( NOM_BUILD_ACTIONS_UNIT "Animations subsystem" ON)
option( NOM_BUILD_CORE_UNIT "Engine core" ON)
option( NOM_BUILD_MATH_UNIT "Math utilities" ON)
option( NOM_BUILD_FILE_UNIT "Filesystem access" ON)
option( NOM_BUILD_AUDIO_UNIT "Audio subsystem" OFF)
option( NOM_BUILD_GRAPHICS_UNIT "Graphics core" ON)
option( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT "Build with scale2x & hqx algorithms" ON)
option( NOM_BUILD_SYSTEM_UNIT "System unit" ON)
option( NOM_BUILD_PTREE_UNIT "Generic container for serialization" ON)
option( NOM_BUILD_SERIALIZERS_UNIT "Serialization for JSON, XML, HTML" ON)
option( NOM_BUILD_GUI_UNIT "GUI subsystem" ON)

#
## install and packaging
#
string(COMPARE EQUAL "${CMAKE_SOURCE_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}" is_top_level)
option(NOM_INSTALL_CMAKE_PACKAGE "Install CMake export package for finding nomlib" "${is_top_level}")
option(NOM_INSTALL_PKGCONFIG_SUPPORT "Install pkg-config helper for finding nomlib" "${is_top_level}")

if(DEFINED nomlib_SHARED_LIBS)
  set(BUILD_SHARED_LIBS "${nomlib_SHARED_LIBS}")
endif()

# IMPORTANT(JEFF): This initializes our engine versioning variables;
# `PROJECT_VERSION_MAJOR` & friends at `include/nomlib/version.hpp.in`
project(nomlib
  VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}"
  DESCRIPTION "C++11 2D game framework using SDL2"
  HOMEPAGE_URL "https://github.com/i8degrees/nomlib"
  #SPDX_LICENSE "BSD-2-Clause-FreeBSD"
  LANGUAGES CXX
)

include("${CMAKE_SOURCE_DIR}/cmake/StandardProjectSettings.cmake")

include(GNUInstallDirs)

# Link this "library" to use the warnings specified in CompilerWarnings.cmake.
add_library(project_warnings INTERFACE)
include("${CMAKE_SOURCE_DIR}/cmake/CompilerWarnings.cmake")
# Add cmake option WARNINGS_AS_ERRORS
set_project_warnings(project_warnings)

#
## Project-wide env
#

# Public API headers base
set(HDR_BASE "${PROJECT_SOURCE_DIR}/include/nomlib")
set(SRC_DIR "${PROJECT_SOURCE_DIR}/src")
# Resources path for engine examples
set(NOMLIB_RESOURCES_DIR "${PROJECT_SOURCE_DIR}/Resources")
set(NOMLIB_SHARED_SUPPORT_DIR "${NOMLIB_RESOURCES_DIR}/SharedSupport")
set(NOM_EXAMPLES_RESOURCES_DIR "${NOMLIB_RESOURCES_DIR}/examples")
# Unit testing resources path
set(NOM_TESTS_RESOURCES_DIR "${NOMLIB_RESOURCES_DIR}/tests")
# Files used with documentation generation
set(PROJECT_DOXYGEN_DIR "${NOMLIB_RESOURCES_DIR}/doxygen")

# Platform-specific implementations & dependencies
#
# See also: nomlib's third-party/README.md
include("${CMAKE_SOURCE_DIR}/vendor/CMakeLists.txt")

#
## Project-wide build settings
#

if(BUILD_SHARED_LIBS)
  set(LIBRARY_OUTPUT_TYPE "SHARED")
else(NOT BUILD_SHARED_LIBS)
  set(LIBRARY_OUTPUT_TYPE "STATIC")
endif(BUILD_SHARED_LIBS)

message(STATUS "Building nomlib as ${LIBRARY_OUTPUT_TYPE} library targets")

# CMAKE_SYSTEM_PREFIX_PATH is searched to find libraries when the find_package
# command is used.
message(STATUS "Library Search Prefix: ${CMAKE_SYSTEM_PREFIX_PATH}")
# Installation prefix path set for our project
message(STATUS "Installation Prefix: ${CMAKE_INSTALL_PREFIX}")

#
## Third-party dependencies
#

# This is a hard requirement for GoogleTest (BUILD_TESTS=on)
find_package(Threads REQUIRED)
find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
if(NOM_BUILD_GUI_UNIT)
  find_package(OpenGL REQUIRED COMPONENTS OpenGL)
  find_package(libRocket CONFIG REQUIRED)
endif()

if(NOM_BUILD_AUDIO_UNIT) # required dependencies
  if(APPLE)
    find_package(OpenAL REQUIRED)
  else(NOT APPLE)
    find_package(OpenAL CONFIG REQUIRED)
  endif(APPLE)
  find_package(SndFile CONFIG REQUIRED)
else(NOT NOM_BUILD_AUDIO_UNIT) # optional dependencies
  if(APPLE)
    find_package(OpenAL)
  else(NOT APPLE)
    find_package(OpenAL CONFIG)
  endif(APPLE)
  find_package(SndFile CONFIG)
endif(NOM_BUILD_AUDIO_UNIT)

if(BUILD_DOCS)
  # Doxygen configuration
  include("${PROJECT_SOURCE_DIR}/cmake/doxygen.cmake")
endif(BUILD_DOCS)

# >> src/<component>/CMakeLists.txt contains our NOM_BUILD_<component> options
add_subdirectory(src)

# nomlib examples configuration
if(BUILD_EXAMPLES)
  add_subdirectory("examples")
endif()

if(BUILD_TESTS)
  # TODO(JEFF): CTest setup goes here
  enable_testing()
  add_subdirectory("tests")
endif()

# Installation phase

# >> We will not be the top-level project if being included via CMake's FetchContent
# >> mechanism. If so, we should try not to assume that the end-user wants anything
# >> to be packaged (installed), and let them decide what they want to do themselves.
if(is_top_level)
  add_subdirectory(src/packages)
endif(is_top_level)
