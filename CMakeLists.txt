cmake_minimum_required ( VERSION 2.8 )

project ( gamelib )

#     CMake Environment
set ( CMAKE_VERBOSE_MAKEFILE true )
set ( CMAKE_BUILD_TYPE Debug )
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/" )

option ( BUILD_SHARED_LIBS "Build as a shared library -- as opposed to static library" on )
option ( BUILD_UNIVERSAL_LIB "Build as OSX Universal Library; i386 and x86_64" off )

if ( BUILD_SHARED_LIBS )
  message ( "Building as a shared library" )
else ( NOT BUILD_SHARED_LIBS )
  message ( "Building as a static library" )
endif ( BUILD_SHARED_LIBS )

if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

if ( CMAKE_GENERATOR STREQUAL Xcode )
  message ( "Setting Xcode-specific project settings " )
  set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
  set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
endif ( CMAKE_GENERATOR STREQUAL Xcode )

#     Platform specific
if ( NOT TTCARDS_NOMLIB_COMPILE ) # Ignore all this if TTcards is compiling us

  if ( CMAKE_SYSTEM_NAME STREQUAL Linux ) # Tested on Ubuntu v12.04-LTS(?) with
                                          # applicable dependencies installed
    message ( "Linux OS detected " )

    set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
    set ( CMAKE_CXX_FLAGS -std=c++0x )
  elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
    message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

    set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
    set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
  else () # Not Linux nor OSX
    message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )

  endif ( CMAKE_SYSTEM_NAME STREQUAL Linux )

endif ( NOT TTCARDS_NOMLIB_COMPILE )

set ( SRC_DIR src ) # project_dir/src

set ( GAMELIB_SOURCE
      # audio
      ${SRC_DIR}/audio/OpenAL_AudioDevice.cpp
      ${SRC_DIR}/audio/OpenAL_Listener.cpp
      ${SRC_DIR}/audio/OpenAL_SoundBuffer.cpp
      ${SRC_DIR}/audio/OpenAL_SoundFile.cpp
      ${SRC_DIR}/audio/OpenAL_SoundSource.cpp
      ${SRC_DIR}/audio/Music.cpp
      ${SRC_DIR}/audio/Sound.cpp
      # gfx
      ${SRC_DIR}/gfx/ImageCache.cpp
      ${SRC_DIR}/gfx/IDisplay.cpp
      ${SRC_DIR}/gfx/SDL_BitmapFont.cpp
      ${SRC_DIR}/gfx/SDL_Canvas.cpp
      ${SRC_DIR}/gfx/SDL_Display.cpp
      ${SRC_DIR}/gfx/SDL_Font.cpp
      ${SRC_DIR}/gfx/SDL_Gradient.cpp
      ${SRC_DIR}/gfx/SDL_Image.cpp
      ${SRC_DIR}/gfx/SDL_Line.cpp
      ${SRC_DIR}/gfx/SDL_Pixel.cpp
      ${SRC_DIR}/gfx/SDL_Rectangle.cpp
      ${SRC_DIR}/gfx/Sprite.cpp
      # sys
      ${SRC_DIR}/sys/FPS.cpp
      ${SRC_DIR}/sys/GameStates.cpp
      ${SRC_DIR}/sys/Logger.cpp
      ${SRC_DIR}/sys/OSXFS.cpp
      ${SRC_DIR}/sys/SDL_App.cpp
      ${SRC_DIR}/sys/SDL_Input.cpp
      ${SRC_DIR}/sys/SDL_Timer.cpp
      # math
      ${SRC_DIR}/math/Color.cpp
      ${SRC_DIR}/math/Coords.cpp
      ${SRC_DIR}/math/Transformable.cpp
      # gui
      ${SRC_DIR}/gui/SDL_Cursor.cpp
      ${SRC_DIR}/gui/SDL_MessageBox.cpp
      # sdl
      ${SRC_DIR}/sdl/utils.cpp
    )

set ( GAMELIB_HEADERS_ROOT
      ${SRC_DIR}/nomlib_config.hpp
      ${SRC_DIR}/nomlib_types.hpp
    )

set ( GAMELIB_HEADERS_AUDIO
      ${SRC_DIR}/audio/OpenAL_AudioDevice.hpp
      ${SRC_DIR}/audio/OpenAL_Listener.hpp
      ${SRC_DIR}/audio/OpenAL_SoundBuffer.hpp
      ${SRC_DIR}/audio/OpenAL_SoundFile.hpp
      ${SRC_DIR}/audio/OpenAL_SoundSource.hpp
      ${SRC_DIR}/audio/Music.hpp
      ${SRC_DIR}/audio/Sound.hpp
    )

set ( GAMELIB_HEADERS_GFX
      ${SRC_DIR}/gfx/ImageCache.hpp
      ${SRC_DIR}/gfx/SDL_Font.hpp
      ${SRC_DIR}/gfx/SDL_Gradient.hpp
      ${SRC_DIR}/gfx/SDL_BitmapFont.hpp
      ${SRC_DIR}/gfx/IDrawable.hpp
      ${SRC_DIR}/gfx/SDL_Drawable.hpp
      ${SRC_DIR}/gfx/SDL_Image.hpp
      ${SRC_DIR}/gfx/SDL_Pixel.hpp
      ${SRC_DIR}/gfx/SDL_Line.hpp
      ${SRC_DIR}/gfx/SDL_Rectangle.hpp
      ${SRC_DIR}/gfx/SDL_Canvas.hpp
      ${SRC_DIR}/gfx/IDisplay.hpp
      ${SRC_DIR}/gfx/SDL_Display.hpp
      ${SRC_DIR}/gfx/Sprite.hpp
    )

set ( GAMELIB_HEADERS_SYS
      ${SRC_DIR}/sys/FPS.hpp
      ${SRC_DIR}/sys/GameStates.hpp
      ${SRC_DIR}/sys/IState.hpp
      ${SRC_DIR}/sys/Logger.hpp
      ${SRC_DIR}/sys/OSXFS.hpp
      ${SRC_DIR}/sys/SDL_App.hpp
      ${SRC_DIR}/sys/SDL_Input.hpp
      ${SRC_DIR}/sys/SDL_Timer.hpp
    )

set ( GAMELIB_HEADERS_MATH
      ${SRC_DIR}/math/Color.hpp
      ${SRC_DIR}/math/Coords.hpp
      ${SRC_DIR}/math/Transformable.hpp
    )

set ( GAMELIB_HEADERS_GUI
      ${SRC_DIR}/gui/SDL_Cursor.hpp
      ${SRC_DIR}/gui/SDL_MessageBox.hpp
    )

set ( GAMELIB_HEADERS_SDL
      ${SRC_DIR}/sdl/utils.hpp
    )

#     Project Header Files
include_directories ( src )

#     Required Dependencies
find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDL_IMAGE_INCLUDE_DIR} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDL_TTF_INCLUDE_DIR} )

find_package ( OpenAL REQUIRED )
include_directories ( ${OPENAL_INCLUDE_DIR} )

find_package ( libsndfile REQUIRED )
include_directories ( ${LIBSNDFILE_INCLUDE_DIR} )

link_libraries (  gamelib # makefile target
                  ${SDL_LIBRARY}
                  ${SDLIMAGE_LIBRARY}
                  ${OPENAL_LIBRARY}
                  ${LIBSNDFILE_LIBRARY}
                  ${SDLTTF_LIBRARY}
               )

if ( NOT BUILD_SHARED_LIBS )
  add_library ( gamelib STATIC ${GAMELIB_SOURCE} )
else ( BUILD_SHARED_LIBS )
  add_library ( gamelib SHARED ${GAMELIB_SOURCE} )
endif ( NOT BUILD_SHARED_LIBS )

set_target_properties ( gamelib # makefile target
                        PROPERTIES SOVERSION "0.1"
                      )

#     FIXME: doxygen docs generation
add_custom_target ( nomlib-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )

if ( NOT TTCARDS_NOMLIB_COMPILE )

  #     Install library
  if ( BUILD_SHARED_LIBS )
    install ( TARGETS gamelib LIBRARY DESTINATION lib )
  else ( NOT BUILD_SHARED_LIBS )
    install ( TARGETS gamelib ARCHIVE DESTINATION lib )
  endif ( BUILD_SHARED_LIBS )

  #     Install headers
  install ( FILES ${GAMELIB_HEADERS_ROOT} DESTINATION include/gamelib )
  install ( FILES ${GAMELIB_HEADERS_AUDIO} DESTINATION include/gamelib/audio )
  install ( FILES ${GAMELIB_HEADERS_GFX} DESTINATION include/gamelib/gfx )
  install ( FILES ${GAMELIB_HEADERS_SYS} DESTINATION include/gamelib/sys )
  install ( FILES ${GAMELIB_HEADERS_MATH} DESTINATION include/gamelib/math )
  install ( FILES ${GAMELIB_HEADERS_GUI} DESTINATION include/gamelib/gui )
  install ( FILES ${GAMELIB_HEADERS_SDL} DESTINATION include/gamelib/sdl )

  #     Uninstall target
  configure_file  (
                    "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                    IMMEDIATE @ONLY
                  )

  add_custom_target ( uninstall
                      COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                    )

endif ( NOT TTCARDS_NOMLIB_COMPILE )
