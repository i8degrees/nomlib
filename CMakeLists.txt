cmake_minimum_required ( VERSION 2.8 )

project ( gamelib )

set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
set ( BUILD_SHARED_LIBS true )
set ( CMAKE_OSX_ARCHITECTURES i386;x86_64 ) # OSX Universal Library

if ( CMAKE_GENERATOR STREQUAL Xcode )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
endif ()

if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                        # applicable dependencies installed
    message ( "Linux OS detected " )
    set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
    set ( CMAKE_CXX_FLAGS -std=gnu++0x )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
    message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3
    set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
    set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
else ()
    message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ()

set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall" )
set ( CMAKE_CXX_FLAGS_RELEASE "-O2 -msse2 -mmx" )

set ( SRC_DIR src ) # project_dir/src

set ( GAMELIB_SOURCE
      ${SRC_DIR}/Transformable.cpp
      ${SRC_DIR}/Coords.cpp
      ${SRC_DIR}/Color.cpp
      ${SRC_DIR}/GameStates.cpp
      ${SRC_DIR}/SDL_App.cpp
      ${SRC_DIR}/audio.cpp
      ${SRC_DIR}/SDL_Font.cpp
      ${SRC_DIR}/SDL_Gradient.cpp
      ${SRC_DIR}/SDLWindowFrame.cpp
      ${SRC_DIR}/SDL_MessageBox.cpp
      ${SRC_DIR}/SDL_BitmapFont.cpp
      ${SRC_DIR}/SDL_Cursor.cpp
      ${SRC_DIR}/IDisplay.cpp
      ${SRC_DIR}/SDL_Image.cpp
      ${SRC_DIR}/SDL_Pixel.cpp
      ${SRC_DIR}/SDL_Line.cpp
      ${SRC_DIR}/SDL_Rectangle.cpp
      ${SRC_DIR}/SDL_Display.cpp
      ${SRC_DIR}/SDL_Canvas.cpp
      ${SRC_DIR}/Sprite.cpp
      ${SRC_DIR}/SDL_Input.cpp
      ${SRC_DIR}/SDL_Timer.cpp
      ${SRC_DIR}/FPS.cpp
      ${SRC_DIR}/OSXFS.cpp
    )

set ( GAMELIB_HEADERS
      ${SRC_DIR}/Transformable.hpp
      ${SRC_DIR}/Coords.hpp
      ${SRC_DIR}/Color.hpp
      ${SRC_DIR}/I_Input.hpp
      ${SRC_DIR}/GameState.hpp
      ${SRC_DIR}/GameStates.hpp
      ${SRC_DIR}/SDL_App.hpp
      ${SRC_DIR}/audio.h
      ${SRC_DIR}/SDL_Font.hpp
      ${SRC_DIR}/SDL_Gradient.hpp
      ${SRC_DIR}/SDLWindowFrame.h
      ${SRC_DIR}/SDL_MessageBox.hpp
      ${SRC_DIR}/SDL_BitmapFont.hpp
      ${SRC_DIR}/SDL_Cursor.h
      ${SRC_DIR}/Drawable.hpp
      ${SRC_DIR}/SDL_Drawable.hpp
      ${SRC_DIR}/SDL_Image.hpp
      ${SRC_DIR}/SDL_Pixel.hpp
      ${SRC_DIR}/SDL_Line.hpp
      ${SRC_DIR}/SDL_Rectangle.hpp
      #${SRC_DIR}/ICanvas.h
      ${SRC_DIR}/SDL_Canvas.hpp
      ${SRC_DIR}/nomlib.hpp
      ${SRC_DIR}/IDisplay.hpp
      ${SRC_DIR}/SDL_Display.hpp
      ${SRC_DIR}/Sprite.hpp
      ${SRC_DIR}/SDL_Input.hpp
      ${SRC_DIR}/SDL_Timer.hpp
      ${SRC_DIR}/FPS.hpp
      ${SRC_DIR}/OSXFS.hpp
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

# FIXME
add_custom_target ( nomlib-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )
