/******************************************************************************

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Display.hpp"

using namespace nom;

SDL_Display::SDL_Display ( void )
{
NOMLIB_LOG_INFO;

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
  }

  atexit ( SDL_Quit );
}

SDL_Display::~SDL_Display ( void )
{
  // As per docs, we must not free the publicly available surface, AKA
  // SDL_Surface *screen. This is explicitly stated as a role of the SDL_Quit()
  // function.
  //
  // http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode

NOMLIB_LOG_INFO;
}

void SDL_Display::createWindow  ( int32_t display_width, int32_t display_height,
                                  int32_t display_colorbit, uint32_t flags )
{
  void* screen = nullptr; // Better safe than sorry!

  screen = SDL_SetVideoMode ( display_width, display_height,
                              display_colorbit, flags
                            );

  if ( screen == nullptr )
NOMLIB_LOG_ERR ( SDL_GetError() );

NOMLIB_ASSERT ( screen != nullptr );
}

// This is important to return precisely as a SDL_Surface for it changes nullptr
// result potentially
void* SDL_Display::get ( void ) const
{
  return dynamic_cast<SDL_Surface*>( SDL_GetVideoSurface() );
}

bool nom::SDL_Display::valid ( void ) const
{
  if ( static_cast<SDL_Surface*> ( this->get() ) != nullptr )
    return true;
  else
    return false;
}

int32_t SDL_Display::getDisplayWidth ( void ) const
{
  return SDL_GetVideoSurface()->w;
}

int32_t SDL_Display::getDisplayHeight ( void ) const
{
  return SDL_GetVideoSurface()->h;
}

int32_t SDL_Display::getDisplayColorBits ( void ) const
{
  return SDL_GetVideoSurface()->format->BitsPerPixel;
}

uint32_t SDL_Display::getDisplayFlags ( void ) const
{
  return SDL_GetVideoSurface()->flags;
}

uint16_t SDL_Display::getDisplayPitch ( void ) const
{
  return SDL_GetVideoSurface()->pitch;
}

void* SDL_Display::getDisplayPixels ( void ) const
{
  return SDL_GetVideoSurface()->pixels;
}

void* SDL_Display::getDisplayPixelsFormat ( void ) const
{
  return SDL_GetVideoSurface()->format;
}

const nom::Coords SDL_Display::getDisplayBounds ( void ) const
{
  SDL_Rect clip = SDL_GetVideoSurface()->clip_rect;
  nom::Coords clip_coords ( clip.x, clip.y, clip.w, clip.h );
  return clip_coords;
}

bool nom::SDL_Display::getCanvasLock ( void ) const
{
  SDL_Surface* buffer = static_cast<SDL_Surface*> ( this->get() );
  return buffer->locked;
}

void nom::SDL_Display::Update ( void )
{
  if ( SDL_Flip ( static_cast<SDL_Surface*> ( this->get() ) ) != 0 )
NOMLIB_LOG_ERR ( SDL_GetError() );
}

void nom::SDL_Display::Update ( const nom::Coords& coords )
{
  SDL_UpdateRect ( static_cast<SDL_Surface*> ( this->get() ), coords.x, coords.y, coords.width, coords.height );
}

void SDL_Display::toggleFullScreenWindow ( int32_t width, int32_t height )
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
const std::string SDL_Display::getWindowTitle ( void ) const
{
  char *window_title;
  SDL_WM_GetCaption ( &window_title, nullptr );
  return std::to_string ( *window_title );
}

// TODO
void* SDL_Display::getWindowIcon ( void ) const
{
  return nullptr;
}

void SDL_Display::setWindowTitle ( const std::string& app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), nullptr );
}

void SDL_Display::setWindowIcon ( const std::string& app_icon )
{
  nom::SDL_Image image; // holds our image in memory during transfer
  std::shared_ptr<void> icon = nullptr;

  if ( this->valid() )
NOMLIB_LOG_ERR ( "SDL video subsystem has already been initiated." );

  icon = std::shared_ptr<void> ( image.loadFromFile ( app_icon ) );
  if ( icon == nullptr )
  {
NOMLIB_LOG_ERR ( "Could not load window icon file: " + app_icon );
    return;
  }

  SDL_WM_SetIcon ( static_cast<SDL_Surface*> ( icon.get() ) , nullptr );
}
