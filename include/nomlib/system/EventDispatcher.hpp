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
#ifndef NOMLIB_SDL2_EVENT_DISPATCHER_HPP
#define NOMLIB_SDL2_EVENT_DISPATCHER_HPP

#include <iostream>
#include <string>

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

class EventDispatcher
{
  public:
    /// Definition type of the event we are dispatching
    enum UserEvent: int32
    {
      Unknown = 0,
      AI,
      Animation,
      Application,
      Audio,
      General,
      Library,
      State,
      UI
    };

    EventDispatcher ( void );
    ~EventDispatcher ( void );

    /// Internal method wrapper for dispatch
    ///
    /// \TODO Use SDL2's register event function; as per SDL2 wiki docs for
    /// SDL_UserEvent.
    int32 push ( SDL_Event* event, int32 code, void* params );

    /// Dispatch an event
    ///
    /// \TODO Use SDL2's register event function; as per SDL2 wiki docs for
    /// SDL_UserEvent.
    int32 dispatch ( enum UserEvent code, void* params = nullptr );
};


} // namespace nom

#endif // include guard defined
