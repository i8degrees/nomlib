# nom_paint example

set(STATIC_ASSETS "${PROJECT_SOURCE_DIR}/Resources/tests/gui")

set(SHARED_ASSETS
  "${STATIC_ASSETS}/OpenSans-Regular.ttf"
)

add_executable(nom_paint
  WIN32
  MACOSX_BUNDLE
  paint.cpp
)

set_target_properties(nom_paint
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_paint
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_include_directories(nom_paint
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
)

target_sources(nom_paint
  PRIVATE
  ${SHARED_ASSETS}
)

target_link_libraries(nom_paint
  PUBLIC
  nomlib-graphics
  nomlib-system
  PRIVATE
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
)

if(APPLE)
  string(TIMESTAMP CURR_YEAR "%Y")
  set_target_properties(nom_paint
    PROPERTIES
      MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_COMPANY_NAMESPACE}.paint"
      MACOSX_BUNDLE_BUNDLE_NAME "nom_paint"
      MACOSX_DEPLOYMENT_TARGET "10.15"
      MACOSX_BUNDLE_COPYRIGHT "(c) COPYRIGHT 2014..${TIMESTAMP} JEFFREY CARPENTER <1329364+i8degrees@users.noreply.github.com>"
      MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/cmake/templates/app.plist.in"
      RESOURCE ${SHARED_ASSETS}
  )
endif()

# For development:
add_custom_command(TARGET nom_paint
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
    "$<TARGET_FILE_DIR:nom_paint>/Resources"
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  ${SHARED_ASSETS}
  "$<TARGET_FILE_DIR:nom_paint>/Resources"
)

# For distribution
install(TARGETS nom_paint
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

install(FILES ${SHARED_ASSETS}
  DESTINATION ${CMAKE_INSTALL_DATADIR}/nomlib/Resources/paint
  COMPONENT Examples
)
