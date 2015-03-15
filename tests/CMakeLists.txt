# Unit testing with Google Test
#
# NOTE: Ensure that when you use the GTEST_ADD_TESTS macro, that you supply the
# necessary arguments -- name of test(s) to run -- otherwise you may notice
# CTest skipping the tests all together!
#
# NOTE: You may need to issue a 'make rebuild_cache' after changing test
# fixture names, in order to get 'make test' to recognize the modified test
# code.
#
# NOTE: To display a unit test's full debugging output (NOM_LOG_ERR, NOM_DUMP
# and friends), run the test's executable directly. This debugging output will
# not be shown when using the 'test' target -- i.e.: make target.
#
# IMPORTANT: Compiling nomlib under Windows without iterator asserts --
# /D_ITERATOR_DEBUG_LEVEL=0 -- will cause GTest to throw SEH exception errors
# before it is able to display actual & expected results when a unit test fails.
#
# NOTE: The custom command line processing used by nom::UnitTest for interactive
# interactive test runs breaks under Windows when building with the target
# library ${GTEST_BOTH_LIBRARIES}, due to GTest's main library overwriting our
# unit test's main functions. In summary, only the library target
# ${GTEST_LIBRARY} should be used, to ensure cross-platform support.

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
endif( SDL2_FOUND )

# nomlib-audio external deps
if( PLATFORM_WINDOWS )
  find_package( OpenAL REQUIRED )
  if( OPENAL_FOUND )
    # Add development header files; al.h, alc.h
    include_directories( ${OPENAL_INCLUDE_DIR} )
  endif( OPENAL_FOUND )
endif( PLATFORM_WINDOWS )

# nomlib-gui external deps
find_package( libRocket REQUIRED )
if( LIBROCKET_FOUND )
  # Add development header files
  include_directories( ${LIBROCKET_INCLUDE_DIRS} )
endif( LIBROCKET_FOUND )

# third-party, headers-only dependency TCLAP
set( TCLAP_INCLUDE_DIR "${NOM_THIRD_PARTY_COMMON_DIR}/tclap" )

if( NOT EXISTS ${TCLAP_INCLUDE_DIR} )
  message(  FATAL_ERROR
            "Could not find header file ${TCLAP_INCLUDE_DIR} for TCLAP; "
            "see nomlib's third-party/README.md for help obtaining this "
            "required dependency."
  )
else( EXISTS ${TCLAP_INCLUDE_DIR} )
  message( STATUS "TCLAP headers found: ${TCLAP_INCLUDE_DIR}" )
endif( NOT EXISTS ${TCLAP_INCLUDE_DIR} )

# Header file inclusion; third-party -- TCLAP
include_directories("${NOM_THIRD_PARTY_COMMON_DIR}")

# Unit test dependencies (common / global)
#
# NOTE: We prefer linking to GTest statically.
if( PLATFORM_OSX )
  set( GTEST_ROOT "${NOMLIB_DEPS_DIR}/osx/gtest" )
else( PLATFORM_WINDOWS )
  set( GTEST_ROOT "${NOMLIB_DEPS_DIR}/windows/gtest" )
else( PLATFORM_LINUX )
  # TODO
endif( PLATFORM_OSX )

find_package( GTest REQUIRED )

include_directories( ${GTEST_INCLUDE_DIRS} )

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

  set( NOM_UNIT_TEST_LIBRARY_DEPS ${GTEST_LIBRARY} nomlib-core nomlib-file )

  if( PLATFORM_WINDOWS )
    list( APPEND NOM_UNIT_TEST_LIBRARY_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  add_library(  nomlib-unit-test ${LIBRARY_OUTPUT_TYPE}
                ${NOM_TESTS_UNIT_TEST_SOURCE} )

  # Link nomlib-unit-test
  target_link_libraries( nomlib-unit-test ${NOM_UNIT_TEST_LIBRARY_DEPS} )

endif( NOM_BUILD_UNIT_TEST_UNIT )

if( NOM_BUILD_VISUAL_UNIT_TEST_UNIT )

  set( NOM_VISUAL_UNIT_TEST_LIBRARY_DEPS nomlib-unit-test nomlib-graphics )

  add_library(  nomlib-visual-unit-test
                ${LIBRARY_OUTPUT_TYPE} ${NOM_TESTS_VTEST_SOURCE} )

  # Link nomlib-visual-unit-test
  target_link_libraries( nomlib-visual-unit-test ${NOM_VISUAL_UNIT_TEST_LIBRARY_DEPS} )

endif( NOM_BUILD_VISUAL_UNIT_TEST_UNIT )

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
  install ( DIRECTORY
            ${SDL2_REDIST_DIR}
            ${SDL2_IMAGE_REDIST_DIR}
            ${SDL2_TTF_REDIST_DIR}
            ${OPENAL_REDIST_DIR}
            ${LIBSNDFILE_REDIST_DIR}
            ${MSVCPP_REDIST_DIR}
            "${GTEST_ROOT}/lib/"        # GTest re-dist libs
            DESTINATION
            ${TESTS_INSTALL_DIR}
            FILES_MATCHING PATTERN "*.dll"
          )
endif ( PLATFORM_WINDOWS )
