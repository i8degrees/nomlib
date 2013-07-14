/******************************************************************************

    SDL-based Timer

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_TIMER_HEADERS
#define NOMLIB_TIMER_HEADERS

#include <iostream>

#include <SDL/SDL.h>

#include "config.hpp"
#include "sys/Logger.hpp"

namespace nom {

class Timer
{
  public:
    Timer ( void );
    virtual ~Timer ( void );

    void Start ( void );
    void Stop ( void );
    void Pause ( void );
    void Unpause ( void );

    // void Update ( void );

    unsigned int getTicks ( void );

    bool isStarted ( void );
    bool isPaused ( void );

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


} // namespace nom

#endif // NOMLIB_TIMER_HEADERS defined
