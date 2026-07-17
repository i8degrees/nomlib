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
set( TESTS_HDR_BASE "${PROJECT_SOURCE_DIR}/tests/include" )

# Use common build output directories for MSVCPP && Xcode project files.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/opt")

# This path is used for the local installation of required dependencies for
# running unit tests, such as dependent resource files. Additionally, when
# building on Windows, this will be the path that dependent DLLs are copied to.
#
# IMPORTANT: This path serves a vital role in the *consistent* usage of CTest
# and GTest across the supported CMake project generators -- Unix Makefiles,
# Xcode and MSVCPP. The path **must** be prepended on every unit test
# executable created using the add_test and GTEST_ADD_TESTS commands.

#set(TESTS_INSTALL_DIR "${CMAKE_INSTALL_DATAROOTDIR}")
set(TESTS_INSTALL_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

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

# GoogleTest; add_test macros
include(GoogleTest)

if(NOM_BUILD_UNIT_TEST_UNIT)

  # >> this will include GoogleTest script and calls test discovery function
  # >>  defined in it, so you donot have to create main function for tests,
  #  >> they will be auto discovered

  add_library(nomlib-unit-test
    ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
  )

  # CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
  # generate_export_header(nomlib-unit-test
  #   BASE_NAME
  #     NOM
  #   EXPORT_FILE_NAME
  #     export.hpp
  # )

  target_sources(nomlib-unit-test
    PRIVATE
    ${TESTS_SRC_DIR}/UnitTest/UnitTest.cpp
    PUBLIC
      FILE_SET HEADERS
      BASE_DIRS ${TESTS_HDR_BASE}
      FILES
      ${TESTS_HDR_BASE}/nomlib/tests/UnitTest.hpp
      ${TESTS_HDR_BASE}/nomlib/tests/UnitTest/UnitTest.hpp
      # FILE_SET generated_headers
      # TYPE HEADERS
      # BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
      # FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
  )

  add_dependencies(nomlib-unit-test gtest)

  target_include_directories(nomlib-unit-test
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIR}
    # TCLAP v1.2.1-release-final headers only distribution
    ${TCLAP_INCLUDE_DIR}
  )

  set_target_properties(nomlib-unit-test
    PROPERTIES
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
      DEBUG_POSTFIX "-d"
      # >> nomlib requires C++14 but GTest 10.x requires min
      # >> of C++17
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSION_REQUIRED OFF
      # >> No support for building test libraries as a shared library target
      # >> on WIN32
      CXX_VISIBILITY_PRESET default
      VISIBILITY_INLINES_HIDDEN FALSE
  )

  if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
    # Skip dllimport/dllexport logic when building as a static library target
    target_compile_definitions(nomlib-unit-test
      PUBLIC
        NOM_STATIC_DEFINE
    )
    # Ensure that our target can be cleanly combined or linked by shared
    # library targets; this adds -fPIC to the build for this unit
    set_target_properties(nomlib-unit-test
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
  endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

  # >> Link nomlib-unit-test to GTest
  target_link_libraries(nomlib-unit-test
    PUBLIC
    nomlib-core
    nomlib-file
    # !! This library target can be removed once nomlib-file becomes a proper
    # !! library target?
    nomlib-system
    PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
  )
endif(NOM_BUILD_UNIT_TEST_UNIT)

if(NOM_BUILD_VISUAL_UNIT_TEST_UNIT)
  add_library(nomlib-visual-unit-test
    ${LIBRARY_OUTPUT_TYPE} # STATIC || SHARED
  )

  # CMake generates our NOM_EXPORT macros, i.e.: __declspec(dllexport)
  # generate_export_header(nomlib-visual-unit-test
  #   BASE_NAME
  #     NOM
  #   EXPORT_FILE_NAME
  #     export.hpp
  # )

  target_sources(nomlib-visual-unit-test
  PRIVATE
    ${TESTS_SRC_DIR}/VisualUnitTest/ImageTestSet.cpp
    ${TESTS_SRC_DIR}/VisualUnitTest/VisualUnitTest.cpp
    ${TESTS_SRC_DIR}/VisualUnitTest/ImageDiff.cpp
    ${TESTS_SRC_DIR}/VisualUnitTest/TestResultWriter.cpp
    ${TESTS_SRC_DIR}/VisualUnitTest/VisualUnitTestResultWriter.cpp
    ${TESTS_SRC_DIR}/VisualUnitTest/HTMLTestResultWriter.cpp
    ${TESTS_SRC_DIR}/VisualUnitTest/MinimalTestResultWriter.cpp
    PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${TESTS_HDR_BASE}
    FILES
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/ImageTestSet.hpp
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/VisualUnitTest.hpp
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/ImageDiff.hpp
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/TestResultWriter.hpp
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/VisualUnitTestResultWriter.hpp
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/HTMLTestResultWriter.hpp
    ${TESTS_HDR_BASE}/nomlib/tests/VisualUnitTest/MinimalTestResultWriter.hpp
    # FILE_SET generated_headers
    # TYPE HEADERS
    # BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}
    # FILES ${CMAKE_CURRENT_BINARY_DIR}/export.hpp
  )
  add_dependencies(nomlib-visual-unit-test gtest)

  target_include_directories(nomlib-visual-unit-test
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${GTEST_INCLUDE_DIR}
    # TCLAP v1.2.1-release-final headers only distribution
    ${TCLAP_INCLUDE_DIR}
  )

  set_target_properties(nomlib-visual-unit-test
    PROPERTIES
      VERSION ${PROJECT_VERSION}
      SOVERSION ${PROJECT_VERSION_MAJOR}
      DEBUG_POSTFIX "-d"
      # >> nomlib requires C++14 but GTest 10.x requires min
      # >> of C++17
      CXX_STANDARD 17
      CXX_STANDARD_REQUIRED ON
      CXX_EXTENSION_REQUIRED OFF
      # >> No support for building test libraries as a shared library target
      # >> on WIN32
      CXX_VISIBILITY_PRESET default
      VISIBILITY_INLINES_HIDDEN FALSE
  )

  if(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")
    # Skip dllimport/dllexport logic when building as a static library target
    target_compile_definitions(nomlib-visual-unit-test
      PUBLIC
        NOM_STATIC_DEFINE
    )
    # Ensure that our target can be cleanly combined or linked by shared
    # library targets; this adds -fPIC to the build for this unit
    set_target_properties(nomlib-visual-unit-test
      PROPERTIES
        POSITION_INDEPENDENT_CODE ON
    )
  endif(LIBRARY_OUTPUT_TYPE MATCHES "STATIC")

  target_link_libraries(nomlib-visual-unit-test
    PUBLIC
    nomlib-core
    nomlib-math
    nomlib-graphics
    nomlib-ptree
    nomlib-serializers
    nomlib-unit-test
    PRIVATE
    Threads::Threads
    ${GTEST_LIBRARY}
  )
endif(NOM_BUILD_VISUAL_UNIT_TEST_UNIT)

if( NOM_BUILD_UNIT_TEST_TESTS )
  add_subdirectory("src/UnitTest")
endif( NOM_BUILD_UNIT_TEST_TESTS )

if( NOM_BUILD_VTEST_TESTS )
  add_subdirectory("src/VisualUnitTest")
endif( NOM_BUILD_VTEST_TESTS )

if( NOM_BUILD_CORE_TESTS )
  add_subdirectory("src/core")
endif( NOM_BUILD_CORE_TESTS )

if(NOM_BUILD_AUDIO_UNIT AND NOM_BUILD_AUDIO_TESTS)
  add_subdirectory("src/audio")
endif()

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
if(WIN32 AND USE_OLD_WINDOWS_CODE)

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
endif(WIN32 AND USE_OLD_WINDOWS_CODE)

# Resources/tests -> <out>/tests/Resources
install_resource_dir( "${NOM_TESTS_RESOURCES_DIR}"
                      "${TESTS_INSTALL_DIR}/Resources")

