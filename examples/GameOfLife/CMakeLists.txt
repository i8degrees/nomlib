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
    nomlib-serializers
    nomlib-system
)

if(APPLE)
  string(TIMESTAMP CURR_YEAR "%Y")
  # Setup a MacOS Application bundle
  # TODO(JEFF): VERIFY ASAP
  set_target_properties(nom_GameOfLife
    PROPERTIES
      MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
      # PROJECT_COMPANY_NAMESPACE="net.i8degrees.nomlib"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_COMPANY_NAMESPACE}.GameOfLife"
      MACOSX_BUNDLE_BUNDLE_NAME "nom_GameOfLife"
      MACOSX_DEPLOYMENT_TARGET "10.15"
      MACOSX_BUNDLE_COPYRIGHT "(c) COPYRIGHT 2014..${TIMESTAMP} JEFFREY CARPENTER <1329364+i8degrees@users.noreply.github.com>"
      MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/cmake/templates/app.plist.in"
      RESOURCE "${MACOSX_ASSETS}"
  )
endif()

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
