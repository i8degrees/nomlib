# Unit testing with Google Test

set ( TESTS_DEPS_INSTALL_PEFIX
      "${CMAKE_CURRENT_BINARY_DIR}/tests/${CMAKE_BUILD_TYPE}"
    )

# NOTE: we set GTEST_ROOT in the dependencies section of our build script.
find_package( GTest REQUIRED )

include_directories( ${GTEST_INCLUDE_DIRS} )

add_executable  ( ptree
                  ${NOM_TESTS_DIR}/ptree.cpp
                )

target_link_libraries ( ptree
                        ${GTEST_BOTH_LIBRARIES}
                        # ${EXTLIBS}
                        ${PROJECT_NAME} # nomlib
                      )

GTEST_ADD_TESTS( ptree ptree ${NOM_TESTS_DIR}/ptree.cpp )

# Install library dependencies into tests output directory so we can always
# execute the binaries with the proper dependency versions.
if ( PLATFORM_WINDOWS )
  install ( DIRECTORY
            # "${SDL2_LIBRARY_DIR}"
            # "${SDL2_IMAGE_LIBRARY_DIR}"
            # "${SDL2_TTF_LIBRARY_DIR}"
            # "${OPENAL_LIBRARY_DIR}"
            # "${LIBSNDFILE_LIBRARY_DIR}"
            ${MSVCPP_LIBRARY_REDIST}
            "${GTEST_ROOT}/lib"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )
endif ( PLATFORM_WINDOWS )
