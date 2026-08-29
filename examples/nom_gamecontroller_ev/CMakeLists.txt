# gamecontroller_ev example

set(STATIC_ASSETS "${PROJECT_SOURCE_DIR}/Resources/SharedSupport")

set(SHARED_ASSETS
  "${gcdb_SOURCE_DIR}/gamecontrollerdb.txt"
)

add_executable(nom_gamecontroller_ev
  #WIN32 # Link SDL2main over WinMain
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

target_sources(nom_gamecontroller_ev
  PRIVATE
  ${SHARED_ASSETS}
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

if(APPLE)
  string(TIMESTAMP CURR_YEAR "%Y")
  set_target_properties(nom_gamecontroller_ev
    PROPERTIES
      MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_COMPANY_NAMESPACE}.gamecontroller_ev"
      MACOSX_BUNDLE_BUNDLE_NAME "nom_gamecontroller_ev"
      MACOSX_DEPLOYMENT_TARGET "10.15"
      MACOSX_BUNDLE_COPYRIGHT "(c) COPYRIGHT 2014..${TIMESTAMP} JEFFREY CARPENTER <1329364+i8degrees@users.noreply.github.com>"
      MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/cmake/templates/app.plist.in"
      RESOURCE ${SHARED_ASSETS}
  )
endif()

# For development:
add_custom_command(TARGET nom_gamecontroller_ev
  POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory
      "$<TARGET_FILE_DIR:nom_gamecontroller_ev>/Resources/SharedSupport"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      ${SHARED_ASSETS}
      "$<TARGET_FILE_DIR:nom_gamecontroller_ev>/Resources/SharedSupport"
)

# For distribution
install(TARGETS nom_gamecontroller_ev
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# For distribution:
install(DIRECTORY ${STATIC_ASSETS}
  DESTINATION
  ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)

install(FILES
  "${gcdb_SOURCE_DIR}/gamecontrollerdb.txt"
  DESTINATION
  "${CMAKE_INSTALL_DATADIR}/nomlib/Resources/SharedSupport"
  COMPONENT Examples
)
