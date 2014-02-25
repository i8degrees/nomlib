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
#ifndef NOMLIB_SYSTEM_EVENTS_GESTURE_EVENT_HPP
#define NOMLIB_SYSTEM_EVENTS_GESTURE_EVENT_HPP

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

/// \brief A structure containing information on a multi-finger gesture (touch)
/// event.
struct GestureEvent
{
  /// \brief The event type.
  ///
  /// \remarks SDL_MULTIGESTURE.
  uint32 type;

  /// \brief The touch device index identifier.
  SDL_TouchID id;

  /// \brief The X coordinate of the event.
  float x;

  /// \brief The Y coordinate of the event.
  float y;

  /// \brief ...???
  float dTheta;

  /// \brief ...???
  float dDist;

  /// \brief The number of fingers used in the gesture event.
  uint16 num_fingers;
};

} // namespace nom

#endif // include guard defined
