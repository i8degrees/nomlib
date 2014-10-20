if( NOM_BUILD_MATH_TESTS )

    add_executable  (
                      MathTest
                      ${NOM_TESTS_SOURCE_DIR}/math/MathTest.cpp
                    )

    target_link_libraries (
                            MathTest
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      MathTest
                      "" # args
                      ${NOM_TESTS_SOURCE_DIR}/math/MathTest.cpp
                    )

endif( NOM_BUILD_MATH_TESTS )
