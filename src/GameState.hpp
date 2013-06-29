/******************************************************************************
    GameState.hpp

  Game State Management

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_GAMESTATE_HEADERS
#define NOMLIB_GAMESTATE_HEADERS

#include <iostream>

#include "SDL_Input.hpp"
#include "gamelib.h"

namespace nom
{
  class GameState: public nom::SDL_Input // "has a" relationship
  {
    public:
      virtual ~GameState()
      {
        #ifdef DEBUG_GAMESTATE_OBJ
          std::cout << "GameState::~GameState (): " << "Goodbye cruel world!" << std::endl << std::endl;
        #endif
      }

      virtual void Load ( void ) = 0;
      virtual void onClose ( void ) = 0;

      virtual void Pause ( void ) = 0;
      virtual void Resume ( void ) = 0;

      virtual void Update ( uint32_t elapsed_time ) = 0;
      virtual void Draw ( void* ) = 0;

    private:
      // ...
  };
}

#endif // NOMLIB_GAMESTATE_HEADERS defined
