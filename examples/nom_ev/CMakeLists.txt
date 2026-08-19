# nom_ev example

# DEPRECATED(JEFF): This example is deprecated and will be removed in a future
# version. See the `nom_app_ex` example for its replacement.

add_executable(nom_ev
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  events.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_ev "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_ev "@origin/../lib")
endif()

target_include_directories(nom_ev
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
  PRIVATE
    ${SDL2_INCLUDE_DIR}
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_ev
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(nom_ev
  PUBLIC
    nomlib-core
    nomlib-file
    nomlib-graphics
    nomlib-math
    # FIXME(JEFF): This library should not be needed, but until we fix the
    # circular dependency within the SpriteSheet class (graphics), we can do
    # nothing more here.
    nomlib-ptree
    # FIXME(JEFF): This library should not be needed, but until we fix the
    # circular dependency within the SpriteSheet class (graphics), we can do
    # nothing more here.
    nomlib-serializers
    nomlib-system
  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(TARGETS nom_ev
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

copy_framework_dep(nom_ev
  nomlib-core
  nomlib-file
  nomlib-math
  nomlib-graphics
  nomlib-ptree
  nomlib-serializers
  nomlib-system
)

