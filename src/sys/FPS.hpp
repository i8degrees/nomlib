/******************************************************************************

    SDL-based Frames Per Second Implementation; inherits from the Timer Class

  Copyright (c) 2013 Jeffrey Carpenter

******************************************************************************/
#ifndef NOMLIB_TIMER_FPS_HEADERS
#define NOMLIB_TIMER_FPS_HEADERS

#include "config.hpp"
#include "sys/Logger.hpp"
#include "sys/SDL_Timer.hpp"

namespace nom {

class FPS: public Timer // "is a" relationship
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
    Timer fps, fps_update;
};


} // namespace nom

#endif // NOMLIB_TIMER_FPS_HEADERS defined
