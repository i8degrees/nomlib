/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
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
#include "nomlib/audio/audio_defs.hpp"
// #include "nomlib/audio/SoundFile.hpp"
#include "nomlib/audio/libsndfile/SoundFileReader.hpp"

// Forward declarations
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/SoundBuffer.hpp"
#include "nomlib/audio/IOAudioEngine.hpp"

namespace nom {
namespace audio {

uint32 channel_format(uint32 num_channels, uint32 channel_format,
                      IOAudioEngine* target)
{
  if(num_channels > 0 && target != nullptr && target->valid() == true) {
    target->channel_format(num_channels, channel_format);
  }

  return channel_format;
}

bool write_info(SoundBuffer* buffer, const SoundInfo& metadata)
{
  if(buffer == nullptr) {
    return false;
  }

  buffer->frame_count = metadata.frame_count;
  buffer->sample_count = metadata.sample_count;
  buffer->sample_rate = metadata.sample_rate;
  buffer->channel_count = metadata.channel_count;
  buffer->channel_format = metadata.channel_format;
  buffer->duration = metadata.duration;
  buffer->total_bytes = metadata.total_bytes;
  buffer->seekable = metadata.seekable;

  return true;
}

void*
create_samples(nom::size_type alloc_bytes, uint32 num_channels,
               uint32 channel_format)
{
  void* samples_buffer = nullptr;

  switch(channel_format) {
    default:
    case AUDIO_FORMAT_UNKNOWN: {
      // ...Err state...
      samples_buffer = nullptr;
    } break;

    case AUDIO_FORMAT_S8:
    case AUDIO_FORMAT_U8:
    case AUDIO_FORMAT_S16: {
      // FIXME(jeff): I'm not sure we need this large of a buffer here; verify!
      samples_buffer = new int16[alloc_bytes * num_channels];
    } break;

    case AUDIO_FORMAT_S24: {
      NOM_ASSERT_INVALID_PATH("Not implemented");
    } break;

    case AUDIO_FORMAT_S32:
    case AUDIO_FORMAT_R32:
    case AUDIO_FORMAT_R64: {
      // FIXME(jeff): I'm not sure we need this large of a buffer here; verify!
      samples_buffer = new real32[alloc_bytes * num_channels];
    } break;
  }

  return samples_buffer;
}

void free_samples(uint32 channel_format, void* data)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // Goodbye buffer!
  if(data != nullptr) {
    switch(channel_format) {

      default:
      case AUDIO_FORMAT_UNKNOWN: {
        // Err state; do nothing
      } break;

      case AUDIO_FORMAT_S8:
      case AUDIO_FORMAT_U8:
      case AUDIO_FORMAT_S16: {
        delete NOM_SCAST(int16*, data);
      } break;

      case AUDIO_FORMAT_S24: {
        NOM_ASSERT_INVALID_PATH("Not implemented");
      } break;

      case AUDIO_FORMAT_S32:
      case AUDIO_FORMAT_R32:
      case AUDIO_FORMAT_R64: {
        delete NOM_SCAST(real32*, data);
      } break;
    }

    data = nullptr;
  }
}

SoundBuffer* create_buffer_memory()
{
  auto buffer = new SoundBuffer();
  return buffer;
}

SoundBuffer*
create_buffer_memory(nom::size_type total_sample_bytes, uint32 num_channels,
                     uint32 channel_format)
{
  auto buffer = audio::create_buffer_memory();
  if(buffer != nullptr) {
    // Generate PCM data that can be used by the audio engine
    buffer->samples =
      audio::create_samples(total_sample_bytes, num_channels, channel_format);
  }

  return buffer;
}

SoundBuffer*
create_buffer(void* samples, const SoundInfo& metadata, IOAudioEngine* target)
{
  SoundBuffer* buffer = audio::create_buffer_memory();
  const nom::size_type CHUNK_SIZE = metadata.total_bytes;
  NOM_ASSERT(CHUNK_SIZE > 0);

  // NOM_ASSERT(buffer != nullptr);
  if(buffer == nullptr) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: out of memory!");
    return buffer;
  }

  if(target == nullptr) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: audio device is invalid.");
    return buffer;
  }

  // Fill our audio buffer with audio samples given to us by the end-user
  buffer->samples = samples;

  if(audio::write_info(buffer, metadata) == false) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: NULL buffer");
    return buffer;
  }

  // NOTE: Fill the audio buffer with the sampled data
  if(target->push_buffer(buffer) == false) {
    return nullptr;
  }

  return buffer;
}

SoundBuffer*
create_buffer(const std::string& filename, ISoundFileReader* fp,
              IOAudioEngine* target)
{
  SoundInfo metadata;
  SoundBuffer* buffer = audio::create_buffer_memory();

  // NOM_ASSERT(buffer != nullptr);
  if(buffer == nullptr) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: out of memory!");
    return buffer;
  }

  if(target == nullptr) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: audio device is invalid.");
    return buffer;
  }

  NOM_ASSERT(fp != nullptr);
  if(fp == nullptr) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: out of memory!");
    return buffer;
  }

  if(fp->open(filename, metadata) == false) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Could not load audio from input file:", filename);
    return buffer;
  }

  if(audio::write_info(buffer, metadata) == false) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to write audio metadata.");
    return buffer;
  }

  const nom::size_type CHUNK_SIZE = metadata.total_bytes;
  NOM_ASSERT(CHUNK_SIZE > 0);

  auto channel_format = buffer->channel_format;
  auto num_channels = buffer->channel_count;

  // Generate PCM data that can be used by the audio engine
  buffer->samples =
    audio::create_samples(CHUNK_SIZE, num_channels, channel_format);

  NOM_ASSERT(buffer->samples != nullptr);
  if(buffer->samples == nullptr) {
    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
                "Failed to create audio buffer: out of memory for samples.");
    return buffer;
  }

  if(fp->read(buffer->samples, channel_format, CHUNK_SIZE) == false) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not read audio samples from file:", filename );
    return buffer;
  }


  // NOTE: Fill the audio buffer with the sampled data
  if(target->push_buffer(buffer) == false) {
    return buffer;
  }

  return buffer;
}

SoundBuffer*
create_buffer(const std::string& filename, IOAudioEngine* target)
{
  SoundBuffer* result = nullptr;
  ISoundFileReader* fp = new SoundFileReader();

  result = audio::create_buffer(filename, fp, target);

  return result;
}

bool valid_buffer(SoundBuffer* buffer, IOAudioEngine* target)
{
  bool result = false;

  if(buffer != nullptr && target->valid() == true) {
    result = target->valid_buffer(buffer);
  }

  return result;
}

bool valid_source(SoundBuffer* buffer, IOAudioEngine* target)
{
  bool result = false;

  if(buffer != nullptr && target->valid() == true) {
    result = target->valid_source(buffer);
  }

  return result;
}

SoundInfo info(SoundBuffer* buffer)
{
  SoundInfo metadata = {};

  if(buffer == nullptr) {
    return metadata;
  }

  metadata.frame_count = buffer->frame_count;
  metadata.sample_count = buffer->sample_count;
  metadata.sample_rate = buffer->sample_rate;
  metadata.channel_count = buffer->channel_count;
  metadata.channel_format = buffer->channel_format;
  metadata.duration = buffer->duration;
  metadata.total_bytes = buffer->total_bytes;
  metadata.seekable = buffer->seekable;

  return metadata;
}

void free_buffer(SoundBuffer* buffer, IOAudioEngine* target)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // Goodbye buffer!
  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    target->free_buffer(buffer);
  }

  NOM_DELETE_PTR(buffer);
}

// audio control

void play(SoundBuffer* buffer, IOAudioEngine* target)
{
  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    target->play(buffer);
  }
}

void
stop(SoundBuffer* buffer, IOAudioEngine* target)
{
  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    target->stop(buffer);
  }
}

void
pause(SoundBuffer* buffer, IOAudioEngine* target)
{
  if( buffer != nullptr && target != nullptr && target->valid() == true) {

    auto state = audio::state(buffer, target);

    if( state != AudioState::AUDIO_STATE_STOPPED ) {
      target->pause(buffer);
    }
  }
}

void
resume(SoundBuffer* buffer, IOAudioEngine* target)
{
  audio::play(buffer, target);
}

uint32
state(SoundBuffer* buffer, IOAudioEngine* target)
{
  uint32 result = AudioState::AUDIO_STATE_STOPPED;

  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    result = target->state(buffer);
  }

  return NOM_SCAST(AudioState, result);
}

real32
pitch(SoundBuffer* buffer, IOAudioEngine* target)
{
  auto result = 0.0f;

  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    result = target->pitch(buffer);
  }

  return result;
}

Point3f position(SoundBuffer* buffer, IOAudioEngine* target)
{
  Point3f p(Point3f::zero);

  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    target->position(buffer);
  }

  return p;
}

Point3f
velocity(SoundBuffer* buffer, IOAudioEngine* target)
{
  Point3f v(Point3f::zero);

  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    v = target->velocity(buffer);
  }

  return v;
}

real32 volume(IOAudioEngine* target)
{
  real32 gain_level = nom::audio::MIN_VOLUME;

  if(target != nullptr && target->valid() == true) {
    gain_level = target->volume();
  }

  return gain_level;
}

Point3f position(IOAudioEngine* target)
{
  Point3f pos(0.0f, 0.0f, 0.0f);

  if(target != nullptr && target->valid() == true) {
    pos = target->position();
  }

  return pos;
}

real32 volume(SoundBuffer* buffer, IOAudioEngine* target)
{
  real32 gain_level = nom::audio::MIN_VOLUME;

  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    gain_level = target->volume(buffer);
  }

  return gain_level;
}

real32
min_volume(SoundBuffer* buffer, IOAudioEngine* target)
{
  real32 min_gain = nom::audio::MIN_VOLUME;

  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    min_gain = target->min_volume(buffer);
  }

  return min_gain;
}

real32
max_volume(SoundBuffer* buffer, IOAudioEngine* target)
{
  real32 max_gain = nom::audio::MIN_VOLUME;

  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    max_gain = target->max_volume(buffer);
  }

  return max_gain;
}

real32
playback_position(SoundBuffer* buffer, IOAudioEngine* target)
{
  real32 pos = 0.0f;

  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    pos = target->playback_position(buffer);
  }

  return pos;
}

real32
playback_samples(SoundBuffer* buffer, IOAudioEngine* target)
{
  real32 pos = 0.0f;

  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    pos = target->playback_samples(buffer);
  }

  return pos;
}

SoundBuffer* clone_buffer(const SoundBuffer* rhs)
{
  SoundBuffer* buffer = new audio::SoundBuffer();
  if(buffer != nullptr) {
    buffer = {};
    buffer->buffer_id = rhs->buffer_id;
    buffer->source_id = rhs->source_id;
    buffer->samples = rhs->samples;
    buffer->frame_count = rhs->frame_count;
    buffer->channel_count = rhs->channel_count;
    buffer->channel_format = rhs->channel_format;
    buffer->sample_rate = rhs->sample_rate;
    buffer->sample_count = rhs->sample_count;
    buffer->duration = rhs->duration;
    buffer->total_bytes = rhs->total_bytes;
    buffer->seekable = rhs->seekable;
  }

  return buffer;
}
#if 0
void
set_state(SoundBuffer* buffer, IOAudioEngine* target, uint32 state)
{
  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_state(buffer, state);
  }
}
#endif
void set_volume(real32 gain, IOAudioEngine* target)
{
  if(gain >= nom::audio::MIN_VOLUME && gain <= nom::audio::MAX_VOLUME) {
    if(target != nullptr && target->valid() == true) {
      target->set_volume(gain);
    }
  }
}

void set_volume(SoundBuffer* buffer, IOAudioEngine* target, real32 gain)
{
  if(gain >= nom::audio::MIN_VOLUME && gain <= nom::audio::MAX_VOLUME) {

    // normalize the current gain level to a number between 0..1
    if( buffer != nullptr && target != nullptr && target->valid() == true) {
      target->set_volume(buffer, gain);
    }
  }
}

void set_min_volume(SoundBuffer* buffer, IOAudioEngine* target, real32 gain)
{
  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_min_volume(buffer, gain);
  }
}

void set_max_volume(SoundBuffer* buffer, IOAudioEngine* target, real32 gain)
{
  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_min_volume(buffer, gain);
  }
}

void set_pitch(SoundBuffer* buffer, IOAudioEngine* target, real32 pitch)
{
  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_pitch(buffer, pitch);
  }
}

void
set_velocity(SoundBuffer* buffer, IOAudioEngine* target, const Point3f& v)
{
  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_velocity(buffer, v);
  }
}

void
set_position(SoundBuffer* buffer, IOAudioEngine* target, const Point3f& pos)
{
  if(buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_position(buffer, pos);
  }
}

void
set_playback_position(SoundBuffer* buffer, IOAudioEngine* target,
                            real32 offset_seconds)
{
  if( buffer != nullptr && target != nullptr && target->valid() == true) {
    target->set_playback_position(buffer, offset_seconds);
  }
}

void suspend(IOAudioEngine* target)
{
  if(target != nullptr && target->valid() == true) {
    target->suspend();
  }
}

void resume(IOAudioEngine* target)
{
  if(target != nullptr) {
    target->resume();
  }
}

} // namespace audio
} // namespace nom
