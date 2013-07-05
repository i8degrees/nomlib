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
  set ( CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wsign-compare" )
elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
  message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

  set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
  set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
  set ( CMAKE_CXX_FLAGS_DEBUG "-gfull -O0 -Wall -Wsign-compare" )
else ()
  message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )
endif ()

set ( CMAKE_CXX_FLAGS_RELEASE "-O2 -msse2 -mmx" )

set ( SRC_DIR src ) # project_dir/src

set ( GAMELIB_SOURCE
      ${SRC_DIR}/math/Transformable.cpp
      ${SRC_DIR}/math/Coords.cpp
      ${SRC_DIR}/math/Color.cpp
      ${SRC_DIR}/sys/GameStates.cpp
      ${SRC_DIR}/sys/SDL_App.cpp
      #${SRC_DIR}/audio.cpp
      ${SRC_DIR}/gfx/SDL_Font.cpp
      ${SRC_DIR}/gfx/SDL_Gradient.cpp
      ${SRC_DIR}/gfx/SDL_MessageBox.cpp
      ${SRC_DIR}/gfx/SDL_BitmapFont.cpp
      ${SRC_DIR}/gfx/SDL_Cursor.cpp
      ${SRC_DIR}/gfx/IDisplay.cpp
      ${SRC_DIR}/gfx/SDL_Image.cpp
      ${SRC_DIR}/gfx/SDL_Pixel.cpp
      ${SRC_DIR}/gfx/SDL_Line.cpp
      ${SRC_DIR}/gfx/SDL_Rectangle.cpp
      ${SRC_DIR}/gfx/SDL_Display.cpp
      ${SRC_DIR}/gfx/SDL_Canvas.cpp
      ${SRC_DIR}/gfx/Sprite.cpp
      ${SRC_DIR}/sys/SDL_Input.cpp
      ${SRC_DIR}/sys/SDL_Timer.cpp
      ${SRC_DIR}/sys/FPS.cpp
      ${SRC_DIR}/sys/OSXFS.cpp
    )

set ( GAMELIB_HEADERS
      ${SRC_DIR}/nomlib_types.hpp
      ${SRC_DIR}/math/Transformable.hpp
      ${SRC_DIR}/math/Coords.hpp
      ${SRC_DIR}/math/Color.hpp
      ${SRC_DIR}/sys/IState.hpp
      ${SRC_DIR}/sys/GameStates.hpp
      ${SRC_DIR}/sys/SDL_App.hpp
      #${SRC_DIR}/audio.h
      ${SRC_DIR}/gfx/SDL_Font.hpp
      ${SRC_DIR}/gfx/SDL_Gradient.hpp
      ${SRC_DIR}/gfx/SDL_MessageBox.hpp
      ${SRC_DIR}/gfx/SDL_BitmapFont.hpp
      ${SRC_DIR}/gfx/SDL_Cursor.hpp
      ${SRC_DIR}/gfx/IDrawable.hpp
      ${SRC_DIR}/gfx/SDL_Drawable.hpp
      ${SRC_DIR}/gfx/SDL_Image.hpp
      ${SRC_DIR}/gfx/SDL_Pixel.hpp
      ${SRC_DIR}/gfx/SDL_Line.hpp
      ${SRC_DIR}/gfx/SDL_Rectangle.hpp
      ${SRC_DIR}/gfx/SDL_Canvas.hpp
      ${SRC_DIR}/nomlib_config.hpp
      ${SRC_DIR}/gfx/IDisplay.hpp
      ${SRC_DIR}/gfx/SDL_Display.hpp
      ${SRC_DIR}/gfx/Sprite.hpp
      ${SRC_DIR}/sys/SDL_Input.hpp
      ${SRC_DIR}/sys/SDL_Timer.hpp
      ${SRC_DIR}/sys/FPS.hpp
      ${SRC_DIR}/sys/OSXFS.hpp
    )

find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDL_IMAGE_INCLUDE_DIR} )

find_package ( SDL_mixer REQUIRED )
include_directories ( ${SDL_MIXER_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDL_TTF_INCLUDE_DIR} )

include_directories ( src )
include_directories ( src/audio )
include_directories ( src/gfx )
include_directories ( src/math )
include_directories ( src/sys )

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
