# Unit testing with Google Test

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
