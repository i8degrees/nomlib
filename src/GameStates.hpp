/******************************************************************************
    GameStates.hpp

    Game states interface class

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_GAMESTATES_HEADERS
#define NOMLIB_GAMESTATES_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "SDL.h"
#include "SDL_Input.hpp"
#include "GameState.hpp" // abstract class interface for our implementation
#include "gamelib.h"

namespace nom
{
  static std::vector<std::unique_ptr<nom::GameState>> states;

  class GameStates
  {
    public:
      static void onEvent ( SDL_Event *event ); /// Game state events handler

      /// Pause method is implemented in each GameState derived class
      /// Resume method is implemented in each GameState derived class
      static void Update ( void* video_buffer );
      static void Draw ( void* video_buffer );

      static void ChangeState ( std::unique_ptr<GameState> state );
      static void PushState ( std::unique_ptr<GameState> state );
      static void PopState ( void );
      static void PopStateThenChangeState ( std::unique_ptr<GameState> state );
    private:
      GameStates ( void );
  };
}

#endif // NOMLIB_GAMESTATES_HEADERS defined
