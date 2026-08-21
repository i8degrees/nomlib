# nom_devinfo example - print out compiled & linked version numbers of libraries

add_executable(nom_devinfo
  #WIN32 # Link SDL2main over WinMain (console app)
  MACOSX_BUNDLE
  device_info.cpp
)

set_target_properties(nom_devinfo
  PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib"
    INSTALL_RPATH_USE_LINK_PATH TRUE
)

if(NOM_BUILD_AUDIO_UNIT)
  target_link_libraries(nom_devinfo
    PUBLIC
    nomlib-audio
    PRIVATE
    OpenAL::OpenAL
    SndFile::sndfile
  )
endif(NOM_BUILD_AUDIO_UNIT)

if(libRocket_FOUND)
  if(OPENGL_FOUND)
      target_link_libraries(nom_devinfo
        PRIVATE
        OpenGL::GL
      )
  endif(OPENGL_FOUND)
endif(libRocket_FOUND)

if(libRocket_FOUND)
  target_link_libraries(nom_devinfo
    PRIVATE
    $<IF:$<TARGET_EXISTS:RocketCore>,RocketCore,RocketCore-static>
    $<IF:$<TARGET_EXISTS:RocketControls>,RocketControls,RocketControls-static>
    $<IF:$<TARGET_EXISTS:RocketDebugger>,RocketDebugger,RocketDebugger-static>
  )
endif(libRocket_FOUND)

target_include_directories(nom_devinfo
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include # include/nomlib
)

target_link_libraries(nom_devinfo
  PRIVATE
  $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
  $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
  $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
  PUBLIC
  nomlib-core
  nomlib-graphics
  nomlib-gui
  nomlib-math
  nomlib-system
)

install(TARGETS nom_devinfo
  # EXPORT examples
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)
