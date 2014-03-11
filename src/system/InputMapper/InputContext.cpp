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
#include "nomlib/system/InputMapper/InputContext.hpp"

namespace nom {

InputContext::InputContext( void )
{
  // NOM_LOG_TRACE( NOM );
}

InputContext::~InputContext( void )
{
  // NOM_LOG_TRACE( NOM );
}

// bool InputContext::add_input_mapping( const std::string& key, const InputAction& action )
// {
//   InputMapping::const_iterator it = this->input_map_.insert( InputAction::Pair( key, action ) );

//   // No dice; the insertion failed for some reason!
//   if( it == this->input_map_.end() )
//   {
//     NOM_LOG_ERR( NOM, "ERROR: Could not insert nom::Action with key: " + std::string( key ) );
//     return false;
//   }

//   return true;
// }

// bool InputContext::remove_input_mapping( const std::string& key )
// {
//   InputMapping::const_iterator itr = this->input_map_.find( key );

//   // No match found
//   if( itr == this->input_map_.end() )
//   {
//     return false;
//   }
//   else // Match found; remove this input mapping
//   {
//     this->input_map_.erase( itr );

//     return true;
//   }

//   return false;
// }

bool InputContext::active( const std::string& state ) const
{
  ContextMap::const_iterator itr = this->contexts_.find( state );

  // No match found; do nothing.
  if( itr == this->contexts_.end() )
  {
    NOM_LOG_ERR( NOM, "Could not check for active state on the specified key: " + state );
    return false;
  }
  else
  {
    return itr->second.active_;
  }
}

bool InputContext::add_context( const std::string& state, const InputMapping& map, bool active )
{
  InputState input_state;
  input_state.active_ = active;
  input_state.actions_ = map;
  auto it = this->contexts_.insert( Pair( state, input_state ) );

  // No dice; the insertion failed for some reason!
  if( it.second == false )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not insert nom::InputMapping state: " + std::string( state ) );
    return false;
  }

  return true;
}

bool InputContext::remove_context( const std::string& state )
{
  ContextMap::const_iterator itr = this->contexts_.find( state );

  // No match found; do nothing.
  if( itr == this->contexts_.end() )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not remove specified state: " + state );
    return false;
  }
  else // Match found; remove the context mapping
  {
    this->contexts_.erase( itr );
    return true;
  }

  return false;
}

bool InputContext::activate_context( const std::string& state )
{
  ContextMap::iterator itr = this->contexts_.find( state );

  // No match found; do nothing.
  if( itr == this->contexts_.end() )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not activate specified state: " + state );
    return false;
  }
  else // Match found; set the input mapping as the active context.
  {
    itr->second.active_ = true;

    return true;
  }

  return false;
}

bool InputContext::disable_context( const std::string& state )
{
  ContextMap::iterator itr = this->contexts_.find( state );

  // No match found; do nothing.
  if( itr == this->contexts_.end() )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not disable specified state: " + state );
    return false;
  }
  else // Match found; disable the requested state
  {
    itr->second.active_ = false;
    return true;
  }

  return false;
}

void InputContext::clear( void )
{
  this->contexts_.clear();
}

// void InputContext::dump_actions( void ) const
// {
//   for( InputMapping::const_iterator itr = this->input_map_.begin(); itr != this->input_map_.end(); ++itr )
//   {
//     // FIXME
//     if( itr->second.key != nullptr )
//     {
//       itr->second.key->dump();
//     }
//     else if ( itr->second.mouse != nullptr )
//     {
//       itr->second.mouse->dump();
//     }
//     else if ( itr->second.jbutton != nullptr )
//     {
//       itr->second.jbutton->dump();
//     }
//     else
//     {
//       NOM_LOG_ERR( NOM, "Invalid input mapping state." );
//     }
//   }
// }

void InputContext::dump_states( void )
{
  for( auto itr = this->contexts_.begin(); itr != this->contexts_.end(); ++itr )
  {
    NOM_DUMP( itr->first );

    if( itr->second.active_ == true )
    {
      InputMapping& input_map = itr->second.actions_;

      for( InputMapping::const_iterator itr = input_map.begin(); itr != input_map.end(); ++itr )
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

void InputContext::on_event( const Event& ev )
{
  for( auto itr = this->contexts_.begin(); itr != this->contexts_.end(); ++itr )
  {
    if( itr->second.active_ == true )
    {
      InputMapping input_map = itr->second.actions_;
      for( InputMapping::const_iterator itr = input_map.begin(); itr != input_map.end(); ++itr )
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

bool InputContext::on_key_press( const InputAction& mapping, const Event& ev )
{
  if( mapping.key == nullptr ) return false;

  if( ev.type != SDL_KEYDOWN && ev.type != SDL_KEYUP ) return false;

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

bool InputContext::on_mouse_button( const InputAction& mapping, const Event& ev )
{
  if( mapping.mouse == nullptr ) return false;

  if( ev.type != SDL_MOUSEBUTTONDOWN && ev.type != SDL_MOUSEBUTTONUP )
  {
    return false;
  }

  if( mapping.mouse->button == ev.mouse.button ) return true;

  return false;
}

bool InputContext::on_mouse_wheel( const InputAction& mapping, const Event& ev )
{
  if( mapping.wheel == nullptr ) return false;

  if( ev.type != SDL_MOUSEWHEEL ) return false;

  // NOTE: X & Y coordinate values depend on the construction of a WheelAction
  // object.
  //
  // Note that all values are inverted here, despite the documentation on the
  // SDL2 wiki, on my test systems (Windows 7 & Mac OS X v10.9.x with a Logitech
  // and Apple Mighty Mouse).
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

bool InputContext::on_joystick_button( const InputAction& mapping, const Event& ev )
{
  if( mapping.jbutton == nullptr ) return false;

  if( ev.type != SDL_JOYBUTTONDOWN && ev.type != SDL_JOYBUTTONUP ) return false;

  if( mapping.jbutton->id != ev.jbutton.id ) return false;

  if( mapping.jbutton->button == ev.jbutton.button ) return true;

  return false;
}

// FIXME: Implementation is incomplete!
bool InputContext::on_joystick_axis( const InputAction& mapping, const Event& ev )
{
  if( mapping.jaxis == nullptr ) return false;

  if( ev.type != SDL_JOYAXISMOTION ) return false;

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
