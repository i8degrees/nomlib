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
#ifndef NOMLIB_SYSTEM_EVENTS_EVENT_HPP
#define NOMLIB_SYSTEM_EVENTS_EVENT_HPP

#include "nomlib/config.hpp"

#include "nomlib/system/events/DragDropEvent.hpp"
#include "nomlib/system/events/JoystickAxisEvent.hpp"
#include "nomlib/system/events/JoystickButtonEvent.hpp"
#include "nomlib/system/events/FingerEvent.hpp"
#include "nomlib/system/events/FingerTouchEvent.hpp"
#include "nomlib/system/events/GestureEvent.hpp"
#include "nomlib/system/events/KeyboardEvent.hpp"
#include "nomlib/system/events/MouseButtonEvent.hpp"
#include "nomlib/system/events/MouseMotionEvent.hpp"
#include "nomlib/system/events/QuitEvent.hpp"
#include "nomlib/system/events/TextEditingEvent.hpp"
#include "nomlib/system/events/TextInputEvent.hpp"
#include "nomlib/system/events/WheelEvent.hpp"
#include "nomlib/system/events/WindowEvent.hpp"
#include "nomlib/system/events/UserEvent.hpp"

namespace nom {

/// \brief Event handling types.
union Event
{
  /// \brief The event type.
  uint32 type;

  QuitEvent quit;
  WindowEvent window;
  KeyboardEvent key;
  MouseMotionEvent m_motion;
  MouseButtonEvent mouse;
  WheelEvent wheel;
  JoystickButtonEvent jbutton;
  JoystickAxisEvent jaxis;
  FingerTouchEvent touch;
  GestureEvent gesture;
  DragDropEvent drop;
  TextInputEvent text;
  TextEditingEvent edit;
  UserEvent user;
};

} // namespace nom

#endif // include guard defined
