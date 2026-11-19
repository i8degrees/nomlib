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
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

// Private headers
#include "nomlib/core/IObject.hpp"
#include "nomlib/system/File.hpp"

// Private headers (SystemColors)
#include "nomlib/core/unique_ptr.hpp"

// Forward declarations (SystemColors)
#include "nomlib/system/ColorDatabase.hpp"

namespace nom {

// Static initialization
std::unique_ptr<ColorDatabase> SystemColors::colors_ = std::unique_ptr<ColorDatabase>( nullptr );
bool SystemColors::initialized_ = false;

bool SystemColors::initialized( void )
{
  return SystemColors::initialized_;
}

void SystemColors::initialize( void )
{
  // Ensure one-time only initialization
  if( SystemColors::initialized() == false )
  {
    SystemColors::colors_ = nom::make_unique<ColorDatabase>( ColorDatabase() );

    SystemColors::colors_->append_color( "Black", Color4i( 0, 0, 0, 255 ) );
    SystemColors::colors_->append_color( "White", Color4i( 255, 255, 255, 255 ) );
    SystemColors::colors_->append_color( "Red", Color4i( 255, 0, 0, 255 ) );
    SystemColors::colors_->append_color( "Green", Color4i( 0, 255, 0, 255 ) );
    SystemColors::colors_->append_color( "Blue", Color4i( 0, 0, 255, 255 ) );
    SystemColors::colors_->append_color( "Yellow", Color4i( 255, 255, 0, 255 ) );
    SystemColors::colors_->append_color( "Magenta", Color4i( 255, 0, 255, 255 ) );
    SystemColors::colors_->append_color( "Cyan", Color4i( 0, 255, 255, 255 ) );
    SystemColors::colors_->append_color( "Silver", Color4i( 192, 192, 192, 255 ) );
    SystemColors::colors_->append_color( "Purple", Color4i( 128, 0, 128, 255 ) );
    SystemColors::colors_->append_color( "Orange", Color4i( 255, 165, 0, 255 ) );
    SystemColors::colors_->append_color( "LightGray", Color4i( 99, 99, 99, 255 ) );
    SystemColors::colors_->append_color( "Gray", Color4i( 67, 67, 67, 255 ) );
    SystemColors::colors_->append_color( "SkyBlue", Color4i( 110, 144, 190, 255 ) );

    // The default wxWidgets theme colors used on Mac OS X
    SystemColors::colors_->append_color( "WindowBackground", Color4i( 237, 237, 237, 255 ) );
    SystemColors::colors_->append_color( "FocusedWindowTitleBackground", Color4i( 213, 213, 213, 255 ) );

    SystemColors::colors_->append_color( "Tooltip", Color4i( 245, 245, 195, 255 ) );
    SystemColors::colors_->append_color( "TooltipBorder", Color4i( 202, 202, 202, 255 ) );

    SystemColors::colors_->append_color( "ListControlBackground", Color4i( 255, 255, 255, 255 ) );
    SystemColors::colors_->append_color( "ListControlTextSelection", Color4i( 212, 212, 212, 255 ) );
    SystemColors::colors_->append_color( "ListControlRowBackground", Color4i( 255, 255, 255, 255 ) );
    SystemColors::colors_->append_color( "ListControlRowAlternativeBackground", Color4i( 243, 245, 250, 255 ) );

    SystemColors::colors_->append_color( "ScrollBarBackground", Color4i( 194, 194, 194, 255 ) );

    // OS X Aqua color scheme
    SystemColors::colors_->append_color( "WidgetFocusForeground", Color4i( 128, 189, 245, 255 ) );
    SystemColors::colors_->append_color( "WidgetFocusBackground", Color4i( 88, 94, 175, 255 ) );

    // Final Fantasy VIII "TTcards" theme style
    SystemColors::colors_->append_color( "FinalFantasyOuterBorder", Color4i( 41, 41, 41, 255 ) );
    SystemColors::colors_->append_color( "FinalFantasyInnerBorder", Color4i( 133, 133, 133, 255 ) );

    SystemColors::initialized_ = true;
  }
}

ColorDatabase& SystemColors::colors( void )
{
  if( SystemColors::initialized() == false )
  {
    NOM_LOG_INFO( NOM_LOG_CATEGORY_SYSTEM, "Color database was not yet initialized. Initializing..." );
    SystemColors::initialize();
  }

  return *SystemColors::colors_;
}

void SystemColors::shutdown( void )
{
  SystemColors::colors_.reset();  // NULL

  SystemColors::initialized_ = false;
}

bool init_third_party( uint32 flags )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  if( flags == InitHints::DefaultInit )
  {
    flags = InitHints::SDL2_IMAGE | InitHints::SDL2 | InitHints::SDL2_TTF;
  }

  if( flags & InitHints::SDL2_IMAGE )
  {
    if ( IMG_Init ( IMG_INIT_PNG ) != IMG_INIT_PNG )
    {
      NOM_LOG_ERR ( NOM, IMG_GetError() );
      return false;
    }
  }

  if( flags & InitHints::SDL2 )
  {
    if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }
  }

  if( flags & InitHints::SDL2_TTF )
  {
    // We must initialize SDL2_ttf on every instance in order to shutdown properly
    // without crashing when ran within nom::GameStates
    if ( TTF_Init () != 0 )
    {
      NOM_LOG_ERR(NOM, TTF_GetError() );
      return false;
    }
  }

  return true;
}

bool init ( int argc, char* argv[] )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  nom::File dir;
  std::string pwd = "\0";

  // Note that this will always be a POSIX path -- Unix path delimiters -- even
  // on Windows
  pwd = dir.path( argv[0] );

  if( pwd.length() < 1 )
  {
    NOM_LOG_ERR( NOM, "Could not get program's executable working path (pwd is NULL)." );
    return false;
  }

  if( dir.set_path ( pwd ) == false )
  {
    NOM_LOG_ERR( NOM, "Could not set working directory path to: " + pwd );
    return false;
  }

  if( init_third_party( 0 ) == false )
  {
    NOM_LOG_ERR( NOM, "Could not initialize third party libraries." );
    return false;
  }

  return true;
}

void quit( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_SYSTEM );

  TTF_Quit();
  IMG_Quit();

  SDL_Quit();

  NOM_LOG_DEBUG( NOM_LOG_CATEGORY_MEMORY_TOTALS, "Total memory allocation (in bytes): ", IObject::total_alloc_bytes );
  NOM_LOG_DEBUG( NOM_LOG_CATEGORY_MEMORY_TOTALS, "Total memory deallocation (in bytes): ", IObject::total_dealloc_bytes );
}

} // namespace nom
