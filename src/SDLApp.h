/******************************************************************************
    SDLApp.h

    SDL Application Framework

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_SDLAPP_HEADERS
#define GAMELIB_SDLAPP_HEADERS

#include <iostream>
#include <string>

#include "SDL_image.h"

#include "gamelib.h"

class SDLApp
{
  public:
    SDLApp ( void );
    ~SDLApp ( void );

    static bool Init    (   unsigned int sdl_flags = SDL_INIT_EVERYTHING,
                            unsigned int img_flags = IMG_INIT_PNG
                        );

    //static bool isRunning ( void );
    //static void setRunning ( bool app_state );

    // Requires SDL Video Initialization to be called first (OSX specific?)
    static void setTitle ( std::string app_name );

    // Must be called before SDL Video Init
    static bool setIcon (   std::string app_icon,
                            unsigned int r = 0, unsigned int g = 0, unsigned int b = 0,
                            unsigned int flags = SDL_RLEACCEL | SDL_SRCCOLORKEY );



  private:
    //static bool state;
};

#endif // GAMELIB_SDLAPP_HEADERS defined
