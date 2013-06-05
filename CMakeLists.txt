project ( gamelib )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
set ( BUILD_SHARED_LIBS true )
set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

set ( SRC_DIR src ) # project_dir/src

set ( GAMELIB_SOURCE
      ${SRC_DIR}/Coords.cpp
      ${SRC_DIR}/Color.cpp
      ${SRC_DIR}/Logger.cpp
      ${SRC_DIR}/audio.cpp
      #${SRC_DIR}/font.cpp
      ${SRC_DIR}/SDL_TFont.cpp
      ${SRC_DIR}/SDLGradient.cpp
      ${SRC_DIR}/SDLWindowFrame.cpp
      ${SRC_DIR}/SDLMessageBox.cpp
      ${SRC_DIR}/SDL_BitmapFont.cpp
      ${SRC_DIR}/SDL_Cursor.cpp
      ${SRC_DIR}/IDisplay.cpp
      ${SRC_DIR}/SDL_Display.cpp
      ${SRC_DIR}/gfx.cpp
      ${SRC_DIR}/sprite.cpp
      ${SRC_DIR}/SDLInput.cpp
      ${SRC_DIR}/timer.cpp
      ${SRC_DIR}/fps.cpp
      ${SRC_DIR}/OSXFs.cpp
    )

set ( GAMELIB_HEADERS
      ${SRC_DIR}/Coords.h
      ${SRC_DIR}/Color.h
      ${SRC_DIR}/GameState.h
      ${SRC_DIR}/Logger.h
      ${SRC_DIR}/audio.h
      #${SRC_DIR}/font.h
      ${SRC_DIR}/SDL_TFont.h
      ${SRC_DIR}/SDLGradient.h
      ${SRC_DIR}/SDLWindowFrame.h
      ${SRC_DIR}/SDLMessageBox.h
      ${SRC_DIR}/SDL_BitmapFont.h
      ${SRC_DIR}/SDL_Cursor.h
      ${SRC_DIR}/SDLDrawable.h
      ${SRC_DIR}/gamelib.h
      ${SRC_DIR}/IDisplay.h
      ${SRC_DIR}/SDL_Display.h
      ${SRC_DIR}/gfx.h
      ${SRC_DIR}/sprite.h
      ${SRC_DIR}/SDLInput.h
      ${SRC_DIR}/timer.h
      ${SRC_DIR}/fps.h
      ${SRC_DIR}/OSXFs.h
    )

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDLIMAGE_INCLUDE_DIR} )

find_package ( SDL_mixer REQUIRED )
include_directories ( ${SDLMIXER_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDLTTF_INCLUDE_DIR} )

link_libraries (  gamelib # makefile target
                  ${SDL_LIBRARY}
                  ${SDL_IMAGE_LIBRARY}
                  ${SDL_MIXER_LIBRARY}
                  ${SDL_TTF_LIBRARY}
               )

add_library ( gamelib SHARED ${GAMELIB_SOURCE} )

set_target_properties ( gamelib # makefile target
                        PROPERTIES SOVERSION "0.1"
                      )

# install library

install ( TARGETS gamelib
          LIBRARY DESTINATION lib
        )

# install headers

install ( FILES ${GAMELIB_HEADERS}
          DESTINATION include/gamelib
        )

# uninstall target

configure_file  (
                  "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                  IMMEDIATE @ONLY
                )

add_custom_target ( uninstall
                    COMMAND ${CMAKE_COMMAND} -P
                    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                  )
