cmake_minimum_required ( VERSION 2.8 )

set ( PROJECT_VERSION_MAJOR 0 )
set ( PROJECT_VERSION_MINOR 1 )
set ( PROJECT_VERSION_PATCH 0 )

project ( nomlib ) # $PROJECT_NAME

set ( SRC_DIR "${PROJECT_SOURCE_DIR}/src" ) # nomlib/src
set ( NOMLIB_HEADERS_DIR "include" ) # include/nomlib
set ( EXAMPLES_SRC_DIR "${PROJECT_SOURCE_DIR}/examples" ) # nomlib/examples
set ( NOMLIB_RESOURCES_DIR "Resources" ) # folder containing library data (ex.: icon, translations, etc.)

# CMake Environment
set ( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake/modules" )

# Auto-generated at build-time header files
configure_file  ( "${PROJECT_SOURCE_DIR}/cmake/version.hpp.in"
                  "${PROJECT_SOURCE_DIR}/${NOMLIB_HEADERS_DIR}/${PROJECT_NAME}/version.hpp"
                )

# Uninstall target support (always available)
configure_file  ( "${PROJECT_SOURCE_DIR}/cmake/cmake_uninstall.cmake.in"
                  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
                  IMMEDIATE @ONLY
                )

add_custom_target ( uninstall COMMAND ${CMAKE_COMMAND} -P
                    ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake
                  )

# FIXME: doxygen docs generation
add_custom_target ( nomlib-docs COMMAND ${DOXYGEN_COMMAND} ${DOXYGEN_CONFIG_FILE} )

# We only support building nomlib as a dynamic library at the moment
set ( BUILD_SHARED_LIBS on )

# Project Options
option ( VERBOSE "Alias for CMAKE_VERBOSE_MAKEFILE -- nice for debugging make scripts" off )
option ( BUILD_RELEASE "Build release variant; defaults to Debug" off )
option ( BUILD_UNIVERSAL_LIB "Build as an OSX Universal Library" off )
option ( BUILD_FRAMEWORK "Build OSX Frameworks instead of dylib" off )
option ( BUILD_NOMLIB_EXAMPLES "Build nomlib usage examples" on )

if ( VERBOSE )
  set ( CMAKE_VERBOSE_MAKEFILE true )
endif ( VERBOSE )

if ( BUILD_RELEASE)
  set ( CMAKE_BUILD_TYPE Release )
  message ( STATUS "Building Release target" )
else ( NOT BUILD_RELEASE )
  set ( CMAKE_BUILD_TYPE Debug )
  message ( STATUS "Building Debug target" )
endif ( BUILD_RELEASE )

# I was able to shave off close to 50% time compiling nomlib by leaving
# this option off by default.
#
# PPC is not officially supported because I have no means whatsoever of
# testing such a package.
if ( BUILD_UNIVERSAL_LIB )
  set ( CMAKE_OSX_ARCHITECTURES i386; x86_64 )
  message ( STATUS "Building an OSX Universal Library" )
endif ( BUILD_UNIVERSAL_LIB )

#     Platform specific
if ( CMAKE_SYSTEM_NAME STREQUAL "Linux" )

  # Tested regularly on a virtualized Ubuntu v12.04-LTS boxen
  set ( PLATFORM_LINUX true )
  message ( STATUS "Linux OS detected " )

  # Clang is not supported on Linux due to libc++ not being distributed by
  # default yet
  set ( CMAKE_CXX_FLAGS -std=c++0x )

elseif ( CMAKE_SYSTEM_NAME STREQUAL "Darwin" ) # OSX / Apple

  set ( PLATFORM_OSX true )
  message ( STATUS "Darwin OS (Mac OSX) detected" )

  # libc++ requires OSX v10.7+
  set ( CMAKE_CXX_FLAGS "-std=c++11 -stdlib=libc++" )

  if ( CMAKE_GENERATOR STREQUAL Xcode ) # Available only on OSX, I think
    message ( STATUS "Setting Xcode-specific project settings " )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11" )
    set ( CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++" )
  endif ( CMAKE_GENERATOR STREQUAL Xcode )

else () # Not Linux nor OSX

  message ( STATUS "Unsupported build system detected... Hopefully CMake gets it right for you; good luck!" )

endif ( CMAKE_SYSTEM_NAME STREQUAL "Linux" )

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
      ${SRC_DIR}/gfx/BitmapFont.cpp
      ${SRC_DIR}/gfx/TrueTypeFont.cpp
      ${SRC_DIR}/gfx/IDisplay.cpp
      ${SRC_DIR}/gfx/Canvas.cpp
      ${SRC_DIR}/gfx/Display.cpp
      ${SRC_DIR}/gfx/Gradient.cpp
      ${SRC_DIR}/gfx/Image.cpp
      ${SRC_DIR}/gfx/Line.cpp
      ${SRC_DIR}/gfx/Pixel.cpp
      ${SRC_DIR}/gfx/Rectangle.cpp
      ${SRC_DIR}/gfx/Sprite.cpp
      # sys
      ${SRC_DIR}/sys/Clock.cpp
      ${SRC_DIR}/sys/FPS.cpp
      ${SRC_DIR}/sys/GameStates.cpp
      ${SRC_DIR}/sys/Logger.cpp
      ${SRC_DIR}/sys/ObjectCache.cpp
      ${SRC_DIR}/sys/OSXFS.cpp
      ${SRC_DIR}/sys/SDL_App.cpp
      ${SRC_DIR}/sys/Input.cpp
      ${SRC_DIR}/sys/Timer.cpp
      ${SRC_DIR}/sys/Sleep.cpp
      # math
      ${SRC_DIR}/math/Color.cpp
      ${SRC_DIR}/math/Coords.cpp
      ${SRC_DIR}/math/Transformable.cpp
      # gui
      ${SRC_DIR}/gui/Cursor.cpp
      ${SRC_DIR}/gui/MessageBox.cpp
      # sdl
      ${SRC_DIR}/sdl/utils.cpp
    )

#     Project Header Files
include_directories ( ${NOMLIB_HEADERS_DIR} )

#     Required Dependencies
find_package ( SDL REQUIRED )
include_directories ( ${SDL_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${SDL_LIBRARY} )

find_package ( SDL_image REQUIRED )
include_directories ( ${SDL_IMAGE_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${SDLIMAGE_LIBRARY} )

find_package ( SDL_ttf REQUIRED )
include_directories ( ${SDL_TTF_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${SDLTTF_LIBRARY} )

find_package ( OpenAL REQUIRED )
include_directories ( ${OPENAL_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${OPENAL_LIBRARY} )

find_package ( LibSndFile REQUIRED )
include_directories ( ${LIBSNDFILE_INCLUDE_DIR} )
set ( EXTLIBS ${EXTLIBS} ${LIBSNDFILE_LIBRARY} )

find_package ( LibMagic REQUIRED )

if ( ${LIBMAGIC_FOUND} )

  include_directories ( ${LIBMAGIC_INCLUD_DIR} )
  set ( EXTLIBS ${EXTLIBS} ${LIBMAGIC_LIBRARY} )

endif ( ${LIBMAGIC_FOUND} )

link_libraries  ( ${PROJECT_NAME} ${EXTLIBS} )

add_library ( ${PROJECT_NAME} SHARED ${NOMLIB_SOURCE} )

set_target_properties ( ${PROJECT_NAME} # makefile target
                        PROPERTIES SOVERSION
                        "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                      )

if ( PLATFORM_OSX )

  # For distribution in user's frameworks / OSX App Bundle
  # TODO: @rpath
  # TODO: when a dylib, use @executable_path/../Libraries
  set_target_properties ( ${PROJECT_NAME} PROPERTIES
                          BUILD_WITH_INSTALL_RPATH 1
                          INSTALL_NAME_DIR
                          "@executable_path/../Frameworks"
                        )

  if ( BUILD_FRAMEWORK )

    # This allows our Resources, or any other not-quite-standard install command
    # install our data in the right place without further ugliness.

    #set ( CMAKE_INSTALL_PREFIX $ENV{HOME}/Library/Frameworks )
    set ( CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_BINARY_DIR} )
    message ( STATUS "Building nomlib as an OSX Framework bundle" )

    # Frameworks distribution
    set_target_properties ( ${PROJECT_NAME} PROPERTIES
                            FRAMEWORK
                            TRUE
                            MACOSX_FRAMEWORK_INFO_PLIST
                            "${PROJECT_SOURCE_DIR}/cmake/FrameworkInfo.plist.in"
                            MACOSX_FRAMEWORK_NAME
                            "${PROJECT_NAME}"
                            MACOSX_FRAMEWORK_ICON_FILE
                            "nomlib.icns"
                            MACOSX_FRAMEWORK_BUNDLE_VERSION
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH}-${CMAKE_BUILD_TYPE}"
                            MACOSX_FRAMEWORK_SHORT_VERSION_STRING
                            "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
                            MACOSX_FRAMEWORK_IDENTIFIER
                            "org.dev.${PROJECT_NAME}"
                          )

    install ( TARGETS ${PROJECT_NAME} LIBRARY FRAMEWORK DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )

    # Optional Resource files (official library icon, docs, libmagic database, etc.)
    install ( DIRECTORY ${NOMLIB_RESOURCES_DIR} DESTINATION "${PROJECT_NAME}.framework"
              OPTIONAL
              PATTERN ".*" EXCLUDE # do not include dotfiles
            )

    # FIXME; should be ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.framework/Versions/Current/Headers with a symlink at root of framework
    install ( DIRECTORY ${NOMLIB_HEADERS_DIR}/${PROJECT_NAME} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.framework/Headers
              PATTERN ".*" EXCLUDE # do not copy dotfiles
            )

  endif ( BUILD_FRAMEWORK )

endif ( PLATFORM_OSX )

if ( NOT BUILD_FRAMEWORK ) # POSIX install; installs in standard locations

  # Install library in POSIX standard location by default
  install ( TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib )

  # Install nomlib header files
  install ( DIRECTORY ${NOMLIB_HEADERS_DIR}/${PROJECT_NAME} DESTINATION include
          PATTERN ".*" EXCLUDE # do not include dotfiles
          )

if ( PLATFORM_LINUX )
  install(CODE "MESSAGE(\"Post-install: sudo ldconfig.\")") # FIXME (prettify)
endif ( PLATFORM_LINUX )

endif ( NOT BUILD_FRAMEWORK )

# nomlib examples configuration
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
