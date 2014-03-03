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
#ifndef NOMLIB_SYSTEM_EVENTS_ACTION_HPP
#define NOMLIB_SYSTEM_EVENTS_ACTION_HPP

#include <map>

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/system/events/EventCallback.hpp"

namespace nom {

/// \brief Input event for mapping input to a state
///
/// \remarks See also nom::InputMapper, nom::InputContext, nom::InputState
struct InputAction
{
  typedef InputAction SelfType;
  typedef std::pair<std::string, InputAction> Pair;

  /// \brief Default constructor.
  InputAction( void );

  /// \brief Destructor.
  ~InputAction( void );

  /// \brief Construct a full nom::Action event.
  InputAction( uint32 type, uint32 event, const EventCallback& method );

  // \brief Re-initialize the instance with default values.
  void clear( void );

  /// \brief The input device; keyboard, mouse, joystick, finger touch and so
  /// on.
  ///
  /// \remarks SDL_KEYDOWN, SDL_MOUSEBUTTONDOWN, ...
  uint32 type;

  /// \brief The type of event; key press, mouse click, joystick button and so
  /// on.
  uint32 event;

  /// \brief The assigned callback to execute for the action.
  EventCallback callback;
};

/// \TODO Relocate to Action?
typedef std::multimap<std::string, InputAction> InputMapping;

} // namespace nom

#endif // include guard defined
