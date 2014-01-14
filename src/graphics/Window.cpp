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
#include "nomlib/graphics/Window.hpp"
//#include "nomlib/graphics/Renderer.hpp"

namespace nom {

// static initialization
SDL_RENDERER::RawPtr Window::context_ = nullptr;

Window::Window ( void ) : window_
    { SDL_WINDOW::UniquePtr ( nullptr, priv::FreeWindow ) },
    window_id_ ( 0 ), window_display_id_ ( -1 ),
    enabled_ ( false ), fullscreen_ ( false )
{
NOM_LOG_TRACE ( NOM );
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
  this->window_.reset ( SDL_CreateWindow  (
                                            window_title.c_str(),
                                            SDL_WINDOWPOS_UNDEFINED,
                                            SDL_WINDOWPOS_UNDEFINED,
                                            width,
                                            height,
                                            window_flags
                                          )
                      );

  if ( this->window_valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not create SDL window." );
    return false;
  }

  Renderer::create ( this->window(), -1, context_flags );
  if ( this->renderer_valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not create SDL renderer." );
    return false;
  }

  // Track our unique identifiers for our brand spanking new window!
  this->window_id_ = SDL_GetWindowID ( this->window() );
  this->window_display_id_ = SDL_GetWindowDisplayIndex ( this->window() );
  this->enabled_ = true;

  // We must always have an active rendering context before we do any rendering
  // calls; this includes even creating SDL textures!
  if ( context_ == nullptr )
  {
    this->make_current();
  }

  // Try to ensure that we have no leftover artifacts by clearing and filling
  // window with a solid black paint bucket fill.
  this->fill ( Color4i::Black );

  return true;
}

Window::RawPtr Window::get ( void )
{
  return this;
}

SDL_WINDOW::RawPtr Window::window ( void ) const
{
  return this->window_.get();
}

SDL_SURFACE::RawPtr Window::window_surface ( void ) const
{
  return SDL_GetWindowSurface ( this->window() );
}

bool Window::window_valid ( void ) const
{
  if ( this->window() != nullptr ) return true;

  return false;
}

Point2i Window::position ( void ) const
{
  Point2i pos;

  SDL_GetWindowPosition ( this->window(), &pos.x, &pos.y );

  return pos;
}

Point2i Window::size ( void ) const
{
  Point2i size;
  SDL_GetWindowSize ( this->window(), &size.x, &size.y );
  return size;
}

const uint8 Window::bits_per_pixel ( void ) const
{
  return SDL_BITSPERPIXEL ( this->pixel_format() );
}

uint32 Window::window_flags ( void ) const
{
  return SDL_GetWindowFlags ( this->window() );
}

uint32 Window::pixel_format ( void ) const
{
  return SDL_GetWindowPixelFormat ( this->window() );
}

const IntRect Window::display_bounds ( void ) const
{
  SDL_Rect display_bounds;
  IntRect bounds;

  if ( SDL_GetDisplayBounds ( this->window_display_id(), &display_bounds ) != 0 )
  {
    NOM_LOG_ERR ( NOM, SDL_GetError() );
    return IntRect::null;
  }

  bounds.x = display_bounds.x;
  bounds.y = display_bounds.y;
  bounds.w = display_bounds.w;
  bounds.h = display_bounds.h;

  return bounds;
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

bool Window::flip ( void ) const
{
  if ( SDL_UpdateWindowSurface ( this->window() ) != 0 )
  {
NOM_LOG_ERR ( NOM, SDL_GetError() );
    return false;
  }
  return true;
}

bool Window::fullscreen ( void ) const
{
  if ( this->fullscreen_ ) return true;

  return false;
}

void Window::set_fullscreen ( bool state )
{
  this->fullscreen_ = state;
}

bool Window::toggle_fullscreen ( void )
{
  if ( this->toggle_fullscreen ( SDL_WINDOW_FULLSCREEN_DESKTOP ) == false )
  {
    return false;
  }

  return true;
}

bool Window::toggle_fullscreen ( uint32 flags )
{
  if ( this->fullscreen() == true ) // Go back to window state
  {
    if ( SDL_SetWindowFullscreen ( this->window(), 0 ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }
    this->set_fullscreen ( false );
    return true;
  }

  if ( this->fullscreen() == false )
  {
    if ( SDL_SetWindowFullscreen ( this->window(), flags ) != 0 )
    {
      NOM_LOG_ERR ( NOM, SDL_GetError() );
      return false;
    }
    this->set_fullscreen ( true );
    return true;
  }

  return false; // We should *never* reach this statement
}

const std::string Window::window_title ( void ) const
{
  std::string title = SDL_GetWindowTitle ( this->window() );

  return title;
}

void Window::set_window_title ( const std::string& title )
{
  SDL_SetWindowTitle ( this->window(), title.c_str() );
}

bool Window::set_window_icon ( const std::string& filename )
{
  Image icon;

  if ( icon.load ( filename ) == false ) return false;

  if ( icon.valid() == false )
  {
NOM_LOG_ERR ( NOM, "Could not obtain a valid icon." );
    return false;
  }

  SDL_SetWindowIcon ( this->window(), icon.image() );

  return true;
}

void Window::set_size ( int32 width, int32 height )
{
  SDL_SetWindowSize ( this->window(), width, height );
}

void Window::set_position ( int32 x, int32 y )
{
  SDL_SetWindowPosition ( this->window(), x, y );
}

uint32 Window::window_id ( void ) const
{
  return this->window_id_;
}

SDL_Window* Window::window_id ( uint32 id )
{
  return SDL_GetWindowFromID ( id );
}

int Window::window_display_id ( void ) const
{
  return this->window_display_id_;
}

void Window::disable_screensaver ( void )
{
  SDL_DisableScreenSaver();
}

bool Window::screen_saver ( void )
{
  return SDL_IsScreenSaverEnabled();
}

void Window::maximize_window ( void )
{
  SDL_MaximizeWindow ( this->window() );
}

void Window::minimize_window ( void )
{
  SDL_MinimizeWindow ( this->window() );
}

void Window::raise_window ( void )
{
  SDL_RaiseWindow ( this->window() );
}

void Window::restore_window ( void )
{
  SDL_RestoreWindow ( this->window() );
}

void Window::show_window ( void )
{
  SDL_ShowWindow ( this->window() );
}

void Window::hide_window ( void )
{
  SDL_HideWindow ( this->window() );
}

void Window::set_window_grab ( bool grab )
{
  SDL_SetWindowGrab ( this->window(), SDL_BOOL(grab) );
}

void Window::set_minimum_window_size ( int min_width, int min_height )
{
  SDL_SetWindowMaximumSize ( this->window(), min_width, min_height );
}

void Window::set_maximum_window_size ( int max_width, int max_height )
{
  SDL_SetWindowMaximumSize ( this->window(), max_width, max_height );
}

bool Window::save_screenshot ( const std::string& filename ) const
{
  RendererInfo caps = this->caps();
  Image screenshot;
  File fp; // Using File::exists

  // We create a new file path deriving from the given filename string by
  // appending a unique identifier onto the end of the given filename string.
  std::string file_name, basename, prefix, timestamp, extension;

  Point2i renderer_size = Renderer::size(); // Width & height of target in pixels
  SDL_SURFACE::UniquePtr buffer ( nullptr, priv::FreeSurface );

  int bpp = 0; // bits per pixel
  uint32 red_mask = 0;
  uint32 green_mask = 0;
  uint32 blue_mask = 0;
  uint32 alpha_mask = 0;

  if ( SDL_BOOL( SDL_PixelFormatEnumToMasks ( caps.optimal_texture_format(), &bpp, &red_mask, &green_mask, &blue_mask, &alpha_mask ) ) != true )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
    return false;
  }

  screenshot.initialize( Renderer::pixels(), renderer_size.x, renderer_size.y, bpp, (renderer_size.x * 4), red_mask, green_mask, blue_mask, alpha_mask );

  // TODO: additional err checking -- basename & extension can fail!
  basename = fp.basename(filename);
  prefix = "_";
  timestamp = std::to_string( ticks() );
  extension = fp.extension(filename);
  file_name = basename + prefix + timestamp + "." + extension;

  if ( screenshot.save_png( file_name ) == false ) return false;

  // Success!
  NOM_LOG_INFO( NOM, "The screenshot file is saved at: " + file_name );

  return true;
}

void Window::make_current ( void )
{
  set_context ( this->get() );
}

SDL_RENDERER::RawPtr Window::context ( void )
{
  return context_;
}

void Window::set_context ( Window::RawPtr window )
{
  context_ = window->renderer();
}

} // namespace nom
