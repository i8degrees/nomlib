# Project modules source tree root

# Use common build output directories for MSVCPP && Xcode project files.
#
# IMPORTANT: Debug and Release build targets **must** be kept in separate build
# trees!
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/Debug")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/Release")

if(DEBUG)
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG})
else() # Release builds
  set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE})
  set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE})
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE})
endif()

# Third-party dependencies

# SDL2 is required by nomlib-core, which is a required dependency among all
# other modules.
set( SDL2_BUILDING_LIBRARY true ) # Do not link SDL2main; we do this explicitly
find_package( SDL2 REQUIRED )
if( SDL2_FOUND )
  include_directories( ${SDL2_INCLUDE_DIR} )
endif( SDL2_FOUND )

# These variables influence preprocessor definitions for config.hpp.in --
# part of nomlib-core
if( NOM_BUILD_GUI_UNIT )
  set( NOM_USE_LIBROCKET TRUE )
  # TODO: include and library paths need to be added to
  # cmake/modules/FindLibRocket.cmake
  # set( NOM_USE_LIBROCKET_LUA FALSE )
endif( NOM_BUILD_GUI_UNIT )

if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
  set( NOM_USE_SCALEX TRUE )
  set( NOM_USE_HQX TRUE )
endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

find_package(SDL2_IMAGE)
if(SDL2_IMAGE_FOUND)
  # Stub option (not implemented)
  set( NOM_USE_SDL2_IMAGE TRUE )
endif()

# We must do this before cmake reaches `src/core/... to make it in time for
# config.hpp population!`
find_package( SDL2_TTF 2.0...2.22)
if( SDL2_TTF_FOUND )
  set(NOM_USE_SDL2_TTF TRUE)
  set(NOM_USE_SDL_TTF_MAJOR_VERSION 2)
  set(NOM_USE_SDL_TTF_MINOR_VERSION 0)
  set(NOM_USE_SDL_TTF_PATCH_VERSION 12)
endif( SDL2_TTF_FOUND )

find_package( SDL2_TTF 2.23...<3)
if(SDL2_TTF_FOUND)
  set(NOM_USE_SDL2_TTF TRUE)
  set(NOM_USE_SDL_TTF_MAJOR_VERSION 2)
  set(NOM_USE_SDL_TTF_MINOR_VERSION 23)
  set(NOM_USE_SDL_TTF_PATCH_VERSION 0)
endif()

if( SDL2_TTF_FOUND )
  # Add development header files; SDL_ttf.h & friends
  include_directories( ${SDL2_TTF_INCLUDE_DIR} )
endif( SDL2_TTF_FOUND )

# nomlib-core
if( NOM_BUILD_CORE_UNIT )
  add_subdirectory("core")
endif( NOM_BUILD_CORE_UNIT )

# nomlib-file
if( NOM_BUILD_FILE_UNIT )
  add_subdirectory("file")
endif( NOM_BUILD_FILE_UNIT )

# nomlib-math
if( NOM_BUILD_MATH_UNIT )
  add_subdirectory("math")
endif( NOM_BUILD_MATH_UNIT )

# nomlib-ptree
if( NOM_BUILD_PTREE_UNIT )
  add_subdirectory("ptree")
endif( NOM_BUILD_PTREE_UNIT )

# nomlib-serializers
if( NOM_BUILD_SERIALIZERS_UNIT )
  add_subdirectory("serializers")
endif( NOM_BUILD_SERIALIZERS_UNIT )

# nomlib-system
if( NOM_BUILD_SYSTEM_UNIT )
  add_subdirectory("system")
endif( NOM_BUILD_SYSTEM_UNIT )

# nomlib-graphics
if(NOM_BUILD_GRAPHICS_UNIT)
  add_subdirectory("graphics")
endif(NOM_BUILD_GRAPHICS_UNIT)

# nomlib-gui
if( NOM_BUILD_GUI_UNIT )
  add_subdirectory("gui")
endif( NOM_BUILD_GUI_UNIT )

# nomlib-audio
if(NOM_BUILD_AUDIO_UNIT)
  # TODO(jeff): Consider setting NOM_USE_OPENAL when one of these three are
  # set; it might also be good to add a check inside the OpenAL header file
  # ensuring that only one of these three options are toggled at any given
  # time.

  # NOTE(jeff): OpenAL-Soft offers quad, 5.1, 6.1 and 7.1 channel
  # configurations.
  #
  # http://openal.996291.n3.nabble.com/Openal-S-PDIF-and-surround-td2410.html
  # http://steamcommunity.com/sharedfiles/filedetails/?id=230642515
  set(NOM_USE_CREATIVE_OPENAL FALSE)
  set(NOM_USE_OPENAL_SOFT FALSE)
  set(NOM_USE_LIBSNDFILE TRUE)

  if ( PLATFORM_POSIX )
    set(NOM_USE_OPENAL_SOFT TRUE)
  endif ( PLATFORM_POSIX )

  if ( PLATFORM_OSX )
    set(NOM_USE_APPLE_OPENAL TRUE)
  endif ( PLATFORM_OSX )

  add_subdirectory("audio")
endif(NOM_BUILD_AUDIO_UNIT)

if(NOM_BUILD_ACTIONS_UNIT)
  add_subdirectory("actions")
endif(NOM_BUILD_ACTIONS_UNIT)
