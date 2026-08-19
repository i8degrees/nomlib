# nom_GameOfLife example

set(STATIC_ASSETS ${PROJECT_SOURCE_DIR}/Resources/GameOfLife/)

add_executable(nom_GameOfLife
  WIN32
  MACOSX_BUNDLE
  game.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_GameOfLife "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_GameOfLife "@origin/../lib")
endif()

target_include_directories(nom_GameOfLife
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
  PRIVATE
    ${SDL2_INCLUDE_DIR}
)

target_sources(nom_GameOfLife
  PRIVATE
    "${STATIC_ASSETS}/assets/img/bgdark.png"
    "${STATIC_ASSETS}/assets/img/bglight.png"
    "${STATIC_ASSETS}/assets/img/pink.png"
    #"${STATIC_ASSETS}/icon.png"
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_GameOfLife
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(nom_GameOfLife
  PUBLIC
    nomlib-core
    nomlib-graphics
    nomlib-system
)

install(TARGETS nom_GameOfLife
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# For development:
add_custom_command(TARGET nom_GameOfLife
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${STATIC_ASSETS}/assets/img/bgdark.png"
  "${STATIC_ASSETS}/assets/img/bglight.png"
  "${STATIC_ASSETS}/assets/img/pink.png"
  #"${STATIC_ASSETS}/icon.png"
  $<TARGET_FILE_DIR:nom_GameOfLife/Resources> # <build>/<type>/share
)

# For distribution:
install(DIRECTORY ${STATIC_ASSETS}
  DESTINATION
  ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)

copy_framework_dep(nom_GameOfLife
  nomlib-core
  #nomlib-file
  nomlib-graphics
  nomlib-system
)

