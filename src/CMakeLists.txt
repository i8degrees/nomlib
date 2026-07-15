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

if(SDL2_FOUND) # config.hpp.in vars
  # >> Define our internal assertion macros to use SDL2 assert facilities,
  # >> as opposed to the standard C run-time assertions handler.
  set(NOM_USE_SDL2_ASSERT FALSE)
  set(NOM_USE_SDL2 TRUE)
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

# !! We check to see what version of SDL2_ttf we have available so we can
# !! later check for what specific typedef is employed in order to maintain
# !! backwards compatibility with.
if(SDL2_ttf_FOUND)
  include(CheckCSourceCompiles)
  set(CMAKE_REQUIRED_INCLUDES ${SDL2_INCLUDE_DIR}) # check_c_source_compile
  set(NOM_USE_SDL2_TTF TRUE) # recorded in config.hpp

  # >> Introduced in SDL2_ttf v2.0.12
  check_c_source_compiles("
    #include <SDL_ttf.h>
    typedef struct _TTF_Font TTF_Font;
    int main(void) { return 0; }"
  HAVE_SDL2_TTF_212)

  # >> Modified in SDL2_ttf v2.23.0
  check_c_source_compiles("
    #include <SDL_ttf.h>
    typedef struct TTF_Font TTF_Font;
    int main(void) { return 0; }"
  HAVE_SDL2_TTF_223)

  # Recorded in config.hpp
  if(HAVE_SDL2_TTF_212)
    set(NOM_USE_SDL_TTF_MAJOR_VERSION 2)
    set(NOM_USE_SDL_TTF_MINOR_VERSION 0)
    set(NOM_USE_SDL_TTF_PATCH_VERSION 12)
  endif(HAVE_SDL2_TTF_212)

  # Recorded in config.hpp
  if(HAVE_SDL2_TTF_223)
    set(NOM_USE_SDL_TTF_MAJOR_VERSION 2)
    set(NOM_USE_SDL_TTF_MINOR_VERSION 23)
    set(NOM_USE_SDL_TTF_PATCH_VERSION 0)
  endif(HAVE_SDL2_TTF_223)
endif(SDL2_ttf_FOUND)

if(NOM_BUILD_AUDIO_UNIT)
  include(CheckIncludeFileCXX)

  # Check for OpenAL Soft standard header style
  check_include_file_cxx("AL/al.h" HAVE_AL_AL_H)

  # Check for Apple style framework header
  check_include_file_cxx("OpenAL/al.h" HAVE_OPENAL_AL_H)

  if(HAVE_AL_AL_H)
    message(STATUS "Found OpenAL Soft style headers")
    #add_compile_definitions(USING_OPENAL_SOFT)
    set(NOM_USE_OPENAL_SOFT TRUE)
  elseif(HAVE_OPENAL_AL_H AND APPLE)
    message(STATUS "Found Apple OpenAL.framework headers")
    #add_compile_definitions(USING_APPLE_OPENAL)
    set(NOM_USE_APPLE_OPENAL TRUE)
  endif()

  set(NOM_USE_CREATIVE_OPENAL FALSE)
  set(NOM_USE_LIBSNDFILE TRUE)

  # TODO(jeff): Consider setting NOM_USE_OPENAL when one of these three are
  # set; it might also be good to add a check inside the OpenAL header file
  # ensuring that only one of these three options are toggled at any given
  # time.

  # NOTE(jeff): OpenAL-Soft offers quad, 5.1, 6.1 and 7.1 channel
  # configurations.
  #
  # http://openal.996291.n3.nabble.com/Openal-S-PDIF-and-surround-td2410.html
  # http://steamcommunity.com/sharedfiles/filedetails/?id=230642515
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
