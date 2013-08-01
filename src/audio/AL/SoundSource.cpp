/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/audio/AL/SoundSource.hpp"

namespace nom {
  namespace OpenAL {

SoundSource::SoundSource ( void )
{
NOM_LOG_CLASSINFO;

AL_CHECK_ERR ( alGenSources ( 1, &source_id ) );
AL_CHECK_ERR ( alSourcei ( source_id, AL_BUFFER, 0 ) );
}

SoundSource::~SoundSource ( void )
{
NOM_LOG_CLASSINFO;

AL_CHECK_ERR ( alSourcei ( source_id, AL_BUFFER, 0 ) );
AL_CHECK_ERR ( alDeleteSources ( 1, &source_id ) );
}

float SoundSource::getVolume ( void ) const
{
  ALfloat volume;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_GAIN, &volume ) );

  return volume * 100.f;
}

float SoundSource::getMinVolume ( void ) const
{
  ALfloat min_volume;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_MIN_GAIN, &min_volume ) );

  return min_volume;
}

float SoundSource::getMaxVolume ( void ) const
{
  ALfloat max_volume;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_MAX_GAIN, &max_volume ) );

  return max_volume;
}

float SoundSource::getPitch ( void ) const
{
  ALfloat pitch;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_PITCH, &pitch ) );

  return pitch;
}

bool SoundSource::getLooping ( void ) const
{
  ALint looping;

AL_CHECK_ERR ( alGetSourcei ( source_id, AL_LOOPING, &looping ) );

  switch ( looping )
  {
    case AL_TRUE: return true; break;
    case AL_FALSE: return false; break;
  }

  return false;
}

Vector3f SoundSource::getPosition ( void ) const
{
  Vector3f position;

AL_CHECK_ERR  ( alGetSource3f ( source_id, AL_POSITION, &position.x, &position.y,
                          &position.z )
        );

  return position;
}

Vector3f SoundSource::getVelocity ( void ) const
{
  Vector3f velocity;

AL_CHECK_ERR  (  alGetSource3f  ( source_id, AL_VELOCITY, &velocity.x, &velocity.y,
                            &velocity.z )
        );

  return velocity;
}

bool SoundSource::getPositionRelativeToListener ( void ) const
{
  ALint relative;

AL_CHECK_ERR ( alGetSourcei ( source_id, AL_SOURCE_RELATIVE, &relative ) );

  return relative;
}

float SoundSource::getMinDistance ( void ) const
{
  ALfloat distance;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_REFERENCE_DISTANCE, &distance ) );

  return distance;
}

float SoundSource::getAttenuation ( void ) const
{
  ALfloat attenuation;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_ROLLOFF_FACTOR, &attenuation ) );

  return attenuation;
}

int32 SoundSource::getBufferID ( void ) const
{
  ALint buffer_id;

AL_CHECK_ERR ( alGetSourcei ( source_id, AL_BUFFER, &buffer_id ) );

  return buffer_id;
}

float SoundSource::getPlayPosition ( void ) const
{
  ALfloat playback_position;

AL_CHECK_ERR ( alGetSourcef ( source_id, AL_SEC_OFFSET, &playback_position ) );

  return playback_position;
}

SoundStatus SoundSource::getStatus ( void ) const
{
  ALint state;

AL_CHECK_ERR ( alGetSourcei ( source_id, AL_SOURCE_STATE, &state ) );

  switch ( state )
  {
    case AL_INITIAL:
    case AL_STOPPED: return Stopped;
    case AL_PAUSED: return Paused;
    case AL_PLAYING: return Playing;
  }

  return Stopped;
}

void SoundSource::setVolume ( float gain )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_GAIN, gain * 0.01f ) );
}

void SoundSource::setMinVolume ( float gain )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_MIN_GAIN, gain ) );
}

void SoundSource::setMaxVolume ( float gain )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_MAX_GAIN, gain ) );
}

void SoundSource::setPitch ( float pitch )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_PITCH, pitch ) );
}

void SoundSource::setLooping ( bool loops )
{
AL_CHECK_ERR ( alSourcei ( source_id, AL_LOOPING, loops ) );
}

void SoundSource::setPosition ( float x, float y, float z )
{
AL_CHECK_ERR ( alSource3f ( source_id, AL_POSITION, x, y, z ) );
}

void SoundSource::setPosition ( const Vector3f& position )
{
  this->setPosition ( position.x, position.y, position.z );
}

void SoundSource::setVelocity ( float x, float y, float z )
{
AL_CHECK_ERR ( alSource3f ( source_id, AL_VELOCITY, x, y, z ) );
}

void SoundSource::setVelocity ( const Vector3f& velocity )
{
  this->setVelocity ( velocity.x, velocity.y, velocity.z );
}

void SoundSource::setPositionRelativeToListener ( bool relative )
{
AL_CHECK_ERR ( alSourcei ( source_id, AL_SOURCE_RELATIVE, relative ) );
}

void SoundSource::setMinDistance ( float distance )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_REFERENCE_DISTANCE, distance ) );
}

void SoundSource::setAttenuation ( float attenuation )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_ROLLOFF_FACTOR, attenuation ) );
}

void SoundSource::setPlayPosition ( float seconds )
{
AL_CHECK_ERR ( alSourcef ( source_id, AL_SEC_OFFSET, seconds ) );
}


  } // namespace OpenAL
} // namespace nom
