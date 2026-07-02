# nom_cursors example

add_executable(nom_cursors
  WIN32 # Link SDL2main over WinMain
  cursors.cpp
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_cursors
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_include_directories(nom_cursors
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(nom_cursors
  PUBLIC
  nomlib-core
  nomlib-graphics
  PRIVATE
  #$<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(  FILES "${NOMLIB_RESOURCES_DIR}/icon.png"
          DESTINATION "${EXAMPLES_INSTALL_DIR}/Resources" )
