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
#include "nomlib/system/events/InputMapper.hpp"

namespace nom {

InputMapper::InputMapper( void )
{
  // NOM_LOG_TRACE( NOM );
}

InputMapper::~InputMapper( void )
{
  // NOM_LOG_TRACE( NOM );
}

const InputMapping& InputMapper::get( void ) const
{
  return this->input_map_;
}

bool InputMapper::add_input_mapping( const std::string& key, const InputAction& action )
{
  // First, we must initialize our input map if this is the first insertion.
  // if( ! this->valid_input_map() )
  // {
    // this->input_map_ = std::unique_ptr<InputMapping> ( new InputMapping() );
  // }

  // Second, if we have an existing input map key, we must remove it, otherwise
  // we segfault when jumping through game states.
  auto itr = this->input_map_.find( key );

  // Err; no match found!
  if( itr != this->input_map_.end() )
  {
    // Match found; remove the entry before adding a new one.
    // if( itr->second.callback.valid() )
    // {
      // this->remove_input_mapping( key );
    // }
  }

  // Finally, we can insert the new input mapping.
  InputMapping::const_iterator it = this->input_map_.insert( InputAction::Pair( key, action ) );

  // No addition; the insertion failed???
  if( it == this->input_map_.end() )
  {
    NOM_LOG_ERR( NOM, "ERROR: Could not insert nom::Action with key: " + std::string( key ) );
    return false;
  }

  return true;
}

bool InputMapper::remove_input_mapping( const std::string& key )
{
  // if( this->valid_input_map() )
  // {
    InputMapping::const_iterator itr = this->input_map_.find( key );

    // No match found
    if( itr == this->input_map_.end() )
    {
      return false;
    }
    else // Match found; remove this input mapping
    {
      this->input_map_.erase( itr );

      return true;
    }
  // }

  return false;
}

/*
bool InputMapper::valid_input_map( void )
{
  if( this->input_map_.get() != nullptr ) return true;

  return false;
}
*/

/*
bool InputMapper::on_input( const SDL_Event* ev )
{
  // if( this->valid_input_map() )
  // {
    for( InputMapping::const_iterator itr = this->input_map_.begin(); itr != this->input_map_.end(); ++itr )
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
  // }

  return false;
}
*/

} // namespace nom
