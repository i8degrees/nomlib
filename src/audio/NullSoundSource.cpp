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

const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;

NullSoundSource::NullSoundSource()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );
}

NullSoundSource::~NullSoundSource()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );
}

real32 NullSoundSource::volume() const
{
  auto gain_level = nom::MAX_VOLUME;

  return gain_level;
}

real32 NullSoundSource::min_volume() const
{
  auto min_gain = nom::MIN_VOLUME;

  return min_gain;
}

real32 NullSoundSource::max_volume() const
{
  auto max_gain = nom::MAX_VOLUME;

  return max_gain;
}

real32 NullSoundSource::getPitch() const
{
  return 0.0f;
}

bool NullSoundSource::getLooping() const
{
  return false;
}

Point3f NullSoundSource::position() const
{
  auto result = Point3f::zero;

  return(result);
}

Point3f NullSoundSource::velocity() const
{
  auto result = Point3f::zero;

  return result;
}

bool NullSoundSource::getPositionRelativeToListener() const
{
  return false;
}

real32 NullSoundSource::getMinDistance() const
{
  return 0.0f;
}

real32 NullSoundSource::getAttenuation() const
{
  return 0.0f;
}

int32 NullSoundSource::getBufferID() const
{
  return -1;
}

real32 NullSoundSource::getPlayPosition() const
{
  return 0.0f;
}

SoundStatus NullSoundSource::getStatus() const
{
  return SoundStatus::Stopped;
}

void NullSoundSource::set_volume(real32 gain)
{
  // Do nothing
}

void NullSoundSource::set_min_volume(real32 gain)
{
  // Do nothing
}

void NullSoundSource::set_max_volume(real32 gain)
{
  // Do nothing
}

void NullSoundSource::setPitch( real32 pitch )
{
  // Do nothing
}

void NullSoundSource::setLooping( bool loops )
{
  // Do nothing
}

void NullSoundSource::setPosition( real32 x, real32 y, real32 z )
{
  // Do nothing
}

void NullSoundSource::setPosition( const Point3f& position )
{
  // Do nothing
}

void NullSoundSource::set_velocity(real32 x, real32 y, real32 z)
{
  // Do nothing
}

void NullSoundSource::set_velocity(const Point3f& velocity)
{
  // Do nothing
}

void NullSoundSource::setPositionRelativeToListener( bool relative )
{
  // Do nothing
}

void NullSoundSource::setMinDistance( real32 distance )
{
  // Do nothing
}

void NullSoundSource::setAttenuation( real32 attenuation )
{
  // Do nothing
}

void NullSoundSource::setPlayPosition( real32 seconds )
{
  // Do nothing
}

void NullSoundSource::togglePause()
{
  // Do nothing
}

} // namespace nom
