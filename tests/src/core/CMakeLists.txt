# nomlib-core module tests

set(NOM_BUILD_VERSION_INFO_TEST ON)
set(NOM_BUILD_SDL2_LOGGER_TESTS ON)
set(NOM_BUILD_STRINGS_TEST ON)

if( EXISTS "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
  include( "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
endif()

if(NOM_BUILD_VERSION_INFO_TEST)

  # FIXME: We only need nomlib-system for init...
  set( NOM_CORE_TESTS_DEPS ${GTEST_LIBRARY} nomlib-core nomlib-system )

  if( PLATFORM_WINDOWS )
    list( APPEND NOM_CORE_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  add_executable( VersionInfoTest "VersionInfoTest.cpp" )

  target_link_libraries( VersionInfoTest ${NOM_CORE_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/VersionInfoTest
                    "" # args
                    "VersionInfoTest.cpp" )

endif(NOM_BUILD_VERSION_INFO_TEST)

if( NOM_BUILD_SDL2_LOGGER_TESTS )

  set( NOM_CORE_TESTS_DEPS ${GTEST_LIBRARY} nomlib-core nomlib-math )

  if( PLATFORM_WINDOWS )
    list( APPEND NOM_CORE_TESTS_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  add_executable( SDL2LoggerTest "SDL2LoggerTest.cpp" )

  target_link_libraries( SDL2LoggerTest ${NOM_CORE_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/SDL2LoggerTest
                    "" # args
                    "SDL2LoggerTest.cpp" )

  add_executable( ConsoleOutputTest
                  "ConsoleOutputTest.cpp" )

  # NOTE: We don't actually need nomlib-math for this test
  target_link_libraries( ConsoleOutputTest ${NOM_CORE_TESTS_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ConsoleOutputTest
                    "" # args
                    "ConsoleOutputTest.cpp" )

endif( NOM_BUILD_SDL2_LOGGER_TESTS )

if(NOM_BUILD_STRINGS_TEST)

  set(NOM_CORE_TESTS_DEPS ${GTEST_LIBRARY} nomlib-core)

  if(PLATFORM_WINDOWS)
    list(APPEND NOM_CORE_TESTS_DEPS ${SDL2MAIN_LIBRARY})
  endif(PLATFORM_WINDOWS)

  add_executable(StringTest "StringTest.cpp")

  target_link_libraries(StringTest ${NOM_CORE_TESTS_DEPS})

  GTEST_ADD_TESTS(${TESTS_INSTALL_DIR}/StringTest
                  "" # args
                  "StringTest.cpp")

endif(NOM_BUILD_STRINGS_TEST)
