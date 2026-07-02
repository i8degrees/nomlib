# nomlib-system module tests

set( NOM_BUILD_FILE_TESTS ON )
set( NOM_BUILD_FONT_CACHE_TESTS ON )
set( NOM_BUILD_COLOR_DB_TESTS ON )
set( NOM_BUILD_TIMER_TESTS ON )

if( NOM_BUILD_FONT_CACHE_TESTS )

  add_executable(FontCacheTest
    WIN32
    FontCacheTest.cpp
  )

  add_dependencies(FontCacheTest gtest)

  target_include_directories(FontCacheTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
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
    #$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
    #$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/FontCacheTest
                    "" # args
                    "FontCacheTest.cpp" )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/FontCacheTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )
endif( NOM_BUILD_FONT_CACHE_TESTS )

if( NOM_BUILD_COLOR_DB_TESTS )

  add_executable(ColorDatabaseTest
    WIN32
    ColorDatabaseTest.cpp
  )

  add_dependencies(ColorDatabaseTest gtest)

  #if( PLATFORM_WINDOWS )
    #list( APPEND COLOR_DB_DEPS ${SDL2MAIN_LIBRARY} )
  #endif( PLATFORM_WINDOWS )

  target_include_directories(ColorDatabaseTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
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
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ColorDatabaseTest
                    "" # args
                    "ColorDatabaseTest.cpp" )

endif( NOM_BUILD_COLOR_DB_TESTS )

if( NOM_BUILD_FILE_TESTS )

  add_executable(FileInterfaceTest
    WIN32
    FileInterfaceTest.cpp
  )

  add_dependencies(FileInterfaceTest gtest)

  #if( PLATFORM_WINDOWS )
    #list( APPEND FILE_DEPS ${SDL2MAIN_LIBRARY} )
  #endif( PLATFORM_WINDOWS )

  target_include_directories(FileInterfaceTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
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
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/FileInterfaceTest
                    "" # args
                    "FileInterfaceTest.cpp" )

endif( NOM_BUILD_FILE_TESTS )

if( NOM_BUILD_TIMER_TESTS )

  add_executable(TimerTest
    WIN32
    TimerTest.cpp
  )

  add_dependencies(TimerTest gtest)

  #if( PLATFORM_WINDOWS )
    #list( APPEND TIMER_DEPS ${SDL2MAIN_LIBRARY} )
  #endif( PLATFORM_WINDOWS )

  target_include_directories(TimerTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
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
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/TimerTest
                    "" # args
                    "TimerTest.cpp" )

endif( NOM_BUILD_TIMER_TESTS )
