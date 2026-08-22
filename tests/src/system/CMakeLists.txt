# nomlib-system module tests

set(NOM_BUILD_FILE_TESTS ON)
set(NOM_BUILD_FONT_CACHE_TESTS ON)
set(NOM_BUILD_COLOR_DB_TESTS ON)
set(NOM_BUILD_TIMER_TESTS ON)

if(NOM_BUILD_FONT_CACHE_TESTS)

  add_executable(FontCacheTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    FontCacheTest.cpp
  )

  add_dependencies(FontCacheTest gtest)

  target_include_directories(FontCacheTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(FontCacheTest
    PUBLIC
      nomlib-core
      nomlib-graphics
      nomlib-serializers
      nomlib-unit-test
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      #$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/FontCacheTest
                    "" # args
                    "FontCacheTest.cpp" )

  # For development: Copy resource files for the test
  add_custom_command(TARGET FontCacheTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/graphics/FontCacheTest.json"
    $<TARGET_FILE_DIR:FontCacheTest>
  )
endif(NOM_BUILD_FONT_CACHE_TESTS)

if(NOM_BUILD_COLOR_DB_TESTS)

  add_executable(ColorDatabaseTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    ColorDatabaseTest.cpp
  )

  add_dependencies(ColorDatabaseTest gtest)

  target_include_directories(ColorDatabaseTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(ColorDatabaseTest
    PUBLIC
      nomlib-core
      nomlib-system
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ColorDatabaseTest
                    "" # args
                    "ColorDatabaseTest.cpp" )

endif(NOM_BUILD_COLOR_DB_TESTS)

if(NOM_BUILD_FILE_TESTS)

  add_executable(FileInterfaceTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    FileInterfaceTest.cpp
  )

  add_dependencies(FileInterfaceTest gtest)

  target_include_directories(FileInterfaceTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(FileInterfaceTest
    PUBLIC
      nomlib-core
      nomlib-file
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/FileInterfaceTest
                    "" # args
                    "FileInterfaceTest.cpp" )

endif(NOM_BUILD_FILE_TESTS)

if(NOM_BUILD_TIMER_TESTS)

  add_executable(TimerTest
    #WIN32 # Link SDL2main over WINMAIN (console apps)
    TimerTest.cpp
  )

  add_dependencies(TimerTest gtest)

  target_include_directories(TimerTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(TimerTest
    PUBLIC
      nomlib-core
      nomlib-system
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/TimerTest
                    "" # args
                    "TimerTest.cpp" )

endif(NOM_BUILD_TIMER_TESTS)

