add_executable(nom_ev
  WIN32 # Link SDL2main over WinMain
  events.cpp
)

set_target_properties(nom_ev
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

target_include_directories(nom_ev
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
  PRIVATE
  ${SDL2_INCLUDE_DIR}
)

target_link_libraries(nom_ev
  PUBLIC
  nomlib-core
  nomlib-graphics
  nomlib-system
  PRIVATE
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(TARGETS nom_ev
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)
