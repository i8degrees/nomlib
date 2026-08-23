# nom_jsonpp example - JSON deserialization

list(APPEND SHARED_ASSETS
  "${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json/auctions.json"
  "${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json/inventory.json"
  "${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json/omnom.json"
  "${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json/omnom_sample.json"
  "${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json/omnom_sample2.json"
  "${PROJECT_SOURCE_DIR}/Resources/tests/serializers/json/omnom_sample3.json"
)

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
    ${SHARED_ASSETS}
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_jsonpp
    PRIVATE
      $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

if(APPLE)
  string(TIMESTAMP CURR_YEAR "%Y")
  set_target_properties(nom_jsonpp
    PROPERTIES
      MACOSX_BUNDLE_BUNDLE_VERSION "${BUILD_VERSION}"
      MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION}"
      # PROJECT_COMPANY_NAMESPACE="net.i8degrees.nomlib"
      MACOSX_BUNDLE_GUI_IDENTIFIER "${PROJECT_COMPANY_NAMESPACE}.nom_jsonpp"
      MACOSX_BUNDLE_BUNDLE_NAME "nom_jsonpp"
      MACOSX_DEPLOYMENT_TARGET "10.15"
      MACOSX_BUNDLE_COPYRIGHT "(c) COPYRIGHT 2014..${TIMESTAMP} JEFFREY CARPENTER <1329364+i8degrees@users.noreply.github.com>"
      MACOSX_BUNDLE_INFO_PLIST "${PROJECT_SOURCE_DIR}/cmake/templates/application.plist.in"
      RESOURCE "${MACOSX_ASSETS}"
  )
endif()

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
  "${SHARED_ASSETS}/auctions.json"
  "${SHARED_ASSETS}/inventory.json"
  $<TARGET_FILE_DIR:nom_jsonpp> # <build>/<type>/share
)

# For distribution:
install(FILES
  ${SHARED_ASSETS}
  DESTINATION ${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
  COMPONENT Examples
)

# (macos) nomlib libs to app bundle
copy_framework_dep(nom_jsonpp
  nomlib-core
  nomlib-file
  nomlib-math
  nomlib-ptree
  nomlib-serializers
  nomlib-system
)

