/******************************************************************************

    OpenAL SoundSource

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SoundSource.hpp"

namespace nom {
  namespace OpenAL {

SoundSource::SoundSource ( void )
{
NOMLIB_LOG_INFO;

AL_ERR ( alGenSources ( 1, &source_id ) );
AL_ERR ( alSourcei ( source_id, AL_BUFFER, 0 ) );
}

SoundSource::~SoundSource ( void )
{
NOMLIB_LOG_INFO;

AL_ERR ( alSourcei ( source_id, AL_BUFFER, 0 ) );
AL_ERR ( alDeleteSources ( 1, &source_id ) );
}

float SoundSource::getVolume ( void ) const
{
  ALfloat volume;

AL_ERR ( alGetSourcef ( source_id, AL_GAIN, &volume ) );

  return volume * 100.f;
}

float SoundSource::getMinVolume ( void ) const
{
  ALfloat min_volume;

AL_ERR ( alGetSourcef ( source_id, AL_MIN_GAIN, &min_volume ) );

  return min_volume;
}

float SoundSource::getMaxVolume ( void ) const
{
  ALfloat max_volume;

AL_ERR ( alGetSourcef ( source_id, AL_MAX_GAIN, &max_volume ) );

  return max_volume;
}

float SoundSource::getPitch ( void ) const
{
  ALfloat pitch;

AL_ERR ( alGetSourcef ( source_id, AL_PITCH, &pitch ) );

  return pitch;
}

bool SoundSource::getLooping ( void ) const
{
  ALint looping;

AL_ERR ( alGetSourcei ( source_id, AL_LOOPING, &looping ) );

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

AL_ERR  ( alGetSource3f ( source_id, AL_POSITION, &position.x, &position.y,
                          &position.z )
        );

  return position;
}

Vector3f SoundSource::getVelocity ( void ) const
{
  Vector3f velocity;

AL_ERR  (  alGetSource3f  ( source_id, AL_VELOCITY, &velocity.x, &velocity.y,
                            &velocity.z )
        );

  return velocity;
}

bool SoundSource::getPositionRelativeToListener ( void ) const
{
  ALint relative;

AL_ERR ( alGetSourcei ( source_id, AL_SOURCE_RELATIVE, &relative ) );

  return relative;
}

float SoundSource::getMinDistance ( void ) const
{
  ALfloat distance;

AL_ERR ( alGetSourcef ( source_id, AL_REFERENCE_DISTANCE, &distance ) );

  return distance;
}

float SoundSource::getAttenuation ( void ) const
{
  ALfloat attenuation;

AL_ERR ( alGetSourcef ( source_id, AL_ROLLOFF_FACTOR, &attenuation ) );

  return attenuation;
}

int32 SoundSource::getBufferID ( void ) const
{
  ALint buffer_id;

AL_ERR ( alGetSourcei ( source_id, AL_BUFFER, &buffer_id ) );

  return buffer_id;
}

float SoundSource::getPlayPosition ( void ) const
{
  ALfloat playback_position;

AL_ERR ( alGetSourcef ( source_id, AL_SEC_OFFSET, &playback_position ) );

  return playback_position;
}

SoundStatus SoundSource::getStatus ( void ) const
{
  ALint state;

AL_ERR ( alGetSourcei ( source_id, AL_SOURCE_STATE, &state ) );

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
AL_ERR ( alSourcef ( source_id, AL_GAIN, gain * 0.01f ) );
}

void SoundSource::setMinVolume ( float gain )
{
AL_ERR ( alSourcef ( source_id, AL_MIN_GAIN, gain ) );
}

void SoundSource::setMaxVolume ( float gain )
{
AL_ERR ( alSourcef ( source_id, AL_MAX_GAIN, gain ) );
}

void SoundSource::setPitch ( float pitch )
{
AL_ERR ( alSourcef ( source_id, AL_PITCH, pitch ) );
}

void SoundSource::setLooping ( bool loops )
{
AL_ERR ( alSourcei ( source_id, AL_LOOPING, loops ) );
}

void SoundSource::setPosition ( float x, float y, float z )
{
AL_ERR ( alSource3f ( source_id, AL_POSITION, x, y, z ) );
}

void SoundSource::setPosition ( const Vector3f& position )
{
  this->setPosition ( position.x, position.y, position.z );
}

void SoundSource::setVelocity ( float x, float y, float z )
{
AL_ERR ( alSource3f ( source_id, AL_VELOCITY, x, y, z ) );
}

void SoundSource::setVelocity ( const Vector3f& velocity )
{
  this->setVelocity ( velocity.x, velocity.y, velocity.z );
}

void SoundSource::setPositionRelativeToListener ( bool relative )
{
AL_ERR ( alSourcei ( source_id, AL_SOURCE_RELATIVE, relative ) );
}

void SoundSource::setMinDistance ( float distance )
{
AL_ERR ( alSourcef ( source_id, AL_REFERENCE_DISTANCE, distance ) );
}

void SoundSource::setAttenuation ( float attenuation )
{
AL_ERR ( alSourcef ( source_id, AL_ROLLOFF_FACTOR, attenuation ) );
}

void SoundSource::setPlayPosition ( float seconds )
{
AL_ERR ( alSourcef ( source_id, AL_SEC_OFFSET, seconds ) );
}


  } // namespace OpenAL
} // namespace nom
