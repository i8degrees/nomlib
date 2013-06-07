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

#include "SDLInput.h"

#include "gamelib.h"

#include "timer.h"

namespace nom
{
  class GameApp: public SDLInput
  {
    public:
      GameApp ( void );
      ~GameApp ( void );

      virtual bool isRunning ( void );
      virtual void Running ( void );
      virtual void Quit ( void );

      virtual uint32_t getTicks ( void );

      virtual bool isFullScreen ( void );
      virtual void setFullScreen ( bool toggle );
      virtual bool getShowFPS ( void );
      virtual void showFPS ( bool toggle );
      virtual void toggleFPS ( void );

    private:
      bool app_state; // global app state
      bool show_fps; // fps counter
      bool fullscreen; // toggle window & fullscreen states
      Timer appTime;
  };
}

#endif // NOMLIB_GAMEAPP_HEADERS defined
