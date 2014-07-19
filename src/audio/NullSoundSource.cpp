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
#include "nomlib/audio/NullSoundSource.hpp"

namespace nom {

NullSoundSource::NullSoundSource( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, SDL_LOG_PRIORITY_VERBOSE );
}

NullSoundSource::~NullSoundSource( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, SDL_LOG_PRIORITY_VERBOSE );
}

float NullSoundSource::getVolume( void ) const
{
  return 0.0f;
}

float NullSoundSource::getMinVolume( void ) const
{
  return 0.0f;
}

float NullSoundSource::getMaxVolume( void ) const
{
  return 0.0f;
}

float NullSoundSource::getPitch( void ) const
{
  return 0.0f;
}

bool NullSoundSource::getLooping( void ) const
{
  return false;
}

Point3f NullSoundSource::getPosition( void ) const
{
  return Point3f( 0.0f, 0.0f, 0.0f );
}

Point3f NullSoundSource::getVelocity( void ) const
{
  return Point3f( 0.0f, 0.0f, 0.0f );
}

bool NullSoundSource::getPositionRelativeToListener( void ) const
{
  return false;
}

float NullSoundSource::getMinDistance( void ) const
{
  return 0.0f;
}

float NullSoundSource::getAttenuation( void ) const
{
  return 0.0f;
}

int32 NullSoundSource::getBufferID( void ) const
{
  return -1;
}

float NullSoundSource::getPlayPosition( void ) const
{
  return 0.0f;
}

SoundStatus NullSoundSource::getStatus( void ) const
{
  return SoundStatus::Stopped;
}

void NullSoundSource::setVolume( float gain )
{
  // Do nothing
}

void NullSoundSource::setMinVolume( float gain )
{
  // Do nothing
}

void NullSoundSource::setMaxVolume( float gain )
{
  // Do nothing
}

void NullSoundSource::setPitch( float pitch )
{
  // Do nothing
}

void NullSoundSource::setLooping( bool loops )
{
  // Do nothing
}

void NullSoundSource::setPosition( float x, float y, float z )
{
  // Do nothing
}

void NullSoundSource::setPosition( const Point3f& position )
{
  // Do nothing
}

void NullSoundSource::setVelocity( float x, float y, float z )
{
  // Do nothing
}

void NullSoundSource::setVelocity( const Point3f& velocity )
{
  // Do nothing
}

void NullSoundSource::setPositionRelativeToListener( bool relative )
{
  // Do nothing
}

void NullSoundSource::setMinDistance( float distance )
{
  // Do nothing
}

void NullSoundSource::setAttenuation( float attenuation )
{
  // Do nothing
}

void NullSoundSource::setPlayPosition( float seconds )
{
  // Do nothing
}

void NullSoundSource::togglePause( void )
{
  // Do nothing
}

void NullSoundSource::fadeOut( float seconds )
{
  // Do nothing
}

} // namespace nom
