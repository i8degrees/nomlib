# Unit testing with Google Test
#
# NOTE: Ensure that when you use the GTEST_ADD_TESTS macro, that you supply the
# necessary arguments -- name of test(s) to run -- otherwise you may notice
# CTest skipping the tests all together!
#
# NOTE: You may need to issue a 'make rebuild_cache' after changing test
# fixture names, in order to get 'make test' to recognize the modified test
# code.

# Source file inclusion root (directory); must be an absolute path.
set( TESTS_SRC_DIR "${PROJECT_SOURCE_DIR}/tests/src" )

# Header file inclusion root (directory); must be an absolute path.
set( TESTS_INC_DIR "${PROJECT_SOURCE_DIR}/tests/include" )

# Use common build output directories for MSVCPP && Xcode project files.
#
# IMPORTANT: Debug and Release build targets **must** be kept in separate build
# trees!
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/tests/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/tests/Release")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/tests/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/tests/Release")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/tests/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/tests/Release")

if(DEBUG)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
else() # Release builds
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
endif()

# This path is used for the local installation of required dependencies for
# running unit tests, such as dependent resource files. Additionally, when
# building on Windows, this will be the path that dependent DLLs are copied to.
#
# IMPORTANT: This path serves a vital role in the *consistent* usage of CTest
# and GTest across the supported CMake project generators -- Unix Makefiles,
# Xcode and MSVCPP. The path **must** be prepended on every unit test
# executable created using the add_test and GTEST_ADD_TESTS commands.
set( TESTS_INSTALL_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}" )

# TODO: Relocate me?
set( NOM_BUILD_UNIT_TEST_UNIT ON )
set( NOM_BUILD_VISUAL_UNIT_TEST_UNIT ON )

set( NOM_BUILD_CORE_TESTS ON )
set( NOM_BUILD_FILE_TESTS ON )
set( NOM_BUILD_MATH_TESTS ON )
set( NOM_BUILD_PTREE_TESTS ON )
set( NOM_BUILD_SERIALIZERS_TESTS ON )
set( NOM_BUILD_SYSTEM_TESTS ON )
set( NOM_BUILD_AUDIO_TESTS ON )
set( NOM_BUILD_GRAPHICS_TESTS ON )
set( NOM_BUILD_ACTIONS_TESTS ON )
set( NOM_BUILD_GUI_TESTS ON )

# Tests for the testing framework
set( NOM_BUILD_UNIT_TEST_TESTS ON )
set( NOM_BUILD_VTEST_TESTS ON )

if( EXISTS "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
  include( "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
endif()

# Project headers files inclusions; our header files namespace, 'nomlib'
include_directories( "${INC_ROOT_DIR}" )

# Third-party dependencies

# Absolute path to project's third-party 'common' directory
set( NOM_THIRD_PARTY_COMMON_DIR "${PROJECT_SOURCE_DIR}/third-party/common" )

# nomlib-core external deps
find_package( SDL2 REQUIRED )
if( SDL2_FOUND )
  # Add development header files; SDL.h & friends
  include_directories( ${SDL2_INCLUDE_DIR} )
  message ( STATUS "Found SDL2 headers: ${SDL2_INCLUDE_DIR}." )
elseif ( NOT SDL2_FOUND )
  message ( FATAL_ERROR "Missing SDL2 headers at ${SDL2_INCLUDE_DIR}." )
endif ( SDL2_FOUND )

<<<<<<< HEAD
if(NOM_BUILD_AUDIO_UNIT)
  # nomlib-audio external deps
=======
if ( NOM_BUILD_AUDIO_TESTS )
  # nomlib-audio external deps; OpenAL-Soft or Apple's OpenAL
>>>>>>> ba1fb044 (Improve library headers handling)
  find_package( OpenAL REQUIRED )
  if( OPENAL_FOUND )
    include_directories( ${OPENAL_INCLUDE_DIR} )
    message ( STATUS "Found OpenAL headers: ${OPENAL_INCLUDE_DIR}." )
  else ( NOT OPENAL_FOUND )
    message ( FATAL_ERROR "Missing OpenAL headers at ${OPENAL_INCLUDE_DIR}." )
  endif ( OPENAL_FOUND )
<<<<<<< HEAD
endif(NOM_BUILD_AUDIO_UNIT)
=======
endif ( NOM_BUILD_AUDIO_TESTS )
>>>>>>> ba1fb044 (Improve library headers handling)

# nomlib-gui external deps
find_package( LibRocket REQUIRED )
if( LIBROCKET_FOUND )
  # Add development header files
  include_directories( ${LIBROCKET_INCLUDE_DIRS} )
  message ( STATUS "Found libRocket headers: ${LIBROCKET_INCLUDE_DIRS}." )
else ( NOT LIBROCKET_FOUND )
  message ( FATAL_ERROR "Missing libRocket headers at ${LIBROCKET_INCLUDE_DIRS}." )
endif ( LIBROCKET_FOUND )

# third-party, headers-only dependency TCLAP
set( TCLAP_INCLUDE_DIR "${NOM_THIRD_PARTY_COMMON_DIR}/tclap" )

if( NOT EXISTS ${TCLAP_INCLUDE_DIR} )
  message(  FATAL_ERROR
            "Could not find header file ${TCLAP_INCLUDE_DIR} for TCLAP; "
            "see nomlib's third-party/README.md for help obtaining this "
            "required dependency."
  )
else( EXISTS ${TCLAP_INCLUDE_DIR} )
  # Header file inclusion; third-party -- TCLAP
  include_directories("${NOM_THIRD_PARTY_COMMON_DIR}")
  message( STATUS "TCLAP headers found: ${TCLAP_INCLUDE_DIR}" )
endif( NOT EXISTS ${TCLAP_INCLUDE_DIR} )

if ( NOM_BUILD_AUDIO_TESTS )
  find_package( libsndfile REQUIRED )
elseif ( NOT NOM_BUILD_AUDIO_TESTS )
  find_package( libsndfile OPTIONAL )
endif ( NOM_BUILD_AUDIO_TESTS )

if ( NOT LIBSNDFILE_FOUND AND NOM_BUILD_AUDIO_TESTS )
  message ( FATAL_ERROR "libsndfile library not found." )
  # TODO(JEFF): Improve the fatal error message by offering suggestions on how to
  # resolve or bypass the issue.
else ( LIBSNDFILE_FOUND AND NOT NOM_BUILD_AUDIO_TESTS )
  # FIXME(JEFF): Verify where and when we include the header files for libsndfile
  message ( STATUS "libsndfile library found: ${LIBSNDFILE_LIBRARY}" )
endif ()

# Unit test dependencies (common / global)
if( PLATFORM_OSX )
  set( GTEST_ROOT "${NOMLIB_DEPS_DIR}/osx/gtest" )
elseif( PLATFORM_WINDOWS )
  set( GTEST_ROOT "${NOMLIB_DEPS_DIR}/windows/gtest" )
elseif( PLATFORM_LINUX ) # PLATFORM_POSIX (?)
  set( GTEST_ROOT "${NOMLIB_DEPS_DIR}/linux/gtest")
  set( GTEST_INCLUDE_DIR "${NOMLIB_DEPS_DIR}/linux/gtest/include" )
endif ( PLATFORM_OSX )

find_package ( GTest REQUIRED )
if ( GTEST_FOUND )
  message( STATUS "Found GoogleTest ${GTEST_ROOT}." )
  set ( GTEST_INCLUDE_DIRS ${GTEST_ROOT}/include )
  set ( GTEST_LIBRARY ${GTEST_ROOT}/lib/libgtest.a )
  message( STATUS "Using GoogleTest headers: ${GTEST_INCLUDE_DIRS}." )
  message( STATUS "Using GoogleTest library: ${GTEST_LIBRARY}." )
  include_directories("${GTEST_INCLUDE_DIRS}")
else ( NOT GTEST_FOUND )
  message( STATUS "Missing GoogleTest: ${GTEST_ROOT}." )
  message( STATUS "Missing GoogleTest headers: ${GTEST_INCLUDE_DIRS}." )
  message( STATUS "Missing GoogleTest library: ${GTEST_LIBRARY}." )
  message ( FATAL_ERROR "Failed to setup engine unit tests." )
endif ( GTEST_FOUND )

set(  NOM_TESTS_UNIT_TEST_SOURCE
      ${TESTS_SRC_DIR}/UnitTest/UnitTest.cpp
      ${TESTS_INC_DIR}/nomlib/tests/UnitTest/UnitTest.hpp
)

set(  NOM_TESTS_VTEST_SOURCE
      ${TESTS_SRC_DIR}/VisualUnitTest/ImageTestSet.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/ImageTestSet.hpp

      ${TESTS_SRC_DIR}/VisualUnitTest/VisualUnitTest.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/VisualUnitTest.hpp

      ${TESTS_SRC_DIR}/VisualUnitTest/ImageDiff.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/ImageDiff.hpp

      ${TESTS_SRC_DIR}/VisualUnitTest/TestResultWriter.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/TestResultWriter.hpp

      ${TESTS_SRC_DIR}/VisualUnitTest/VisualUnitTestResultWriter.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/VisualUnitTestResultWriter.hpp

      ${TESTS_SRC_DIR}/VisualUnitTest/HTMLTestResultWriter.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/HTMLTestResultWriter.hpp

      ${TESTS_SRC_DIR}/VisualUnitTest/MinimalTestResultWriter.cpp
      ${TESTS_INC_DIR}/nomlib/tests/VisualUnitTest/MinimalTestResultWriter.hpp
)

if( NOM_BUILD_UNIT_TEST_UNIT )
  set( NOM_UNIT_TEST_LIBRARY_DEPS nomlib-core nomlib-file )

  if( PLATFORM_WINDOWS )
    list( APPEND NOM_UNIT_TEST_LIBRARY_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  # >> this will include GoogleTest script and calls test discovery function
  # >>  defined in it, so you donot have to create main function for tests,
  #  >> they will be auto discovered
  add_library(  nomlib-unit-test ${LIBRARY_OUTPUT_TYPE}
                ${NOM_TESTS_UNIT_TEST_SOURCE} )
  # >> Link nomlib-unit-test to GTest
  target_link_libraries(nomlib-unit-test ${GTEST_LIBRARY} ${NOM_UNIT_TEST_LIBRARY_DEPS} )
endif( NOM_BUILD_UNIT_TEST_UNIT )

if( NOM_BUILD_VISUAL_UNIT_TEST_UNIT )
  set( NOM_VISUAL_UNIT_TEST_LIBRARY_DEPS nomlib-unit-test nomlib-graphics )
  add_library(  nomlib-visual-unit-test
                ${LIBRARY_OUTPUT_TYPE} ${NOM_TESTS_VTEST_SOURCE} )
  target_link_libraries(nomlib-visual-unit-test ${GTEST_LIBRARY} ${NOM_VISUAL_UNIT_TEST_LIBRARY_DEPS} )
endif( NOM_BUILD_VISUAL_UNIT_TEST_UNIT )

# >> tests dev headers
include_directories("${TESTS_INC_DIR}")

if( NOM_BUILD_UNIT_TEST_TESTS )
  add_subdirectory("src/UnitTest")
endif( NOM_BUILD_UNIT_TEST_TESTS )

if( NOM_BUILD_VTEST_TESTS )
  add_subdirectory("src/VisualUnitTest")
endif( NOM_BUILD_VTEST_TESTS )

if( NOM_BUILD_CORE_TESTS )
  add_subdirectory("src/core")
endif( NOM_BUILD_CORE_TESTS )

if( NOM_BUILD_AUDIO_TESTS )
  add_subdirectory("src/audio")
endif( NOM_BUILD_AUDIO_TESTS )

if( NOM_BUILD_GRAPHICS_TESTS )
  add_subdirectory("src/graphics")
endif( NOM_BUILD_GRAPHICS_TESTS )

if( NOM_BUILD_ACTIONS_TESTS )
  add_subdirectory("src/actions")
endif( NOM_BUILD_ACTIONS_TESTS )

if( NOM_BUILD_GUI_TESTS )
  add_subdirectory("src/gui")
endif( NOM_BUILD_GUI_TESTS )

if( NOM_BUILD_MATH_TESTS )
  add_subdirectory("src/math")
endif( NOM_BUILD_MATH_TESTS )

if( NOM_BUILD_PTREE_TESTS )
  add_subdirectory("src/ptree")
endif( NOM_BUILD_PTREE_TESTS )

if( NOM_BUILD_SYSTEM_TESTS )
  add_subdirectory("src/system")
endif( NOM_BUILD_SYSTEM_TESTS )

if( NOM_BUILD_SERIALIZERS_TESTS )
  add_subdirectory("src/serializers")
endif( NOM_BUILD_SERIALIZERS_TESTS )

# Install library dependencies into tests output directory so we can always
# execute the binaries with the proper dependency versions.
if ( PLATFORM_WINDOWS )

  if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
    set( GTEST_LIBRARY_REDIST_DLL "${GTEST_ROOT}/lib/gtestd.dll" )
  else()
    set( GTEST_LIBRARY_REDIST_DLL "${GTEST_ROOT}/lib/gtest.dll" )
  endif()

  install(  FILES ${GTEST_LIBRARY_REDIST_DLL}
            DESTINATION ${TESTS_INSTALL_DIR}
            RENAME "gtest.dll" )

  install ( DIRECTORY
            ${SDL2_REDIST_DIR}
            ${SDL2_IMAGE_REDIST_DIR}
            ${SDL2_TTF_REDIST_DIR}
            ${OPENAL_REDIST_DIR}
            ${LIBSNDFILE_REDIST_DIR}
            ${MSVCPP_REDIST_DIR}
            DESTINATION
            ${TESTS_INSTALL_DIR}
            FILES_MATCHING PATTERN "*.dll"
          )
endif( PLATFORM_WINDOWS )

if( PLATFORM_LINUX )
  install_resource_dir( "${NOM_TESTS_RESOURCES_DIR}"
                        "${TESTS_INSTALL_DIR}/Resources")
endif ( PLATFORM_LINUX )
