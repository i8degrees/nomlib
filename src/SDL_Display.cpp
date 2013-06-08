/******************************************************************************
    SDL_Display.cpp

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Display.h"

using namespace nom;

SDL_Display::SDL_Display ( void )
{
  uint32_t img_flags = IMG_INIT_PNG;

  #ifdef DEBUG_SDL_DISPLAY_OBJ
    std::cout << "nom::SDL_Display::SDL_Display(): " << "Hello world!" << std::endl << std::endl;
  #endif

  if ( SDL_Init ( SDL_INIT_VIDEO ) != 0 )
  {
    std::cout << "ERR in SDL_Display::SDL_Display() at SDL_Init(): " << SDL_GetError() << std::endl;
  }

  if ( IMG_Init ( img_flags ) != img_flags )
  {
    #ifdef DEBUG_SDL_DISPLAY_OBJ
      std::cout << "ERR in SDL_Display::SDL_Display() at IMG_Init(): " << IMG_GetError() << std::endl;
    #endif
  }
}

SDL_Display::~SDL_Display ( void )
{
  // As per docs, we must not free the publicly available surface, AKA
  // SDL_Surface *screen. This is explicitly stated as a role of the SDL_Quit()
  // function.
  //
  // http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode

  #ifdef DEBUG_SDL_DISPLAY_OBJ
    std::cout << "nom::SDL::Display::~Display(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  IMG_Quit ();
}

void SDL_Display::createWindow  ( int32_t display_width, int32_t display_height,
                                  int32_t display_colorbit, uint32_t flags )
{
  SDL_Surface *screen = SDL_SetVideoMode  ( display_width, display_height,
                                            display_colorbit, flags
                                          );

  if ( screen == NULL )
  {
    #ifdef DEBUG_SDL_DISPLAY
      std::cout << "ERR in SDL_Display::SDL_Display(): " << SDL_GetError() << std::endl;
    #endif
  }

  assert ( screen != NULL );
}

void* SDL_Display::get ( void ) const
{
  return dynamic_cast<SDL_Surface*>( SDL_GetVideoSurface() );
}

const int32_t SDL_Display::getDisplayWidth ( void ) const
{
  return SDL_GetVideoSurface()->w;
}

const int32_t SDL_Display::getDisplayHeight ( void ) const
{
  return SDL_GetVideoSurface()->h;
}

const int32_t SDL_Display::getDisplayColorBits ( void ) const
{
  return SDL_GetVideoSurface()->format->BitsPerPixel;
}

const uint32_t SDL_Display::getDisplayFlags ( void ) const
{
  return SDL_GetVideoSurface()->flags;
}

const uint16_t SDL_Display::getDisplayPitch ( void ) const
{
  return SDL_GetVideoSurface()->pitch;
}

const void* SDL_Display::getDisplayPixels ( void ) const
{
  return SDL_GetVideoSurface()->pixels;
}

void* SDL_Display::getDisplayPixelsFormat ( void ) const
{
  return SDL_GetVideoSurface()->format;
}

const Coords SDL_Display::getDisplayClip ( void ) const
{
  SDL_Rect clip = SDL_GetVideoSurface()->clip_rect;
  Coords clip_coords ( clip.x, clip.y, clip.w, clip.h );
  return clip_coords;
}

void SDL_Display::Update ( void )
{
  SDL_Surface *screen = (SDL_Surface*)this->get();

  if ( SDL_Flip ( screen ) != 0 )
  {
    #ifdef DEBUG_SDL_DISPLAY
      std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    #endif
  }
}

const void SDL_Display::toggleFullScreenWindow ( int32_t width, int32_t height ) const
{
  SDL_Surface *screen = (SDL_Surface*)this->get();
  uint32_t flags; // save our current flags before attempting to switch

  flags = screen->flags;

  screen = SDL_SetVideoMode ( width, height, 0, screen->flags ^ SDL_FULLSCREEN );

  //  If for whatever reason, we cannot toggle fullscreen, try reverting
  //  back to our previous configuration
  if ( screen == NULL )
  {
    screen = SDL_SetVideoMode ( width, height, 0, flags );
  }

  assert ( screen != NULL );  // something went terribly wrong here if we
                              // are still NULL here
}

// FIXME
const std::string SDL_Display::getWindowTitle ( void ) const
{
  char *window_title;
  SDL_WM_GetCaption ( &window_title, NULL );
  return std::to_string ( *window_title );
}

void* SDL_Display::getWindowIcon ( void ) const
{
  return NULL;
}

void SDL_Display::setWindowTitle ( const std::string& app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), NULL );
}

void SDL_Display::setWindowIcon ( const std::string& app_icon )
{
  SDL_Surface *icon_buffer = NULL;

  if ( this->get() != NULL )
  {
    std::cout << "ERR in Gfx::SetWindowIcon(): " << "SDL_SetVideoMode() has already been called." << std::endl;
  }

  icon_buffer = SDL_LoadBMP ( app_icon.c_str() );

  if ( icon_buffer == NULL )
  {
    std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
  }

  // TODO; flags, colorkey?
  if ( Gfx::setTransparent ( icon_buffer, Color ( 0, 0, 0 ) ) == false )
  {
    std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
  }

  SDL_WM_SetIcon ( icon_buffer, NULL );

  SDL_FreeSurface ( icon_buffer );
  icon_buffer = NULL;
}

