# nomlib-graphics module tests

set( NOM_BUILD_RENDERWINDOW_TESTS ON )
set( NOM_BUILD_GRADIENT_TESTS ON )
set( NOM_BUILD_BITMAP_FONT_TEST ON )
set( NOM_BUILD_TRUETYPE_FONT_TEST ON )
set( NOM_BUILD_BMFONT_TEST ON )
set( NOM_BUILD_SPRITE_TESTS ON )

if( NOM_BUILD_RENDERWINDOW_TESTS )

  add_executable(RenderWindowTest
    WIN32 # GUI app
    RenderWindowTest.cpp
  )

  add_dependencies(RenderWindowTest gtest)

  target_include_directories(RenderWindowTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(RenderWindowTest
    PUBLIC
      nomlib-core
      nomlib-graphics
    PRIVATE
      Threads::Threads
      ${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  GTEST_ADD_TESTS ( ${TESTS_INSTALL_DIR}/RenderWindowTest
                    "" # args
                    "RenderWindowTest.cpp" )

  # For development: Copy nomlib's window icon resource.
  add_custom_command(TARGET RenderWindowTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${PROJECT_SOURCE_DIR}/Resources/icon.png"
    $<TARGET_FILE_DIR:RenderWindowTest>
  )

endif( NOM_BUILD_RENDERWINDOW_TESTS )

if( NOM_BUILD_GRADIENT_TESTS )

  add_executable(GradientTest
    WIN32 # GUI app
    GradientTest.cpp
  )

  add_dependencies(GradientTest gtest)

  target_include_directories(GradientTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(GradientTest
    PUBLIC
      nomlib-core
      nomlib-graphics
      nomlib-visual-unit-test
    PRIVATE
      #Threads::Threads
      #${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
      #$<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  )

  nom_add_visual_test(GradientTest
    "${TESTS_INSTALL_DIR}/GradientTest"
  )

  # For development: Copy resource files for the test
  add_custom_command(TARGET GradientTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/graphics/GradientTest.json"
    $<TARGET_FILE_DIR:GradientTest>
  )

endif( NOM_BUILD_GRADIENT_TESTS )

if(NOM_BUILD_BITMAP_FONT_TEST)

  add_executable(BitmapFontTest
    WIN32 # GUI app
    BitmapFontTest.cpp
  )

  add_dependencies(BitmapFontTest gtest)

  target_include_directories(BitmapFontTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include # include/nomlib
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
    ${GTEST_INCLUDE_DIR}
    ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(BitmapFontTest
    PUBLIC
      nomlib-core
      nomlib-graphics
      nomlib-visual-unit-test
    PRIVATE
      #Threads::Threads
      #${GTEST_LIBRARY}
  )

  nom_add_visual_test(  BitmapFontTest
                        "${TESTS_INSTALL_DIR}/BitmapFontTest" )

  # For development: Copy resource files for the test
  add_custom_command(TARGET BitmapFontTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/graphics/BitmapFontTest.json"
    $<TARGET_FILE_DIR:BitmapFontTest>
  )

endif(NOM_BUILD_BITMAP_FONT_TEST)

if(NOM_BUILD_TRUETYPE_FONT_TEST)

  add_executable(TrueTypeFontTest
    WIN32 # GUI app
    TrueTypeFontTest.cpp
  )

  add_dependencies(TrueTypeFontTest gtest)

  target_include_directories(TrueTypeFontTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${GTEST_INCLUDE_DIR}
      ${SDL2_INCLUDE_DIR}
  )

  target_link_libraries(TrueTypeFontTest
    PUBLIC
      nomlib-core
      nomlib-graphics
      nomlib-visual-unit-test
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  nom_add_visual_test(  TrueTypeFontTest
                        "${TESTS_INSTALL_DIR}/TrueTypeFontTest" )

  # For development: Copy resource files for the test
  add_custom_command(TARGET TrueTypeFontTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/graphics/TrueTypeFontTest.json"
    $<TARGET_FILE_DIR:TrueTypeFontTest>
  )

endif(NOM_BUILD_TRUETYPE_FONT_TEST)

if(NOM_BUILD_BMFONT_TEST)

  add_executable(BMFontTest
    WIN32 # GUI app
    BMFontTest.cpp
  )

  add_dependencies(BMFontTest gtest)

  target_include_directories(BMFontTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/include # include/nomlib
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${SDL2_INCLUDE_DIR}
      ${GTEST_INCLUDE_DIR}
  )

  target_link_libraries(BMFontTest
    PUBLIC
      nomlib-core
      nomlib-graphics
      nomlib-visual-unit-test
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  nom_add_visual_test(  BMFontTest
                        "${TESTS_INSTALL_DIR}/BMFontTest" )


  # For development: Copy resource files for the test
  add_custom_command(TARGET BMFontTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/graphics/BMFontTest.json"
    $<TARGET_FILE_DIR:BMFontTest>
  )

endif(NOM_BUILD_BMFONT_TEST)

if(NOM_BUILD_SPRITE_TESTS)

  add_executable(SpriteTest
    WIN32 # GUI app
    SpriteTest.cpp
  )

  add_dependencies(SpriteTest gtest)

  target_include_directories(SpriteTest
    PUBLIC
      ${PROJECT_SOURCE_DIR}/tests/include
    PRIVATE
      ${SDL2_INCLUDE_DIR}
      ${GTEST_INCLUDE_DIR}
  )

  target_link_libraries(SpriteTest
    PUBLIC
      nomlib-core
      nomlib-graphics
      nomlib-visual-unit-test
    PRIVATE
      #Threads::Threads
      #${GTEST_LIBRARY}
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )

  nom_add_visual_test(  SpriteTest
                        ${TESTS_INSTALL_DIR}/SpriteTest )

  # For development: Copy resource files for the test
  add_custom_command(TARGET SpriteTest POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${NOM_TESTS_RESOURCES_DIR}/graphics/SpriteTest.json"
    $<TARGET_FILE_DIR:SpriteTest>
  )
endif(NOM_BUILD_SPRITE_TESTS)
