# nom_joystick_ev example

# DEPRECATED(JEFF): This example is deprecated and will be removed in a future
# version. See the `nom_gamecontroller_ev` example as its replacement.

add_executable(nom_joystick_ev
    WIN32 # Link SDL2main over WinMain
    MACOSX_BUNDLE
    joystick_events.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_joystick_ev "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_joystick_ev "@origin/../lib")
endif()

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_joystick_ev
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_include_directories(nom_joystick_ev
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
)

target_link_libraries(nom_joystick_ev
  PUBLIC
    nomlib-graphics
    nomlib-system
  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(TARGETS nom_joystick_ev
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

copy_framework_dep(nom_joystick_ev
  nomlib-core
  #nomlib-file
  nomlib-graphics
  nomlib-system
)

