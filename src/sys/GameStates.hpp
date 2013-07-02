/******************************************************************************
    GameStates.hpp

  (FSM) States management

  Copyright (c) 2013 Jeffrey Carpenter

  Portions Copyright (c) 2013 Fielding Johnston
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_GAMESTATES_HEADERS
#define NOMLIB_GAMESTATES_HEADERS

#include <iostream>
#include <vector>
#include <memory>
#include <cassert>

#include "SDL.h"
#include "IState.hpp" // abstract class interface for our implementation
#include "nomlib.hpp"

namespace nom
{
  static std::vector<std::unique_ptr<nom::IState>> states;

  // Rename to FSM, States or similar?
  class GameStates
  {
    public:
      /// State events handling
      static void onEvent ( SDL_Event *event );

      /// State logic
      static void Update ( float delta_time );

      /// State rendering
      static void Draw ( void* video_buffer );

      /// State management
      static void ChangeState ( std::unique_ptr<IState> state );
      static void PushState ( std::unique_ptr<IState> state );
      static void PopState ( void );
      static void PopStateThenChangeState ( std::unique_ptr<IState> state );

    private:
      GameStates ( void );
  };
}

#endif // NOMLIB_GAMESTATES_HEADERS defined
