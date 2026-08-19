# gamecontroller_ev example

set(STATIC_ASSETS "${PROJECT_SOURCE_DIR}/Resources/SharedSupport")

add_executable(nom_gamecontroller_ev
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  gamecontroller_events.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_gamecontroller_ev "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_gamecontroller_ev "@origin/../lib")
endif()

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

# For development:
add_custom_command(TARGET nom_gamecontroller_ev
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${STATIC_ASSETS}/InputDevices.json"
  $<TARGET_FILE_DIR:nom_gamecontroller_ev> # <build>/<type>/share
)

if(NOM_INSTALL_GCDB)
  add_custom_command(TARGET nom_gamecontroller_ev
    POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${gcdb_SOURCE_DIR}/gamecontrollerdb.txt"
    $<TARGET_FILE_DIR:nom_gamecontroller_ev> # <build>/<type>/share
  )
endif()

# For distribution
install(TARGETS nom_gamecontroller_ev
  # EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# For distribution
install(FILES
  "${PROJECT_SOURCE_DIR}/Resources/SharedSupport/InputDevices.json"
  "${gcdb_SOURCE_DIR}/gamecontrollerdb.txt"
  DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/nomlib/Resources"
  OPTIONAL
  COMPONENT Examples
)

copy_framework_dep(nom_gamecontroller_ev
  nomlib-core
  nomlib-file
  nomlib-graphics
  nomlib-math
  nomlib-ptree
  nomlib-serializers
  nomlib-system
)

