# nom_sfx example

set(STATIC_ASSET_CONFIG "${PROJECT_SOURCE_DIR}/Resources/examples/audio.json")

add_executable(nom_sfx
  WIN32 # Link SDL2main over WinMain
  MACOSX_BUNDLE
  audio.cpp
)

# TODO(JEFF): This should be a cmake option that can be modified by the
# end-user for sake of packaging and what not.
if(APPLE)
  set_rpath(nom_sfx "@loader_path/../Frameworks;@loader_path/../lib;@loader_path")
else()
  # Use standard Unix file hier layout for run path
  set_rpath(nom_sfx "@origin/../lib")
endif()

target_sources(nom_sfx
  PRIVATE
  ${STATIC_ASSET_CONFIG}
)

target_include_directories(nom_sfx
  PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  PRIVATE
  ${SDL2_INCLUDE_DIR}
  ${TCLAP_INCLUDE_DIR} # tclap.h
)

if(TARGET SDL2::SDL2main)
  target_link_libraries(nom_sfx
    PRIVATE
    $<TARGET_NAME_IF_EXISTS:SDL2::SDL2main>
  )
endif(TARGET SDL2::SDL2main)

target_link_libraries(nom_sfx
  PUBLIC
    nomlib-actions
    nomlib-audio
    nomlib-core
    nomlib-math
    nomlib-serializers
    nomlib-system
  PRIVATE
    Threads::Threads
    $<IF:$<TARGET_EXISTS:SDL2::SDL2>,SDL2::SDL2,SDL2::SDL2-static>
    $<IF:$<TARGET_EXISTS:SDL2_image::SDL2_image>,SDL2_image::SDL2_image,SDL2_image::SDL2_image-static>
    $<IF:$<TARGET_EXISTS:SDL2_ttf::SDL2_ttf>,SDL2_ttf::SDL2_ttf,SDL2_ttf::SDL2_ttf-static>
    $<IF:$<TARGET_EXISTS:OpenAL::OpenAL>,OpenAL::OpenAL,OpenAL::OpenAL-static>
    $<IF:$<TARGET_EXISTS:SndFile::sndfile>,SndFile::sndfile,SndFile::sndfile-static>

)

install(TARGETS nom_sfx
  EXPORT nomlib
  DESTINATION ${CMAKE_INSTALL_BINDIR}
  COMPONENT Examples
)

# Copy resource files for the example
# For development:
add_custom_command(TARGET nom_sfx
  POST_BUILD COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${STATIC_ASSET_CONFIG}"
  $<TARGET_FILE_DIR:nom_sfx> # <build>/<type>/share
)

# For distribution:
#install(DIRECTORY ${PROJECT_SOURCE_DIR}/Resources/tests/audio
#DESTINATION
#${CMAKE_INSTALL_DATADIR}/nomlib/Resources # <prefix/share>/nomlib/Resources
#COMPONENT Examples
#)

# FIXME(JEFF): This dependency can be removed once we decouple from 
# nomlib-actions
copy_framework_dep(nom_sfx
  nomlib-actions
  nomlib-audio
  nomlib-core
  nomlib-file
  nomlib-graphics
  nomlib-math
  nomlib-ptree
  nomlib-serializers
  nomlib-system
)

