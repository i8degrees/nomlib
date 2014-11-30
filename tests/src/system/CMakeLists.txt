# nomlib-system module tests

set( NOM_BUILD_FILE_TESTS ON )
set( NOM_BUILD_FONT_CACHE_TESTS ON )
set( NOM_BUILD_COLOR_DB_TESTS ON )

if( NOM_BUILD_FONT_CACHE_TESTS )

  add_executable( FontCacheTest "FontCacheTest.cpp" )

  set( FONT_CACHE_DEPS nomlib-unit-test nomlib-graphics nomlib-serializers )

  target_link_libraries( FontCacheTest ${FONT_CACHE_DEPS} )

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

  add_executable( ColorDatabaseTest "ColorDatabaseTest.cpp" )

  set( COLOR_DB_DEPS ${GTEST_LIBRARY} nomlib-system )

  if( PLATFORM_WINDOWS )
    list( APPEND COLOR_DB_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  target_link_libraries( ColorDatabaseTest ${COLOR_DB_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ColorDatabaseTest
                    "" # args
                    "ColorDatabaseTest.cpp" )

endif( NOM_BUILD_COLOR_DB_TESTS )

if( NOM_BUILD_FILE_TESTS )

  add_executable( FileInterfaceTest "FileInterfaceTest.cpp" )

  set( FILE_DEPS ${GTEST_LIBRARY} nomlib-file )

  if( PLATFORM_WINDOWS )
    list( APPEND FILE_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  target_link_libraries( FileInterfaceTest ${FILE_DEPS} )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/FileInterfaceTest
                    "" # args
                    "FileInterfaceTest.cpp" )

endif( NOM_BUILD_FILE_TESTS )
