/******************************************************************************
    GameState.h

  Game State Management

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston

******************************************************************************/
#ifndef GAMEAPP_GAMESTATE_HEADERS
#define GAMEAPP_GAMESTATE_HEADERS

#include <iostream>
#include <string>

#include "SDL.h"

#include "SDLInput.h"

#include "gamelib.h"

class GameState: public nom::SDLInput
{
  public:
    virtual void Pause ( void ) = 0;
    virtual void Resume ( void ) = 0;

    virtual void HandleInput ( void ) = 0;
    virtual void Update ( void ) = 0;
    virtual void Draw ( void ) = 0;

  private:
    // ...
};

#endif // GAMEAPP_GAMESTATE_HEADERS defined
