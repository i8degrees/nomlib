# nomlib-gui module tests

set( NOM_BUILD_GUI_BASE_TEST ON )
set( NOM_BUILD_GUI_DATAGRID_TESTS ON )

if( NOM_BUILD_GUI_BASE_TEST )

  add_executable(libRocketTest
    WIN32 # GUI app
    libRocketTest.cpp
  )

  add_dependencies(libRocketTest gtest)

  target_include_directories(libRocketTest
    PUBLIC
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
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  nom_add_visual_test(  libRocketTest
                        ${TESTS_INSTALL_DIR}/libRocketTest )

  # For development: Search paths to use in finding resources path
  add_custom_command(TARGET libRocketTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/libRocketTest.json"
    $<TARGET_FILE_DIR:libRocketTest>
  )

endif( NOM_BUILD_GUI_BASE_TEST )

if( NOM_BUILD_GUI_DATAGRID_TESTS )

  add_executable(libRocketDataGridTest
    WIN32 # GUI app
    datagrid/Card.cpp
    datagrid/CardCollection.cpp
    datagrid/CardsPageDataSource.cpp
    libRocketDataGridTest.cpp
  )

  add_dependencies(libRocketDataGridTest gtest)

  target_include_directories(libRocketDataGridTest
    PUBLIC
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
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
      $<IF:$<TARGET_EXISTS:RocketControls>,RocketControls,RocketControls-static>
  )

  nom_add_visual_test(  libRocketDataGridTest
                        ${TESTS_INSTALL_DIR}/libRocketDataGridTest )

  # For development: Search paths file to use in finding resources path
  add_custom_command(TARGET libRocketDataGridTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/libRocketDataGridTest.json"
    $<TARGET_FILE_DIR:libRocketDataGridTest>
  )

endif( NOM_BUILD_GUI_DATAGRID_TESTS )

