# nomlib-gui module tests

set( NOM_BUILD_GUI_BASE_TEST ON )
set( NOM_BUILD_GUI_DATAGRID_TESTS ON )

if( NOM_BUILD_GUI_BASE_TEST )

  add_executable(libRocketTest
    WIN32 # ...
    libRocketTest.cpp
  )

  add_dependencies(libRocketTest gtest)

  target_include_directories(libRocketTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${libRocket_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIR}
  )

  target_link_libraries(libRocketTest
    PUBLIC
    nomlib-core
    nomlib-gui
    nomlib-visual-unit-test
    PRIVATE
    $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
  )

  nom_add_visual_test(  libRocketTest
                        ${TESTS_INSTALL_DIR}/libRocketTest )

  # Search paths to use in finding resources path
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/libRocketTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif( NOM_BUILD_GUI_BASE_TEST )

if( NOM_BUILD_GUI_DATAGRID_TESTS )

  add_executable(libRocketDataGridTest
    WIN32 # ...
    datagrid/Card.cpp
    datagrid/CardCollection.cpp
    datagrid/CardsPageDataSource.cpp
    libRocketDataGridTest.cpp
  )

  add_dependencies(libRocketDataGridTest gtest)

  target_include_directories(libRocketDataGridTest
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
    ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${libRocket_INCLUDE_DIRS}
    ${GTEST_INCLUDE_DIR}
  )

  target_link_libraries(libRocketDataGridTest
    PUBLIC
    nomlib-core
    nomlib-gui
    nomlib-visual-unit-test
    PRIVATE
    $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
    $<IF:$<TARGET_EXISTS:RocketControls>,RocketControls,RocketControls-static>
  )

  nom_add_visual_test(  libRocketDataGridTest
                        ${TESTS_INSTALL_DIR}/libRocketDataGridTest )

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
