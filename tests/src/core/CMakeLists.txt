# nomlib-core module tests

set(NOM_BUILD_VERSION_INFO_TEST ON)
set(NOM_BUILD_SDL2_LOGGER_TESTS ON)
set(NOM_BUILD_STRINGS_TEST ON)

if(NOM_BUILD_VERSION_INFO_TEST)

  add_executable(VersionInfoTest
    #WIN32 # Link SDL2main over WINMAIN for console apps
    VersionInfoTest.cpp
  )

  add_dependencies(VersionInfoTest gtest)

  target_include_directories(VersionInfoTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
  )

  # FIXME: We only need nomlib-system for init...
  target_link_libraries(VersionInfoTest
    PUBLIC
      nomlib-core
      nomlib-system
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/VersionInfoTest
                    "" # args
                    "VersionInfoTest.cpp" )

endif(NOM_BUILD_VERSION_INFO_TEST)

if( NOM_BUILD_SDL2_LOGGER_TESTS )

  add_executable(SDL2LoggerTest
    #WIN32 # Link SDL2main over WINMAIN for console apps
    SDL2LoggerTest.cpp
  )

  add_dependencies(SDL2LoggerTest gtest)

  target_include_directories(SDL2LoggerTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${SDL2_INCLUDE_DIR}
      ${GTEST_INCLUDE_DIR}
  )

  target_link_libraries(SDL2LoggerTest
    PUBLIC
      nomlib-core
      nomlib-math
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/SDL2LoggerTest
                    "" # args
                    "SDL2LoggerTest.cpp" )

  add_executable(ConsoleOutputTest
    #WIN32 # Link SDL2main over WINMAIN for console apps
    ConsoleOutputTest.cpp
  )

  add_dependencies(ConsoleOutputTest gtest)

  target_include_directories(ConsoleOutputTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${SDL2_INCLUDE_DIR}
      ${GTEST_INCLUDE_DIR}
  )

  # NOTE: We don't actually need nomlib-math for this test
  target_link_libraries(ConsoleOutputTest
    PUBLIC
      nomlib-core
      #nomlib-math
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ConsoleOutputTest
                    "" # args
                    "ConsoleOutputTest.cpp" )

endif( NOM_BUILD_SDL2_LOGGER_TESTS )

if(NOM_BUILD_STRINGS_TEST)

  add_executable(StringTest
    #WIN32 # Link SDL2main over WINMAIN for console apps
    StringTest.cpp
  )

  add_dependencies(StringTest gtest)

  target_include_directories(StringTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${SDL2_INCLUDE_DIR}
      ${GTEST_INCLUDE_DIR}
  )

  target_link_libraries(StringTest
    PUBLIC
      nomlib-core
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(${TESTS_INSTALL_DIR}/StringTest
                  "" # args
                  "StringTest.cpp")

endif(NOM_BUILD_STRINGS_TEST)
