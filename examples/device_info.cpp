/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include <SDL.h>
#include <SDL_opengl.h>
#include "SDL_revision.h"

// Must be included before NOM_USE_* preprocessor definitions are checked
#include "nomlib/config.hpp"
#include "nomlib/platforms.hpp"

#if defined( NOM_USE_SDL2_IMAGE )
  #include <SDL_image.h>
#endif

#if defined( NOM_USE_SDL2_TTF )
  #include <SDL_ttf.h>
#endif

#if defined( NOM_USE_OPENAL )
  #include "nomlib/audio/AL/OpenAL.hpp"
#endif

#if defined(NOM_USE_OPENAL) && defined(NOM_USE_LIBSNDFILE)
  #include "nomlib/audio/AL/SoundFile.hpp"
#endif

#if defined( NOM_USE_LIBROCKET )
  #include "Rocket/Core.h"
#endif

#include "nomlib/version.hpp"
#include "nomlib/graphics.hpp"
#include "nomlib/system.hpp"

namespace nom {

void nomlib_version_info( void )
{
  NOM_LOG_INFO  (
                  NOM_LOG_CATEGORY_APPLICATION,
                  "nomlib version: ",
                  nom::NOM_VERSION.version_string()
                );

  NOM_LOG_INFO  (
                  NOM_LOG_CATEGORY_APPLICATION,
                  "nomlib source revision: ",
                  nom::revision()
                );
}

void SDL2_version_info( void )
{
  SDL_version compiled_ver;
  SDL_version linked_ver;
  SDL_VERSION( &compiled_ver );
  SDL_GetVersion( &linked_ver );

  NOM_LOG_INFO  (
                  NOM_LOG_CATEGORY_APPLICATION,
                  "SDL2 compiled against version: ",
                  NOM_SCAST( int, compiled_ver.major ),
                  ".",
                  NOM_SCAST( int, compiled_ver.minor ),
                  ".",
                  NOM_SCAST( int, compiled_ver.patch )
                );

  NOM_LOG_INFO  (
                  NOM_LOG_CATEGORY_APPLICATION,
                  "SDL2 linked against version: ",
                  NOM_SCAST( int, linked_ver.major ),
                  ".",
                  NOM_SCAST( int, linked_ver.minor ),
                  ".",
                  NOM_SCAST( int, linked_ver.patch )
                );

  NOM_LOG_INFO  (
                  NOM_LOG_CATEGORY_APPLICATION,
                  "SDL2 source revision: ",
                  SDL_GetRevision()
                );
}

void SDL2_image_version_info( void )
{
  #if defined( NOM_USE_SDL2_IMAGE )

    SDL_version compiled_ver;
    const SDL_version *linked_ver = IMG_Linked_Version();
    SDL_IMAGE_VERSION( &compiled_ver );

    NOM_LOG_INFO  (
                    NOM_LOG_CATEGORY_APPLICATION,
                    "SDL2_image compiled version: ",
                    NOM_SCAST( int, compiled_ver.major ),
                    ".",
                    NOM_SCAST( int, compiled_ver.minor ),
                    ".",
                    NOM_SCAST( int, compiled_ver.patch )
                  );

    NOM_LOG_INFO  (
                    NOM_LOG_CATEGORY_APPLICATION,
                    "SDL2_image linked version: ",
                    NOM_SCAST( int, linked_ver->major ),
                    ".",
                    NOM_SCAST( int, linked_ver->minor ),
                    ".",
                    NOM_SCAST( int, linked_ver->patch )
                  );
  #endif
}

void SDL2_ttf_version_info( void )
{
  #if defined( NOM_USE_SDL2_TTF )

    SDL_version compiled_ver;
    const SDL_version *linked_ver = TTF_Linked_Version();
    SDL_TTF_VERSION( &compiled_ver );

    NOM_LOG_INFO  (
                    NOM_LOG_CATEGORY_APPLICATION,
                    "SDL2_ttf compiled version: ",
                    NOM_SCAST( int, compiled_ver.major ),
                    ".",
                    NOM_SCAST( int, compiled_ver.minor ),
                    ".",
                    NOM_SCAST( int, compiled_ver.patch )
                  );

    NOM_LOG_INFO  (
                    NOM_LOG_CATEGORY_APPLICATION,
                    "SDL2_ttf linked version: ",
                    NOM_SCAST( int, linked_ver->major ),
                    ".",
                    NOM_SCAST( int, linked_ver->minor ),
                    ".",
                    NOM_SCAST( int, linked_ver->patch )
                  );
  #endif
}

void OpenAL_version_info( void )
{
  #if defined( NOM_USE_OPENAL )

    struct OpenALVersionInfo
    {
      std::string version;
      std::string renderer;
      std::string vendor;
      std::string extensions;
    };

    OpenALVersionInfo info;

    AL_CHECK_ERR( info.version = alGetString( AL_VERSION ) );
    AL_CHECK_ERR( info.renderer = alGetString( AL_RENDERER ) );
    AL_CHECK_ERR( info.vendor = alGetString( AL_VENDOR ) );
    AL_CHECK_ERR( info.extensions = alGetString( AL_EXTENSIONS ) );

    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "OpenAL version: ", info.version );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "OpenAL renderer: ", info.renderer );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "OpenAL vendor: ", info.vendor );
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "OpenAL extensions: ", info.extensions );
  #endif
}

void libsndfile_version_info( void )
{
  #if defined(NOM_USE_OPENAL) && defined(NOM_USE_LIBSNDFILE)
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                  "libsndfile version:", nom::libsndfile_version() );
  #endif
}

void libs_version_info( void )
{
  std::cout << std::endl;
  nomlib_version_info();
  std::cout << std::endl;
  SDL2_version_info();
  std::cout << std::endl;
  SDL2_image_version_info();
  std::cout << std::endl;
  SDL2_ttf_version_info();
  std::cout << std::endl;
  OpenAL_version_info();
  std::cout << std::endl;

  libsndfile_version_info();
  std::cout << std::endl;

  #if defined( NOM_USE_LIBROCKET )
    std::cout << "libRocket version: " << Rocket::Core::GetVersion().CString()
    << std::endl;
  #endif
}

} // namespace nom

int main ( int argc, char* argv[] )
{
  using namespace nom;

  nom::RenderWindow window;
  nom::Size2i window_size( nom::Size2i::zero );
  nom::RendererInfo renderer_info;
  uint32 window_flags = SDL_WINDOW_HIDDEN | SDL_WINDOW_OPENGL;

  // We need SDL2 video initialization so we can obtain the available rendering
  // caps
  if( nom::init ( argc, argv ) == false )
  {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION, "Could not initialize nomlib." );
    exit( NOM_EXIT_FAILURE );
  }

  atexit(nom::quit);

  auto spec = nom::platform_info();
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Platform name:", spec.name);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "Number of CPUs:",
               spec.num_cpus);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "L1 Cache Size (bytes):",
               spec.cpu_cache_size);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_APPLICATION, "System RAM (bytes):",
               spec.total_ram);

  // Fix for getting incorrect OpenGL version of 2.1 on my MacBook Air
  // (Mid 2011) -- when in reality, it is v3.3. We must request a core
  // profile.
  //
  // Source: http://stackoverflow.com/questions/19865463/opengl-4-1-under-mavericks
  // SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  // Output the versions used of nomlib and its dependencies.
  libs_version_info();

  if( window.create("device_info", window_size, window_flags) == false ) {
    NOM_LOG_CRIT( NOM_LOG_CATEGORY_APPLICATION,
                  "Could not create a window." );
    exit(NOM_EXIT_FAILURE);
  }

  renderer_info = window.caps();

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                "Primary display name:", window.display_name() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION,
                "Refresh Rate:", window.refresh_rate() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Renderer: ", renderer_info.name() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "SDL_RENDERER_TARGETTEXTURE: ", renderer_info.target_texture() ? "YES" : "NO" );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "SDL_RENDERER_ACCELERATED: ", renderer_info.accelerated() ? "YES" : "NO" );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "SDL_RENDERER_PRESENTVSYNC: ", renderer_info.vsync() ? "YES" : "NO" );

  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Maximum texture width: ", renderer_info.texture_width() );
  NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, "Maximum texture height: ", renderer_info.texture_height() );

  nom::uint index = 0;
  for( auto itr = renderer_info.texture_formats.begin(); itr != renderer_info.texture_formats.end(); ++itr )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_APPLICATION, nom::PIXEL_FORMAT_NAME( *itr ), index == 0 ? " (optimal)" : "" );
    ++index;
  }

  std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

  return NOM_EXIT_SUCCESS;
}
