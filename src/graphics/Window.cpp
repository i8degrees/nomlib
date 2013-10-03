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
#include "nomlib/graphics/Window.hpp"

namespace nom {

std::shared_ptr<SDL_Renderer> context = nullptr;

Window::Window ( void )
{
NOM_LOG_TRACE ( NOM );

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
  }

  atexit ( SDL_Quit );
}

Window::~Window ( void )
{
NOM_LOG_TRACE ( NOM );
}

bool Window::create (
                      const std::string& window_title, int32 width, int32 height,
                      uint32 window_flags, uint32 context_flags
                    )
{
  this->window = std::shared_ptr<SDL_Window>  (
                                                SDL_CreateWindow
                                                (
                                                  window_title.c_str(),
                                                  SDL_WINDOWPOS_CENTERED,
                                                  SDL_WINDOWPOS_CENTERED,
                                                  width,
                                                  height,
                                                  window_flags
                                                ), priv::FreeWindow
                                              );

  if ( this->valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not create SDL window." );
    return false;
  }

  context = std::shared_ptr<SDL_Renderer> ( SDL_CreateRenderer
                                            (
                                              this->window.get(),
                                              -1,
                                              context_flags
                                            ),
                                            priv::FreeRenderTarget
                                          );
  if ( context == nullptr )
  {
NOM_LOG_ERR ( NOM, "Could not create rendering context." );
    return false;
  }

  return true;
}

SDL_Window* Window::get ( void ) const
{
  return this->window.get();
}

bool Window::valid ( void ) const
{
  if ( this->get() != nullptr )
  {
    return true;
  }
  else
  {
    return false;
  }
}

Point2i Window::getPosition ( void ) const
{
  Point2i pos;

  SDL_GetWindowPosition ( this->window.get(), &pos.x, &pos.y );

  return pos;
}

const uint8 Window::getDisplayColorBits ( void ) const
{
/*
  SDL_Surface* screen = this->get();

  // We prevent a segmentation fault here by providing a means of accessing the
  // video modes without already having initialized the video display via
  // SDL_SetVideoMode.
  if ( screen == nullptr )
  {
    //const SDL_VideoInfo* bpp = SDL_GetVideoInfo();
    //return bpp->vfmt->BitsPerPixel;
  }

  return screen->format->BitsPerPixel;
*/
    return 0;
}

uint32 Window::getDisplayFlags ( void ) const
{
  return SDL_GetWindowFlags ( this->get() );
}

uint16 Window::getDisplayPitch ( void ) const
{
  //return SDL_GetVideoSurface()->pitch;
    return 0;
}

void* Window::getDisplayPixels ( void ) const
{
  //return SDL_GetVideoSurface()->pixels;
    return 0;
}

SDL_PixelFormat* Window::getDisplayPixelsFormat ( void ) const
{
  //return SDL_GetVideoSurface()->format;
    return nullptr;
}

const Coords Window::getDisplayBounds ( void ) const
{
  //SDL_Rect clip = SDL_GetVideoSurface()->clip_rect;
  //Coords clip_coords ( clip.x, clip.y, clip.w, clip.h );
  //return clip_coords;
    return Coords(0, 0);
}

VideoModeList Window::getVideoModes ( void ) const
{
/*
  VideoModeList modes;
  SDL_Rect** mode;

  mode = SDL_ListModes ( nullptr, SDL_FULLSCREEN );

  if ( mode == nullptr )
  {
NOM_LOG_INFO ( NOM, "Any video mode is supported." ); // FIXME?
    return modes;
  }
  else if ( mode == ( SDL_Rect**) - 1 )
  {
NOM_LOG_INFO ( NOM, "No video modes are supported." );
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
*/
    return VideoModeList();
}
/*
bool Window::getCanvasLock ( void ) const
{
  //return this->get()->locked;
    return false;
}
*/
/*
bool Window::mustLock ( void ) const
{
  if ( SDL_MUSTLOCK ( this->get() ) )
  {
    return true;
  }
  else
  {
    return false;
  }
    return false;
}
*/
/*
bool Window::lock ( void ) const
{
  if ( this->mustLock() == true )
  {
    if ( SDL_LockSurface ( this->get() ) == -1 )
    {
NOM_LOG_ERR ( NOM, "Could not lock video surface memory." );
      return false;
    }
  }
  return true;
}
*/
/*
void Window::unlock ( void ) const
{
  SDL_UnlockSurface ( this->get() );
}
*/
bool Window::flip ( void ) const
{
  if ( SDL_UpdateWindowSurface ( this->get() ) != 0 )
  {
NOM_LOG_ERR ( NOM, "Could not update window surface: " + std::string ( SDL_GetError() ) );
    return false;
  }
  return true;
}

bool Window::toggleFullScreen ( uint32 flags )
{
  Point2i pos;

  pos = this->getPosition();

  if ( SDL_SetWindowFullscreen ( this->get(), flags ) != 0 )
  {
    NOM_LOG_ERR ( NOM, "Could not toggle SDL fullscreen mode." );
    return false;
  }

  // We must reset the window position back to the center of the display if we
  // initialize in windowed state, otherwise upon exiting full-screen, we end
  // up with a misaligned window.
  //
  // Possible bug on OS X
  if ( pos.x == SDL_WINDOWPOS_CENTERED && pos.y == SDL_WINDOWPOS_CENTERED )
  {
    this->setPosition ( SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED );
  }

  return true;
/*
  uint32 flags = 0; // save our current flags before attempting to switch

  flags = this->getDisplayFlags();

  this->createWindow ( width, height, 0, flags ^ SDL_FULLSCREEN );

  //  If for whatever reason, we cannot toggle fullscreen, try reverting
  //  back to our previous configuration
  if ( ! this->valid() )
    this->createWindow ( width, height, 0, flags );
*/
}

const std::string Window::getWindowTitle ( void ) const
{
  std::string title = SDL_GetWindowTitle ( this->get() );

  return title;
}

void Window::setWindowTitle ( const std::string& title )
{
  SDL_SetWindowTitle ( this->get(), title.c_str() );
}

bool Window::setWindowIcon ( const std::string& filename )
{
  //Image image; // holds our image in memory during transfer
  std::shared_ptr<SDL_Surface> icon;

// FIXME
// (Windows does not like using IMG_Load (SDL2_image extension) for some
// reason, which limits us solely to BMP (Windows Bitmap) files, which
// arguably is inconvenient ;-P I think I just need to take another look at
// the SDL documentation to see if this is a known limitation of their icon
// loader on Windows platform.
//
// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-01
#if defined ( NOM_PLATFORM_WINDOWS )
  icon.reset ( SDL_LoadBMP ( filename.c_str() ), priv::FreeSurface );
#else // assume POSIX platform
  icon.reset ( IMG_Load ( filename.c_str() ), priv::FreeSurface );
#endif

  if ( icon == nullptr )
  {
    NOM_LOG_ERR ( NOM, "Could not set application icon." );
    return false;
  }

  SDL_SetWindowIcon ( this->get(), icon.get() );

  return true;
}

void Window::setPosition ( int32 x, int32 y )
{
  SDL_SetWindowPosition ( this->get(), x, y );
}

} // namespace nom
