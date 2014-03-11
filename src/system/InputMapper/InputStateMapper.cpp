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
  auto it = this->states_.insert( Pair( key, input_state ) );

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
    NOM_DUMP( itr->first );

    if( itr->second.active == true )
    {
      InputActionMapper::ActionMap& input_map = itr->second.actions;

      for( InputActionMapper::ActionMap::const_iterator itr = input_map.begin(); itr != input_map.end(); ++itr )
      {
        NOM_DUMP( itr->first );

        if( itr->second.key != nullptr )
        {
          itr->second.key->dump();
        }
        else if ( itr->second.mouse != nullptr )
        {
          itr->second.mouse->dump();
        }
        else if ( itr->second.jbutton != nullptr )
        {
          itr->second.jbutton->dump();
        }
        else
        {
          NOM_LOG_ERR( NOM, "Invalid input mapping state." );
        }
      }
    }
    else
    {
      NOM_LOG_ERR( NOM, "Input action is inactive." );
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
        if( this->on_key_press( itr->second, ev ) )
        {
          itr->second.delegate();
        }
        else if( this->on_mouse_button( itr->second, ev ) )
        {
          itr->second.delegate();
        }
        else if( this->on_mouse_wheel( itr->second, ev ) )
        {
          itr->second.delegate();
        }
        else if( this->on_joystick_button( itr->second, ev ) )
        {
          itr->second.delegate();
        }
        else if( this->on_joystick_axis( itr->second, ev ) )
        {
          itr->second.delegate();
        }
      } // end input_map iteration
    } // end conditional active input state
  }
}

bool InputStateMapper::on_key_press( const InputAction& mapping, const Event& ev )
{
  if( mapping.key == nullptr ) return false;

  if( mapping.key->type != ev.type ) return false;

  if( mapping.key->mod != KMOD_NONE )
  {
    if( mapping.key->sym == ev.key.sym && mapping.key->mod == ev.key.mod )
    {
      return true;
    }
  }
  else if( mapping.key->sym == ev.key.sym )
  {
    return true;
  }

  return false;
}

bool InputStateMapper::on_mouse_button( const InputAction& mapping, const Event& ev )
{
  if( mapping.mouse == nullptr ) return false;

  if( mapping.mouse->type != ev.type ) return false;

  if( mapping.mouse->button == ev.mouse.button ) return true;

  return false;
}

bool InputStateMapper::on_mouse_wheel( const InputAction& mapping, const Event& ev )
{
  if( mapping.wheel == nullptr ) return false;

  if( mapping.wheel->type != ev.type ) return false;

  // NOTE: X & Y coordinate values depend on the construction of a WheelAction
  // object.
  if( mapping.wheel->axis == MouseWheelAction::AXIS_X )
  {
    // Left
    if( mapping.wheel->x >= MouseWheelAction::LEFT && ev.wheel.x >= MouseWheelAction::LEFT )
    {
      return true;
    }

    // Right
    if( mapping.wheel->x <= MouseWheelAction::RIGHT && ev.wheel.x <= MouseWheelAction::RIGHT )
    {
      return true;
    }
  }
  else if( mapping.wheel->axis == MouseWheelAction::AXIS_Y )
  {
    // Up
    if( mapping.wheel->y >= MouseWheelAction::UP && ev.wheel.y >= MouseWheelAction::UP )
    {
      return true;
    }

    // Down
    if( mapping.wheel->y <= MouseWheelAction::DOWN && ev.wheel.y <= MouseWheelAction::DOWN )
    {
      return true;
    }
  }

  // Initialized to an invalid state!
  NOM_ASSERT( mapping.wheel->x == 0 || mapping.wheel->y == 0 );

  return false;
}

bool InputStateMapper::on_joystick_button( const InputAction& mapping, const Event& ev )
{
  if( mapping.jbutton == nullptr ) return false;

  if( mapping.jbutton->type != ev.type ) return false;

  if( mapping.jbutton->id != ev.jbutton.id ) return false;

  if( mapping.jbutton->button == ev.jbutton.button ) return true;

  return false;
}

// FIXME: Implementation is incomplete!
bool InputStateMapper::on_joystick_axis( const InputAction& mapping, const Event& ev )
{
  if( mapping.jaxis == nullptr ) return false;

  if( mapping.jaxis->type != ev.type ) return false;

  if( mapping.jaxis->id != ev.jaxis.id ) return false;

  if( mapping.jaxis->axis != ev.jaxis.axis ) return false;

  // Within dead-zone tolerance
  if( ( ev.jaxis.value < -3200 ) || ( ev.jaxis.value > 3200 ) )
  {
    // Up-down axis (Sony PS3 game controller)
    if( mapping.jaxis->axis == 0 )
    {
      // Up
      if( mapping.jaxis->value < 0 ) return true;

      // Down
      if( mapping.jaxis->value > 0 ) return true;
    }

    // Left-right axis (Sony PS3 game controller)
    if( mapping.jaxis->axis == 1 )
    {
      // Left
      if( mapping.jaxis->value < 0 ) return true;

      // Right
      if( mapping.jaxis->value > 0 ) return true;
    }
  }

  return false;
}

} // namespace nom
