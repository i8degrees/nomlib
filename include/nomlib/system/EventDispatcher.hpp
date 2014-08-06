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
#ifndef NOMLIB_SDL2_SYSTEM_EVENT_DISPATCHER_HPP
#define NOMLIB_SDL2_SYSTEM_EVENT_DISPATCHER_HPP

#include <iostream>
#include <string>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/core/clock.hpp"
#include "nomlib/system/Event.hpp"

namespace nom {

/// \brief Events dispatcher; a wrapper for SDL2 user events API.
class EventDispatcher
{
  public:
    /// \brief Default constructor.
    EventDispatcher( void );

    /// \brief Destructor.
    ~EventDispatcher( void );

    /// \brief Dispatch an event.
    ///
    /// \returns Boolean TRUE when a message has successfully been dispatched or
    /// boolean FALSE on failure, ~~such as when we have hit the allocation cap
    /// on user-defined events.~~
    bool dispatch( const Event& ev );

  private:
    /// \brief Internal method wrapper for dispatch.
    bool push_event( const Event& ev );

    /// \brief Allocate a set of user-defined events.
    ///
    /// \param The number of events to be allocated.
    ///
    /// \returns ~~The beginning event number for the set of events or (uint32)-1
    /// if there are not enough user-defined events left.~~
    ///
    /// \note This method call is currently not used; there may be a potential
    /// risk of us running out of room in SDL2's events queue! See
    /// our class file (EventDispatcher.cpp) for the incomplete, broken code;
    /// I could not ever get more than one user event dispatched.
    uint32 register_events( int num_events );
};

} // namespace nom

#endif // include guard defined
