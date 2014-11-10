
if( NOM_BUILD_FONT_CACHE_TESTS )

  add_executable  (
                    FontCacheTest
                    ${NOM_TESTS_COMMON_SOURCE} # nom::UnitTest
                    ${NOM_TESTS_SOURCE_DIR}/system/FontCacheTest.cpp
                    )

  target_link_libraries (
                          FontCacheTest
                          ${GTEST_LIBRARY}
                          ${PROJECT_NAME} # nomlib
                        )

  GTEST_ADD_TESTS (
                    FontCacheTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/system/FontCacheTest.cpp
                    )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/FontCacheTest.json"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
          )

endif( NOM_BUILD_FONT_CACHE_TESTS )

if( NOM_BUILD_COLOR_DB_TESTS )

  if( NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      color_database
                      ${NOM_TESTS_SOURCE_DIR}/system/ColorDatabaseTest.cpp
                    )

    target_link_libraries (
                            color_database
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      color_database
                      "ColorDatabaseTest"
                      ${NOM_TESTS_SOURCE_DIR}/system/ColorDatabaseTest.cpp
                    )

  endif( NOM_BUILD_MATH_UNIT AND NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_COLOR_DB_TESTS )

if( NOM_BUILD_FILE_TESTS )

  if( NOM_BUILD_SYSTEM_UNIT )

    add_executable  (
                      file_interface
                      ${NOM_TESTS_SOURCE_DIR}/system/FileInterfaceTest.cpp
                    )

    target_link_libraries (
                            file_interface
                            ${GTEST_LIBRARY}
                            ${PROJECT_NAME} # nomlib
                          )

    GTEST_ADD_TESTS (
                      file_interface
                      "FileInterfaceTest"
                      ${NOM_TESTS_SOURCE_DIR}/system/FileInterfaceTest.cpp
                    )

  endif( NOM_BUILD_SYSTEM_UNIT )

endif( NOM_BUILD_FILE_TESTS )
