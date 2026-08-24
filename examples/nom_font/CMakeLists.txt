# nom_font example - render a set of glyphs with a font of your choice

set(SHARED_ASSETS
  "${PROJECT_SOURCE_DIR}/Resources/tests/graphics/TrueTypeFontTest/"
)

add_executable(nom_font
  #WIN32 # Link SDL2main over WinMain for console apps
  MACOSX_BUNDLE
  renderfont.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_font "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_font "@origin/../lib")
endif()

target_sources(nom_font
  PRIVATE
    "${SHARED_ASSETS}/OpenSans-Bold.ttf"
    "${SHARED_ASSETS}/OpenSans-Regular.ttf"
)
if(APPLE)
  set(MACOSX_ASSETS
    ${SHARED_ASSETS}
    "${PROJECT_SOURCE_DIR}/Resources/nomlib.icns"
  )

  target_sources(nom_font
    PUBLIC
      ${MACOSX_ASSETS}
  )
endif()

# console app
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
  "${SHARED_ASSETS}/OpenSans-Bold.ttf"
  "${SHARED_ASSETS}/OpenSans-Regular.ttf"
  $<TARGET_FILE_DIR:nom_font> # <build>/<type>/share
)

# For distribution:
install(DIRECTORY
  ${SHARED_ASSETS}
  DESTINATION
    ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
  OPTIONAL
)

copy_framework_dep(nom_font
  nomlib-core
  nomlib-file
  nomlib-graphics
  nomlib-math
  nomlib-ptree
  nomlib-serializers
  nomlib-system
)

