# nom_GameOfLife example

set(SHARED_ASSETS
  "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/img/bgdark.png"
  "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/img/bglight.png"
  "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/img/creep.png"
  "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/img/creep_16.png"
  #"${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/icn/creep.png"
  "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/img/pink.png"
  "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/font/Comfortaa-Regular.ttf"
)

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

if(WIN32)
  target_sources(nom_GameOfLife
    PUBLIC
      ${SHARED_ASSETS}
      "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/icn/creep.ico"
      "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/app.rc"
      "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/App.manifest"
  )
endif()

if(APPLE)
  set(MACOSX_ASSETS
    ${SHARED_ASSETS}
    "${PROJECT_SOURCE_DIR}/Resources/GameOfLife/assets/icn/creep.icns"
  )

  target_sources(nom_GameOfLife
    PUBLIC
      ${MACOSX_ASSETS}
  )
endif()

if(LINUX)
  target_sources(nom_GameOfLife
    PRIVATE
      ${SHARED_ASSETS}
  )
endif()

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
    # FIXME(JEFF): This library should not be a dependency -- once we
    # decouple the SpriteSheet class (graphics) from the use of a serializing
    # pointer, we can safely remove nomlib-serializers from here.
    nomlib-serializers
    nomlib-system
)

if(APPLE)
  set_target_properties(nom_GameOfLife
    PROPERTIES
      #MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
      #MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION}
      MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/cmake/templates/Info.plist.in"
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
  ${SHARED_ASSETS}
  $<TARGET_FILE_DIR:nom_GameOfLife> # <build>/<type>/share
)

# For distribution:
install(FILES
  ${SHARED_ASSETS}
  DESTINATION
  ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)

copy_framework_dep(nom_GameOfLife
  nomlib-core
  #nomlib-file
  nomlib-graphics
  # FIXME(JEFF): This library should not be a dependency -- once we
  # decouple the SpriteSheet class (graphics) from the use of a serializing
  # pointer, we can safely remove nomlib-serializers from here.
  nomlib-serializers
  nomlib-system
)

