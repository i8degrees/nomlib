if( NOM_BUILD_GUI_TESTS )

  add_executable  (
                    libRocketTest
                    ${NOM_TESTS_SOURCE_DIR}/gui/libRocketTest.cpp
                  )

  target_link_libraries( libRocketTest UnitTest VisualUnitTest )

    GTEST_ADD_TESTS (
                      libRocketTest
                      "" # args
                      ${NOM_TESTS_SOURCE_DIR}/gui/libRocketTest.cpp
                    )

    # Search paths to use in finding resources path
    install (
              FILES
              "${NOM_TESTS_RESOURCES_DIR}/libRocketTest.json"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )

endif( NOM_BUILD_GUI_TESTS )

if( NOM_BUILD_GUI_DATAGRID_TESTS )

  add_executable  (
                    libRocketDataGridTest
                    ${NOM_TESTS_SOURCE_DIR}/gui/datagrid/Card.cpp
                    ${NOM_TESTS_SOURCE_DIR}/gui/datagrid/CardCollection.cpp
                    ${NOM_TESTS_SOURCE_DIR}/gui/datagrid/CardsPageDataSource.cpp
                    ${NOM_TESTS_SOURCE_DIR}/gui/libRocketDataGridTest.cpp
                  )

  target_link_libraries( libRocketDataGridTest UnitTest VisualUnitTest )

    GTEST_ADD_TESTS (
                      libRocketDataGridTest
                      "" # args
                      ${NOM_TESTS_SOURCE_DIR}/gui/libRocketDataGridTest.cpp
                    )

    # Search paths file to use in finding resources path
    install (
              FILES
              "${NOM_TESTS_RESOURCES_DIR}/libRocketDataGridTest.json"
              DESTINATION
              "${TESTS_DEPS_INSTALL_PEFIX}"
            )

endif( NOM_BUILD_GUI_DATAGRID_TESTS )

# Install binary dependencies for the unit tests
if ( PLATFORM_WINDOWS )

  install ( DIRECTORY
            "${SDL2_LIBRARY_DIR}"
            "${SDL2_IMAGE_LIBRARY_DIR}"
            "${SDL2_TTF_LIBRARY_DIR}"
            "${OPENAL_LIBRARY_DIR}"
            "${LIBSNDFILE_LIBRARY_DIR}"
            "${MSVCPP_LIBRARY_REDIST}"
            "${GTEST_ROOT}/lib/"        # GTest re-dist libs
            "${GLEW_REDIST_DIR}"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
            FILES_MATCHING PATTERN "*.dll"
          )

  if( NOM_BUILD_GUI_UNIT )

    install_dependencies  (
                            "${LIBROCKET_REDIST_DIRS}"
                            "${TESTS_DEPS_INSTALL_PEFIX}"
                            "*.dll"
                          )

  endif( NOM_BUILD_GUI_UNIT )

endif ( PLATFORM_WINDOWS )

