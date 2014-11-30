# Project modules source tree root

# IDE project generators such as Xcode and MSVC will append the build
# configuration type, i.e.: '<cmake_build_dir>/Debug' or
# '<cmake_build_dir>/Release' to this output path.
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

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

if( NOM_BUILD_AUDIO_UNIT )
  set( NOM_USE_OPENAL TRUE )
  set( NOM_USE_LIBSNDFILE TRUE )
endif( NOM_BUILD_AUDIO_UNIT )

if( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )
  set( NOM_USE_SCALEX TRUE )
  set( NOM_USE_HQX TRUE )
endif( NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT )

# Stub option (not implemented)
set( NOM_USE_SDL2_IMAGE TRUE )

# Stub option (not implemented)
set( NOM_USE_SDL2_TTF TRUE )

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
add_subdirectory("graphics")

# nomlib-gui
if( NOM_BUILD_GUI_UNIT )
  add_subdirectory("gui")
endif( NOM_BUILD_GUI_UNIT )

# nomlib-audio
add_subdirectory("audio")
