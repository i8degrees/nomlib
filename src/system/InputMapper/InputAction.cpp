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

InputAction::InputAction()
{
  // NOM_LOG_TRACE( NOM );

  this->event_ = {};
}

InputAction::~InputAction()
{
  // NOM_LOG_TRACE( NOM );
}

const Event& InputAction::event( void ) const
{
  return this->event_;
}

const nom::event_callback& InputAction::callback() const
{
  return this->callback_;
}

void InputAction::set_callback(const nom::event_callback& delegate)
{
  this->callback_ = delegate;
}

void InputAction::operator()(const Event& evt) const
{
  this->callback_(evt);
}

// KeyboardAction

KeyboardAction::~KeyboardAction()
{
  // NOM_LOG_TRACE( NOM );
}

KeyboardAction::KeyboardAction(int32 sym, InputState state)
{
  // NOM_LOG_TRACE( NOM );

  if( state == InputState::PRESSED ) {
    this->event_.type = Event::KEY_PRESS;
  } else {
    this->event_.type = Event::KEY_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.key.sym = sym;
  this->event_.key.mod = KMOD_NONE;
  this->event_.key.repeat = 0;
  this->event_.key.state = state;
  this->event_.key.window_id = 0;
}

KeyboardAction::KeyboardAction(int32 sym, uint16 mod, InputState state)
{
  // NOM_LOG_TRACE( NOM );

  if( state == InputState::PRESSED ) {
    this->event_.type = Event::KEY_PRESS;
  } else {
    this->event_.type = Event::KEY_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.key.sym = sym;
  this->event_.key.mod = mod;
  this->event_.key.repeat = 0;
  this->event_.key.state = state;
  this->event_.key.window_id = 0;
}

KeyboardAction::
KeyboardAction(int32 sym, uint16 mod, uint8 repeat, InputState state)
{
  // NOM_LOG_TRACE( NOM );

  if( state == InputState::PRESSED ) {
    this->event_.type = Event::KEY_PRESS;
  } else {
    this->event_.type = Event::KEY_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.key.sym = sym;
  this->event_.key.mod = mod;
  this->event_.key.repeat = repeat;
  this->event_.key.state = state;
  this->event_.key.window_id = 0;
}

// MouseButtonAction

MouseButtonAction::~MouseButtonAction()
{
  // NOM_LOG_TRACE( NOM );
}

MouseButtonAction::MouseButtonAction(uint8 button, InputState state)
{
  // NOM_LOG_TRACE( NOM );

  if( state == InputState::PRESSED ) {
    this->event_.type = Event::MOUSE_BUTTON_CLICK;
  } else {
    this->event_.type = Event::MOUSE_BUTTON_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.mouse.x = 0;
  this->event_.mouse.y = 0;
  this->event_.mouse.button = button;
  this->event_.mouse.clicks = 1;
  this->event_.mouse.state = state;
  this->event_.mouse.window_id = 0;
}

MouseButtonAction::
MouseButtonAction(uint8 button, uint8 clicks, InputState state)
{
  // NOM_LOG_TRACE( NOM );

  if( state == nom::InputState::PRESSED ) {
    this->event_.type = Event::MOUSE_BUTTON_CLICK;
  } else {
    this->event_.type = Event::MOUSE_BUTTON_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.mouse.x = 0;
  this->event_.mouse.y = 0;
  this->event_.mouse.button = button;
  this->event_.mouse.clicks = clicks;
  this->event_.mouse.state = state;
  this->event_.mouse.window_id = 0;
}

// MouseWheelAction

MouseWheelAction::~MouseWheelAction()
{
  // NOM_LOG_TRACE( NOM );
}

MouseWheelAction::MouseWheelAction(uint8 axis, int32 value)
{
  // NOM_LOG_TRACE( NOM );


  this->event_.type = Event::MOUSE_WHEEL;
  this->event_.timestamp = 0;
  this->event_.wheel.id = 0;
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

// JoystickButtonAction

JoystickButtonAction::~JoystickButtonAction()
{
  // NOM_LOG_TRACE( NOM );
}

JoystickButtonAction::
JoystickButtonAction(JoystickID id, uint8 button, InputState state)

{
  // NOM_LOG_TRACE( NOM );

  if( state == InputState::PRESSED ) {
    this->event_.type = Event::JOYSTICK_BUTTON_PRESS;
  } else {
    this->event_.type = Event::JOYSTICK_BUTTON_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.jbutton.id = id;
  this->event_.jbutton.button = button;
  this->event_.jbutton.state = state;
}

// JoystickAxisAction

JoystickAxisAction::~JoystickAxisAction()
{
  // NOM_LOG_TRACE( NOM );
}

JoystickAxisAction::JoystickAxisAction(JoystickID id, uint8 axis)
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = Event::JOYSTICK_AXIS_MOTION;
  this->event_.timestamp = 0;
  this->event_.jaxis.id = id;
  this->event_.jaxis.axis = axis;
  this->event_.jaxis.value = 0;
}

// JoystickHatAction

JoystickHatAction::~JoystickHatAction()
{
  // NOM_LOG_TRACE( NOM );
}

JoystickHatAction::JoystickHatAction(JoystickID id, uint8 hat, uint8 value)
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = Event::JOYSTICK_HAT_MOTION;
  this->event_.timestamp = 0;
  this->event_.jhat.id = id;
  this->event_.jhat.hat = hat;
  this->event_.jhat.value = value;
}

// GameControllerButtonAction

GameControllerButtonAction::~GameControllerButtonAction()
{
  // NOM_LOG_TRACE( NOM );
}

GameControllerButtonAction::
GameControllerButtonAction( JoystickID id, GameController::Button button,
                            InputState state )
{
  // NOM_LOG_TRACE( NOM );

  if( state == InputState::PRESSED ) {
    this->event_.type = Event::GAME_CONTROLLER_BUTTON_PRESS;
  } else {
    this->event_.type = Event::GAME_CONTROLLER_BUTTON_RELEASE;
  }

  this->event_.timestamp = 0;
  this->event_.cbutton.id = id;
  this->event_.cbutton.button = button;
  this->event_.cbutton.state = state;
}

// GameControllerAxisAction

GameControllerAxisAction::~GameControllerAxisAction()
{
  // NOM_LOG_TRACE( NOM );
}

GameControllerAxisAction::
GameControllerAxisAction(JoystickID id, GameController::Axis axis)
{
  // NOM_LOG_TRACE( NOM );

  this->event_.type = Event::GAME_CONTROLLER_AXIS_MOTION;
  this->event_.timestamp = 0;
  this->event_.caxis.id = id;
  this->event_.caxis.axis = axis;
  this->event_.caxis.value = 0;
}

} // namespace nom
