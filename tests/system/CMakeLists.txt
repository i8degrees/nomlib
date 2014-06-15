set( NOM_BUILD_FILE_TESTS on )

set( NOM_BUILD_FONT_CACHE_TESTS on )
set( NOM_BUILD_COLOR_DB_TESTS on )

if( NOM_BUILD_FONT_CACHE_TESTS )

  if( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      font_cache
                      ${NOM_TESTS_DIR}/system/FontCacheTest.cpp
                      )

    target_link_libraries (
                            font_cache
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      font_cache
                      "FontCacheTest"
                      ${NOM_TESTS_DIR}/system/FontCacheTest.cpp
                      )

    # FIXME:
    if( PLATFORM_WINDOWS )
      install (
                DIRECTORY
                "${NOMLIB_FONTS}" # Resources/fonts
                DESTINATION
                "${TESTS_DEPS_INSTALL_PEFIX}/Resources"
                PATTERN ".*" EXCLUDE # Just say no to dotfiles
              )
    endif( PLATFORM_WINDOWS )

  endif( NOM_BUILD_GRAPHICS_UNIT AND NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_FONT_CACHE_TESTS )

if( NOM_BUILD_COLOR_DB_TESTS )

  if( NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      color_database
                      ${NOM_TESTS_DIR}/system/ColorDatabaseTest.cpp
                    )

    target_link_libraries (
                            color_database
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      color_database
                      "ColorDatabaseTest"
                      ${NOM_TESTS_DIR}/system/ColorDatabaseTest.cpp
                    )

  endif( NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_COLOR_DB_TESTS )

if( NOM_BUILD_FILE_TESTS )

  if( NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      file_interface
                      ${NOM_TESTS_DIR}/system/FileInterfaceTest.cpp
                    )

    target_link_libraries (
                            file_interface
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      file_interface
                      "FileInterfaceTest"
                      ${NOM_TESTS_DIR}/system/FileInterfaceTest.cpp
                    )

  endif( NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_FILE_TESTS )
