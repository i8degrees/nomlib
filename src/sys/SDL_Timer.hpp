/******************************************************************************

    SDL-based Timer

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_TIMER_HEADERS
#define NOMLIB_TIMER_HEADERS

#include <iostream>
#include <algorithm>

#include <SDL/SDL.h>

#include "nomlib_config.hpp"

namespace nom
{
  class Timer
  {
    public:
      Timer ( void );
      ~Timer ( void );

      void Start ( void );
      void Stop ( void );
      void Pause ( void );
      void Unpause ( void );

      // void Update ( void );

      unsigned int getTicks ( void );

      bool isStarted ( void );
      bool isPaused ( void );

      /// SDL_Delay wrapper
      ///
      /// Values below 10 milliseconds are clamped to 10
      void sleep ( float milliseconds );

      /// Helper method; conversion from milliseconds to seconds
      uint32_t seconds( float seconds ) const;

    private:
      /// Milliseconds since timer start
      unsigned int elapsed_ticks;
      /// Holds ticks while paused
      unsigned int paused_ticks;
      /// Tracks whether we are paused or not
      bool paused;
      /// Tracks whether we are started or not
      bool started;
  };
}

#endif // NOMLIB_TIMER_HEADERS defined
