# nomlib-graphics module tests

set( NOM_BUILD_RENDERWINDOW_TESTS ON )
set( NOM_BUILD_GRADIENT_TESTS ON )
set( NOM_BUILD_BITMAP_FONT_TEST ON )
set( NOM_BUILD_TRUETYPE_FONT_TEST ON )
set( NOM_BUILD_BMFONT_TEST ON )
set( NOM_BUILD_SPRITE_TESTS ON )

if( EXISTS "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
  include( "${CMAKE_CURRENT_LIST_DIR}/local_env.cmake" )
endif()

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

  nom_add_visual_test(  GradientTest
                        ${TESTS_INSTALL_DIR}/GradientTest )

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

  nom_add_visual_test(  BitmapFontTest
                        "${TESTS_INSTALL_DIR}/BitmapFontTest" )

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

  nom_add_visual_test(  TrueTypeFontTest
                        "${TESTS_INSTALL_DIR}/TrueTypeFontTest" )

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

  nom_add_visual_test(  BMFontTest
                        "${TESTS_INSTALL_DIR}/BMFontTest" )


  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/BMFontTest.json"
            DESTINATION
            "${TESTS_INSTALL_DIR}"
          )

endif(NOM_BUILD_BMFONT_TEST)

if(NOM_BUILD_SPRITE_TESTS)

  add_executable(SpriteTest "SpriteTest.cpp")

  target_link_libraries(SpriteTest nomlib-graphics nomlib-visual-unit-test)

  nom_add_visual_test(  SpriteTest
                        ${TESTS_INSTALL_DIR}/SpriteTest )

  # Copy resource files for the test
  install(  FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/SpriteTest.json"
            DESTINATION "${TESTS_INSTALL_DIR}" )

endif(NOM_BUILD_SPRITE_TESTS)
