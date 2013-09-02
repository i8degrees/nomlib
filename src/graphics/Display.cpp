/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/graphics/Display.hpp"

namespace nom {
  namespace priv {

void Display_FreeSurface ( SDL_Surface* video_buffer )
{
  // As per docs, we must not free the publicly available surface, AKA
  // SDL_Surface *screen. This is explicitly stated as a role of the SDL_Quit()
  // function.
  //
  // http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode
}

  } // namespace priv
} // namespace nom


namespace nom {

Display::Display ( void )
{
NOM_LOG_TRACE;

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
NOM_LOG_ERR ( SDL_GetError() );
  }

  atexit ( SDL_Quit );
}

Display::~Display ( void )
{
NOM_LOG_TRACE;
}

void Display::createWindow  ( int32_t display_width, int32_t display_height,
                                  int32_t display_colorbit, uint32_t flags )
{
  void* screen = nullptr; // Better safe than sorry!

  screen = SDL_SetVideoMode ( display_width, display_height,
                              display_colorbit, flags
                            );

  if ( screen == nullptr )
NOM_LOG_ERR ( SDL_GetError() );

NOM_ASSERT ( screen != nullptr );
}

void* Display::get ( void ) const
{
  return SDL_GetVideoSurface();
}

bool Display::valid ( void ) const
{
  if ( static_cast<SDL_Surface*> ( this->get() ) != nullptr )
    return true;
  else
    return false;
}

int32_t Display::getDisplayWidth ( void ) const
{
  return SDL_GetVideoSurface()->w;
}

int32_t Display::getDisplayHeight ( void ) const
{
  return SDL_GetVideoSurface()->h;
}

const uint8 Display::getDisplayColorBits ( void ) const
{
  SDL_Surface* screen = static_cast<SDL_Surface*> ( this->get() );

  // We prevent a segmentation fault here by providing a means of accessing the
  // video modes without already having initialized the video display via
  // SDL_SetVideoMode.
  if ( screen == nullptr )
  {
    const SDL_VideoInfo* bpp = SDL_GetVideoInfo();
    return bpp->vfmt->BitsPerPixel;
  }

  return screen->format->BitsPerPixel;
}

uint32_t Display::getDisplayFlags ( void ) const
{
  return SDL_GetVideoSurface()->flags;
}

uint16_t Display::getDisplayPitch ( void ) const
{
  return SDL_GetVideoSurface()->pitch;
}

void* Display::getDisplayPixels ( void ) const
{
  return SDL_GetVideoSurface()->pixels;
}

void* Display::getDisplayPixelsFormat ( void ) const
{
  return SDL_GetVideoSurface()->format;
}

const Coords Display::getDisplayBounds ( void ) const
{
  SDL_Rect clip = SDL_GetVideoSurface()->clip_rect;
  Coords clip_coords ( clip.x, clip.y, clip.w, clip.h );
  return clip_coords;
}

VideoModeList Display::getVideoModes ( void ) const
{
  VideoModeList modes;
  SDL_Rect** mode;

  mode = SDL_ListModes ( nullptr, SDL_FULLSCREEN );

  if ( mode == nullptr )
  {
NOM_LOG_INFO ( "Any video mode is supported." ); // FIXME?
    return modes;
  }
  else if ( mode == ( SDL_Rect**) - 1 )
  {
NOM_LOG_INFO ( "No video modes are supported." );
    return modes;
  }
  else
  {
    for ( int32 idx = 0; mode[idx]; idx++ )
    {
      modes.push_back ( VideoMode ( mode[idx]->w, mode[idx]->h, this->getDisplayColorBits() ) );
    }

    std::sort ( modes.begin(), modes.end(), std::greater<VideoMode>()  );
  }
  return modes;
}

bool Display::getCanvasLock ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->get() );
  return buffer->locked;
}

bool Display::mustLock ( void ) const
{
  if ( SDL_MUSTLOCK ( static_cast<SDL_Surface*> ( this->get() ) ) )
    return true;
  else
    return false;
}

bool Display::lock ( void ) const
{
  if ( this->mustLock() == true )
  {
    if ( SDL_LockSurface ( static_cast<SDL_Surface*> ( this->get() ) ) == -1 )
    {
NOM_LOG_ERR ( "Could not lock video surface memory." );
      return false;
    }
  }
  return true;
}

void Display::unlock ( void ) const
{
  SDL_UnlockSurface ( static_cast<SDL_Surface*> ( this->get() ) );
}

void Display::Update ( void )
{
  if ( SDL_Flip ( static_cast<SDL_Surface*> ( this->get() ) ) != 0 )
NOM_LOG_ERR ( SDL_GetError() );
}

void Display::Update ( const Coords& coords )
{
  SDL_UpdateRect ( static_cast<SDL_Surface*> ( this->get() ), coords.x, coords.y, coords.width, coords.height );
}

void Display::toggleFullScreenWindow ( int32_t width, int32_t height )
{
  uint32_t flags = 0; // save our current flags before attempting to switch

  flags = this->getDisplayFlags();

  this->createWindow ( width, height, 0, flags ^ SDL_FULLSCREEN );

  //  If for whatever reason, we cannot toggle fullscreen, try reverting
  //  back to our previous configuration
  if ( ! this->valid() )
    this->createWindow ( width, height, 0, flags );
}

// FIXME
const std::string Display::getWindowTitle ( void ) const
{
  char *window_title;
  SDL_WM_GetCaption ( &window_title, nullptr );
  return std::to_string ( *window_title );
}

// TODO
void* Display::getWindowIcon ( void ) const
{
  return nullptr;
}

void Display::setWindowTitle ( const std::string& app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), nullptr );
}

void Display::setWindowIcon ( const std::string& app_icon )
{
  Image image; // holds our image in memory during transfer
  std::shared_ptr<void> icon = nullptr;

  if ( this->valid() )
NOM_LOG_ERR ( "SDL video subsystem has already been initiated." );

  icon = std::shared_ptr<void> ( image.load ( app_icon ) );
  if ( icon == nullptr )
  {
NOM_LOG_ERR ( "Could not load window icon file: " + app_icon );
    return;
  }

  SDL_WM_SetIcon ( static_cast<SDL_Surface*> ( icon.get() ) , nullptr );
}


} // namespace nom
