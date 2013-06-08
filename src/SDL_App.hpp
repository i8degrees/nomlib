/******************************************************************************
    SDL_App.hpp

    SDL app interface class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_SDL_APP_HEADERS
#define NOMLIB_SDL_APP_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <memory>

#include "SDL_Input.hpp"

#include "gamelib.h"

#include "timer.h"

namespace nom
{
  class SDL_App: public nom::SDL_Input
  {
    public:
      SDL_App ( void );
      ~SDL_App ( void );

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
      bool app_state; /// global app state
      bool show_fps; /// fps counter
      bool fullscreen; /// toggle window & fullscreen states
      Timer appTime; /// global app timer
  };
}

#endif // NOMLIB_SDL_APP_HEADERS defined
