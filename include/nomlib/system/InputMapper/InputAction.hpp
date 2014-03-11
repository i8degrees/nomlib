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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_ACTION_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_INPUT_ACTION_HPP

#include <map> // Remove after relocating std::multimap typedef

#include "nomlib/config.hpp"
#include "nomlib/system/EventCallback.hpp"
#include "nomlib/system/InputMapper/KeyboardAction.hpp"
#include "nomlib/system/InputMapper/MouseButtonAction.hpp"
#include "nomlib/system/InputMapper/MouseWheelAction.hpp"
#include "nomlib/system/InputMapper/JoystickButtonAction.hpp"
#include "nomlib/system/InputMapper/JoystickAxisAction.hpp"

namespace nom {

/// \brief Input event for mapping an action to a state.
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

  InputAction( const KeyboardAction& ev, const EventCallback& method );

  InputAction( const MouseButtonAction& ev, const EventCallback& method );
  InputAction( const MouseWheelAction& ev, const EventCallback& method );

  InputAction( const JoystickButtonAction& ev, const EventCallback& method );
  InputAction( const JoystickAxisAction& ev, const EventCallback& method );

  /// \brief The type of event; key press, mouse click, joystick button and so
  /// on.
  KeyboardAction* key;
  MouseButtonAction* mouse;
  MouseWheelAction* wheel;
  JoystickButtonAction* jbutton;
  JoystickAxisAction* jaxis;

  /// \brief The assigned delegate to execute for the action event.
  EventCallback delegate;
};

/// \remarks Note that we are a multi-map, therefore multiple duplicate action
/// strings can be held within this container.
typedef std::multimap<std::string, InputAction> InputMapping;

} // namespace nom

#endif // include guard defined
