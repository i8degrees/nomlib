/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#ifndef NOMLIB_SDL2_APP_HEADERS
#define NOMLIB_SDL2_APP_HEADERS

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <cstdlib>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/Input.hpp"
#include "nomlib/system/Timer.hpp"

namespace nom {

// This is an inheritance-only class
class SDL_App: public Input
{
  public:
    SDL_App ( void );
    virtual ~SDL_App ( void );

    virtual bool onInit ( void );

    /// Re-implements nom::Input::onQuit()
    virtual void onQuit ( void );

    virtual void onEvent ( SDL_Event* event );

    bool isRunning ( void );
    void Running ( void );
    void Quit ( void );

    uint32 ticks ( void );

    bool isFullScreen ( void );
    void setFullScreen ( bool toggle );
    bool getShowFPS ( void );
    void showFPS ( bool toggle );
    void toggleFPS ( void );

    /// Let the user know if there are pending events
    bool PollEvents ( SDL_Event* );

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


} // namespace nom

#endif // include guard defined
