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
#include "nomlib/system/SDLJoystick.hpp"

namespace nom {

SDLJoystick::SDLJoystick( void ) :
  device_{ SDLJoystick::UniquePtr( nullptr, priv::Free_Joystick ) }
{
  // NOM_LOG_TRACE( NOM );
}

SDLJoystick::~SDLJoystick( void )
{
  // NOM_LOG_TRACE( NOM );

  priv::Free_Joystick( this->device_.get() );

  SDLJoystick::shutdown();
}

bool SDLJoystick::initialize( void )
{
  if ( SDL_WasInit( SDL_INIT_JOYSTICK ) == false )
  {
    if ( SDL_InitSubSystem( SDL_INIT_JOYSTICK ) < 0 )
    {
      NOM_LOG_ERR( NOM, SDL_GetError() );
      return false;
    }
  }

  if( this->set_event_state( SDL_ENABLE ) != 1 )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );

    return false;
  }

  return true;
}

void SDLJoystick::shutdown( void )
{
  // Implemented in nom::quit; see src/system/init.cpp
  //
  // FIXME?:
  //
  // We cannot shutdown the system here due to issues with the joystick object
  // we have in EventHandler being destructed in between game states.
}

int SDLJoystick::num_joysticks( void ) const
{
  int num_joysticks = 0;

  num_joysticks = SDL_NumJoysticks();

  if( num_joysticks < 0 )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
  }

  return num_joysticks;
}

bool SDLJoystick::attached( void ) const
{
  SDL_bool ret;

  ret = SDL_JoystickGetAttached( this->device_.get() );

  if( ret == SDL_TRUE ) return true;

  // SDL_FALSE
  return false;
}

int SDLJoystick::id( void ) const
{
  SDL_JoystickID device_id;

  device_id = SDL_JoystickInstanceID( this->device_.get() );

  if( device_id < 0 )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
  }

  return device_id;
}

const std::string SDLJoystick::name( void ) const
{
  std::string device_name = SDL_JoystickName( this->device_.get() );

  if( device_name.length() < 1 )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
  }

  return device_name;
}

bool SDLJoystick::open( int idx )
{
  SDL_Joystick* device = nullptr;

  device = SDL_JoystickOpen( idx );

  if( device != nullptr )
  {
    this->device_.reset( device );

    return true;
  }

  // Not open?
  return false;
}

void SDLJoystick::close( void )
{
  SDL_JoystickClose( this->device_.get() );
}

SDL_JoystickGUID SDLJoystick::device_guid( int idx ) const
{
  SDL_JoystickGUID id;

  id = SDL_JoystickGetDeviceGUID( idx );

  // if( id.data[0] == 0 )
  // {
  //   NOM_LOG_ERR( NOM, SDL_GetError() );
  // }

  return id;
}

SDL_JoystickGUID SDLJoystick::device_guid( void ) const
{
  SDL_JoystickGUID id;

  id = SDL_JoystickGetGUID( this->device_.get() );

  // if( id.data[0] == 0 )
  // {
  //   NOM_LOG_ERR( NOM, SDL_GetError() );
  // }

  return id;
}

const std::string SDLJoystick::device_guid_string( void ) const
{
  const uint GUID_STRING_LENGTH = 64;
  char guid_string[GUID_STRING_LENGTH];

  SDL_JoystickGUID id;

  if( this->attached() )
  {
    id = this->device_guid();

    SDL_JoystickGetGUIDString( id, guid_string, GUID_STRING_LENGTH );

    return std::string( guid_string );
  }

  NOM_LOG_ERR( NOM, "Could not obtain joystick device GUID: device is not open." );

  return "\0";
}

// FIXME
bool SDLJoystick::game_controller( void ) const
{
  SDL_bool ret;
  int id = 0;

  id = this->id();

  if( id >= 0 )
  {
    ret = SDL_IsGameController( id );

    if( ret == SDL_TRUE )
    {
      return true;
    }
  }

  // SDL_FALSE
  return false;
}

int SDLJoystick::set_event_state( int state ) const
{
  int ret = 0;

  ret = SDL_JoystickEventState( state );

  if( ret < 0 )
  {
    NOM_LOG_ERR( NOM, SDL_GetError() );
  }

  return ret;
}

void SDLJoystick::update( void ) const
{
  SDL_JoystickUpdate();
}

namespace priv {

void Free_Joystick( SDL_Joystick* joy )
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
