/******************************************************************************
    SDLApp.cpp

    SDL Application Framework

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#include "SDLApp.h"

SDLApp::SDLApp ( void )
{
  #ifdef DEBUG_SDLAPP_OBJ
    std::cout << "SDLApp::SDLApp(): Hello, world!" << "\n" << std::endl;
  #endif
}

SDLApp::~SDLApp ( void )
{
  #ifdef DEBUG_SDLAPP_OBJ
    std::cout << "SDLApp::~SDLApp(): " << "Goodbye cruel world!" << "\n" << std::endl;
  #endif

  IMG_Quit ();

  SDL_Quit ();
}

bool SDLApp::Init ( unsigned int sdl_flags, unsigned int img_flags )
{
  #ifdef DEBUG_SDLAPP_OBJ
    std::cout << "SDLApp::SDLApp(): Hello, world!" << "\n" << std::endl;
  #endif

  if ( SDL_Init ( sdl_flags ) != 0 )
  {
    #ifdef DEBUG_SDLAPP_OBJ
      std::cout << "ERR in SDLApp::SDLApp() at SDL_Init(): " << SDL_GetError() << std::endl;
    #endif

    return false;
  }

  if ( IMG_Init ( img_flags ) != img_flags )
  {
    #ifdef DEBUG_SDLAPP_OBJ
      std::cout << "ERR in SDLApp::SDLApp() at IMG_Init(): " << IMG_GetError() << std::endl;
    #endif

    return false;
  }

  return true;
}

void SDLApp::setTitle ( std::string app_name )
{
  SDL_WM_SetCaption ( app_name.c_str(), app_name.c_str() );
}

// NOTE: *MUST* be called before the first call to SDL_SetVideoMode is made
bool SDLApp::setIcon (  std::string app_icon,
                        unsigned int r, unsigned int g, unsigned int b,
                        unsigned int flags )
{
  SDL_Surface *icon_buffer = NULL;

  icon_buffer = SDL_LoadBMP ( app_icon.c_str() );

  if ( icon_buffer == NULL )
  {
    #ifdef DEBUG_SDLAPP
      std::cout << "ERR in SDLApp::setIcon(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  if ( Gfx::setTransparent ( icon_buffer, r, g, b, flags ) == false )
  {
    #ifdef DEBUG_SDLAPP
      std::cout << "ERR in SDLApp::setIcon(): " << SDL_GetError() << std::endl;
    #endif
    return false;
  }

  SDL_WM_SetIcon ( icon_buffer, NULL );

  SDL_FreeSurface ( icon_buffer );
  icon_buffer = NULL;

  return true;
}
