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

if( PLATFORM_WINDOWS )

  # Path under Windows should resolve to: build/tests/Debug
  set ( TESTS_DEPS_INSTALL_PEFIX
        "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_BUILD_TYPE}"
      )

else( NOT PLATFORM_WINDOWS )

  # Path should resolve to a directory: build/tests
  set ( TESTS_DEPS_INSTALL_PEFIX
        "${CMAKE_CURRENT_BINARY_DIR}"
      )

endif( PLATFORM_WINDOWS )

# NOTE: we set GTEST_ROOT in the dependencies section of our build script.
find_package( GTest REQUIRED )

include_directories( ${GTEST_INCLUDE_DIRS} )
include_directories( "${NOM_TESTS_DIR}" )

include( "${NOM_TESTS_DIR}/graphics/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/math/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/system/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/gui/CMakeLists.txt" )

include( "${NOM_TESTS_DIR}/serializers/CMakeLists.txt" )
include( "${NOM_TESTS_DIR}/ptree/CMakeLists.txt" )

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
                          ${GTEST_BOTH_LIBRARIES}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    Template
                    "Template"
                    "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/UnitTestTemplate.cpp"
                  )

endif( NOM_BUILD_TEMPLATES_UNIT )
