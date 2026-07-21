# nom_GameOfLife example

set(STATIC_ASSETS ${PROJECT_SOURCE_DIR}/Resources/GameOfLife/)

add_executable(nom_GameOfLife
  WIN32
  MACOSX_BUNDLE
  game.cpp
)

set_target_properties(nom_GameOfLife
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
    XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
    XCODE_ATTRIBUTE_CODE_SIGN_STYLE "Manual"
    XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Developer ID Application"
    XCODE_ATTRIBUTE_DEVELOPMENT_TEAM "YOUR_TEAM_ID"
)

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


target_link_libraries(nom_GameOfLife
  PUBLIC
  nomlib-core
  nomlib-graphics
  nomlib-serializers
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
  $<TARGET_FILE_DIR:nom_GameOfLife> # <build>/<type>/share
)

# For distribution:
install(DIRECTORY ${STATIC_ASSETS}
  DESTINATION
  ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)
