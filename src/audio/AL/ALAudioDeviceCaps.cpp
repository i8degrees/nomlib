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
// FIXME:
#include "ALAudioDeviceCaps.hpp"
// #include "ALAudioEngine.hpp"

#include "nomlib/audio/AL/OpenAL.hpp"

// Forward declarations
#include "SoundBuffer_priv.hpp"

namespace nom {

const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;

static
uint32 generate_buffer_id()
{
  uint32 buffer_id = 0;

  // Generate an audio buffer identifier from OpenAL
  AL_CLEAR_ERR();
  alGenBuffers(1, &buffer_id);
  AL_CHECK_ERR_VOID();

  return buffer_id;
}

static
uint32 generate_sound_id()
{
  uint32 source_id = 0;

  AL_CLEAR_ERR();
  alGenSources(1, &source_id);
  AL_CHECK_ERR_VOID();

  return source_id;
}

IOAudioEngine::~IOAudioEngine()
{
  //
}

// protected
IOAudioEngine::IOAudioEngine()
{
  //
}

ALAudioEngine::ALAudioEngine()
{
  //
}

ALAudioEngine::~ALAudioEngine()
{
  //
}

void ALAudioEngine::set_state(SoundBuffer* target, AudioState state)
{
  // normalize the current gain level to a number between 0..1
  if( target != nullptr ) {
    if( state == AUDIO_STATE_LOOPING ) {
      AL_CLEAR_ERR();
      alSourcei(target->sound_id, AL_LOOPING, state);
      AL_CHECK_ERR_VOID();
    } else {
      AL_CLEAR_ERR();
      alSourcei(target->sound_id, AL_SOURCE_STATE, state);
      AL_CHECK_ERR_VOID();
    }
  }
}

void ALAudioEngine::set_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {
    // normalize the current gain level
    auto normalized_gain = gain * 0.01f;

    AL_CLEAR_ERR();
    alListenerf(AL_GAIN, normalized_gain);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_volume(SoundBuffer* target, real32 gain)
{
  // normalize the current gain level to a number between 0..1
  if( target != nullptr ) {
    auto sound_id = target->sound_id;

    AL_CLEAR_ERR();
    alSourcef(sound_id, AL_GAIN, gain * 0.01f);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_velocity(SoundBuffer* target, const Point3f& v)
{
  if( target != nullptr ) {
    auto sound_id = target->sound_id;

    AL_CLEAR_ERR();
    alSource3f(sound_id, AL_VELOCITY, v.x, v.y, v.z);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_pitch(SoundBuffer* buffer, real32 pitch)
{
  if( buffer != nullptr ) {
    AL_CLEAR_ERR();
    alSourcef(buffer->sound_id, AL_PITCH, pitch);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_playback_position(SoundBuffer* target,
                                              real32 offset_seconds)
{
  if( target != nullptr ) {
    auto sound_id = target->sound_id;

    AL_CLEAR_ERR();
    alSourcef(sound_id, AL_SEC_OFFSET, offset_seconds);
    AL_CHECK_ERR_VOID();
  }
}

bool ALAudioEngine::fill_audio_buffer(SoundBuffer* buffer)
{
  if( buffer != nullptr ) {
    // Generate an audio buffer identifier from OpenAL
    buffer->buffer_id = generate_buffer_id();

    auto data = buffer->samples;
    auto format = buffer->channel_format;
    auto frequency = buffer->sample_rate;
    auto bytes = buffer->audio_bytes;

    NOM_ASSERT(data.size() > 0);

    // Fill the audio buffer with file's sample data
    AL_CLEAR_ERR();
    alBufferData(buffer->buffer_id, format, &data.front(),
                 bytes, frequency);
    AL_CHECK_ERR_VOID();

    // NOTE: Generate a unique source identifier for our audio buffer
    buffer->sound_id = generate_sound_id();

    // NOTE: Attach the audio buffer as the sound source
    AL_CLEAR_ERR();
    alSourcei(buffer->sound_id, AL_BUFFER, buffer->buffer_id);
    AL_CHECK_ERR_VOID();

    return true;
  }

  return false;
}

uint32 ALAudioEngine::state(SoundBuffer* buffer)
{
  auto result = AUDIO_STATE_STOPPED;
  int audio_state = AUDIO_STATE_STOPPED;
  int loop_state = AL_FALSE;

  if( buffer != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcei(buffer->sound_id, AL_SOURCE_STATE, &audio_state);
    AL_CHECK_ERR_VOID();

    AL_CLEAR_ERR();
    alGetSourcei(buffer->sound_id, AL_LOOPING, &loop_state);
    AL_CHECK_ERR_VOID();
  }

  switch(audio_state)
  {
    default:
    case AL_INITIAL:
    case AL_STOPPED:
    {
      result = AUDIO_STATE_STOPPED;
    } break;

    case AL_PAUSED:
    {
      result = AUDIO_STATE_PAUSED;
    } break;

    case AL_PLAYING:
    {
      result = AUDIO_STATE_PLAYING;
    } break;
  }

  if( loop_state == AL_TRUE ) {
    result = AUDIO_STATE_LOOPING;
  }

  return result;
}

real32 ALAudioEngine::pitch(SoundBuffer* buffer)
{
  auto pitch = 1.0f;

  if( buffer != nullptr ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_PITCH, &pitch);
    AL_CHECK_ERR_VOID();
  }

  return pitch;
}

void ALAudioEngine::play(SoundBuffer* buffer)
{
  if( buffer != nullptr ) {
    AL_CLEAR_ERR();
    alSourcePlay(buffer->sound_id);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::stop(SoundBuffer* buffer)
{
  if( buffer != nullptr ) {
    if( this->state(buffer) != AUDIO_STATE_STOPPED ) {
      AL_CLEAR_ERR();
      alSourceStop(buffer->sound_id);
      AL_CHECK_ERR_VOID();
    }
  }
}

void ALAudioEngine::pause(SoundBuffer* buffer)
{
  if( buffer != nullptr ) {
    AL_CLEAR_ERR();
    alSourcePause(buffer->sound_id);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::resume(SoundBuffer* buffer)
{
  this->play(buffer);
}

} // namespace nom
