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
set( NOM_BUILD_COMMON_TESTS off )

# Graphics tests
set( NOM_BUILD_RENDERWINDOW_TESTS off )

# GUI tests
set( NOM_BUILD_GUI_UI_ITEM_CONTAINER_TESTS on )
set( NOM_BUILD_GUI_UI_EVENT_DISPATCHER_TESTS on )
set( NOM_BUILD_GUI_UI_WIDGET_TESTS on )
set( NOM_BUILD_GUI_LISTBOX_LAYOUT_TESTS off )
set( NOM_BUILD_GUI_BUTTON_LAYOUT_TESTS off )
set( NOM_BUILD_GUI_DATAVIEWLIST_TESTS on )

# Math tests
set( NOM_BUILD_MATH_TESTS off )

# Property Tree tests
set( NOM_BUILD_PTREE_TESTS off )

# Serializer tests
set( NOM_BUILD_SERIALIZERS_JSON_TESTS off )
set( NOM_BUILD_SERIALIZERS_XML_TESTS off )
set( NOM_BUILD_JSON_CONFIG_FILE_TESTS off )
set( NOM_BUILD_SERIALIZERS_HTML_TESTS off )

# System tests
set( NOM_BUILD_FILE_TESTS off )
set( NOM_BUILD_FONT_CACHE_TESTS off )
set( NOM_BUILD_COLOR_DB_TESTS off )

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
      ${NOM_TESTS_COMMON_SOURCE}
      ${NOM_TESTS_SOURCE_DIR}/common/VisualTestSet.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTest.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/ImageDiff.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/TestResultWriter.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/VisualUnitTestResultWriter.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/HTMLTestResultWriter.cpp
      ${NOM_TESTS_SOURCE_DIR}/common/MinimalTestResultWriter.cpp
    )

include_directories( "${NOM_TESTS_HEADERS_DIR}" )

include( "${NOM_TESTS_SOURCE_DIR}/common/CMakeLists.txt" )
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

  add_executable  (
                    Template
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

  target_link_libraries ( Template
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    Template
                    "Template"
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

endif( NOM_BUILD_TEMPLATES_UNIT )
