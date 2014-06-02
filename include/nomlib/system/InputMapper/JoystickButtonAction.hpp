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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_JOYSTICK_BUTTON_ACTION_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_JOYSTICK_BUTTON_ACTION_HPP

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

/// \brief A structure containing information on a joystick button action.
struct JoystickButtonAction
{
  /// \brief Default constructor; initializes the object to an invalid action
  /// state.
  JoystickButtonAction( void ) :
    type( 0 ),
    id( 0 ),
    button( 0 ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Destructor.
  ~JoystickButtonAction( void )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Constructor for initializing an object to a valid action state.
  JoystickButtonAction( SDL_JoystickID id, uint32 type, uint8 button ) :
    type( type ),
    id( id ),
    button( button ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Diagnostic output of the object state.
  void dump( void ) const
  {
    NOM_DUMP( type );
    NOM_DUMP( id );
    NOM_DUMP( NOM_SCAST( int, button ) );
    NOM_DUMP( window_id );
  }

  /// \brief The event type.
  ///
  /// \remarks SDL_JOYBUTTONDOWN or SDL_JOYBUTTON_UP.
  uint32 type;

  /// \brief The index identifier of the joystick device (as recognized by SDL).
  SDL_JoystickID id;

  /// \brief The index of the button.
  uint8 button;

  /// \brief The identifier of the window at the moment of the event.
  ///
  /// \remarks This field is not used and is reserved for future implementation.
  uint32 window_id;
};

} // namespace nom

#endif // include guard defined
