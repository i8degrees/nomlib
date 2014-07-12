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

Joystick::Joystick( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_EVENT );

  this->impl_ = IJoystick::UniquePtr ( new SDLJoystick() );
}

Joystick::~Joystick( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_EVENT );

  this->shutdown();
}

bool Joystick::initialize( void )
{
  if( this->impl_ && this->impl_->initialize() ) return true;

  return false;
}

void Joystick::shutdown( void )
{
  if( this->impl_ ) this->impl_->shutdown();
}

Joystick::JoystickID Joystick::first_joystick( void ) const
{
  auto itr = this->joysticks_.begin();

  return itr->first;
}

Joystick::JoystickID Joystick::last_joystick( void ) const
{
  auto itr = this->joysticks_.end();

  return itr->first;
}

int Joystick::num_joysticks( void ) const
{
  if( this->impl_ )
  {
    return this->impl_->num_joysticks();
  }

  return 0;
}

const std::string& Joystick::name( JoystickID idx )
{
  return this->joysticks_[idx];
}

const Joystick::JoystickNames Joystick::names( void ) const
{
  JoystickNames joysticks;

  for( auto itr = this->joysticks_.begin(); itr != this->joysticks_.end(); ++itr )
  {
    joysticks.push_back( itr->second );
  }

  return joysticks;
}

void Joystick::enumerate_devices( void )
{
  int num_joysticks = 0;

  if( ! impl_ )
  {
    NOM_LOG_ERR( NOM, "No joysticks are available: impl_ is NULL." );
    return;
  }

  num_joysticks = this->impl_->num_joysticks();

  NOM_LOG_INFO( NOM_LOG_CATEGORY_EVENT, num_joysticks, "joysticks were found" );

  for( uint idx = 0; idx < num_joysticks; ++idx )
  {
    if( this->impl_->open( idx ) )
    {
      NOM_LOG_INFO( NOM_LOG_CATEGORY_EVENT, this->impl_->name() );

      Joystick::Pair p( this->impl_->id(), this->impl_->name() );
      this->joysticks_.insert( p );

      // if( res == this->joysticks_.end() )
      // {
      //   NOM_LOG_ERR( NOM, "Could not insert joystick into map." );
      // }
    }
  }
}

} // namespace nom
