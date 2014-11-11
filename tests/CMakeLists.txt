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
# NOTE: Compiling nomlib under Windows without iterator asserts --
# /D_ITERATOR_DBUG_LEVEL=0 -- will cause GTest to throw SEH exception errors
# before it is able to display actual & expected results when a unit test fails.
#
# NOTE: The custom command line processing used by nom::UnitTest for interactive
# interactive test runs breaks under Windows when building with the target
# library ${GTEST_BOTH_LIBRARIES}, due to GTest's main library overwriting our
# unit test's main functions. In summary, only the library target
# ${GTEST_LIBRARY} should be used, to ensure cross-platform support.

# UnitTest framework tests
set( NOM_BUILD_COMMON_TESTS TRUE )
set( NOM_BUILD_VTEST_TESTS TRUE )

# Core engine tests
set( NOM_BUILD_VERSION_INFO_TEST TRUE )
set( NOM_BUILD_SDL2_LOGGER_TESTS TRUE )

# Audio subsystem tests
set( NOM_BUILD_OPENAL_AUDIO_TESTS TRUE )

# Graphics tests
set( NOM_BUILD_RENDERWINDOW_TESTS TRUE )
set( NOM_BUILD_GRADIENT_TESTS TRUE )
set( NOM_BUILD_BITMAP_FONT_TEST TRUE )
set( NOM_BUILD_TRUETYPE_FONT_TEST TRUE )
set( NOM_BUILD_BMFONT_TEST TRUE )

# GUI tests
set( NOM_BUILD_GUI_TESTS TRUE )
set( NOM_BUILD_GUI_DATAGRID_TESTS TRUE )

# Math tests
set( NOM_BUILD_MATH_TESTS TRUE )

# Property Tree tests
set( NOM_BUILD_PTREE_TESTS TRUE )

# Serializer tests
set( NOM_BUILD_SERIALIZERS_JSON_TESTS TRUE )
set( NOM_BUILD_SERIALIZERS_XML_TESTS TRUE )
set( NOM_BUILD_JSON_CONFIG_FILE_TESTS TRUE )
set( NOM_BUILD_SERIALIZERS_HTML_TESTS TRUE )

# System tests
set( NOM_BUILD_FILE_TESTS TRUE )
set( NOM_BUILD_FONT_CACHE_TESTS TRUE )
set( NOM_BUILD_COLOR_DB_TESTS TRUE )

if( PLATFORM_WINDOWS OR CMAKE_GENERATOR STREQUAL "Xcode" )

  # Path under Windows should resolve to: build/tests/Debug
  set ( TESTS_DEPS_INSTALL_PEFIX
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
      )

else( NOT PLATFORM_WINDOWS )

  # Path should resolve to a directory: build/tests
  set ( TESTS_DEPS_INSTALL_PEFIX
        "${CMAKE_CURRENT_BINARY_DIR}"
      )

endif( PLATFORM_WINDOWS OR CMAKE_GENERATOR STREQUAL "Xcode" )

# NOTE: we set GTEST_ROOT in the dependencies section of our project's
# root build script.
find_package( GTest REQUIRED )

include_directories( ${GTEST_INCLUDE_DIRS} )

# TODO: Header file list is used for adding to project files (IDE)
#
# See also: src/CMakeLists.txt, source_group command
file ( GLOB NOM_TESTS_COMMON_HEADERS ${NOM_TESTS_COMMON_HEADERS} "${NOM_TESTS_HEADERS_DIR}/${PROJECT_NAME}/tests/common/*.hpp" )

set (
      NOM_TESTS_COMMON_SOURCE
      ${NOM_TESTS_SOURCE_DIR}/common/UnitTest.cpp
    )

set (
      NOM_TESTS_VTEST_SOURCE
      ${NOM_TESTS_SOURCE_DIR}/common/ImageTestSet.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTest.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/ImageDiff.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/TestResultWriter.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTestResultWriter.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/HTMLTestResultWriter.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/MinimalTestResultWriter.cpp
    )

if( BUILD_SHARED_LIBS )
  set( LIBRARY_OUTPUT_TYPE "SHARED" )
else( NOT BUILD_SHARED_LIBS )
  set( LIBRARY_OUTPUT_TYPE "STATIC" )
endif( BUILD_SHARED_LIBS )

# Link the nom::UnitTest library
add_library( UnitTest ${LIBRARY_OUTPUT_TYPE} ${NOM_TESTS_COMMON_SOURCE} )
target_link_libraries( UnitTest ${GTEST_LIBRARY} ${PROJECT_NAME} )

# Link the nom::VisualUnitTest library
add_library( VisualUnitTest ${LIBRARY_OUTPUT_TYPE} ${NOM_TESTS_VTEST_SOURCE} )
target_link_libraries( VisualUnitTest UnitTest ${GTEST_LIBRARY} ${PROJECT_NAME} )

include_directories( "${NOM_TESTS_HEADERS_DIR}" )

include( "${NOM_TESTS_SOURCE_DIR}/common/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/core/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/audio/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/graphics/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/gui/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/math/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/ptree/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/system/CMakeLists.txt" )
include( "${NOM_TESTS_SOURCE_DIR}/serializers/CMakeLists.txt" )

# Install library dependencies into tests output directory so we can always
# execute the binaries with the proper dependency versions.
if ( PLATFORM_WINDOWS )
  install ( DIRECTORY
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            "${MSVCPP_LIBRARY_REDIST}"
            "${GTEST_ROOT}/lib/"        # GTest re-dist libs
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )
endif ( PLATFORM_WINDOWS )

if( NOM_BUILD_TEMPLATES_UNIT )

  set( EXECUTABLE_NAME "UnitTestTemplate" )

  add_executable  (
                    ${EXECUTABLE_NAME}
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

  target_link_libraries( ${EXECUTABLE_NAME} UnitTest )

  GTEST_ADD_TESTS (
                    ${EXECUTABLE_NAME}
                    "" # args
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

  set( EXECUTABLE_NAME "VisualUnitTestTemplate" )

  add_executable  (
                    ${EXECUTABLE_NAME}
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/VisualUnitTestTemplate.cpp"
                  )

  target_link_libraries( ${EXECUTABLE_NAME} VisualUnitTest )

endif( NOM_BUILD_TEMPLATES_UNIT )
