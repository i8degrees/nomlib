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
#include "nomlib/system/events/InputContext.hpp"

namespace nom {

InputContext::InputContext( void ) //:
  // active_ ( false )
{
  // NOM_LOG_TRACE( NOM );
}

InputContext::~InputContext( void )
{
  // NOM_LOG_TRACE( NOM );
}

bool InputContext::active( const std::string& state ) const
{
  ContextMap::const_iterator itr = this->contexts_.find( state );

  // No match found; do nothing.
  if( itr == this->contexts_.end() )
  {
    return false;
  }
  else
  {
    return itr->second.active_;
    // Match found; set the input mapping as the active context.
    // return true;
  }
}

// bool InputContext::add_context( const std::string& state, const InputMapping& map )
bool InputContext::add_context( const std::string& state, const InputMapping& map )
{
  // First, we must initialize our input map if this is the first insertion.
  // if( ! this->valid_input_map() )
  // {
    // this->input_map_ = std::unique_ptr<InputMapping> ( new InputMapping() );
  // }

  // Second, if we have an existing input map key, we must remove it, otherwise
  // we segfault when jumping through game states.
  // ContextMap::const_iterator itr = this->contexts_.find( state );

  // Err; no match found!
  // if( itr == this->contexts_.end() ) return false;

  // Finally, we can insert the new input mapping.
  InputState input_state;
  input_state.active_ = true;
  input_state.actions_ = map;
  auto it = this->contexts_.insert( Pair( state, input_state ) );

  // No addition; the insertion failed???
  if( it.second == false )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not insert nom::InputMapping state: " + std::string( state ) );
    return false;
  }

// if( this->activate_context( state ) == false )
// {
//   NOM_LOG_ERR( NOM, "ERROR: Could not activate nom::InputMapping state: " + std::string( state ) );
//   return false;
// }

  return true;
}

bool InputContext::remove_context( const std::string& state )
{
  // if( this->valid_input_map() )
  // {
    ContextMap::const_iterator itr = this->contexts_.find( state );

    // No match found; do nothing.
    if( itr == this->contexts_.end() )
    {
      return false;
    }
    else // Match found; remove the context mapping
    {
      this->contexts_.erase( itr );
      return true;
    }
  // }

  return false;
}

bool InputContext::activate_context( const std::string& state )
{
  ContextMap::iterator itr = this->contexts_.find( state );

  // No match found; do nothing.
  if( itr == this->contexts_.end() )
  {
    return false;
  }
  else // Match found; set the input mapping as the active context.
  {
    // this->active_context_ = itr->second;
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
    return false;
  }
  else // Match found; set the input mapping as the active context.
  {
    itr->second.active_ = false;
    return true;
  }

  return false;
}

bool InputContext::on_input( const SDL_Event* ev )
{
  // if( this->valid_input_map() )
  // {
    // InputMapping& input_map = this->contexts_["state0"];
  for( auto itr = this->contexts_.begin(); itr != this->contexts_.end(); ++itr )
  {
    if( itr->second.active_ == true )
    {
      InputMapping& input_map = itr->second.actions_;
      for( InputMapping::const_iterator itr = input_map.begin(); itr != input_map.end(); ++itr )
      {
        if(
            itr->second.type == ev->type  &&
            itr->second.event == ev->key.keysym.sym
          )
        {
          itr->second.callback();
          return true;
        }
        else if (
                  itr->second.type == ev->type &&
                  itr->second.event == ev->button.button
                )
        {
          itr->second.callback();
          return true;
        }
        else if (
                  itr->second.type == ev->type  &&
                  itr->second.event == ev->jbutton.button
                )
        {
          itr->second.callback();
          return true;
        }
      }
    }
  }
  // }

  return false;
}

} // namespace nom
