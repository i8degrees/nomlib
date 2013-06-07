/******************************************************************************
    GameApp.hpp

    Game app interface class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_GAMEAPP_HEADERS
#define NOMLIB_GAMEAPP_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <memory>

//#include "SDLInput.h"

#include "gamelib.h"

#include "timer.h"

namespace nom
{
  class GameApp
  {
    public:
      GameApp ( void );
      ~GameApp ( void );

      bool isRunning ( void );
      void Run ( void );
      void Quit ( void );

      uint32_t getTicks ( void );

      bool isFullScreen ( void );
      void setFullScreen ( bool toggle );
      bool getShowFPS ( void );
      void showFPS ( bool toggle );
      void toggleFPS ( void );

    private:
      bool running; // global app state
      bool show_fps; // fps counter
      bool fullscreen; // toggle window & fullscreen states
      Timer appTime;
  };
}

#endif // NOMLIB_GAMEAPP_HEADERS defined
