/******************************************************************************
    input.h

    Input Events Handling

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_INPUT_HEADERS
#define GAMELIB_INPUT_HEADERS

#include <iostream>
#include <string>

#include "SDL/SDL.h"

class SDLInput
{
  public:
    SDLInput ( void );
    ~SDLInput ( void );

  private:
    SDL_Event input; // events struct for input subsystem
};

#endif // GAMELIB_INPUT_HEADERS defined
