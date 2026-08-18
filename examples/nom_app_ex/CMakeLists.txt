# nom_app_ex example

# Search paths file to use in finding the resources path
set(STATIC_ASSET_CONFIG "${PROJECT_SOURCE_DIR}/Resources/examples/app.json")

add_executable(nom_app_ex
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  app.cpp
)

set_target_properties(nom_app_ex
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

target_sources(nom_app_ex
  PRIVATE
  ${STATIC_ASSET_CONFIG}
)

target_include_directories(nom_app_ex
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_app_ex
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(nom_app_ex
  PUBLIC
  nomlib-core
  nomlib-actions
  nomlib-graphics
  nomlib-gui
  nomlib-serializers
  nomlib-system
  PRIVATE
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
  $<IF:$<TARGET_EXISTS:RocketControls>,RocketControls,RocketControls-static>
  $<IF:$<TARGET_EXISTS:RocketDebugger>,RocketDebugger,RocketDebugger-static>
)

install(TARGETS nom_app_ex
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

#
## Copy assets into app bundle
#

# For development:
add_custom_command(TARGET nom_app_ex
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${STATIC_ASSET_CONFIG}"
  $<TARGET_FILE_DIR:nom_app_ex> # <build>/<type>/share
)

# For distribution:
install(DIRECTORY
  ${PROJECT_SOURCE_DIR}/Resources/examples/app
  DESTINATION
    ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix>/share/nomlib/Resources
  COMPONENT Examples
)

#
## Copy dependencies
#

# For development (SDL2):
add_custom_command(TARGET nom_app_ex POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  $<TARGET_FILE:SDL2::SDL2>
  $<TARGET_FILE_DIR:nom_app_ex>
)

# For distribution (SDL2):
install(FILES $<TARGET_FILE:SDL2::SDL2>
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Examples
)

if(TARGET SDL2_image::SDL2_image-static)
  # For development (SDL2_image):
  add_custom_command(TARGET nom_app_ex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2_image::SDL2_image-static>
    $<TARGET_FILE_DIR:nom_app_ex>
  )
  # For distribution (SDL2_image):
  install(FILES $<TARGET_FILE:SDL2_image::SDL2_image-static>
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Examples
  )
else(TARGET SDL2_image::SDL2_image)
  # For development (SDL2_image):
  add_custom_command(TARGET nom_app_ex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2_image::SDL2_image>
    $<TARGET_FILE_DIR:nom_app_ex>
  )
  # For distribution (SDL2_image):
  install(FILES $<TARGET_FILE:SDL2_image::SDL2_image>
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Examples
  )
endif(TARGET SDL2_image::SDL2_image-static)

if(TARGET SDL2_ttf::SDL2_ttf-static)
  # For development (SDL2_ttf):
  add_custom_command(TARGET nom_app_ex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2_ttf::SDL2_ttf-static>
    $<TARGET_FILE_DIR:nom_app_ex>
  )
  # For distribution (SDL2_ttf):
  install(FILES $<TARGET_FILE:SDL2_ttf::SDL2_ttf-static>
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Examples
  )
else(TARGET SDL2_ttf::SDL2_ttf)
  # For development (SDL2_ttf):
  add_custom_command(TARGET nom_app_ex POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
    $<TARGET_FILE_DIR:nom_app_ex>
  )
  # For distribution (SDL2_ttf):
  install(FILES $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
    DESTINATION ${CMAKE_INSTALL_LIBDIR}
    COMPONENT Examples
  )
endif(TARGET SDL2_ttf::SDL2_ttf-static)

# For development (libRocketCore):
# add_custom_command(TARGET nom_app_ex POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#   $<TARGET_FILE:RocketCore>
#   $<TARGET_FILE_DIR:nom_app_ex>
# )

# For distribution (libRocketCore):
# install(FILES $<TARGET_FILE:RocketCore>
#   DESTINATION ${CMAKE_INSTALL_BINDIR}
#   COMPONENT Examples
# )

# For development (libRocketControls):
# add_custom_command(TARGET nom_app_ex POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#   $<TARGET_FILE:RocketControls>
#   $<TARGET_FILE_DIR:nom_app_ex>
# )

# For distribution (libRocketControls):
# install(FILES $<TARGET_FILE:RocketControls>
#   DESTINATION ${CMAKE_INSTALL_BINDIR}
#   COMPONENT Examples
# )

# For development (libRocketDebugger):
# add_custom_command(TARGET nom_app_ex POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy_if_different
#   $<TARGET_FILE:RocketDebugger>
#   $<TARGET_FILE_DIR:nom_app_ex>
# )

# For distribution (libRocketDebugger):
# install(FILES $<TARGET_FILE:RocketDebugger>
#   DESTINATION ${CMAKE_INSTALL_BINDIR}
#   COMPONENT Examples
# )

# add_custom_command(TARGET nom_app_ex POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E create_symlink $<TARGET_FILE:RocketCore>
#   $<TARGET_FILE_DIR:nom_app_ex>/libRocketCore.so.1
# )

# For distribution (libRocketCore):
create_symlink(nom_app_ex $<TARGET_FILE:RocketCore>
  $<TARGET_FILE_DIR:nom_app_ex>/libRocketCore.so.1
)

# For distribution (libRocketControls):
create_symlink(nom_app_ex $<TARGET_FILE:RocketControls>
  $<TARGET_FILE_DIR:nom_app_ex>/libRocketControls.so.1
)

# For distribution (libRocketDebugger):
create_symlink(nom_app_ex $<TARGET_FILE:RocketDebugger>
  $<TARGET_FILE_DIR:nom_app_ex>/libRocketDebugger.so.1
)

install(FILES $<TARGET_FILE_DIR:nom_app_ex>/libRocketCore.so.1
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Examples
)

install(FILES $<TARGET_FILE_DIR:nom_app_ex>/libRocketControls.so.1
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Examples
)

install(FILES $<TARGET_FILE_DIR:nom_app_ex>/libRocketDebugger.so.1
  DESTINATION ${CMAKE_INSTALL_LIBDIR}
  COMPONENT Examples
)
