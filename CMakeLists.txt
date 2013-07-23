cmake_minimum_required ( VERSION 2.8 )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 1 )
set ( PROJECT_VERSION_PATCH 0 )

project ( nomlib ) # $PROJECT_NAME

set ( SRC_DIR "${PROJECT_SOURCE_DIR}/src" ) # nomlib/src
set ( NOMLIB_HEADERS_DIR "include" ) # include/nomlib/
set ( EXAMPLES_SRC_DIR "${PROJECT_SOURCE_DIR}/examples" ) # nomlib/examples

configure_file  ( "${PROJECT_SOURCE_DIR}/cmake/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${NOMLIB_HEADERS_DIR}/${PROJECT_NAME}/version.hpp"
                )

#     CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules" )

option ( BUILD_RELEASE "Build release variant; defaults to Debug" off )
option ( BUILD_SHARED_LIBS "Build as a shared library -- as opposed to static library" on )
option ( BUILD_UNIVERSAL_LIB "Build as OSX Universal Library; i386 and x86_64" off )
option ( BUILD_FRAMEWORKS "Build OSX Frameworks instead of dylib" off )
option ( BUILD_NOMLIB_EXAMPLES "Build nomlib usage examples" on )

# Creative OpenAL was a tad bit buggy for me on audio device destruction
# OpenAL-Soft is significantly more crash prone for me, so perhaps your mileage
# will vary! Pick your poison! :-) Creative OpenAL is the default.
#
# Either option ought to compile & generally run -- OpenAL-Soft is backwards
# compatible with Creative's API.
option ( USE_OPENALSOFT_LIB "Use OpenAL-Soft library" off )

if ( BUILD_RELEASE)
  set ( CMAKE_BUILD_TYPE Release )
  #set ( CMAKE_VERBOSE_MAKEFILE false )
  message ( "Building Release target" )
else ( NOT BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Debug )
  #set ( CMAKE_VERBOSE_MAKEFILE true )
  message ( "Building Debug target" )
endif ( BUILD_RELEASE )

if ( BUILD_SHARED_LIBS )
  message ( "Building as a shared library" )

  if ( BUILD_FRAMEWORKS )
    message ( "Building as a OSX Frameworks library" )
  endif ( BUILD_FRAMEWORKS )

else ( NOT BUILD_SHARED_LIBS )
  message ( "Building as a static library" )
endif ( BUILD_SHARED_LIBS )

if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

if ( USE_OPENALSOFT_LIB )
  message ( "Building with OpenAL-Soft library" )
else ( NOT USE_OPENALSOFT_LIB )
  message ( "Building with OpenAL library" )
endif ( USE_OPENALSOFT_LIB )

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

    #set ( CMAKE_CXX_COMPILER "/usr/bin/c++" )
    set ( CMAKE_CXX_FLAGS -std=c++0x )
  elseif ( CMAKE_SYSTEM_NAME STREQUAL Darwin )
    message ( "Darwin OS (Mac OSX) detected" )  # Developed 100% on OSX v10.8.3

    #set ( CMAKE_CXX_COMPILER "/usr/bin/clang++" )
    set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )

  else () # Not Linux nor OSX
    message ( "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )

  endif ( CMAKE_SYSTEM_NAME STREQUAL Linux )

endif ( NOT TTCARDS_NOMLIB_COMPILE )

set ( NOMLIB_SOURCE
      # audio
      ${SRC_DIR}/audio/AL/AudioDevice.cpp
      ${SRC_DIR}/audio/AL/Listener.cpp
      ${SRC_DIR}/audio/AL/OpenAL.cpp
      ${SRC_DIR}/audio/AL/SoundBuffer.cpp
      ${SRC_DIR}/audio/AL/SoundFile.cpp
      ${SRC_DIR}/audio/AL/SoundSource.cpp
      ${SRC_DIR}/audio/AL/Music.cpp
      ${SRC_DIR}/audio/AL/Sound.cpp
      # gfx
      ${SRC_DIR}/gfx/Text.cpp
      ${SRC_DIR}/gfx/SDL_BitmapFont.cpp
      ${SRC_DIR}/gfx/SDL_Font.cpp
      ${SRC_DIR}/gfx/IDisplay.cpp
      ${SRC_DIR}/gfx/SDL_Canvas.cpp
      ${SRC_DIR}/gfx/SDL_Display.cpp
      ${SRC_DIR}/gfx/SDL_Gradient.cpp
      ${SRC_DIR}/gfx/SDL_Image.cpp
      ${SRC_DIR}/gfx/SDL_Line.cpp
      ${SRC_DIR}/gfx/SDL_Pixel.cpp
      ${SRC_DIR}/gfx/SDL_Rectangle.cpp
      ${SRC_DIR}/gfx/Sprite.cpp
      # sys
      ${SRC_DIR}/sys/Clock.cpp
      ${SRC_DIR}/sys/FPS.cpp
      ${SRC_DIR}/sys/GameStates.cpp
      ${SRC_DIR}/sys/Logger.cpp
      ${SRC_DIR}/sys/ObjectCache.cpp
      ${SRC_DIR}/sys/OSXFS.cpp
      ${SRC_DIR}/sys/SDL_App.cpp
      ${SRC_DIR}/sys/SDL_Input.cpp
      ${SRC_DIR}/sys/SDL_Timer.cpp
      ${SRC_DIR}/sys/Sleep.cpp
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

#     Project Header Files
include_directories ( ${NOMLIB_HEADERS_DIR} )

#     Required Dependencies
find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDL_LIBRARY} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDL_IMAGE_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDLIMAGE_LIBRARY} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDL_TTF_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${SDLTTF_LIBRARY} )

if ( USE_OPENALSOFT_LIB )
  find_package ( OpenALSoft REQUIRED )
  include_directories ( ${OPENALSOFT_INCLUDE_DIR} )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${OPENALSOFT_LIBRARY} )
else ( NOT USE_OPENALSOFT_LIB )
  find_package ( OpenAL REQUIRED )
  include_directories ( ${OPENAL_INCLUDE_DIR} )
  set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${OPENAL_LIBRARY} )
endif ( USE_OPENALSOFT_LIB )

find_package ( LibSndFile REQUIRED )
include_directories ( ${LIBSNDFILE_INCLUDE_DIR} )
set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${LIBSNDFILE_LIBRARY} )

find_package ( LibMagic REQUIRED )

  if ( ${LIBMAGIC_FOUND} )
    include_directories ( ${LIBMAGIC_INCLUD_DIR} )
    set ( REQUIRED_LIBS ${REQUIRED_LIBS} ${LIBMAGIC_LIBRARY} )

  endif ( ${LIBMAGIC_FOUND} )

link_libraries  ( ${PROJECT_NAME} ${REQUIRED_LIBS} )

if ( NOT BUILD_SHARED_LIBS )
  add_library ( ${PROJECT_NAME} STATIC ${NOMLIB_SOURCE} )
else ( BUILD_SHARED_LIBS )
  add_library ( ${PROJECT_NAME} SHARED ${NOMLIB_SOURCE} )
endif ( NOT BUILD_SHARED_LIBS )

set_target_properties ( ${PROJECT_NAME} # makefile target
                        PROPERTIES SOVERSION
                        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                      )

if ( BUILD_SHARED_LIBS AND CMAKE_SYSTEM_NAME STREQUAL Darwin )

  if ( BUILD_FRAMEWORKS )

    # Frameworks distribution
    set_target_properties ( ${PROJECT_NAME} PROPERTIES
                            FRAMEWORK
                            TRUE
                            FRAMEWORK_VERSION
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}"
                            MACOSX_FRAMEWORK_IDENTIFIER
                            "org.dev.${PROJECT_NAME}"
                            MACOSX_FRAMEWORK_SHORT_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                            MACOSX_FRAMEWORK_BUNDLE_VERSION
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                          )

  endif ( BUILD_FRAMEWORKS )

  # For distribution in user's frameworks / OSX App Bundle
  # TODO: @rpath
  # TODO: when a dylib, use @executable_path/../Libraries
  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          BUILD_WITH_INSTALL_RPATH 1
                          INSTALL_NAME_DIR
                          "@executable_path/../Frameworks"
                        )

endif ( BUILD_SHARED_LIBS AND CMAKE_SYSTEM_NAME STREQUAL Darwin )

if ( NOT TTCARDS_NOMLIB_COMPILE )

  if ( BUILD_FRAMEWORKS AND BUILD_SHARED_LIBS )
    install ( TARGETS ${PROJECT_NAME} LIBRARY FRAMEWORK DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
  else ( NOT BUILD_FRAMEWORKS AND BUILD_SHARED_LIBS )

#     Install library
    if ( BUILD_SHARED_LIBS )
      install ( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib )
    else ( NOT BUILD_SHARED_LIBS )
      install ( TARGETS ${PROJECT_NAME} ARCHIVE DESTINATION lib )
    endif ( BUILD_SHARED_LIBS )

  endif ( BUILD_FRAMEWORKS AND BUILD_SHARED_LIBS )

  if ( BUILD_FRAMEWORKS )
    # FIXME; should be ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.framework/Versions/Current/Headers with a symlink at root of framework
    #                   ${NOMLIB_HEADERS_DIR}/$PROJECT_NAME
    install ( DIRECTORY ${NOMLIB_HEADERS_DIR}/${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.framework/Headers
              PATTERN ".*" EXCLUDE # do not copy dotfiles
            )

  else ( NOT BUILD_FRAMEWORKS )

#     Install headers; do not include dotfiles -- *cough* .DS_Store *cough*
#                       ${NOMLIB_HEADERS_DIR}/$PROJECT_NAME
    install ( DIRECTORY ${NOMLIB_HEADERS_DIR}/${PROJECT_NAME} DESTINATION include
              PATTERN ".*" EXCLUDE
            )

  endif ( BUILD_FRAMEWORKS )

  if ( CMAKE_SYSTEM_NAME STREQUAL Linux )
    install(CODE "MESSAGE(\"Post-install: sudo ldconfig.\")")
  endif ( CMAKE_SYSTEM_NAME STREQUAL Linux )

  #     Uninstall target
  configure_file  (
                    "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                    "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                    IMMEDIATE @ONLY
                  )

  add_custom_target ( uninstall
                      COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                    )

endif ( NOT TTCARDS_NOMLIB_COMPILE )

if ( BUILD_NOMLIB_EXAMPLES )
  set ( EXECUTABLE_OUTPUT_PATH "${EXAMPLES_SRC_DIR}" )
  set ( NOMLIB_EXAMPLES_AUDIO
        ${EXAMPLES_SRC_DIR}/audio/audio.cpp
      )
  set ( NOMLIB_EXAMPLES_APP
        ${EXAMPLES_SRC_DIR}/app/app.cpp
      )
  set ( NOMLIB_EXAMPLES_FONTS
      ${EXAMPLES_SRC_DIR}/fonts/font.cpp
    )

  link_libraries ( ${PROJECT_NAME} SDLmain )
  add_executable ( audio/audio ${NOMLIB_EXAMPLES_AUDIO} )
  add_executable ( app/app ${NOMLIB_EXAMPLES_APP} )
  add_executable ( fonts/font ${NOMLIB_EXAMPLES_FONTS} )
endif ( BUILD_NOMLIB_EXAMPLES )

#     FIXME: doxygen docs generation
add_custom_target ( nomlib-docs
                    COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE}
                  )
