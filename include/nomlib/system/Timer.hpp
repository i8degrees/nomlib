/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_SDL_TIMER_HEADERS
#define NOMLIB_SDL_TIMER_HEADERS

#include <iostream>
#include <string>

#include "nomlib/config.hpp"

namespace nom {

class Timer
{
  public:
    Timer ( void );
    virtual ~Timer ( void );

    void start ( void );
    void stop ( void );

    /// Alias for start
    void restart ( void );

    void pause ( void );
    void unpause ( void );

    uint32 ticks ( void ) const;

    bool started ( void ) const;
    bool paused ( void ) const;

    const std::string ticksAsString ( void ) const;

    /// Helper method; conversion from milliseconds to seconds
    uint32 seconds( float milliseconds ) const;

  private:
    /// Milliseconds since timer start
    uint32 elapsed_ticks;

    /// Holds ticks while paused
    uint32 paused_ticks;

    /// Tracks whether we are paused or not
    bool timer_paused;

    /// Tracks whether we are started or not
    bool timer_started;
};


} // namespace nom

#endif // NOMLIB_SDL_TIMER_HEADERS defined
