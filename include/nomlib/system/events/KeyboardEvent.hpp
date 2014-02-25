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
#ifndef NOMLIB_SYSTEM_EVENTS_KEYBOARD_EVENT_HPP
#define NOMLIB_SYSTEM_EVENTS_KEYBOARD_EVENT_HPP

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

struct KeyboardSymbolCode
{
  // TODO
};

struct KeyboardScanCode
{
  // TODO
};

/// \brief A structure containing information on a keyboard symbol.
struct KeyboardSymbolEvent
{
  /// \brief The event type.
  ///
  /// \remarks Not used; reserved for future use.
  uint32 type;

  /// \brief The physical key code of the key press event.
  ///
  /// \TODO Implement using the KeyboardScanCode structure.
  SDL_Scancode scan_code;

  /// \brief Virtual key code of the key press event.
  ///
  /// \TODO Implement using the KeyboardSymbolCode structure.
  SDL_Keycode sym;

  /// \brief The modifiers of the key press event; CTRL, ALT, ...
  uint16 mod;
};

/// \brief A structure containing information on a keyboard event.
struct KeyboardEvent
{
  /// \brief The event type.
  ///
  /// \remarks SDL_KEYDOWN or SDL_KEYUP.
  uint32 type;

  /// \brief Symbol of the key press event.
  ///
  /// \TODO Implement using the KeyboardSymbol structure.
  // SDL_Keysym sym;
  uint32 sym;

  /// \brief The modifiers of the key press event; CTRL, ALT, ...
  uint16 mod;

  /// \brief The state of the key press event.
  ///
  /// \remarks SDL_PRESSED or SDL_RELEASED.
  ///
  /// \TODO Investigate as to why we see no value when
  /// NOM_DEBUG_SDL2_KEYBOARD_INPUT is enabled.
  uint8 state;

  /// \brief Non-zero if this is a repeating key press event.
  ///
  /// \TODO Investigate as to why we see no value when
  /// NOM_DEBUG_SDL2_KEYBOARD_INPUT is enabled.
  uint8 repeat;

  /// \brief The identifier of the window at the moment of the event.
  uint32 window_id;

  /// \brief The recorded time at the moment of the event.
  uint32 timestamp;
};

} // namespace nom

#endif // include guard defined
