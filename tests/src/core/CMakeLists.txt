# Core engine tests

if( NOM_BUILD_SDL2_LOGGER_TESTS )

  # SDL2Logger
  add_executable  (
                    SDL2LoggerTest
                    ${NOM_TESTS_SOURCE_DIR}/core/SDL2LoggerTest.cpp
                  )

  target_link_libraries (
                          SDL2LoggerTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    SDL2LoggerTest
                    ""
                    ${NOM_TESTS_SOURCE_DIR}/core/SDL2LoggerTest.cpp
                  )

  # ConsoleOutput

  add_executable  (
                    console_output_test
                    ${NOM_TESTS_SOURCE_DIR}/core/ConsoleOutputTest.cpp
                  )

  target_link_libraries (
                          console_output_test
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    console_output_test
                    ""
                    ${NOM_TESTS_SOURCE_DIR}/core/ConsoleOutputTest.cpp
                  )

endif( NOM_BUILD_SDL2_LOGGER_TESTS )
