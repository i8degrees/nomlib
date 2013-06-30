/******************************************************************************
    GameStates.hpp

  States management

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

  class GameStates
  {
    public:
      static void onEvent ( SDL_Event *event ); /// Game state events handler

      /// (experimental) onLoad method is implemented in each IState derived class
      /// (experimental) onClose method is implemented in each IState derived class
      /// Pause method is implemented in each IState derived class
      /// Resume method is implemented in each IState derived class
      static void Update ( float elapsed_time );
      static void Draw ( void* video_buffer );

      static void ChangeState ( std::unique_ptr<IState> state );
      static void PushState ( std::unique_ptr<IState> state );
      static void PopState ( void );
      static void PopStateThenChangeState ( std::unique_ptr<IState> state );
      //std::vector<std::string>& Peek ( void ) const;

    private:
      GameStates ( void );
  };
}

#endif // NOMLIB_GAMESTATES_HEADERS defined
