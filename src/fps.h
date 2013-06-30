/******************************************************************************
    fps.h

    SDL-based Frames Per Second Implementation; inherits from the Timer Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef GAMELIB_TIMER_FPS_HEADERS
#define GAMELIB_TIMER_FPS_HEADERS

#include "timer.h" // base class

// Requires SDL Video Initialization to be called first
class FPS: public nom::Timer
{
  public:
    FPS ( void );
    ~FPS ( void );

    void Start ( void );
    void Stop ( void );

    unsigned int getFrames ( void );
    unsigned int getFPS ( void );
    void Update ( void );

  private:
    unsigned int total_frames;
    nom::Timer fps, fps_update;
};

#endif // GAMELIB_TIMER_FPS_HEADERS defined
