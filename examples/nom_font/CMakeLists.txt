# nom_font example - render a set of glyphs with a font of your choice

set(STATIC_ASSETS ${PROJECT_SOURCE_DIR}/Resources/tests/graphics/TrueTypeFontTest)

add_executable(nom_font
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  renderfont.cpp
)

set_target_properties(nom_font
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

target_sources(nom_font
  PRIVATE
  ${STATIC_ASSETS}/OpenSans-Bold.ttf
  ${STATIC_ASSETS}/OpenSans-Regular.ttf
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_font
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_include_directories(nom_font
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  PRIVATE
  ${TCLAP_INCLUDE_DIR}
)

target_link_libraries(nom_font
  PUBLIC
  nomlib-core
  nomlib-graphics
  nomlib-math
  nomlib-system
  PRIVATE
  #$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(TARGETS nom_font
  # EXPORT examples
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# For development:
add_custom_command(TARGET nom_font
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${STATIC_ASSETS}/OpenSans-Bold.ttf"
  "${STATIC_ASSETS}/OpenSans-Regular.ttf"
  $<TARGET_FILE_DIR:nom_font> # <build>/<type>/share
)

# For distribution:
install(DIRECTORY ${STATIC_ASSETS}
  DESTINATION
  ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)
