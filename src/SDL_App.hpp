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
#include "timer.h"
#include "gamelib.h"

namespace nom
{
  // This is an inheritance-only class
  class SDL_App: public nom::SDL_Input
  {
    protected:
      SDL_App ( void );
      ~SDL_App ( void );

      bool isRunning ( void );
      void Running ( void );
      void Quit ( void );

      uint32_t getTicks ( void );

      bool isFullScreen ( void );
      void setFullScreen ( bool toggle );
      bool getShowFPS ( void );
      void showFPS ( bool toggle );
      void toggleFPS ( void );
    private:
      bool app_state; /// global app state
      bool show_fps; /// fps counter
      bool fullscreen; /// toggle window & fullscreen states
      Timer appTime; /// global app timer
  };
}

#endif // NOMLIB_SDL_APP_HEADERS defined
