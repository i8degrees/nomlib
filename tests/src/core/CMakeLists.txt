# Core engine tests

if(NOM_BUILD_VERSION_INFO_TEST)

  add_executable  (
                    VersionInfoTest
                    ${NOM_TESTS_SOURCE_DIR}/core/VersionInfoTest.cpp
                  )

  target_link_libraries( VersionInfoTest UnitTest )

  GTEST_ADD_TESTS (
                    VersionInfoTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/core/VersionInfoTest.cpp
                  )

endif(NOM_BUILD_VERSION_INFO_TEST)

if( NOM_BUILD_SDL2_LOGGER_TESTS )

  # SDL2Logger
  add_executable  (
                    SDL2LoggerTest
                    ${NOM_TESTS_SOURCE_DIR}/core/SDL2LoggerTest.cpp
                  )

  target_link_libraries( SDL2LoggerTest UnitTest )

  GTEST_ADD_TESTS (
                    SDL2LoggerTest
                    ""
                    ${NOM_TESTS_SOURCE_DIR}/core/SDL2LoggerTest.cpp
                  )

  # ConsoleOutput

  add_executable  (
                    ConsoleOutputTest
                    ${NOM_TESTS_SOURCE_DIR}/core/ConsoleOutputTest.cpp
                  )

  target_link_libraries( ConsoleOutputTest UnitTest )

  GTEST_ADD_TESTS (
                    ConsoleOutputTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/core/ConsoleOutputTest.cpp
                  )

endif( NOM_BUILD_SDL2_LOGGER_TESTS )
