# nomlib-gui module tests

set( NOM_BUILD_GUI_BASE_TEST ON )
set( NOM_BUILD_GUI_DATAGRID_TESTS ON )

if( EXISTS "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
  include( "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
endif()

if( NOM_BUILD_GUI_BASE_TEST )

  add_executable( libRocketTest "libRocketTest.cpp" )

  target_link_libraries( libRocketTest nomlib-visual-unit-test nomlib-gui )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/libRocketTest
                    "" # args
                    "libRocketTest.cpp" )

  # Search paths to use in finding resources path
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/libRocketTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif( NOM_BUILD_GUI_BASE_TEST )

if( NOM_BUILD_GUI_DATAGRID_TESTS )

  add_executable( libRocketDataGridTest
                  "datagrid/Card.cpp"
                  "datagrid/CardCollection.cpp"
                  "datagrid/CardsPageDataSource.cpp"
                  "libRocketDataGridTest.cpp" )

  target_link_libraries( libRocketDataGridTest nomlib-visual-unit-test nomlib-gui )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/libRocketDataGridTest
                    "" # args
                    "libRocketDataGridTest.cpp" )

  # Search paths file to use in finding resources path
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/libRocketDataGridTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif( NOM_BUILD_GUI_DATAGRID_TESTS )

# Install additional binary dependencies for the unit tests
if ( PLATFORM_WINDOWS )

  install(  DIRECTORY
            ${LIBROCKET_REDIST_DIR}
            ${LIBROCKET_EXT_REDIST_DIR}
            DESTINATION ${TESTS_INSTALL_DIR}
            OPTIONAL # In case end-user overrides third-party dependencies
            FILES_MATCHING PATTERN "*.dll"
  )

endif ( PLATFORM_WINDOWS )

