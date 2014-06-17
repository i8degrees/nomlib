if( NOM_BUILD_MATH_TESTS )

  if( NOM_BUILD_MATH_UNIT )

    add_executable  (
                      math
                      ${NOM_TESTS_SOURCE_DIR}/math/MathTest.cpp
                    )

    target_link_libraries (
                            math
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      math
                      "MathTest"
                      ${NOM_TESTS_SOURCE_DIR}/math/MathTest.cpp
                    )

  endif( NOM_BUILD_MATH_UNIT )

endif( NOM_BUILD_MATH_TESTS )
