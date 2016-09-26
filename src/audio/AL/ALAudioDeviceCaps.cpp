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
#include "nomlib/audio/AL/ALAudioDeviceCaps.hpp"

// Private headers
#include "nomlib/core/SDL2Logger.hpp" // TODO: REMOVE

#include "nomlib/core/err.hpp"
#include "nomlib/core/strings.hpp"
#include "nomlib/audio/audio_defs.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio/SoundFile.hpp"

// Forward declarations
#include "nomlib/audio/AL/ALAudioDevice.hpp"
#include "nomlib/audio/SoundBuffer.hpp"

namespace nom {
namespace audio {

static
bool valid_buffer(uint32 buffer_id)
{
  bool result = alIsBuffer(buffer_id);

  return result;
}

static
bool valid_source(uint32 source_id)
{
  bool result = alIsSource(source_id);

  return result;
}

static
void attach_buffer(uint32 source_id, uint32 buffer_id)
{
  // NOTE(jeff): Attach a single buffer to the sound source
  AL_CLEAR_ERR();
  alSourcei(source_id, AL_BUFFER, buffer_id);

  ALenum error = alGetError();
  if(error != AL_NO_ERROR) {
    auto al_err_str = nom::integer_to_string(error);
    // TODO(jeff): Improve err handling
    std::string err_str = "OpenAL error code: " + al_err_str;
    nom::set_error(err_str);
  }
}

static
void clear_buffer(uint32 source_id)
{
  uint32 buffer_id = 0;

  audio::attach_buffer(source_id, buffer_id);
}

// TODO(jeff): Should this be accessible to the end-user..?
static
nom::size_type processed_buffers(SoundBuffer* target)
{
  ALint result = 0;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcei(target->source_id, AL_BUFFERS_PROCESSED, &result);
    AL_CHECK_ERR_VOID();
  }

  return result;
}

// TODO(jeff): Should this be accessible to the end-user..?
static
nom::size_type queued_buffers(SoundBuffer* target)
{
  ALint result = 0;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcei(target->source_id, AL_BUFFERS_QUEUED, &result);
    AL_CHECK_ERR_VOID();
  }

  return result;
}

// TODO(jeff): Should this be accessible to the end-user..?
static
void free_stream(SoundBuffer* target)
{
  uint32 num_sources = 1;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alSourceUnqueueBuffers(target->source_id, num_sources, &target->buffer_id);
    AL_CHECK_ERR_VOID();
  }
}

// TODO(jeff): Should this be accessible to the end-user..?
static
void free_source(SoundBuffer* target)
{
  uint32 num_sources = 1;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alDeleteSources(num_sources, &target->source_id);
    AL_CHECK_ERR_VOID();
  }
}

void free_buffer(uint32 buffer_id)
{
  uint32 num_sources = 1;

  // NOTE(jeff): Free the audio buffer from our scope
  AL_CLEAR_ERR();
  alDeleteBuffers(num_sources, &buffer_id);
  AL_CHECK_ERR_VOID();
}

void free_source(uint32 source_id)
{
  uint32 num_sources = 1;

  // NOTE(jeff): Free the audio buffer from our scope
  AL_CLEAR_ERR();
  alDeleteBuffers(num_sources, &source_id);
  AL_CHECK_ERR_VOID();
}

uint32 buffer_id(const SoundBuffer* target)
{
  uint32 id = 0;

  if(target != nullptr) {
    id = target->buffer_id;
  }

  return id;
}

uint32 source_id(const SoundBuffer* target)
{
  uint32 id = 0;

  if(target != nullptr) {
    id = target->source_id;
  }

  return id;
}

uint32 next_buffer_id()
{
  uint32 id = 0;
  uint32 num_buffers = 1;

  AL_CLEAR_ERR();
  alGenBuffers(num_buffers, &id);
  ALenum error_code = alGetError();
  if(error_code != AL_NO_ERROR) {
    // TODO(jeff): Handle errors here; not sure how we ought to do this
    // just yet -- OpenAL error handling value range is between
    // 40961..40965
    // id = 0;
  }

  return id;
}

uint32* next_buffer_id(uint32 num_buffers)
{
  uint32* buffers = nullptr;

  buffers = new uint32[num_buffers];

  AL_CLEAR_ERR();
  alGenBuffers(num_buffers, buffers);
  ALenum error_code = alGetError();
  if(error_code != AL_NO_ERROR) {
    // TODO(jeff): Handle errors here; not sure how we ought to do this
    // just yet -- OpenAL error handling value range is between
    // 40961..40965
    // id = 0;
  }

  return buffers;
}

uint32 next_source_id()
{
  uint32 id = 0;
  uint32 num_sources = 1;

  AL_CLEAR_ERR();
  alGenSources(num_sources, &id);
  ALenum error_code = alGetError();
  if(error_code != AL_NO_ERROR) {
    // TODO(jeff): Handle errors here; not sure how we ought to do this
    // just yet -- OpenAL error handling value range is between
    // 40961..40965
    // id = 0;
  }

  return id;
}

uint32* next_source_id(uint32 num_sources)
{
  uint32* sources = nullptr;

  sources = new uint32[num_sources];

  AL_CLEAR_ERR();
  alGenBuffers(num_sources, sources);
  ALenum error_code = alGetError();
  if(error_code != AL_NO_ERROR) {
    // TODO(jeff): Handle errors here; not sure how we ought to do this
    // just yet -- OpenAL error handling value range is between
    // 40961..40965
    // id = 0;
  }

  return sources;
}

#if 0
uint32 buffer_id(uint32 num_buffers, SoundBuffer* target)
                 // uint32 generated_buffer_ids[])
{
  // uint32 id[num_buffers];
  uint32 id = 0;

  if(buffer != nullptr) {
    // NOTE(jeff): Generate a new unique identifier for the audio buffer when
    // the ID is unused, otherwise, use the existing identifier found.
    if(target->buffer_id == 0) {
      // NOTE(jeff): The local variable id stores the buffer's ID, given to us from
      // OpenAL.
      AL_CLEAR_ERR();
      alGenBuffers(num_buffers, &id);

      ALenum error_code = alGetError();
      if(error_code != AL_NO_ERROR) {
        // TODO(jeff): Handle errors here; not sure how we ought to do this
        // just yet -- OpenAL error handling value range is between
        // 40961..40965
        // id = 0;
      }
    } else {
      id = target->buffer_id;
    }
  }

  return id;
}

uint32 buffer_id(SoundBuffer* target)
{
  uint32 id = 0;
  uint32 num_buffers = 1;

  id = audio::buffer_id(num_buffers, buffer);

  return id;
}
#endif

#if 0
uint32 sound_id(uint32 num_sources, SoundBuffer* target)
{
  uint32 id = 0;

  if(buffer != nullptr) {
    // NOTE(jeff): Generate a new unique identifier for the sound source when
    // the source ID is not set. Otherwise, use the existing identifier that
    // has presumably been assigned to it by a previous call to OpenAL.
    if(target->source_id == 0) {
      AL_CLEAR_ERR();
      alGenSources(num_sources, &id);

      ALenum error_code = alGetError();
      if(error_code != AL_NO_ERROR) {
        // TODO(jeff): Handle errors here; not sure how we ought to do this
        // just yet -- OpenAL error handling value range is between
        // 40961..40965
        // id = 0;
      }
    } else {
      id = target->source_id;
    }
  }

  return id;
}

uint32 sound_id(SoundBuffer* target)
{
  uint32 id = 0;
  uint32 num_sources = 1;

  id = audio::sound_id(num_sources, buffer);

  return id;
}
#endif

ALAudioEngine::ALAudioEngine()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  // TODO(jeff): Remove
  NOM_LOG_INFO(NOM, "Audio device initialized");
}

ALAudioEngine::~ALAudioEngine()
{
  if(this->valid() == true) {
    this->close();
  }
}

ALAudioEngine::ALAudioEngine(ALAudioDevice* driver)
{
  this->impl_ = driver;
}

void ALAudioEngine::init(void* driver)
{
  this->impl_ = (ALAudioDevice*)driver;
}

bool ALAudioEngine::valid() const
{
  bool result = false;

  if(this->impl_ != nullptr) {
    auto dev = this->impl_->dev;
    auto ctx = this->impl_->ctx;
    result = (dev != nullptr && ctx != nullptr);
  }

  return result;
}

uint32 ALAudioEngine::caps() const
{
  uint32 result = 0;
  if(this->impl_ != nullptr) {
    result = this->impl_->capabilities;
  }

  return result;
}

void ALAudioEngine::set_cap(uint32 format)
{
  this->impl_->capabilities |= format;
}

bool ALAudioEngine::connected() const
{
  auto device = this->impl_->dev;
  int result = 0;

  // NOTE(jeff): Defined in file at openal-soft.git/include/AL/alext.h
  #if defined(ALC_EXT_disconnect)
    alcGetIntegerv(device, ALC_CONNECTED, sizeof(int), &result);
  #endif

  if(result == 1) {
    return true;
  }

  return false;
}

// auto dev = NOM_SCAST(ALCdevice_struct*, target->device());
// uint32 value = alcGetEnumValue(dev, "AL_FORMAT_STEREO_FLOAT32");
// NOM_DUMP(value);
// uint32 error_code = alcGetError(dev);
// NOM_DUMP(error_code);
uint32 ALAudioEngine::channel_format(uint32 num_channels, uint32 channel_format)
{
  ALenum format = 0;
  auto channel_count = num_channels;

  // FIXME(jeff): I'm unable to test 8-bit audio formats yet, due to a lacking
  // implementation of data handling within nom::SoundFileReader. We are unable
  // to interpret the 8-bit data natively using libsndfile.
  if(channel_format == AUDIO_FORMAT_U8 || channel_format == AUDIO_FORMAT_S8) {
    // channel_format = AUDIO_FORMAT_S16;
  }

  if(this->impl_ != nullptr) {
    auto caps = this->caps();
    if(caps & CAPS_UNDEFINED) {
      NOM_DUMP("???undefined???");
    }

    if(audio::cap(caps, CAPS_FORMAT_MONO_S8) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_MONO_S8");
    }

    if(audio::cap(caps, CAPS_FORMAT_STEREO_S8) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_STEREO_S8");
    }

    if(audio::cap(caps, CAPS_FORMAT_MONO_U8) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_MONO_U8");
    }

    if(audio::cap(caps, CAPS_FORMAT_STEREO_U8) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_STEREO_U8");
    }

    if(audio::cap(caps, CAPS_FORMAT_MONO_S16) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_MONO_S16");
    }

    if(audio::cap(caps, CAPS_FORMAT_STEREO_S16) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_STEREO_S16");
    }

    if(audio::cap(caps, CAPS_FORMAT_QUAD_U8) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_QUAD_U8");
    }

    if(audio::cap(caps, CAPS_FORMAT_QUAD_S16) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_QUAD_S16");
    }

    if(audio::cap(caps, CAPS_FORMAT_MONO_FLOAT32) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_MONO_FLOAT32");
    }

    if(audio::cap(caps, CAPS_FORMAT_STEREO_FLOAT32) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_STEREO_FLOAT32");
    }

    if(audio::cap(caps, CAPS_FORMAT_MONO_FLOAT64) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_MONO_FLOAT64");
    }

    if(audio::cap(caps, CAPS_FORMAT_STEREO_FLOAT64) == false) {
      NOM_DUMP("missing caps for CAPS_FORMAT_STEREO_FLOAT64");
    }

    switch(channel_count) {
      default:
      case 1: {
        if(channel_format == AUDIO_FORMAT_S8) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_S8) == true) {
            format = audio::enum_available("AL_FORMAT_MONO8");
          }
        } else if(channel_format == AUDIO_FORMAT_U8) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_U8) == true) {
            format = audio::enum_available("AL_FORMAT_MONO8");
          }
        } else if(channel_format == AUDIO_FORMAT_S16) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_S16) == true) {
            format = audio::enum_available("AL_FORMAT_MONO16");
          }
        } else if(channel_format == AUDIO_FORMAT_S24) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_S24) == true) {
            format = audio::enum_available("AL_FORMAT_MONO24");
          }
        } else if(channel_format == AUDIO_FORMAT_S32) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_S32) == true) {
            format = audio::enum_available("AL_FORMAT_MONO32");
          }
        } else if(channel_format == AUDIO_FORMAT_R32) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_FLOAT32) == true) {
            format = audio::enum_available("AL_FORMAT_MONO_FLOAT32");
          }
        } else if(channel_format == AUDIO_FORMAT_R64) {
          if(audio::cap(caps, CAPS_FORMAT_MONO_FLOAT64) == true) {
            format = audio::enum_available("AL_FORMAT_MONO_DOUBLE_EXT");
          }
        }
      } break;

      case 2: {
        // ...
      } break;

      case 3: {
        // ...
      } break;

      case 4: {
        if(channel_format == AUDIO_FORMAT_S8) {
          if(audio::cap(caps, CAPS_FORMAT_QUAD_S8) == true) {
            format = audio::enum_available("AL_FORMAT_QUAD8");
          }
        } else if(channel_format == AUDIO_FORMAT_U8) {
          if(audio::cap(caps, CAPS_FORMAT_QUAD_U8) == true) {
            format = audio::enum_available("AL_FORMAT_QUAD8");
          }
        } else if(channel_format == AUDIO_FORMAT_S16) {
          if(audio::cap(caps, CAPS_FORMAT_QUAD_S16) == true) {
            format = audio::enum_available("AL_FORMAT_QUAD16");
          }
        } else if(channel_format == AUDIO_FORMAT_S24) {
          if(audio::cap(caps, CAPS_FORMAT_QUAD_S24) == true) {
            format = audio::enum_available("AL_FORMAT_QUAD24");
          }
        } else if(channel_format == AUDIO_FORMAT_S32) {
          if(audio::cap(caps, CAPS_FORMAT_QUAD_S32) == true) {
            format = audio::enum_available("AL_FORMAT_QUAD32");
          }
        } else if(channel_format == AUDIO_FORMAT_R32) {
          if(audio::cap(caps, CAPS_FORMAT_QUAD_R32) == true) {
            format = audio::enum_available("AL_FORMAT_QUAD32"); // Invalid!
          }
        } else if(channel_format == AUDIO_FORMAT_R64) {
          format = audio::enum_available("AL_FORMAT_QUAD32"); // Invalid!
        }
      } break;

      case 5: {
        // ...
      } break;

      case 6: {
        // ...
      } break;

      case 7: {
        // ...
      } break;
    }
  }

  switch(channel_format) {

    default:
    case AUDIO_FORMAT_UNKNOWN: {
      return 0;
    } break;

// IMPORTANT(jeff): Fixes testing of resource file [1].
// 1. Resources/tests/audio/ALAudioTest/sinewave-4CHN-u8_1s-900.wav
#if 0
    case AUDIO_FORMAT_U8:
    case AUDIO_FORMAT_S8: {
      // NOM_ASSERT_INVALID_PATH("Not implemented within SoundFileReader");

      switch(channel_count) {
        case 1: {
          format = audio::enum_available("AL_FORMAT_MONO8");
        } break;

        case 4: {
          format = audio::enum_available("AL_FORMAT_QUAD8");
        } break;

        case 5: {
          format = audio::enum_available("AL_FORMAT_51CHN8");
        } break;

        case 6: {
          format = audio::enum_available("AL_FORMAT_61CHN8");
        } break;

        case 7: {
          format = audio::enum_available("AL_FORMAT_71CHN8");
        } break;

        default:
        case 2: {
          format = audio::enum_available("AL_FORMAT_STEREO8");
        } break;
      }
    } break;
#else

    case AUDIO_FORMAT_S8:
    case AUDIO_FORMAT_U8:
#endif
    case AUDIO_FORMAT_S16: {
      switch(channel_count) {

        case 1:
        default: {
          format = audio::enum_available("AL_FORMAT_MONO16");
        } break;

        // stereo (2) channel
        case 2: {
          format = audio::enum_available("AL_FORMAT_STEREO16");
        } break;

        // quad (4) channel
        case 4: {
          // format = alGetEnumValue("AL_FORMAT_QUAD16");
          format = audio::enum_available("AL_FORMAT_QUAD16");
        } break;

        // 5.1 Dolby Surround Sound
        case 5: {
          format = audio::enum_available("AL_FORMAT_51CHN16");
        } break;

        // 6.1 Dolby Surround Sound
        case 6: {
          format = audio::enum_available("AL_FORMAT_61CHN16");
        } break;

        // 7.1 Dolby Surround Sound
        case 7: {
          format = audio::enum_available("AL_FORMAT_71CHN16");
        } break;
      } break;
    } break;

    case AUDIO_FORMAT_S32:
    case AUDIO_FORMAT_R32:
    case AUDIO_FORMAT_R64: {
      switch(channel_count) {
        case 1: {
          format = audio::enum_available("AL_FORMAT_MONO_FLOAT32");
        } break;

        // TODO(jeff): These need to be explicitly tested!
        case 4: {
          format = audio::enum_available("AL_FORMAT_QUAD32");
        } break;

        case 5: {
          format = audio::enum_available("AL_FORMAT_51CHN32");
        } break;

        case 6: {
          format = audio::enum_available("AL_FORMAT_61CHN32");
        } break;

        case 7: {
          format = audio::enum_available("AL_FORMAT_71CHN32");
        } break;

        // case 4:
        // case 5:
        // case 6:
        // case 7: {
        //   NOM_ASSERT_INVALID_PATH("Not implemented");
        // } break;

        default:
        case 2: {
          format = audio::enum_available("AL_FORMAT_STEREO_FLOAT32");
        } break;
      }
    } break;
  }

  // NOM_ASSERT(channel_format != NOM_UINT32_MAX);

  // NOTE(jeff): Apple's OpenAL implementation returns negative one (-1), as
  // opposed to zero (0), when an unknown enumeration has been given.
  //
  // This is a last ditch efforts to offer a valid channel format for OpenAL
  // processing
  if(format == AL_INVALID || format == AL_NONE) {
    format = audio::enum_available("AL_FORMAT_STEREO16");
  }

  return format;
}

bool ALAudioEngine::valid_buffer(SoundBuffer* target)
{
  bool result = false;

  if(target != nullptr) {
    result = audio::valid_buffer(target->buffer_id);
  }

  return result;
}

bool ALAudioEngine::valid_source(SoundBuffer* target)
{
  bool result = false;

  if(target != nullptr) {
    result = audio::valid_source(target->source_id);
  }

  return result;
}

uint32 ALAudioEngine::state(SoundBuffer* target)
{
  auto result = AUDIO_STATE_STOPPED;
  ALint audio_state = AL_STOPPED;
  ALint loop_state = AL_FALSE;

  if(target != nullptr && this->valid_source(target) == true) {
    AL_CLEAR_ERR();
    alGetSourcei(target->source_id, AL_SOURCE_STATE, &audio_state);
    AL_CHECK_ERR_VOID();

    AL_CLEAR_ERR();
    alGetSourcei(target->source_id, AL_LOOPING, &loop_state);
    AL_CHECK_ERR_VOID();
  }

  switch(audio_state) {
    default:
    case AL_INITIAL: {
      result = AUDIO_STATE_INITIAL;
    } break;

    case AL_STOPPED: {
      result = AUDIO_STATE_STOPPED;
    } break;

    case AL_PAUSED: {
      result = AUDIO_STATE_PAUSED;
    } break;

    case AL_PLAYING: {
      result = AUDIO_STATE_PLAYING;
    } break;

    case AL_STREAMING: {
      result = AUDIO_STATE_STREAMING;
    } break;
  }

  if(loop_state == AL_TRUE) {
    result = AUDIO_STATE_LOOPING;
  }

  return result;
}

real32 ALAudioEngine::pitch(SoundBuffer* target)
{
  auto pitch = 1.0f;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcef(target->source_id, AL_PITCH, &pitch);
    AL_CHECK_ERR_VOID();
  }

  return pitch;
}

real32 ALAudioEngine::volume() const
{
  auto gain_level = nom::audio::MIN_VOLUME;

  AL_CLEAR_ERR();
  alGetListenerf(AL_GAIN, &gain_level);
  AL_CHECK_ERR_VOID();

  // De-normalized gain level; 0..1 -> 0..100
  return(gain_level * 100.0f);
}

Point3f ALAudioEngine::position() const
{
  // Defaults as per /System/Library/Frameworks/OpenAL/Headers/al.h
  Point3f p(0.0f, 0.0f, 0.0f);

  AL_CLEAR_ERR();
  alGetListener3f(AL_POSITION, &p.x, &p.y, &p.z);
  AL_CHECK_ERR_VOID();

  return p;
}

real32 ALAudioEngine::volume(SoundBuffer* target) const
{
  real32 gain_level = nom::audio::MIN_VOLUME;

  if(target != nullptr) {
    auto sound_id = target->source_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_GAIN, &gain_level);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  gain_level = gain_level * 100.0f;

  return gain_level;
}

real32 ALAudioEngine::min_volume(SoundBuffer* target)
{
  real32 min_gain = nom::audio::MIN_VOLUME;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcef(target->source_id, AL_MIN_GAIN, &min_gain);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  min_gain = min_gain * 100.0f;

  return min_gain;
}

real32 ALAudioEngine::max_volume(SoundBuffer* target)
{
  real32 max_gain = nom::audio::MIN_VOLUME;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcef(target->source_id, AL_MAX_GAIN, &max_gain);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  max_gain = max_gain * 100.0f;

  return max_gain;
}

Point3f ALAudioEngine::velocity(SoundBuffer* target)
{
  Point3f v(Point3f::zero);

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSource3f(target->source_id, AL_VELOCITY, &v.x, &v.y, &v.z);
    AL_CHECK_ERR_VOID();
  }

  return v;
}

Point3f ALAudioEngine::position(SoundBuffer* target)
{
  Point3f p(Point3f::zero);

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSource3f(target->source_id, AL_POSITION, &p.x, &p.y, &p.z);
    AL_CHECK_ERR_VOID();
  }

  return p;
}

real32 ALAudioEngine::playback_position(SoundBuffer* target)
{
  real32 pos = 0.0f;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcef(target->source_id, AL_SEC_OFFSET, &pos);
    AL_CHECK_ERR_VOID();
  }

  return pos;
}

real32 ALAudioEngine::playback_samples(SoundBuffer* target)
{
  real32 samples = 0.0f;

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alGetSourcef(target->source_id, AL_SAMPLE_OFFSET, &samples);
    AL_CHECK_ERR_VOID();
  }

  return samples;
}
#if 0
void ALAudioEngine::set_state(SoundBuffer* target, uint32 state)
{
  ALint al_state = AL_STOPPED;

  if(target != nullptr) {
    switch(state) {
      default:
      case AUDIO_STATE_INITIAL: {
        al_state = AL_INITIAL;
      } break;

      case AUDIO_STATE_STOPPED: {
        al_state = AL_STOPPED;
      } break;

      case AUDIO_STATE_PAUSED: {
        al_state = AL_PAUSED;
      } break;

      case AUDIO_STATE_PLAYING: {
        al_state = AL_PLAYING;
      } break;

      case AUDIO_STATE_STREAMING: {
        al_state = AL_STREAMING;
      } break;
    }

    // TODO(jeff): Handle OpenAL source loop state
    if(state == AUDIO_STATE_LOOPING) {
      al_state = AL_LOOPING;
    }

    if(state == AUDIO_STATE_LOOPING) {
      AL_CLEAR_ERR();
      alSourcei(target->source_id, AL_LOOPING, true);
      AL_CHECK_ERR_VOID();
    } else {
      AL_CLEAR_ERR();
      alSourcei(target->source_id, AL_SOURCE_STATE, state);
      AL_CHECK_ERR_VOID();
    }
  }
}
#endif
void ALAudioEngine::set_volume(real32 gain)
{
  if(gain >= nom::audio::MIN_VOLUME && gain <= nom::audio::MAX_VOLUME) {
    // normalize the current gain level
    auto normalized_gain = gain * 0.01f;

    AL_CLEAR_ERR();
    alListenerf(AL_GAIN, normalized_gain);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_position(const Point3f& p)
{
  AL_CLEAR_ERR();
  alListener3f(AL_POSITION, p.x, p.y, p.z);
  AL_CHECK_ERR_VOID();
}

void ALAudioEngine::set_volume(SoundBuffer* target, real32 gain)
{
  if(gain >= nom::audio::MIN_VOLUME && gain <= nom::audio::MAX_VOLUME) {
    // normalize the current gain level to a number between 0..1
    gain = gain * 0.01f;

    if(target != nullptr) {
      AL_CLEAR_ERR();
      alSourcef(target->source_id, AL_GAIN, gain);
      AL_CHECK_ERR_VOID();
    }
  }
}

void ALAudioEngine::set_min_volume(SoundBuffer* target, real32 gain)
{
  if(gain >= nom::audio::MIN_VOLUME && gain <= nom::audio::MAX_VOLUME) {
    // normalize the current gain level to a number between 0..1
    gain = gain * 0.01f;
  }

  if(target != nullptr) {
    AL_CLEAR_ERR();
    alSourcef(target->source_id, AL_MIN_GAIN, gain);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_max_volume(SoundBuffer* target, real32 gain)
{
  if(gain >= nom::audio::MIN_VOLUME && gain <= nom::audio::MAX_VOLUME) {
    // normalize the current gain level to a number between 0..1
    gain = gain * 0.01f;
  }

  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSourcef(target->source_id, AL_MAX_GAIN, gain);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_velocity(SoundBuffer* target, const Point3f& v)
{
  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSource3f(target->source_id, AL_VELOCITY, v.x, v.y, v.z);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_position(SoundBuffer* target, const Point3f& p)
{
  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSource3f(target->source_id, AL_POSITION, p.x, p.y, p.z);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_pitch(SoundBuffer* target, real32 pitch)
{
  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSourcef(target->source_id, AL_PITCH, pitch);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::set_playback_position(SoundBuffer* target,
                                              real32 offset_seconds)
{
  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSourcef(target->source_id, AL_SEC_OFFSET, offset_seconds);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::play(SoundBuffer* target)
{
  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSourcePlay(target->source_id);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::stop(SoundBuffer* target)
{
  if(target != nullptr && this->valid() == true) {
    if(this->state(target) != AUDIO_STATE_STOPPED) {
      AL_CLEAR_ERR();
      alSourceStop(target->source_id);
      AL_CHECK_ERR_VOID();
    }
  }
}

void ALAudioEngine::pause(SoundBuffer* target)
{
  if(target != nullptr && this->valid() == true) {
    AL_CLEAR_ERR();
    alSourcePause(target->source_id);
    AL_CHECK_ERR_VOID();
  }
}

void ALAudioEngine::resume(SoundBuffer* target)
{
  this->play(target);
}

bool ALAudioEngine::push_buffer(SoundBuffer* target)
{
  if(this->fill_buffer(target) == false) {
    std::string err_str = "Failed to fill audio buffer: " + nom::error();
    nom::set_error(err_str);
    return false;
  }

  audio::attach_buffer(target->source_id, target->buffer_id);

  // TODO(jeff): Improve err handling here!
  return true;
}

// TODO(jeff): Create test for streaming sources in ALAudioTest
bool ALAudioEngine::queue_buffer(SoundBuffer* target)
{
  if(this->fill_buffer(target) == false) {
    std::string err_str = "Failed to fill audio buffer: " + nom::error();
    nom::set_error(err_str);
    return false;
  }

  // NOTE(jeff): Push the buffer to the sound source queue
  AL_CLEAR_ERR();
  alSourceQueueBuffers(target->source_id, 1, &target->buffer_id);
  ALenum error = alGetError();
  if(error != AL_NO_ERROR) {
    auto al_err_str = nom::integer_to_string(error);
    std::string err_str = "OpenAL error code: " + al_err_str;
    nom::set_error(err_str);
  }

  ALenum streaming_source = AL_UNDETERMINED;
  alGetSourcei(target->source_id, AL_SOURCE_TYPE, &streaming_source);
  if(streaming_source == AL_STREAMING) {
    target->stream_source = true;
  }

#if 1
  auto processed_buffers = audio::processed_buffers(target);
  auto queued_buffers = audio::queued_buffers(target);
  if(target != nullptr) {
    NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                  "streaming_source:", target->stream_source,
                  "processed_buffers:", processed_buffers,
                  "queued_buffers:", queued_buffers);
  }
#endif

  // TODO(jeff): Improve err handling here!
  return true;
}

void ALAudioEngine::suspend()
{
  // auto ctx = NOM_SCAST(ALCcontext*, this->context());
  auto impl = NOM_SCAST(ALAudioDevice*, this->impl_);
  auto ctx = NOM_SCAST(ALCcontext*, impl->ctx);
  NOM_ASSERT(ctx != nullptr);

  AL_CLEAR_ERR();
  alcSuspendContext(ctx);
  AL_CHECK_ERR_VOID();

  AL_CLEAR_ERR();
  alcMakeContextCurrent(ctx);
  AL_CHECK_ERR_VOID();
}

void ALAudioEngine::resume()
{
  // auto ctx = NOM_SCAST(ALCcontext*, this->context());
  auto impl = NOM_SCAST(ALAudioDevice*, this->impl_);
  auto ctx = NOM_SCAST(ALCcontext*, impl->ctx);
  NOM_ASSERT(ctx != nullptr);

  AL_CLEAR_ERR();
  alcMakeContextCurrent(ctx);
  AL_CHECK_ERR_VOID();

  AL_CLEAR_ERR();
  alcProcessContext(ctx);
  AL_CHECK_ERR_VOID();
}

void ALAudioEngine::close()
{
  this->close_context();
  this->close_device();

  delete NOM_SCAST(ALAudioDevice*, this->impl_);
  this->impl_ = nullptr;
}

void ALAudioEngine::free_buffer(SoundBuffer* target)
{
  uint32 num_sources = 1;

  if(target != nullptr) {

    if(this->valid_source(target) == true) {

      // NOTE(jeff): Streaming audio buffers are a special use case
      if(target->stream_source == true) {
        auto processed_buffers = audio::processed_buffers(target);
        if(target != nullptr) {
          NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                        "processed_buffers:", processed_buffers);
        }

        audio::free_stream(target);
        if(target != nullptr) {
          auto queued_buffers = audio::queued_buffers(target);
          NOM_LOG_DEBUG(NOM_LOG_CATEGORY_TEST,
                        "queued_buffers:", queued_buffers);
        }
        // NOM_ASSERT(queued_buffers == 0);
      }

      // NOTE(jeff): Free the sound source from our scope
      audio::free_source(target);
    } // end if target buffer is a valid source

    // NOTE(jeff): Free the audio buffer from our scope
    if(this->valid_buffer(target) == true) {
      AL_CLEAR_ERR();
      alDeleteBuffers(num_sources, &target->buffer_id);
      AL_CHECK_ERR_VOID();
    }

    if(target->samples) {
      audio::free_samples(target->channel_format, target->samples);
      target->samples = nullptr;
    }
  }
}

// private members

bool ALAudioEngine::fill_buffer(SoundBuffer* target)
{
  if(target == nullptr) {
    std::string buffer_id_str = nom::integer_to_string(target->buffer_id);
    std::string err_str = "Invalid buffer (NULL)";
    nom::set_error(err_str);
    return false;
  }

  if(target->samples == nullptr) {
    return true;
  }

  // if(target->buffer_id == 0) {
    // NOTE(jeff): Generate an audio buffer id for identification and
    // communication between OpenAL and the calling library -- us.
    target->buffer_id = audio::next_buffer_id();
    if(this->valid_buffer(target) == false) {
      std::string buffer_id_str = nom::integer_to_string(target->buffer_id);
      std::string err_str = "Invalid buffer ID " + buffer_id_str;
      nom::set_error(err_str);
      return false;
    }
  // } else {
    // TODO(jeff): Restructure this branch!
    // return true;
  // }

  auto buffer_id = target->buffer_id;
  auto samples = target->samples;
  auto channel_count = target->channel_count;
  auto channel_format = target->channel_format;
  auto frequency = target->sample_rate;
  auto bytes = target->total_bytes;
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "buffer_id:", buffer_id);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "channel_count:", channel_count);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "channel_format:", channel_format);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "frequency:", frequency);
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "bytes:", bytes);

  // static_buffer = alGetProcAddress((const ALCchar*) token);

  // NOTE(jeff): Fill the audio buffer with file's sample data
  // FIXME(jeff): Implement support for buffers attached to more than one
  // sound source.
  //
  // I think we need to add an API for handling the attachment of one or more
  // sound sources per buffer by letting the end-user explicitly set it up.
  auto format = this->channel_format(channel_count, channel_format);
  AL_CLEAR_ERR();
  alBufferData(buffer_id, format, samples, bytes, frequency);
  AL_CHECK_ERR_VOID();
  NOM_LOG_INFO(NOM_LOG_CATEGORY_TEST, "format:", format);

  // TODO(jeff): Check for success before freeing the buffer!
  // audio::free_samples(format, samples);
  // target->samples = nullptr;

  // NOTE(jeff): Generate a sound source identifier for storing it in memory,
  // i.e.: repeated playback of the audio buffer samples.
  target->source_id = audio::next_source_id();
  if(this->valid_source(target) == false) {
    std::string source_id_str = nom::integer_to_string(target->source_id);
    std::string err_str = "Invalid source ID " + source_id_str;
    nom::set_error(err_str);
    return false;
  }

  // IMPORTANT(jeff): Ensure that the reserved memory for this attached audio
  // source buffer is cleared before potentially being re-used!
  audio::clear_buffer(target->source_id);

  return true;
}

void ALAudioEngine::close_context()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  if(this->impl_ != nullptr) {
    auto ctx = this->impl_->ctx;
    audio::free_audio_context(ctx);
    ctx = nullptr;

    // TODO(jeff): Swap back
    // NOM_LOG_DEBUG(NOM_LOG_PRIORITY_DEBUG, "Audio context released");
    NOM_LOG_INFO(NOM, "Audio context released");
  }
}

void ALAudioEngine::close_device()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  if(this->impl_ != nullptr) {
    auto dev = this->impl_->dev;
    audio::free_audio_device(dev);
    dev = nullptr;

    // TODO(jeff): Swap back
    // NOM_LOG_DEBUG(NOM_LOG_PRIORITY_DEBUG, "Audio device released");
    NOM_LOG_INFO(NOM, "Audio device released");
  }
}

} // namespace audio
} // namespace nom
