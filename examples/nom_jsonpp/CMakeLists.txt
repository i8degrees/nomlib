# nom_jsonpp example - JSON deserialization

set(STATIC_ASSETS ${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json)

add_executable(nom_jsonpp
  #WIN32 # Link SDL2main over WinMain for console apps
  MACOSX_BUNDLE
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_jsonpp "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_jsonpp "@origin/../lib")
endif()

target_include_directories(nom_jsonpp
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include # include/nomlib
)

target_sources(nom_jsonpp
  PRIVATE
    nom_jsonpp.cpp
    ${STATIC_ASSETS}/auctions.json
    ${STATIC_ASSETS}/inventory.json
    ${STATIC_ASSETS}/omnom.json
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_jsonpp
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(nom_jsonpp
  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  PUBLIC
    nomlib-core
    nomlib-ptree
    nomlib-serializers
    nomlib-system
)

install(TARGETS nom_jsonpp
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# For development:
add_custom_command(TARGET nom_jsonpp
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${STATIC_ASSETS}/auctions.json"
  "${STATIC_ASSETS}/inventory.json"
  $<TARGET_FILE_DIR:nom_jsonpp> # <build>/<type>/share
)

# For distribution:
install(DIRECTORY ${STATIC_ASSETS}
  DESTINATION
  ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)

copy_framework_dep(nom_jsonpp
  nomlib-core
  nomlib-file
  nomlib-math
  nomlib-ptree
  nomlib-serializers
  nomlib-system
)

