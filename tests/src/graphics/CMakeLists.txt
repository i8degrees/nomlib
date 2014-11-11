if( NOM_BUILD_RENDERWINDOW_TESTS )

  add_executable  (
                    RenderWindowTest
                    ${NOM_TESTS_SOURCE_DIR}/graphics/RenderWindowTest.cpp
                  )

  target_link_libraries( RenderWindowTest ${NOM_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    RenderWindowTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/graphics/RenderWindowTest.cpp
                  )

  # Copy nomlib's window icon resource.
  install (
            FILES
            "${PROJECT_SOURCE_DIR}/${NOMLIB_RESOURCES_DIR}/icon.png"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
          )

endif( NOM_BUILD_RENDERWINDOW_TESTS )

if( NOM_BUILD_GRADIENT_TESTS )

  add_executable  (
                    GradientTest
                    ${NOM_TESTS_SOURCE_DIR}/graphics/GradientTest.cpp
                  )

  target_link_libraries(  GradientTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    GradientTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/graphics/GradientTest.cpp
                  )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/GradientTest.json"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
          )

endif( NOM_BUILD_GRADIENT_TESTS )

if(NOM_BUILD_BITMAP_FONT_TEST)

  add_executable  (
                    BitmapFontTest
                    ${NOM_TESTS_SOURCE_DIR}/graphics/BitmapFontTest.cpp
                  )

  target_link_libraries(  BitmapFontTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    BitmapFontTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/graphics/BitmapFontTest.cpp
                  )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/BitmapFontTest.json"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
          )

endif(NOM_BUILD_BITMAP_FONT_TEST)

if(NOM_BUILD_TRUETYPE_FONT_TEST)

  add_executable  (
                    TrueTypeFontTest
                    ${NOM_TESTS_SOURCE_DIR}/graphics/TrueTypeFontTest.cpp
                  )

  target_link_libraries(  TrueTypeFontTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    TrueTypeFontTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/graphics/TrueTypeFontTest.cpp
                  )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/TrueTypeFontTest.json"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
          )

endif(NOM_BUILD_TRUETYPE_FONT_TEST)

if(NOM_BUILD_BMFONT_TEST)

  add_executable  (
                    BMFontTest
                    ${NOM_TESTS_SOURCE_DIR}/graphics/BMFontTest.cpp
                  )

  target_link_libraries(  BMFontTest
                          ${NOM_UNIT_TEST_LIBRARY}
                          ${NOM_VISUAL_UNIT_TEST_LIBRARY} )

  GTEST_ADD_TESTS (
                    BMFontTest
                    "" # args
                    ${NOM_TESTS_SOURCE_DIR}/graphics/BMFontTest.cpp
                  )

  # Copy resource files for the test
  install (
            FILES
            "${NOM_TESTS_RESOURCES_DIR}/graphics/BMFontTest.json"
            DESTINATION
            "${TESTS_DEPS_INSTALL_PEFIX}"
          )

endif(NOM_BUILD_BMFONT_TEST)
