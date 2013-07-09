/******************************************************************************

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
#include <cstdlib>

#include <SDL/SDL.h>

#include "sys/SDL_Input.hpp"
#include "sys/SDL_Timer.hpp"
#include "config.hpp"

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

      /// Set the rate at which we would like to receive keyboard input
      void enableKeyRepeat ( int32 delay, int32 interval );

    private:
      /// global app state
      bool app_state;
      /// fps counter
      bool show_fps;
       /// toggle windowed & fullscreen states
      bool fullscreen;
      /// global app timer
      Timer appTime;
  };
}

#endif // NOMLIB_SDL_APP_HEADERS defined
