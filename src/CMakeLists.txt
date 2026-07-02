# src/CMakeLists.txt:jeff
#
# Project modules source tree

# Use common build output directories for MSVCPP && Xcode project files.
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Auto-generation for NOM_EXPORT macros, i.e.: __declspec(dllexport) when
# building DLL shared library targets (MS Windows)
include(GenerateExportHeader)

# This module is required for finding the current git SHA we are at
include(GetGitRevisionNumber)

set(SDL2_BUILDING_LIBRARY true) # Do not link SDL2main; we do this explicitly

#
## config.hpp.in (CMake template) -> config.hpp
#
### Environment variables we set that influence the settings in config.hpp
#

if(SDL2_FOUND)
  set(NOM_USE_SDL2_ASSERT FALSE)
  set(NOM_USE_SDL2 TRUE)         # TODO: add to config.hpp.in
endif()

if(NOM_BUILD_GUI_UNIT)
  set(NOM_USE_LIBROCKET TRUE)
  # set(NOM_USE_LIBROCKET_LUA FALSE)
  # set(NOM_USE_LIBROCKET_PYTHON FALSE)
endif(NOM_BUILD_GUI_UNIT)

if(NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT)
  set(NOM_USE_SCALEX TRUE)
  set(NOM_USE_HQX TRUE)
endif(NOM_BUILD_EXTRA_RESCALE_ALGO_UNIT)

if(SDL2_image_FOUND)
  set(NOM_USE_SDL2_IMAGE TRUE)
endif(SDL2_image_FOUND)

find_package(SDL2_TTF 2.0...2.22)
if(SDL2_TTF_FOUND)
  set(NOM_USE_SDL2_TTF TRUE)
  set(NOM_USE_SDL_TTF_MAJOR_VERSION 2)
  set(NOM_USE_SDL_TTF_MINOR_VERSION 0)
  set(NOM_USE_SDL_TTF_PATCH_VERSION 12)
endif(SDL2_TTF_FOUND)

find_package(SDL2_TTF 2.23...<3)
if(SDL2_TTF_FOUND)
  set(NOM_USE_SDL2_TTF TRUE)
  set(NOM_USE_SDL_TTF_MAJOR_VERSION 2)
  set(NOM_USE_SDL_TTF_MINOR_VERSION 23)
  set(NOM_USE_SDL_TTF_PATCH_VERSION 0)
endif(SDL2_TTF_FOUND)

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

  if(PLATFORM_POSIX)
    set(NOM_USE_OPENAL_SOFT TRUE)
  endif(PLATFORM_POSIX)

  if(PLATFORM_OSX)
    set(NOM_USE_APPLE_OPENAL TRUE)
  endif(PLATFORM_OSX)
endif(NOM_BUILD_AUDIO_UNIT)

#
## Auto-generation of header files must occur before build of library targets
#

# Used in revision.hpp.in
git_rev_number(GIT_REVISION "--short")
set(GIT_REVISION "${GIT_REVISION}")

# config.hpp.in -> include/nomlib/config.hpp
configure_file("${PROJECT_SOURCE_DIR}/cmake/templates/config.hpp.in"
  "${PROJECT_SOURCE_DIR}/include/nomlib/config.hpp"
)

# revision.hpp.in -> include/nomlib/revision.hpp
configure_file("${PROJECT_SOURCE_DIR}/cmake/templates/revision.cpp.in"
  "${PROJECT_SOURCE_DIR}/src/revision.cpp"
)

# version.hpp.in -> include/nomlib/version.hpp
configure_file("${PROJECT_SOURCE_DIR}/cmake/templates/version.hpp.in"
  "${PROJECT_SOURCE_DIR}/include/nomlib/version.hpp"
)

# nomlib-actions
if(NOM_BUILD_ACTIONS_UNIT)
  add_subdirectory("actions")
endif(NOM_BUILD_ACTIONS_UNIT)

# nomlib-audio
if(NOM_BUILD_AUDIO_UNIT)
  add_subdirectory("audio")
endif(NOM_BUILD_AUDIO_UNIT)

# nomlib-core
if(NOM_BUILD_CORE_UNIT)
  add_subdirectory("core")
endif(NOM_BUILD_CORE_UNIT)

# nomlib-file
if(NOM_BUILD_FILE_UNIT)
  add_subdirectory("file")
endif(NOM_BUILD_FILE_UNIT)

# nomlib-graphics
if(NOM_BUILD_GRAPHICS_UNIT)
  add_subdirectory("graphics")
endif(NOM_BUILD_GRAPHICS_UNIT)

# nomlib-gui
if(NOM_BUILD_GUI_UNIT)
  add_subdirectory("gui")
endif(NOM_BUILD_GUI_UNIT)

# nomlib-math
if(NOM_BUILD_MATH_UNIT)
  add_subdirectory("math")
endif(NOM_BUILD_MATH_UNIT)

# nomlib-ptree
if(NOM_BUILD_PTREE_UNIT)
  add_subdirectory("ptree")
endif(NOM_BUILD_PTREE_UNIT)

# nomlib-serializers
if(NOM_BUILD_SERIALIZERS_UNIT)
  add_subdirectory("serializers")
endif(NOM_BUILD_SERIALIZERS_UNIT)

# nomlib-system
if(NOM_BUILD_SYSTEM_UNIT)
  add_subdirectory("system")
endif(NOM_BUILD_SYSTEM_UNIT)
