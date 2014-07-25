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
#include "nomlib/system/InputMapper/InputAction.hpp"

namespace nom {

// InputAction (base class)

InputAction::~InputAction( void )
{
  // NOM_LOG_TRACE( NOM );
}

const Event& InputAction::event( void ) const
{
  return this->event_;
}

const EventCallback& InputAction::callback( void ) const
{
  return this->callback_;
}

void InputAction::set_callback( const EventCallback& delegate )
{
  this->callback_ = delegate;
}

void InputAction::operator() ( const Event& evt ) const
{
  this->callback_( evt );
}

void InputAction::dump( void ) const
{
  // Event type and timestamp info
  this->event_.dump();
}

// KeyboardAction

KeyboardAction::~KeyboardAction( void )
{
  // NOM_LOG_TRACE( NOM );
}

KeyboardAction::KeyboardAction( uint32 type, int32 sym )
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = type;
  this->event_.key.sym = sym;
  this->event_.key.mod = KMOD_NONE;
  this->event_.key.repeat = 0;
  this->event_.key.window_id = 0;
}

KeyboardAction::KeyboardAction( uint32 type, int32 sym, uint16 mod )
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = type;
  this->event_.key.sym = sym;
  this->event_.key.mod = mod;
  this->event_.key.repeat = 0;
  this->event_.key.window_id = 0;
}

KeyboardAction::KeyboardAction( uint32 type, int32 sym, uint16 mod, uint8 repeat )
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = type;
  this->event_.key.sym = sym;
  this->event_.key.mod = mod;
  this->event_.key.repeat = repeat;
  this->event_.key.window_id = 0;
}

void KeyboardAction::dump( void ) const
{
  // Event type & timestamp info
  InputAction::dump();

  this->event_.key.dump();
}

// MouseButtonAction

MouseButtonAction::~MouseButtonAction( void )
{
  // NOM_LOG_TRACE( NOM );
}

MouseButtonAction::MouseButtonAction( uint32 type, uint8 button )
{
  // NOM_LOG_TRACE( NOM );
  this->event_.type = type;
  this->event_.mouse.x = 0;
  this->event_.mouse.y = 0;
  this->event_.mouse.button = button;
  this->event_.mouse.window_id = 0;
}

void MouseButtonAction::dump( void ) const
{
  // Event type & timestamp info
  InputAction::dump();

  this->event_.mouse.dump();
}

// MouseWheelAction

MouseWheelAction::~MouseWheelAction( void )
{
  // NOM_LOG_TRACE( NOM );
}

MouseWheelAction::MouseWheelAction( uint32 type, uint8 axis, int32 value )
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = type;
  // this->event_.wheel.axis = axis;
  this->event_.wheel.window_id = 0;

  // Wheel direction is left or right
  // if( this->event_.wheel.axis == AXIS_X )
  if( axis == AXIS_X )
  {
    this->event_.wheel.x = value;
    this->event_.wheel.y = MouseWheelAction::null;
  }
  // Wheel direction is up or down
  // else if( this->event_.wheel.axis == AXIS_Y )
  else if( axis == AXIS_Y )
  {
    this->event_.wheel.y = value;
    this->event_.wheel.x = MouseWheelAction::null;
  }
  else // Invalid state
  {
    this->event_.wheel.x = MouseWheelAction::null;
    this->event_.wheel.y = MouseWheelAction::null;
  }
}

void MouseWheelAction::dump( void ) const
{
  // Event type & timestamp info
  InputAction::dump();

  this->event_.wheel.dump();
}

// JoystickButtonAction

JoystickButtonAction::~JoystickButtonAction( void )
{
  // NOM_LOG_TRACE( NOM );
}

JoystickButtonAction::JoystickButtonAction( SDL_JoystickID id, uint32 type, uint8 button )
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = type;
  this->event_.jbutton.id = id;
  this->event_.jbutton.button = button;
}

void JoystickButtonAction::dump( void ) const
{
  // Event type & timestamp info
  InputAction::dump();

  this->event_.jbutton.dump();
}

// JoystickAxisAction

JoystickAxisAction::~JoystickAxisAction( void )
{
  // NOM_LOG_TRACE( NOM );
}

JoystickAxisAction::JoystickAxisAction( SDL_JoystickID id, uint32 type, uint8 axis, int16 value )
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = type;
  this->event_.jaxis.id = id;
  this->event_.jaxis.axis = axis;
  this->event_.jaxis.value = value;
}

void JoystickAxisAction::dump( void ) const
{
  // Event type & timestamp info
  InputAction::dump();

  this->event_.jaxis.dump();
}

} // namespace nom
