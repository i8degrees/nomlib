# Locate nomlib
#
# This module defines:
#
# NOMLIB_FOUND, TRUE when all the specified components are found, FALSE when
# one or more components are not found.
#
# NOMLIB_XXX_FOUND, TRUE when the specified component (debug or release) is
# found, FALSE otherwise.
#
# NOMLIB_LIBRARIES, the list of libraries corresponding to the found
# components.
#
# NOMLIB_INCLUDE_DIR, the path where nomlib's development header files are
# located.
#
# NOMLIB_EXTERNAL_INCLUDE_DIRS, the paths where required external development
# header files are located.
#
# NOMLIB_DEPENDENCIES, the paths where the required external libraries are
# located.
#
# NOTE: On OS X, framework libraries are preferred (chosen) over dynamic
# libraries.
#
# Usage
#
# find_package( nomlib [QUIET] COMPONENTS ... [REQUIRED] [OPTIONAL_COMPONENTS] ... )
#
# QUIET, disables output messages when the component cannot be found.
#
# REQUIRED, stops CMake processing with an error message when the component
# cannot be found.
#
# If nomlib is not installed in a standard path -- one that is automatically
# searched by CMake -- you can set the NOMLIB_ROOT CMake variable before
# invoking find_package( nomlib ... ) with the root installation prefix.
#
# The NOMLIB_ROOT variable can also be set from the environment and invoked at
# the time of CMake generation (cmake ..).
#
# Example
#
# find_package( nomlib COMPONENTS core file math audio system graphics REQUIRED
#               OPTIONAL_COMPONENTS audio )
#
# include_directories( ${NOMLIB_INCLUDE_DIR} )
# add_executable( app ... )
# target_link_libraries( app ${NOMLIB_LIBRARIES} )
#
# Findnomlib.cmake TODO
#
# - [ ] Support find_package version syntax, i.e.:
# ```find_package( nomlib 0.11.0 ... )```
# - [ ] Support explicit use of debug && release libraries.
# - [ ] Ensure that the proper libraries are selected, depending on
# architecture (i.e.: 32-bit VS 64-bit).
#
# Copyright (c) 2014 Jeffrey Carpenter <i8degrees@gmail.com>
#
# Distributed under the Simplified BSD License; see accompanying file
# LICENSE.md.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
#
# To distribute this file outside of CMake, substitute the full license text
# for the above reference.

if( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )
  set( NOM_PLATFORM_OSX TRUE )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Windows" )
  set( NOM_PLATFORM_WINDOWS TRUE )
elseif( CMAKE_SYSTEM_NAME STREQUAL "Linux" )
  set( NOM_PLATFORM_LINUX TRUE )
endif( CMAKE_SYSTEM_NAME STREQUAL "Darwin" )

# The path **prefixes** used in the search for nomlib library components; used
# in calls made to find_library (CMake).
set(  NOM_SEARCH_PREFIX_PATHS
      ${NOMLIB_ROOT}
      $ENV{NOMLIB_ROOT}
      ~/Library/Frameworks
      /Library/Frameworks
      /usr/local # homebrew
      /usr
      /sw # Fink
      /opt/local # DarwinPorts
      /opt/csw # Blastwave
      /opt
)

# Additional path **suffixes** used in the search for nomlib library components
# in Windows land; used in calls made to find_library (CMake).
set(  VC_LIB_PATH_SUFFIX
      lib/x86
      lib/x64
)
# if(CMAKE_SIZEOF_VOID_P EQUAL 8)
#   set(VC_LIB_PATH_SUFFIX lib/x64)
# else()
#   set(VC_LIB_PATH_SUFFIX lib/x86)
# endif()

# The path **suffixes** used in the search for nomlib library components; used
# in calls made to find_library (CMake).
set(  NOM_LIBRARY_PATH_SUFFIXES
      lib64
      libs64
      libs/Win64 # OpenAL
      lib
      libs
      libs/Win32 # OpenAL
      ${VC_LIB_PATH_SUFFIX}
)

find_path(  NOMLIB_INCLUDE_DIR nomlib/config.hpp
            PATH_SUFFIXES include
            PATHS ${NOM_SEARCH_PREFIX_PATHS}
)

# Initialize default state of module
set( NOMLIB_FOUND TRUE )
set( NOMLIB_DEPENDENCIES "")
set( NOM_MISSING "" )

foreach( component ${nomlib_FIND_COMPONENTS} )

  string( TOLOWER ${component} NOM_COMPONENT_LOWER )
  string( TOUPPER ${component} NOM_COMPONENT_UPPER )
  set( NOM_COMPONENT_NAME "nomlib-${NOM_COMPONENT_LOWER}" )

  find_library( NOMLIB_${NOM_COMPONENT_UPPER}_LIBRARY
                NAMES ${NOM_COMPONENT_NAME} ${NOM_COMPONENT_NAME}-d
                PATHS ${NOM_SEARCH_PREFIX_PATHS}
                PATH_SUFFIXES ${NOM_LIBRARY_PATH_SUFFIXES}
  )

  if( NOMLIB_${NOM_COMPONENT_UPPER}_LIBRARY )
    set( NOMLIB_${NOM_COMPONENT_UPPER}_FOUND TRUE )
    list( APPEND NOMLIB_LIBRARIES ${NOMLIB_${NOM_COMPONENT_UPPER}_LIBRARY} )
  else()
    # Err: missing nomlib component that is marked required
    if( nomlib_FIND_REQUIRED_${NOM_COMPONENT_LOWER} )
      list( APPEND NOM_MISSING ${NOM_COMPONENT_LOWER} )
      set( NOMLIB_FOUND FALSE )
    endif()

  endif( NOMLIB_${NOM_COMPONENT_UPPER}_LIBRARY )

endforeach( component ${nomlib_FIND_COMPONENTS} )

# Locate third-party libraries
#
# output_var  The variable to assign results output to
# output_name The name to use as a NOTFOUND result (the err condition)
# ...         Library names to search for
macro( find_nom_dependency output_var output_name )

  set( SUFFIX_LIB_NAMES "" )

  # Additional path suffixes used in the search for libraries (.lib files) in
  # Windows land
  foreach( lib_name ${ARGN} )

    # Path to Windows lib files
    list( APPEND SUFFIX_LIB_NAMES "${lib_name}/lib/x86" )
    list( APPEND SUFFIX_LIB_NAMES "${lib_name}/lib/x64" )

    # Required by our OpenAL path convention for locating Windows lib files
    list( APPEND SUFFIX_LIB_NAMES "${lib_name}/libs/Win32" )
    list( APPEND SUFFIX_LIB_NAMES "${lib_name}/libs/Win64" )

    # Required for finding libRocket libs on OSX
    list( APPEND SUFFIX_LIB_NAMES "${lib_name}/lib" )
  endforeach( lib_name ${ARGN} )

  # Try to find the external dependencies from the pre-packaged locations
  # first
  find_library( ${output_var} NAMES ${ARGN}
                PATHS
                ${NOMLIB_DEPS_ROOT}
                $ENV{NOMLIB_DEPS_ROOT}
                ${NOM_SEARCH_PREFIX_PATHS}
                PATH_SUFFIXES
                ${NOM_LIBRARY_PATH_SUFFIXES} # osx, *nix
                ${SUFFIX_LIB_NAMES} # windows
  )

  if( ${output_var} STREQUAL "${output_var}-NOTFOUND" )
    # unset( output_var )
    list( APPEND NOM_MISSING ${output_name} )
    set( NOMLIB_FOUND FALSE )
  endif( ${output_var} STREQUAL "${output_var}-NOTFOUND" )

endmacro( find_nom_dependency output_var output_name )

if( NOMLIB_CORE_FOUND )

  if( NOM_PLATFORM_WINDOWS )
    list( APPEND NOMLIB_DEPS_ROOT "${NOMLIB_DEPS_ROOT}/SDL2/lib/x86" )

    find_nom_dependency( SDL2MAIN_LIBRARY "SDL2main" SDL2main )
    list( APPEND NOMLIB_CORE_DEPENDENCIES "${SDL2MAIN_LIBRARY}" )
  endif( NOM_PLATFORM_WINDOWS )

  if( NOMLIB_STATIC_LIBRARIES OR NOM_PLATFORM_WINDOWS )
    find_nom_dependency( SDL2_LIBRARY "SDL2" SDL2 )

    list( APPEND NOMLIB_CORE_DEPENDENCIES "${SDL2_LIBRARY}" )
    list( APPEND NOMLIB_DEPENDENCIES ${NOMLIB_CORE_DEPENDENCIES} )
  endif( NOMLIB_STATIC_LIBRARIES OR NOM_PLATFORM_WINDOWS )

  # Add the required external project includes
  find_path(  SDL2_INCLUDE_DIR SDL.h
              PATHS
              ${NOMLIB_DEPS_ROOT}
              $ENV{NOMLIB_DEPS_ROOT}
              ${NOM_SEARCH_PREFIX_PATHS}
              PATH_SUFFIXES SDL2/include include
  )

  if( ${SDL2_INCLUDE_DIR} STREQUAL "SDL2_INCLUDE_DIR-NOTFOUND" )
    set( NOM_MISSING "SDL.h" )
    set( NOMLIB_FOUND FALSE )
  else()
    list( APPEND NOMLIB_EXTERNAL_INCLUDE_DIRS ${SDL2_INCLUDE_DIR} )
  endif( ${SDL2_INCLUDE_DIR} STREQUAL "SDL2_INCLUDE_DIR-NOTFOUND" )

endif( NOMLIB_CORE_FOUND )

if( NOMLIB_SYSTEM_FOUND )

  if( NOMLIB_STATIC_LIBRARIES OR NOM_PLATFORM_WINDOWS )
    find_nom_dependency( SDL2_IMAGE_LIBRARY "SDL2_IMAGE" SDL2_image )
    find_nom_dependency( SDL2_TTF_LIBRARY "SDL2_TTF" SDL2_ttf )

    list( APPEND NOMLIB_SYSTEM_DEPENDENCIES "${SDL2_IMAGE_LIBRARY}" )
    list( APPEND NOMLIB_SYSTEM_DEPENDENCIES "${SDL2_TTF_LIBRARY}" )
    list( APPEND NOMLIB_DEPENDENCIES ${NOMLIB_SYSTEM_DEPENDENCIES} )
  endif( NOMLIB_STATIC_LIBRARIES OR NOM_PLATFORM_WINDOWS )

endif( NOMLIB_SYSTEM_FOUND )

if( NOMLIB_GUI_FOUND )

  if( NOM_PLATFORM_WINDOWS )
    # System library
    list( APPEND NOMLIB_GUI_DEPENDENCIES "opengl32" )
  endif( NOM_PLATFORM_WINDOWS )

  find_nom_dependency( LIBROCKET_CORE_LIBRARY "RocketCore" libRocket RocketCore.1 RocketCore )
  find_nom_dependency( LIBROCKET_CONTROLS_LIBRARY "RocketControls" libRocket RocketControls.1 RocketControls )
  find_nom_dependency( LIBROCKET_DEBUGGER_LIBRARY "RocketDebugger" libRocket RocketDebugger.1 RocketDebugger )

  list( APPEND NOMLIB_GUI_DEPENDENCIES "${LIBROCKET_CORE_LIBRARY}" )
  list( APPEND NOMLIB_GUI_DEPENDENCIES "${LIBROCKET_CONTROLS_LIBRARY}" )
  list( APPEND NOMLIB_GUI_DEPENDENCIES "${LIBROCKET_DEBUGGER_LIBRARY}" )

  list( APPEND NOMLIB_DEPENDENCIES ${NOMLIB_GUI_DEPENDENCIES} )

  # Add the required external project includes
  find_path(  LIBROCKET_INCLUDE_DIR Rocket/Core
              PATHS
              ${NOMLIB_DEPS_ROOT}
              $ENV{NOMLIB_DEPS_ROOT}
              ${NOM_SEARCH_PREFIX_PATHS}
              PATH_SUFFIXES libRocket/include include
  )

  if( ${LIBROCKET_INCLUDE_DIR} STREQUAL "LIBROCKET_INCLUDE_DIR-NOTFOUND" )
    set( NOM_MISSING "libRocket development header files" )
    set( NOMLIB_FOUND FALSE )
  else()
    list( APPEND NOMLIB_EXTERNAL_INCLUDE_DIRS ${LIBROCKET_INCLUDE_DIR} )
  endif( ${LIBROCKET_INCLUDE_DIR} STREQUAL "LIBROCKET_INCLUDE_DIR-NOTFOUND" )

endif( NOMLIB_GUI_FOUND )

if( NOMLIB_AUDIO_FOUND )

  if( NOM_PLATFORM_WINDOWS )
    find_nom_dependency( OPENAL_LIBRARY "OpenAL" OpenAL32 OpenAL )
    list( APPEND NOMLIB_AUDIO_DEPENDENCIES "${OPENAL_LIBRARY}" )
  endif( NOM_PLATFORM_WINDOWS )

  if( NOMLIB_STATIC_LIBRARIES OR NOM_PLATFORM_WINDOWS )
    find_nom_dependency( LIBSNDFILE_LIBRARY "libsndfile" libsndfile libsndfile-1 sndfile )

    list( APPEND NOMLIB_AUDIO_DEPENDENCIES "${LIBSNDFILE_LIBRARY}" )
    list( APPEND NOMLIB_DEPENDENCIES ${NOMLIB_AUDIO_DEPENDENCIES} )
  endif( NOMLIB_STATIC_LIBRARIES OR NOM_PLATFORM_WINDOWS )

endif( NOMLIB_AUDIO_FOUND )

# Handle errors
if( NOT NOMLIB_INCLUDE_DIR )
  # Err: missing installation of developer header files for nomlib -- have you
  # tried setting installation prefix with NOMLIB_ROOT?
  set( NOM_MISSING "development header files 'include/nomlib'" )
  set( NOMLIB_FOUND FALSE )
endif( NOT NOMLIB_INCLUDE_DIR )

# Handle missing nomlib components with respect given to QUIET, REQUIRED
# options
if( NOT NOMLIB_FOUND )
  if( nomlib_FIND_REQUIRED )
    message( FATAL_ERROR "Could not find nomlib (missing: ${NOM_MISSING})" )
  elseif( NOT nomlib_FIND_QUIETLY )
    message( "Could not find nomlib (missing: ${NOM_MISSING})" )
  endif()
endif( NOT NOMLIB_FOUND )

if( NOMLIB_FOUND )
  message( STATUS "Found nomlib: ${NOMLIB_INCLUDE_DIR}" )
endif( NOMLIB_FOUND )

# Hide from cmake-gui
# TODO
# mark_as_advanced( NOM_DEPENDENCIES_DIR )
