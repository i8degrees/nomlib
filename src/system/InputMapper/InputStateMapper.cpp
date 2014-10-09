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
#include "nomlib/system/InputMapper/InputStateMapper.hpp"

namespace nom {

InputStateMapper::InputStateMapper( void )
{
  // NOM_LOG_TRACE( NOM );
}

InputStateMapper::~InputStateMapper( void )
{
  // NOM_LOG_TRACE( NOM );
}

bool InputStateMapper::active( const std::string& key ) const
{
  InputStateMap::const_iterator itr = this->states_.find( key );

  // No match found; do nothing.
  if( itr == this->states_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not check for active state on the specified key: " + key );
    return false;
  }
  else
  {
    return itr->second.active;
  }
}

bool InputStateMapper::insert( const std::string& key, const InputActionMapper& map, bool active )
{
  InputActionState input_state;
  input_state.active = active;
  input_state.actions = map.get();

  InputStateMapper::Pair p( key, input_state );

  auto it = this->states_.insert( p );

  // No dice; the insertion failed for some reason!
  if( it.second == false )
  {
    NOM_LOG_ERR( NOM, "Could not insert state key: " + key );
    return false;
  }

  return true;
}

bool InputStateMapper::erase( const std::string& key )
{
  InputStateMap::const_iterator itr = this->states_.find( key );

  // No match found; do nothing.
  if( itr == this->states_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not remove specified state key: " + key );
    return false;
  }
  else // Match found; remove the context mapping
  {
    this->states_.erase( itr );
    return true;
  }

  return false;
}

bool InputStateMapper::activate( const std::string& key )
{
  InputStateMap::iterator itr = this->states_.find( key );

  // No match found; do nothing.
  if( itr == this->states_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not activate specified state key: " + key );
    return false;
  }
  else // Match found; set the input mapping as the active context.
  {
    itr->second.active = true;

    return true;
  }

  return false;
}

bool InputStateMapper::disable( const std::string& key )
{
  InputStateMap::iterator itr = this->states_.find( key );

  // No match found; do nothing.
  if( itr == this->states_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not disable specified state key: " + key );
    return false;
  }
  else // Match found; disable the requested state
  {
    itr->second.active = false;
    return true;
  }

  return false;
}

void InputStateMapper::disable( void )
{
  for( auto itr = this->states_.begin(); itr != this->states_.end(); ++itr )
  {
    itr->second.active = false;
  }
}

bool InputStateMapper::activate_only( const std::string& key )
{
  // First, we need to make sure that the requested state even exists before we
  // flag anything inactive.
  InputStateMap::iterator it = this->states_.find( key );

  if( it == this->states_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not find state key: " + key );
    return false;
  }

  // Iterate through all of our action-to-states and flag them inactive.
  for( InputStateMap::iterator itr = this->states_.begin(); itr != this->states_.end(); ++itr )
  {
    itr->second.active = false;
  }

  it = this->states_.find( key );

  // No match found; do nothing more!
  if( it == this->states_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not activate only state key: " + key );
    return false;
  }
  else // Match found; enable the requested state
  {
    it->second.active = true;
    return true;
  }

  return false;
}

void InputStateMapper::clear( void )
{
  this->states_.clear();
}

void InputStateMapper::dump( void )
{
  for( auto itr = this->states_.begin(); itr != this->states_.end(); ++itr )
  {
    if( itr->second.active == true )
    {
      InputActionMapper::ActionMap& input_map = itr->second.actions;

      for( InputActionMapper::ActionMap::const_iterator itr = input_map.begin(); itr != input_map.end(); ++itr )
      {
        NOM_DUMP( itr->first );

        if ( itr->second != nullptr )
        {
          itr->second->dump();
        }
        else
        {
          NOM_LOG_ERR( NOM, "Invalid input mapping state." );
        }
      }
    }
  }
}

void InputStateMapper::on_event( const Event& ev )
{
  for( auto itr = this->states_.begin(); itr != this->states_.end(); ++itr )
  {
    if( itr->second.active == true )
    {
      InputActionMapper::ActionMap input_map = itr->second.actions;
      for( InputActionMapper::ActionMap::const_iterator itr = input_map.begin(); itr != input_map.end(); ++itr )
      {
        if( ev.type == SDL_KEYDOWN || ev.type == SDL_KEYUP )
        {
          if( this->on_key_press( *itr->second, ev ) )
          {
            itr->second->operator()( ev );
          }
        }
        else if( ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP )
        {
          if( this->on_mouse_button( *itr->second, ev ) )
          {
            itr->second->operator()( ev );
          }
        }
        else if( ev.type == SDL_MOUSEWHEEL || ev.type == SDL_MOUSEWHEEL )
        {
          if( this->on_mouse_wheel( *itr->second, ev ) )
          {
            itr->second->operator()( ev );
          }
        }
        else if( ev.type == SDL_JOYBUTTONDOWN || ev.type == SDL_JOYBUTTONUP )
        {
          if( this->on_joystick_button( *itr->second, ev ) )
          {
            itr->second->operator()( ev );
          }
        }
        else if( ev.type == SDL_JOYAXISMOTION )
        {
          if( this->on_joystick_axis( *itr->second, ev ) )
          {
            itr->second->operator()( ev );
          }
        }
      } // end input_map iteration
    } // end conditional active input state
  }
}

bool InputStateMapper::on_key_press( const InputAction& mapping, const Event& ev )
{
  Event evt = mapping.event();

  if( evt.type != ev.type ) return false;

  // Handle a keyboard action with repeat; only trigger if the event is a
  // repeating one
  if( evt.key.repeat != 0 )
  {
    if( evt.key.sym == ev.key.sym && evt.key.mod == ev.key.mod &&
        evt.key.repeat == ev.key.repeat )
    {
      // Matched
      return true;
    }
  }
  else  // Handle normal keyboard action; repeating makes no difference to us
  {
    if( evt.key.sym == ev.key.sym && evt.key.mod == ev.key.mod ) {
      // Matched
      return true;
    }
  }

  // No match to any mapped action
  return false;
}

bool InputStateMapper::on_mouse_button( const InputAction& mapping, const Event& ev )
{
  Event evt = mapping.event();

  if( evt.type != ev.type ) return false;

  // Successful match is a mouse click that matches both the button used
  // (left, middle, right, ...) and its number of clicks (single, double, ...)
  if( evt.mouse.clicks == ev.mouse.clicks && evt.mouse.button == ev.mouse.button )
  {
    // Match
    return true;
  }

  // No match; number of clicks mismatch
  return false;
}

bool InputStateMapper::on_mouse_wheel( const InputAction& mapping, const Event& ev )
{
  Event evt = mapping.event();

  // if( mapping.wheel == nullptr ) return false;

  if( evt.type != ev.type ) return false;

  // NOTE: X & Y coordinate values depend on the construction of a WheelAction
  // object.
  // if( evt.wheel.axis == MouseWheelAction::AXIS_X )
  if( evt.wheel.x != MouseWheelAction::null )
  {
    // Left
    if( evt.wheel.x >= MouseWheelAction::LEFT && ev.wheel.x >= MouseWheelAction::LEFT )
    {
      return true;
    }

    // Right
    if( evt.wheel.x <= MouseWheelAction::RIGHT && ev.wheel.x <= MouseWheelAction::RIGHT )
    {
      return true;
    }
  }
  // else if( evt.wheel.axis == MouseWheelAction::AXIS_Y )
  else if( evt.wheel.y != MouseWheelAction::null )
  {
    // Up
    if( evt.wheel.y >= MouseWheelAction::UP && ev.wheel.y >= MouseWheelAction::UP )
    {
      return true;
    }

    // Down
    if( evt.wheel.y <= MouseWheelAction::DOWN && ev.wheel.y <= MouseWheelAction::DOWN )
    {
      return true;
    }
  }

  // Initialized to an invalid state!
  NOM_ASSERT( evt.wheel.x == 0 || evt.wheel.y == 0 );

  return false;
}

bool InputStateMapper::on_joystick_button( const InputAction& mapping, const Event& ev )
{
  Event evt = mapping.event();

  // if( mapping.jbutton == nullptr ) return false;

  if( evt.type != ev.type ) return false;

  if( evt.jbutton.id != ev.jbutton.id ) return false;

  if( evt.jbutton.button == ev.jbutton.button ) return true;

  return false;
}

// FIXME: Implementation is incomplete!
bool InputStateMapper::on_joystick_axis( const InputAction& mapping, const Event& ev )
{
  Event evt = mapping.event();

  // if( mapping.jaxis == nullptr ) return false;

  if( evt.type != ev.type ) return false;

  if( evt.jaxis.id != ev.jaxis.id ) return false;

  if( evt.jaxis.axis != ev.jaxis.axis ) return false;

  // Within dead-zone tolerance
  if( ( ev.jaxis.value < -3200 ) || ( ev.jaxis.value > 3200 ) )
  {
    // Up-down axis (Sony PS3 game controller)
    if( evt.jaxis.axis == 0 )
    {
      // Up
      if( evt.jaxis.value < 0 ) return true;

      // Down
      if( evt.jaxis.value > 0 ) return true;
    }

    // Left-right axis (Sony PS3 game controller)
    if( evt.jaxis.axis == 1 )
    {
      // Left
      if( evt.jaxis.value < 0 ) return true;

      // Right
      if( evt.jaxis.value > 0 ) return true;
    }
  }

  return false;
}

} // namespace nom
