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
#ifndef NOMLIB_SYSTEM_INPUT_MAPPER_MOUSE_WHEEL_ACTION_HPP
#define NOMLIB_SYSTEM_INPUT_MAPPER_MOUSE_WHEEL_ACTION_HPP

#include "nomlib/config.hpp"

namespace nom {

/// \brief A structure containing information on a mouse wheel action.
struct MouseWheelAction
{
  /// \brief Default constructor; initializes the object to an invalid action
  /// state.
  MouseWheelAction( void ) :
    type( 0 ),
    axis( INVALID ),
    x( null ),
    y( null ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Destructor.
  ~MouseWheelAction( void )
  {
    // NOM_LOG_TRACE( NOM );
  }

  /// \brief Constructor for initializing an object to a valid action state.
  MouseWheelAction( uint32 type, uint8 axis, int32 value ) :
    type( type ),
    axis( axis ),
    window_id( 0 )
  {
    // NOM_LOG_TRACE( NOM );

    // Wheel direction is left or right
    if( this->axis == AXIS_X )
    {
      this->x = value;
      this->y = MouseWheelAction::null;
    }
    // Wheel direction is up or down
    else if( this->axis == AXIS_Y )
    {
      this->y = value;
      this->x = MouseWheelAction::null;
    }
    else // Invalid state
    {
      this->x = MouseWheelAction::null;
      this->y = MouseWheelAction::null;
    }
  }

  /// \brief Diagnostic output of the object state.
  void dump( void ) const
  {
    NOM_DUMP( type );
    NOM_DUMP( NOM_SCAST( int, axis ) );
    NOM_DUMP( x );
    NOM_DUMP( y );
    // NOM_DUMP( window_id );
  }

  /// \brief Left-right and up-down axis.
  ///
  /// \remarks Conceptually, a wheel action event has been modeled similarly to a
  /// joystick axis -- note the axis field -- with the left-right axis being zero
  /// (0) and the up-down axis being one (1).
  enum: int
  {
    INVALID = -1,
    AXIS_X = 0,
    AXIS_Y = 1
  };

  /// \brief Sensitivity ranges to action mapping.
  enum: int
  {
    UP = 1,
    DOWN = -1,
    RIGHT = -1,
    LEFT = 1
  };

  /// \brief An enumeration of the invalid state(s).
  enum: int
  {
    null = 0
  };

  /// \brief The event type.
  ///
  /// \note SDL_MOUSEWHEEL.
  uint32 type;

  /// \brief The direction of the wheel.
  ///
  /// \remarks The axis value may be one of the two enumerations: AXIS_X, AXIS_Y
  uint8 axis;

  /// \brief X axis value; relative to the direction of the wheel.
  int32 x;

  /// \brief Y axis value, relative to the direction of the wheel.
  int32 y;

  /// \brief The identifier of the window at the moment of the event.
  ///
  /// \remarks This field is not used and is reserved for future implementation.
  uint32 window_id;
};

} // namespace nom

#endif // include guard defined
