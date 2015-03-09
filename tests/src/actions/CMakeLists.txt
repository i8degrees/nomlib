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

  # Any frame rate that your hardware can handle should be fine here; a
  # conservative value is chosen here chiefly to lower system utilization
  # during test runs.
  set( ACTION_TEST_ARGS --fps 30 )

  # Helper function for adding action tests
  macro( add_action_test test_name )
    set( ACTION_TEST_BIN "${TESTS_INSTALL_DIR}/ActionTest" )

    # Extended tests are ignored by default; they are intended to be explicitly
    # ran by invoking ctest with the "EXT" build configuration: ctest -C EXT
    if( ${test_name} MATCHES "Extended*" )
      set( ACTION_TEST_CFG_TYPE "EXT" )
    endif()

    add_test( NAME
              ${test_name}
              CONFIGURATIONS
              ${ACTION_TEST_CFG_TYPE}
              COMMAND
              ${ACTION_TEST_BIN}
              ${ARGN} )
  endmacro()

  # Fractional speed modifier values, such as 22.5 and the oddball number of 45
  # seemed to carry the most bugs during development of this feature.
  add_action_test(  ActionTest_SpeedModifierAt22.5
                    ${ACTION_TEST_ARGS} --speed 22.5 --timing-mode linear )

  # NOTE: To conserve on total test duration, we only run a minimal selection
  # of tests for this engine feature -- the following tests below are intended
  # to be explicitly called by the end-user, i.e.: 'ctest -C EXT -R <test>'.
  # Running these tests in parallel is possible with the -j option, and will
  # save considerable time; see also: man 1 ctest.
  #
  # It is worth noting that the act of running these tests in parallel can in
  # itself risk false positive test failures, due to the inherit relationship
  # between performance and frame rates. It should go without saying that,
  # ideally, these tests should be ran when more than adequate system resources
  # are able to be given out.

  # Tests for checking the frame time stability of the speed modifiers
  add_action_test(  ActionTestExtended_SpeedModifierAt0.5
                    ${ACTION_TEST_ARGS} --speed 0.5 )

  add_action_test(  ActionTestExtended_SpeedModifierAt1.0
                    ${ACTION_TEST_ARGS} --speed 1.0 )

  # NOTE: Only the standard five (5) timing modes are tested -- there are
  # several more timing modes available in the testing unit; see test
  # documentation by passing '--help' to ActionTest from the command line.
  add_action_test(  ActionTestExtended_QuadEaseIn
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quad_ease_in )

  add_action_test(  ActionTestExtended_QuadEaseOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quad_ease_out )

  add_action_test(  ActionTestExtended_QuadEaseInOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quad_ease_in_out )

  add_action_test(  ActionTestExtended_CubicEaseIn
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode cubic_ease_in )

  add_action_test(  ActionTestExtended_CubicEaseOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode cubic_ease_out )

  add_action_test(  ActionTestExtended_CubicEaseInOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode cubic_ease_in_out )

  add_action_test(  ActionTestExtended_QuartEaseIn
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quart_ease_in )

  add_action_test(  ActionTestExtended_QuartEaseOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quart_ease_out )

  add_action_test(  ActionTestExtended_QuartEaseInOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quart_ease_in_out )

  add_action_test(  ActionTestExtended_QuintEaseIn
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quint_ease_in )

  add_action_test(  ActionTestExtended_QuintEaseOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quint_ease_out )

  add_action_test(  ActionTestExtended_QuintEaseInOut
                    ${ACTION_TEST_ARGS} --speed 100.0
                    --timing-mode quint_ease_in_out )

  # Copy resource files for the test
  install(  FILES
            "${NOM_TESTS_RESOURCES_DIR}/actions/ActionTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
  )

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
