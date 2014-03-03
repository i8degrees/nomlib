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
#include "nomlib/system/Joystick.hpp"

namespace nom {

Joystick::Joystick( void ) :
  joystick_{ Joystick::JoystickUniquePtr( nullptr, priv::Free_Joystick ) },
  joystick_id_( 0 )
{
  // NOM_LOG_TRACE( NOM );
}

Joystick::~Joystick( void )
{
  // NOM_LOG_TRACE( NOM );
}

bool Joystick::initialize( void )
{
  if ( SDL_WasInit( SDL_INIT_JOYSTICK ) == false )
  {
    if ( SDL_InitSubSystem( SDL_INIT_JOYSTICK ) < 0 )
    {
      NOM_LOG_ERR( NOM, SDL_GetError() );
      return false;
    }
  }

  std::string num_joysticks = std::to_string( this->num_joysticks() );

  NOM_LOG_INFO( NOM, num_joysticks + " joysticks were found" );

  if( this->enumerate_devices() == false )
  {
    return false;
  }

  return true;
}

SDL_JoystickID Joystick::id( void ) const
{
  return this->joystick_id_;
}

int Joystick::num_joysticks( void ) const
{
  return SDL_NumJoysticks();
}

const std::string Joystick::name( void ) const
{
  NOM_STUBBED( NOM );

  return "\0";
}

bool Joystick::enumerate_devices( void )
{
  if ( this->num_joysticks() > 0 )
  {
    this->joystick_.reset( SDL_JoystickOpen( 0 ) );

    if ( this->joystick_.get() )
    {
      for( int idx = 0; idx < this->num_joysticks(); ++idx )
      {
        NOM_LOG_INFO( NOM, SDL_JoystickNameForIndex( idx ) );
      }

      SDL_JoystickEventState( SDL_ENABLE );

      this->joystick_id_ = SDL_JoystickInstanceID( this->joystick_.get() );
    }

    return true;
  }

  // No devices available!
  return false;
}

namespace priv {

void Free_Joystick ( SDL_Joystick* joy )
{
  if ( joy != nullptr )
  {
    if ( SDL_JoystickGetAttached( joy ) )
    {
      SDL_JoystickClose( joy );
      joy = nullptr;
    }
  }
}

} // namespace priv

} // namespace nom
