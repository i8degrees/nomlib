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
#include "nomlib/system/init.hpp"

// Private headers (third-party)
#include "SDL.h"
#include "SDL_image.h"
#include "SDL_ttf.h"

// Private headers
#include "nomlib/system/IObject.hpp"
#include "nomlib/system/File.hpp"

namespace nom {

// Static initialization
std::shared_ptr<FontCache> SystemFonts::cache_ = std::shared_ptr<FontCache>( nullptr );
bool SystemFonts::initialized_ = false;

bool SystemFonts::initialized( void )
{
  return SystemFonts::initialized_;
}

void SystemFonts::initialize( void )
{
  // Ensure one-time only initialization
  if( SystemFonts::initialized() == false )
  {
    SystemFonts::cache_ = std::make_shared<FontCache>( FontCache() );
    SystemFonts::initialized_ = true;
  }
}

FontCache& SystemFonts::cache( void )
{
  if( SystemFonts::initialized() == false )
  {
    NOM_LOG_ERR( NOM, "System fonts cache was not yet initialized. Initializing..." );
    SystemFonts::initialize();
  }

  return *SystemFonts::cache_;
}

void SystemFonts::shutdown( void )
{
  // Clear the global fonts cache; note how this is very similar to how we use
  // to have to deal with nom::TrueTypeFont destruction.
  SystemFonts::cache_.reset();  // NULL

  SystemFonts::initialized_ = false;
}

bool init_third_party ( uint32 flags )
{
  if ( IMG_Init ( IMG_INIT_PNG ) != IMG_INIT_PNG )
  {
    NOM_LOG_ERR ( NOM, IMG_GetError() );
    return false;
  }

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
  }

  // We must initialize SDL2_ttf on every instance in order to shutdown properly
  // without crashing when ran within nom::GameStates
  if ( TTF_Init () != 0 )
  {
    NOM_LOG_ERR(NOM, TTF_GetError());
  }

  return true;
}

bool init ( int argc, char* argv[] )
{
  NOM_LOG_TRACE( NOM );

  nom::File dir;
  std::string pwd = "\0";

  pwd = dir.path ( argv[0] );

  if ( pwd.length() < 1 ) return false;

  if ( dir.set_path ( pwd ) == false ) return false;

  if ( init_third_party(0) == false ) return false;

  // Create a resource cache for letting either SDLApp or the end-user add
  // fonts for ease of access.
  // SystemFonts::initialize();

  return true;
}

void quit( void )
{
  NOM_LOG_TRACE( NOM );

  // FontCache* fonts = SystemFonts::cache();

  // We must clear the cache of nom::Font objects before shutting down SDL2_ttf,
  // because otherwise we'll be freeing memory that we do not own (FreeType
  // engine within is the rightful owner).
  SystemFonts::shutdown();

  // if( fonts != nullptr )
  // {
    // delete fonts;
    // fonts = nullptr;
  // }

  TTF_Quit();
  IMG_Quit();

  if( SDL_WasInit( SDL_INIT_JOYSTICK ) )
  {
    SDL_QuitSubSystem( SDL_INIT_JOYSTICK );
  }

  SDL_Quit();

  // NOM_LOG_INFO( NOM, "Total memory allocation (in bytes): " + std::to_string( IObject::total_alloc_bytes ) );
  // NOM_LOG_INFO( NOM, "Total memory deallocation (in bytes): " + std::to_string( IObject::total_dealloc_bytes ) );
}

} // namespace nom
