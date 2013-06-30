/******************************************************************************
    timer.h

    SDL-based Timer Implementation Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_TIMER_HEADERS
#define GAMELIB_TIMER_HEADERS

#include "SDL.h"

#include <iostream>

class Timer
{
  public:
    Timer ( void );
    ~Timer ( void );

    void Start ( void );
    void Stop ( void );
    void Pause ( void );
    void Unpause ( void );

    unsigned int getTicks ( void );

    bool isStarted ( void );
    bool isPaused ( void );

    /// SDL_Delay wrapper
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

#endif // GAMELIB_TIMER_HEADERS defined
