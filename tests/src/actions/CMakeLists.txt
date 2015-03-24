# nomlib-actions module tests

set( NOM_BUILD_ACTION_TESTS ON )
set( NOM_BUILD_ACTION_TIMING_CURVES_TESTS ON )

if( EXISTS "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
  include( "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
endif()

if( NOM_BUILD_ACTION_TESTS )

  set(  ACTION_SRC
        ${ACTION_SRC}
        "ActionTest.cpp"
        "ActionTest_ActionPlayer.cpp"
        "ActionTest_AlphaBlending.cpp"
        "ActionTest_Reversed.cpp"
        "ActionTest_Demos.cpp"
        "ActionTest_Repeating.cpp"
  )

  add_executable( ActionTest ${ACTION_SRC} )

  target_link_libraries( ActionTest nomlib-visual-unit-test nomlib-graphics )

  # NOTE: Any frame rate that your hardware can handle should be fine here; a
  # conservative value is chosen here chiefly to lower system utilization
  # during test runs.
  set( ACTION_TEST_ARGS --fps 30 )

  # Helper function for adding action tests
  macro( add_action_test test_name )
    set( ACTION_TEST_BIN "${TESTS_INSTALL_DIR}/ActionTest" )
    # NOTE: Avoid using the newer add_test syntax, i.e.:
    # add_test(NAME <name> COMMAND <command>), because these tests are not
    # added to the default test configuration! Using the newer add_test
    # syntax leads me to this err message when running ctest from the project's
    # build directory:
    #   "Test not available without configuration. (Missing "-C <config>"?)"
    add_test( ${test_name} ${ACTION_TEST_BIN} ${ARGN} )
  endmacro()

  # Fractional speed modifier values, such as 22.5 and the oddball number of 45
  # seemed to carry the most bugs during development of this feature.
  add_action_test(  ActionTest_SpeedModifierAt22.5
                    ${ACTION_TEST_ARGS} --speed 22.5 --timing-mode linear )

  # NOTE: The total test duration of these tests when ran sequentially is
  # around ~900 seconds. Therefore, it is strongly recommended that you run
  # these tests in parallel when possible -- whether you have more than one
  # physical processing core or not does not matter so much, as all but one
  # test is designed to be light on system load.
  #
  #   See also: man 1 ctest, --parallel <jobs>

  add_action_test(  ActionTest_SpeedModifierAt0.5
                    ${ACTION_TEST_ARGS} --speed 0.5 )

  # NOTE: Only the standard five timing curve functions are tested here; there
  # are many more modes available in the testing suite!
  #
  #     See also: ActionTest --help, --timing-mode
  set( ACTION_TEST_SPEED_ARGS --speed 100.0 )
  add_action_test(  ActionTest_QuadEaseIn
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quad_ease_in )

  add_action_test(  ActionTest_QuadEaseOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quad_ease_out )

  add_action_test(  ActionTest_QuadEaseInOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quad_ease_in_out )

  add_action_test(  ActionTest_CubicEaseIn
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode cubic_ease_in )

  add_action_test(  ActionTest_CubicEaseOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode cubic_ease_out )

  add_action_test(  ActionTest_CubicEaseInOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode cubic_ease_in_out )

  add_action_test(  ActionTest_QuartEaseIn
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quart_ease_in )

  add_action_test(  ActionTest_QuartEaseOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quart_ease_out )

  add_action_test(  ActionTest_QuartEaseInOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quart_ease_in_out )

  add_action_test(  ActionTest_QuintEaseIn
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quint_ease_in )

  add_action_test(  ActionTest_QuintEaseOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quint_ease_out )

  add_action_test(  ActionTest_QuintEaseInOut
                    ${ACTION_TEST_ARGS} ${ACTION_TEST_SPEED_ARGS}
                    --timing-mode quint_ease_in_out )

  # Copy resource files for the test
  install(  FILES
            "${NOM_TESTS_RESOURCES_DIR}/actions/ActionTest.json"
            DESTINATION "${TESTS_INSTALL_DIR}" )

endif( NOM_BUILD_ACTION_TESTS )

if( NOM_BUILD_ACTION_TIMING_CURVES_TESTS )

  set(  ACTION_TIMING_CURVES_SRC
        ${ACTION_TIMING_CURVES_SRC}
        "ActionTimingCurvesTest.cpp" )

  add_executable( ActionTimingCurvesTest ${ACTION_TIMING_CURVES_SRC} )

  target_link_libraries(  ActionTimingCurvesTest nomlib-graphics
                          nomlib-unit-test )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/ActionTimingCurvesTest
                    "" # args
                    ${ACTION_TIMING_CURVES_SRC} )

endif( NOM_BUILD_ACTION_TIMING_CURVES_TESTS )
