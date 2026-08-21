# nomlib-actions module tests

set( NOM_BUILD_ACTION_TESTS ON )
set( NOM_BUILD_ACTION_TIMING_CURVES_TESTS ON )

if( NOM_BUILD_ACTION_TESTS )

  add_executable(ActionTest
    WIN32 # gui app
    ActionTest.cpp
    ActionTest_ActionPlayer.cpp
    ActionTest_AlphaBlending.cpp
    ActionTest_Reversed.cpp
    ActionTest_Demos.cpp
    ActionTest_Repeating.cpp
  )

  add_dependencies(ActionTest gtest)

  target_include_directories(ActionTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
      # TCLAP v1.2.1-release-final headers only distribution
      ${TCLAP_INCLUDE_DIR}
  )

  target_link_libraries(ActionTest
    PUBLIC
      nomlib-actions
      nomlib-core
      nomlib-graphics
      nomlib-visual-unit-test
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  set( ACTION_TEST_BIN "${TESTS_INSTALL_DIR}/ActionTest" )

  # NOTE: Any frame rate that your hardware can handle should be fine here; a
  # conservative value is chosen here chiefly to lower system utilization
  # during test runs.
  set( ACTION_TEST_ARGS --fps 30 )

  # Fractional speed modifier values, such as 22.5 and the oddball number of 45
  # seemed to carry the most bugs during development of this feature.
  nom_add_test( ActionTest_SpeedModifierAt22.5 ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --speed 22.5 --timing-mode linear )

  # NOTE: The total test duration of these tests when ran sequentially is
  # around ~900 seconds. Therefore, it is strongly recommended that you run
  # these tests in parallel when possible -- whether you have more than one
  # physical processing core or not does not matter so much, as all but one
  # test is designed to be light on system load.
  #
  #   See also: man 1 ctest, --parallel <jobs>
  nom_add_test( ActionTest_SpeedModifierAt0.5 ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --speed 0.5 )

  # NOTE: Only the standard five timing curve functions are tested here; there
  # are many more modes available in the testing suite!
  #
  #     See also: ActionTest --help, --timing-mode
  set( ACTION_TEST_ARGS ${ACTION_TEST_ARGS} --speed 100.0 )

  nom_add_test( ActionTest_QuadEaseIn ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quad_ease_in )

  nom_add_test( ActionTest_QuadEaseOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quad_ease_out )

  nom_add_test( ActionTest_QuadEaseInOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quad_ease_in_out )

  nom_add_test( ActionTest_CubicEaseIn ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode cubic_ease_in )

  nom_add_test( ActionTest_CubicEaseOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode cubic_ease_out )

  nom_add_test( ActionTest_CubicEaseInOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode cubic_ease_in_out )

  nom_add_test( ActionTest_QuartEaseIn ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quart_ease_in )

  nom_add_test( ActionTest_QuartEaseOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quart_ease_out )

  nom_add_test( ActionTest_QuartEaseInOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quart_ease_in_out )

  nom_add_test( ActionTest_QuintEaseIn ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quint_ease_in )

  nom_add_test( ActionTest_QuintEaseOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quint_ease_out )

  nom_add_test( ActionTest_QuintEaseInOut ${ACTION_TEST_BIN}
                ${ACTION_TEST_ARGS} --timing-mode quint_ease_in_out )

  # For development: Copy resource files for the test
  add_custom_command(TARGET ActionTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/actions/ActionTest.json"
    $<TARGET_FILE_DIR:ActionTest>
  )

endif( NOM_BUILD_ACTION_TESTS )

if( NOM_BUILD_ACTION_TIMING_CURVES_TESTS )

  add_executable(ActionTimingCurvesTest
    WIN32 # GUI app
    ActionTimingCurvesTest.cpp
  )

  add_dependencies(ActionTimingCurvesTest gtest)

  target_include_directories(ActionTimingCurvesTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(ActionTimingCurvesTest
    PUBLIC
      nomlib-actions
      nomlib-core
      nomlib-unit-test
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ActionTimingCurvesTest
                    "" # args
                    ActionTimingCurvesTest.cpp)

endif( NOM_BUILD_ACTION_TIMING_CURVES_TESTS )
