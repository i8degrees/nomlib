project ( gamelib )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
set ( BUILD_SHARED_LIBS true )
set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

set ( SRC_DIR src ) # project_dir/src

set ( GAMELIB_SOURCE
      ${SRC_DIR}/Transformable.cpp
      ${SRC_DIR}/Coords.cpp
      ${SRC_DIR}/Color.cpp
      ${SRC_DIR}/GameStates.cpp
      ${SRC_DIR}/SDL_App.cpp
      ${SRC_DIR}/Logger.cpp
      ${SRC_DIR}/audio.cpp
      ${SRC_DIR}/SDL_Font.cpp
      ${SRC_DIR}/SDLGradient.cpp
      ${SRC_DIR}/SDLWindowFrame.cpp
      ${SRC_DIR}/SDLMessageBox.cpp
      ${SRC_DIR}/SDL_BitmapFont.cpp
      ${SRC_DIR}/SDL_Cursor.cpp
      ${SRC_DIR}/IDisplay.cpp
      ${SRC_DIR}/SDL_Image.cpp
      ${SRC_DIR}/SDL_Primitive.cpp
      ${SRC_DIR}/SDL_Rectangle.cpp
      ${SRC_DIR}/SDL_Display.cpp
      ${SRC_DIR}/SDL_Canvas.cpp
      ${SRC_DIR}/gfx.cpp
      ${SRC_DIR}/sprite.cpp
      ${SRC_DIR}/SDL_Input.cpp
      ${SRC_DIR}/timer.cpp
      ${SRC_DIR}/fps.cpp
      ${SRC_DIR}/OSXFs.cpp
    )

set ( GAMELIB_HEADERS
      ${SRC_DIR}/Transformable.hpp
      ${SRC_DIR}/Coords.h
      ${SRC_DIR}/Color.h
      ${SRC_DIR}/I_Input.hpp
      ${SRC_DIR}/GameState.hpp
      ${SRC_DIR}/GameStates.hpp
      ${SRC_DIR}/SDL_App.hpp
      ${SRC_DIR}/Logger.h
      ${SRC_DIR}/audio.h
      ${SRC_DIR}/SDL_Font.h
      ${SRC_DIR}/SDLGradient.h
      ${SRC_DIR}/SDLWindowFrame.h
      ${SRC_DIR}/SDLMessageBox.h
      ${SRC_DIR}/SDL_BitmapFont.h
      ${SRC_DIR}/SDL_Cursor.h
      ${SRC_DIR}/Drawable.hpp
      ${SRC_DIR}/SDL_Drawable.hpp
      ${SRC_DIR}/SDL_Image.hpp
      ${SRC_DIR}/SDL_Primitive.hpp
      ${SRC_DIR}/SDL_Rectangle.hpp
      ${SRC_DIR}/ICanvas.h
      ${SRC_DIR}/SDL_Canvas.h
      ${SRC_DIR}/gamelib.h
      ${SRC_DIR}/IDisplay.h
      ${SRC_DIR}/SDL_Display.h
      ${SRC_DIR}/gfx.h
      ${SRC_DIR}/sprite.h
      ${SRC_DIR}/SDL_Input.hpp
      ${SRC_DIR}/timer.h
      ${SRC_DIR}/fps.h
      ${SRC_DIR}/OSXFs.h
    )

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDL_IMAGE_INCLUDE_DIR} )

find_package ( SDL_mixer REQUIRED )
include_directories ( ${SDL_MIXER_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDL_TTF_INCLUDE_DIR} )

link_libraries (  gamelib # makefile target
                  ${SDL_LIBRARY}
                  ${SDLIMAGE_LIBRARY}
                  ${SDLMIXER_LIBRARY}
                  ${SDLTTF_LIBRARY}
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
