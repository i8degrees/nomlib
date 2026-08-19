# nom_cursors example

add_executable(nom_cursors
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  cursors.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_cursors "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_cursors "@origin/../lib")
endif()

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
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

install(FILES
  "${PROJECT_SOURCE_DIR}/Resources/icon.png"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib"
)

copy_framework_dep(nom_cursors
  nomlib-core
  nomlib-file
  nomlib-graphics
  nomlib-system
)

