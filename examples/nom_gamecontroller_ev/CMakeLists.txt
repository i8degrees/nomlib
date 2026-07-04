# gamecontroller_ev example

add_executable(nom_gamecontroller_ev
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  gamecontroller_events.cpp
)

set_target_properties(nom_gamecontroller_ev
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_gamecontroller_ev
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_include_directories(nom_gamecontroller_ev
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
)

target_link_libraries(nom_gamecontroller_ev
  PUBLIC
  nomlib-graphics
  nomlib-system
  PRIVATE
  # $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(TARGETS nom_gamecontroller_ev
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

install(FILES
  "${NOMLIB_SHARED_SUPPORT_DIR}/InputDevices.json"
  DESTINATION
  "${EXAMPLES_INSTALL_DIR}"
  COMPONENT Examples
)
