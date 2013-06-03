/******************************************************************************
    SDL_Display.cpp

    SDL Display Class Interface Definition

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDL_Display.h"

nom::SDL::Display::Display ( unsigned int flags )
{
  std::cout << "nom::SDL::Display::Display(): Hello, world!" << "\n" << std::endl;

  if ( SDL_Init ( flags ) != 0 )
  {
    std::cout << "ERR in nom::SDL::Display::Display() at SDL_Init(): " << SDL_GetError() << std::endl;

    exit ( EXIT_FAILURE );
  }
}

nom::SDL::Display::~Display ( void )
{
  // As per docs, we must not free the publicly available surface, AKA
  // SDL_Surface *screen. This is explicitly stated as a role of the SDL_Quit()
  // function.
  //
  // http://sdl.beuc.net/sdl.wiki/SDL_SetVideoMode

  std::cout << "nom::SDL::Display::~Display(): " << "Goodbye cruel world!" << "\n" << std::endl;

  SDL_Quit ();
}

bool nom::SDL::Display::CreateWindow (  unsigned int display_width,
                          unsigned int display_height,
                          unsigned int display_colorbit,
                          unsigned int flags
                        )
{
  SDL_Surface *screen = NULL;

  screen = SDL_SetVideoMode ( display_width, display_height, display_colorbit, flags );

  if ( screen == NULL )
  {
    std::cout << "ERR in Gfx::SetVideoMode(): " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

SDL_Surface* nom::SDL::Display::getDisplay ( void )
{
  return SDL_GetVideoSurface();
}

const unsigned int nom::SDL::Display::getDisplayWidth ( void ) const
{
  SDL_Surface *screen = NULL;
  screen = SDL_GetVideoSurface();
  return screen->w;
}

const unsigned int nom::SDL::Display::getDisplayHeight ( void ) const
{
  SDL_Surface *screen = NULL;
  screen = SDL_GetVideoSurface();
  return screen->h;
}

const unsigned int nom::SDL::Display::getDisplayColorBits ( void ) const
{
  SDL_Surface *screen = NULL;

  screen = SDL_GetVideoSurface();

  return screen->format->BitsPerPixel;
}

const unsigned int nom::SDL::Display::getDisplayFlags ( void ) const
{
  SDL_Surface *screen = NULL;

  screen = SDL_GetVideoSurface();

  return screen->flags;
}

bool nom::SDL::Display::updateDisplay ( void )
{
  SDL_Surface *screen = NULL;

  screen = this->getDisplay();

  if ( SDL_Flip ( screen ) != 0 )
  {
    std::cout << "ERR in Gfx::UpdateScreen(): " << SDL_GetError() << std::endl;
    return false;
  }

  return true;
}

void nom::SDL::Display::setTitle ( const std::string& app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), NULL );
}

bool nom::SDL::Display::setIcon ( const std::string& app_icon )
{
  SDL_Surface *icon_buffer = NULL;

  if ( this->getDisplay() != NULL )
  {
    std::cout << "ERR in Gfx::SetWindowIcon(): " << "SDL_SetVideoMode() has already been called." << std::endl;
    return false;
  }

  icon_buffer = SDL_LoadBMP ( app_icon.c_str() );

  if ( icon_buffer == NULL )
  {
    std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
    return false;
  }

  // TODO; flags, colorkey?
  if ( Gfx::setTransparent ( icon_buffer, Color ( 0, 0, 0 ) ) == false )
  {
    std::cout << "ERR in Gfx::SetWindowIcon(): " << SDL_GetError() << std::endl;
    return false;
  }

  SDL_WM_SetIcon ( icon_buffer, NULL );

  SDL_FreeSurface ( icon_buffer );
  icon_buffer = NULL;

  return true;
}

