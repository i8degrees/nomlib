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
#include "nomlib/audio/AL/SoundSource.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/core/clock.hpp"

namespace nom {

const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;

SoundSource::SoundSource()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

SoundSource::~SoundSource()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

real32 SoundSource::volume() const
{
  real32 gain_level = nom::MAX_VOLUME;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_GAIN, &gain_level);
  AL_CHECK_ERR_VOID();

  // De-normalize; 0..1 -> 0..100
  gain_level = gain_level * 100.0f;

  return(gain_level);
}

real32 SoundSource::min_volume() const
{
  real32 min_gain = nom::MIN_VOLUME;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_MIN_GAIN, &min_gain);
  AL_CHECK_ERR_VOID();

  // De-normalize; 0..1 -> 0..100
  min_gain = min_gain * 100.0f;

  return(min_gain);
}

real32 SoundSource::max_volume() const
{
  real32 max_gain = nom::MAX_VOLUME;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_MAX_GAIN, &max_gain);
  AL_CHECK_ERR_VOID();

  // De-normalize; 0..1 -> 0..100
  max_gain = max_gain * 100.0f;

  return(max_gain);
}

real32 SoundSource::getPitch() const
{
  auto pitch = 1.0f;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_PITCH, &pitch);
  AL_CHECK_ERR_VOID();

  return pitch;
}

bool SoundSource::getLooping() const
{
  ALint looping;

  AL_CLEAR_ERR();
  alGetSourcei(this->source_id_, AL_LOOPING, &looping);
  AL_CHECK_ERR_VOID();

  switch(looping)
  {
    case AL_TRUE: return true; break;
    case AL_FALSE: return false; break;
  }

  return false;
}

Point3f SoundSource::position() const
{
  Point3f p;

  AL_CLEAR_ERR();
  alGetSource3f(this->source_id_, AL_POSITION, &p.x, &p.y, &p.z);
  AL_CHECK_ERR_VOID();

  return p;
}

Point3f SoundSource::velocity() const
{
  Point3f v(Point3f::zero);

  AL_CLEAR_ERR();
  alGetSource3f(this->source_id_, AL_VELOCITY, &v.x, &v.y, &v.z);
  AL_CHECK_ERR_VOID();

  return v;
}

bool SoundSource::getPositionRelativeToListener() const
{
  ALint relative;

  alGetSourcei(this->source_id_, AL_SOURCE_RELATIVE, &relative);
  AL_CHECK_ERR_VOID();

  return (relative != 0);
}

real32 SoundSource::getMinDistance() const
{
  ALfloat distance;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_REFERENCE_DISTANCE, &distance);
  AL_CHECK_ERR_VOID();

  return distance;
}

real32 SoundSource::getAttenuation() const
{
  real32 attenuation;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_ROLLOFF_FACTOR, &attenuation);
  AL_CHECK_ERR_VOID();

  return attenuation;
}

int32 SoundSource::getBufferID() const
{
  ALint buffer_id;

  AL_CLEAR_ERR();
  alGetSourcei(this->source_id_, AL_BUFFER, &buffer_id);
  AL_CHECK_ERR_VOID();

  return buffer_id;
}

real32 SoundSource::getPlayPosition() const
{
  ALfloat playback_position;

  AL_CLEAR_ERR();
  alGetSourcef(this->source_id_, AL_SEC_OFFSET, &playback_position);
  AL_CHECK_ERR_VOID();

  return playback_position;
}

SoundStatus SoundSource::getStatus() const
{
  ALint state;
  auto result = Stopped;

  AL_CLEAR_ERR();
  alGetSourcei(this->source_id_, AL_SOURCE_STATE, &state);
  AL_CHECK_ERR_VOID();

  switch(state)
  {
    default:
    case AL_INITIAL:
    case AL_STOPPED:
    {
      result = Stopped;
    } break;

    case AL_PAUSED:
    {
      result = Paused;
    } break;

    case AL_PLAYING:
    {
      result = Playing;
    } break;
  }

  return result;
}

void SoundSource::set_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {
    AL_CLEAR_ERR();

    // normalize the gain
    alSourcef(this->source_id_, AL_GAIN, gain * 0.01f);
    AL_CHECK_ERR_VOID();
  }
}

void SoundSource::set_min_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {
    AL_CLEAR_ERR();

    // normalize the gain
    alSourcef(this->source_id_, AL_MIN_GAIN, gain * 0.01f);
    AL_CHECK_ERR_VOID();
  }
}

void SoundSource::set_max_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {
    AL_CLEAR_ERR();

    // normalize the gain
    alSourcef(this->source_id_, AL_MAX_GAIN, gain * 0.01f);
    AL_CHECK_ERR_VOID();
  }
}

void SoundSource::setPitch(real32 pitch)
{
  AL_CLEAR_ERR();
  alSourcef(this->source_id_, AL_PITCH, pitch);
  AL_CHECK_ERR_VOID();
}

void SoundSource::setLooping ( bool loops )
{
  AL_CLEAR_ERR();
  alSourcei(this->source_id_, AL_LOOPING, loops);
  AL_CHECK_ERR_VOID();
}

void SoundSource::setPosition ( real32 x, real32 y, real32 z )
{
  AL_CLEAR_ERR();
  alSource3f(this->source_id_, AL_POSITION, x, y, z);
  AL_CHECK_ERR_VOID();
}

void SoundSource::setPosition ( const Point3f& position )
{
  this->setPosition ( position.x, position.y, position.z );
}

void SoundSource::set_velocity(real32 x, real32 y, real32 z)
{
  AL_CLEAR_ERR();
  alSource3f(this->source_id_, AL_VELOCITY, x, y, z);
  AL_CHECK_ERR_VOID();
}

void SoundSource::set_velocity(const Point3f& v)
{
  this->set_velocity(v.x, v.y, v.z);
}

void SoundSource::setPositionRelativeToListener ( bool relative )
{
  AL_CLEAR_ERR();
  alSourcei(this->source_id_, AL_SOURCE_RELATIVE, relative);
  AL_CHECK_ERR_VOID();
}

void SoundSource::setMinDistance(real32 distance)
{
  AL_CLEAR_ERR();
  alSourcef(this->source_id_, AL_REFERENCE_DISTANCE, distance);
  AL_CHECK_ERR_VOID();
}

void SoundSource::setAttenuation(real32 attenuation)
{
  AL_CLEAR_ERR();
  alSourcef(this->source_id_, AL_ROLLOFF_FACTOR, attenuation);
  AL_CHECK_ERR_VOID();
}

void SoundSource::setPlayPosition ( real32 seconds )
{
  AL_CLEAR_ERR();
  alSourcef(this->source_id_, AL_SEC_OFFSET, seconds);
  AL_CHECK_ERR_VOID();
}

void SoundSource::togglePause()
{
  if( this->getStatus() == SoundStatus::Paused ) {
    this->Play();
  } else if( this->getStatus() == SoundStatus::Playing ) {
    this->Pause();
  }
}

} // namespace nom
