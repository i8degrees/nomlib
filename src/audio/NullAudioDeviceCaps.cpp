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
#include "nomlib/audio/NullAudioDeviceCaps.hpp"

// Private headers
#include "nomlib/audio/audio_defs.hpp"

// FIXME(jeff): enums
#include "nomlib/audio/AL/SoundSource.hpp"

namespace nom {
namespace audio {

NullAudioEngineCaps::NullAudioEngineCaps()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);
}

NullAudioEngineCaps::~NullAudioEngineCaps()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);
}

uint32
NullAudioEngineCaps::channel_format(uint32 num_channels, uint32 channel_format)
{
  channel_format = 0;
  return channel_format;
}

bool NullAudioEngineCaps::valid_audio_buffer(SoundBuffer* buffer)
{
  bool valid = false;
  return valid;
}

bool NullAudioEngineCaps::valid_sound_buffer(SoundBuffer* buffer)
{
  bool valid = false;
  return valid;
}

uint32 NullAudioEngineCaps::state(SoundBuffer* buffer)
{
  uint32 result = AUDIO_STATE_STOPPED;
  return result;
}
real32 NullAudioEngineCaps::pitch(SoundBuffer* buffer)
{
  auto pitch = 0.0f;
  return pitch;
}

real32 NullAudioEngineCaps::volume() const
{
  auto gain_level = nom::audio::MIN_VOLUME;
  return gain_level;
}

Point3f NullAudioEngineCaps::position() const
{
  // Defaults as per /System/Library/Frameworks/OpenAL/Headers/al.h
  Point3f p(0.0f, 0.0f, 0.0f);
  return p;
}

real32 NullAudioEngineCaps::volume(SoundBuffer* buffer) const
{
  real32 gain_level = nom::audio::MIN_VOLUME;
  return gain_level;
}

real32 NullAudioEngineCaps::min_volume(SoundBuffer* buffer)
{
  real32 min_gain = nom::audio::MIN_VOLUME;
  return min_gain;
}

real32 NullAudioEngineCaps::max_volume(SoundBuffer* buffer)
{
  real32 max_gain = nom::audio::MIN_VOLUME;
  return max_gain;
}

Point3f NullAudioEngineCaps::velocity(SoundBuffer* buffer)
{
  Point3f v(Point3f::zero);
  return v;
}

Point3f NullAudioEngineCaps::position(SoundBuffer* buffer)
{
  Point3f p(Point3f::zero);
  return p;
}

real32 NullAudioEngineCaps::playback_position(SoundBuffer* buffer)
{
  real32 pos = 0.0f;
  return pos;
}

real32 NullAudioEngineCaps::playback_samples(SoundBuffer* buffer)
{
  real32 samples = 0.0f;
  return samples;
}

void NullAudioEngineCaps::set_state(SoundBuffer* target, uint32 state)
{

}

void NullAudioEngineCaps::set_volume(real32 gain)
{

}

void NullAudioEngineCaps::set_position(const Point3f& p)
{
}

void NullAudioEngineCaps::set_volume(SoundBuffer* target, real32 gain)
{

}

void NullAudioEngineCaps::set_min_volume(SoundBuffer* target, real32 gain)
{

}

void NullAudioEngineCaps::set_max_volume(SoundBuffer* target, real32 gain)
{

}

void NullAudioEngineCaps::set_velocity(SoundBuffer* target, const Point3f& v)
{

}

void NullAudioEngineCaps::set_position(SoundBuffer* target, const Point3f& p)
{

}

void NullAudioEngineCaps::set_pitch(SoundBuffer* buffer, real32 pitch)
{

}

void NullAudioEngineCaps::set_playback_position(SoundBuffer* target,
                                              real32 offset_seconds)
{

}

void NullAudioEngineCaps::play(SoundBuffer* buffer)
{

}

void NullAudioEngineCaps::stop(SoundBuffer* buffer)
{

}

void NullAudioEngineCaps::pause(SoundBuffer* buffer)
{

}

void NullAudioEngineCaps::resume(SoundBuffer* buffer)
{

}

bool NullAudioEngineCaps::fill_audio_buffer(SoundBuffer* buffer)
{
  bool result = false;
  return result;
}

void NullAudioEngineCaps::free_buffer(SoundBuffer* buffer)
{

}

} // namespace audio
} // namespace nom
