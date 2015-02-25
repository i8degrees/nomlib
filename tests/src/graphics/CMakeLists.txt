# nomlib-graphics module tests

# set( NOM_BUILD_RENDERWINDOW_TESTS ON )
# set( NOM_BUILD_GRADIENT_TESTS ON )
# set( NOM_BUILD_BITMAP_FONT_TEST ON )
# set( NOM_BUILD_TRUETYPE_FONT_TEST ON )
# set( NOM_BUILD_BMFONT_TEST ON )
set( NOM_BUILD_ANIMATION_TESTS ON )
# set( NOM_BUILD_SPRITE_TESTS ON )

if( NOM_BUILD_RENDERWINDOW_TESTS )

  add_executable( RenderWindowTest "RenderWindowTest.cpp" )

  set( RENDERWINDOW_DEPS ${GTEST_LIBRARY} nomlib-graphics )

  if( PLATFORM_WINDOWS )
    list( APPEND RENDERWINDOW_DEPS ${SDL2MAIN_LIBRARY} )
  endif( PLATFORM_WINDOWS )

  target_link_libraries( RenderWindowTest ${RENDERWINDOW_DEPS} )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/RenderWindowTest
                    "" # args
                    "RenderWindowTest.cpp" )

  # Copy nomlib's window icon resource.
  install (
            FILES
            "${NOMLIB_RESOURCES_DIR}/icon.png"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif( NOM_BUILD_RENDERWINDOW_TESTS )

if( NOM_BUILD_GRADIENT_TESTS )

  add_executable( GradientTest "GradientTest.cpp" )

  target_link_libraries( GradientTest nomlib-graphics nomlib-visual-unit-test )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/GradientTest
                    "" # args
                    "GradientTest.cpp"
                  )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/GradientTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif( NOM_BUILD_GRADIENT_TESTS )

if(NOM_BUILD_BITMAP_FONT_TEST)

  add_executable( BitmapFontTest "BitmapFontTest.cpp" )

  target_link_libraries( BitmapFontTest nomlib-graphics nomlib-visual-unit-test )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/BitmapFontTest
                    "" # args
                    "BitmapFontTest.cpp"
                  )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/BitmapFontTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif(NOM_BUILD_BITMAP_FONT_TEST)

if(NOM_BUILD_TRUETYPE_FONT_TEST)

  add_executable( TrueTypeFontTest "TrueTypeFontTest.cpp" )

  target_link_libraries( TrueTypeFontTest nomlib-graphics nomlib-visual-unit-test )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/TrueTypeFontTest
                    "" # args
                    "TrueTypeFontTest.cpp" )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/TrueTypeFontTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif(NOM_BUILD_TRUETYPE_FONT_TEST)

if(NOM_BUILD_BMFONT_TEST)

  add_executable( BMFontTest "BMFontTest.cpp" )

  target_link_libraries( BMFontTest nomlib-graphics nomlib-visual-unit-test )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/BMFontTest
                    "" # args
                    "BMFontTest.cpp" )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/BMFontTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif(NOM_BUILD_BMFONT_TEST)

if(NOM_BUILD_ANIMATION_TESTS)

  set(  ANIMATION_SRC
        ${ANIMATION_SRC}
        "AnimationTest.cpp"
        "AnimationTest_AnimationPlayerTest.cpp"
        "AnimationTest_AlphaBlending.cpp"
        "AnimationTest_Reversed.cpp"
        "AnimationTest_Demos.cpp"
        "AnimationTest_Repeating.cpp"
  )

  add_executable( AnimationTest ${ANIMATION_SRC} )

  target_link_libraries( AnimationTest nomlib-visual-unit-test nomlib-graphics )

  # Any frame rate that your hardware can handle should be fine here; a
  # conservative value is chosen here chiefly to lower system utilization
  # during test runs.
  set( ANIMATION_TEST_ARGS --fps 30 )

  # Fractional speed modifier values, such as 22.5 and the oddball number of 45
  # seemed to carry the most bugs during development of this feature.
  add_animation_test( AnimationTest_SpeedModifierAt22.5
                      ${ANIMATION_TEST_ARGS} --speed 22.5 --timing-mode linear )

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
  add_animation_test( AnimationTestExtended_SpeedModifierAt0.5
                      ${ANIMATION_TEST_ARGS} --speed 0.5 )

  add_animation_test( AnimationTestExtended_SpeedModifierAt1.0
                      ${ANIMATION_TEST_ARGS} --speed 1.0 )

  # NOTE: Only the standard five (5) timing modes are tested -- there are
  # several more timing modes available in the testing unit; see test
  # documentation by passing '--help' to AnimationTest from the command line.
  add_animation_test( AnimationTestExtended_QuadEaseIn
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quad_ease_in )

  add_animation_test( AnimationTestExtended_QuadEaseOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quad_ease_out )

  add_animation_test( AnimationTestExtended_QuadEaseInOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quad_ease_in_out )

  add_animation_test( AnimationTestExtended_CubicEaseIn
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode cubic_ease_in )

  add_animation_test( AnimationTestExtended_CubicEaseOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode cubic_ease_out )

  add_animation_test( AnimationTestExtended_CubicEaseInOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode cubic_ease_in_out )

  add_animation_test( AnimationTestExtended_QuartEaseIn
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quart_ease_in )

  add_animation_test( AnimationTestExtended_QuartEaseOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quart_ease_out )

  add_animation_test( AnimationTestExtended_QuartEaseInOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quart_ease_in_out )

  add_animation_test( AnimationTestExtended_QuintEaseIn
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quint_ease_in )

  add_animation_test( AnimationTestExtended_QuintEaseOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quint_ease_out )

  add_animation_test( AnimationTestExtended_QuintEaseInOut
                      ${ANIMATION_TEST_ARGS} --speed 100.0
                      --timing-mode quint_ease_in_out )

  # Copy resource files for the test
  install(  FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/AnimationTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
  )

  set(  ANIMATION_TIMING_MODES_SRC
        ${ANIMATION_TIMING_MODES_SRC}
        "AnimationTimingModesTest.cpp"
  )

  add_executable( AnimationTimingModesTest ${ANIMATION_TIMING_MODES_SRC} )

  target_link_libraries(  AnimationTimingModesTest nomlib-graphics
                          nomlib-unit-test )

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/AnimationTimingModesTest
                    "" # args
                    ${ANIMATION_TIMING_MODES_SRC}
  )

endif(NOM_BUILD_ANIMATION_TESTS)

if(NOM_BUILD_SPRITE_TESTS)

  add_executable(SpriteTest "SpriteTest.cpp")

  target_link_libraries(SpriteTest nomlib-graphics nomlib-visual-unit-test)

  GTEST_ADD_TESTS(  ${TESTS_INSTALL_DIR}/SpriteTest
                    "" # args
                    "SpriteTest.cpp" )

  # Copy resource files for the test
  install(  FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/SpriteTest.json"
            DESTINATION "${TESTS_INSTALL_DIR}" )

endif(NOM_BUILD_SPRITE_TESTS)
