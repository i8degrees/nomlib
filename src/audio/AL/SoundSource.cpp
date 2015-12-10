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
#include "nomlib/audio/AL/SoundFile.hpp"
#include "nomlib/core/clock.hpp"

// Forward declarations
// #include "nomlib/audio/AL/SoundBuffer.hpp"
#include "SoundBuffer_priv.hpp"
#include "ALAudioDeviceCaps.hpp"
#include "nomlib/audio/AL/AudioDevice.hpp"

// Forward declarations (third-party)
#include <sndfile.h>

namespace nom {

const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;


#if 0
// TOOD: REMOVE
SoundSource::SoundSource()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

// TOOD: REMOVE
SoundSource::~SoundSource()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  this->stop();

  // if( this->buffer_ != nullptr ) {
    // AL_CLEAR_ERR();
    // alDeleteSources(1, &this->buffer_->sound_id);
    // AL_CHECK_ERR_VOID();
  // }

  if( this->buffer_ != nullptr && this->buffer_created_ == true ) {
    nom::free_buffer(this->buffer_);
  }
}

// TOOD: REMOVE
uint32 SoundSource::sound_id() const
{
  if( this->buffer_ != nullptr ) {
    return this->buffer_->sound_id;
  } else {
    // Err
    return 0;
  }
}

// TODO: REMOVE
// real64 SoundSource::duration() const
// {
//   real64 result = 0.0f;

//   if( this->buffer_ != nullptr ) {
//     result = this->buffer_->duration;
//   }

//   return result;
// }

// TOOD: REMOVE
real32 SoundSource::volume() const
{
  real32 gain_level = nom::MIN_VOLUME;

  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcef(this->buffer_->sound_id, AL_GAIN, &gain_level);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  gain_level = gain_level * 100.0f;

  return(gain_level);
}

// TOOD: REMOVE
real32 SoundSource::min_volume() const
{
  real32 min_gain = nom::MIN_VOLUME;

  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcef(this->buffer_->sound_id, AL_MIN_GAIN, &min_gain);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  min_gain = min_gain * 100.0f;

  return(min_gain);
}

// TOOD: REMOVE
real32 SoundSource::max_volume() const
{
  real32 max_gain = nom::MIN_VOLUME;

  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcef(this->buffer_->sound_id, AL_MAX_GAIN, &max_gain);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  max_gain = max_gain * 100.0f;

  return(max_gain);
}

// TOOD: REMOVE
real32 SoundSource::pitch() const
{
  auto pitch = 1.0f;

  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcef(this->buffer_->sound_id, AL_PITCH, &pitch);
    AL_CHECK_ERR_VOID();
  }

  return pitch;
}

// TOOD: REMOVE
bool SoundSource::loop() const
{
  auto result = 0;
  return result;
}

// TOOD: REMOVE
Point3f SoundSource::position() const
{
  auto result = Point3f::null;
  return result;
}

// TOOD: REMOVE
Point3f SoundSource::velocity() const
{
  auto result = Point3f::null;
  return result;
}

// bool SoundSource::position_relative_listener() const
bool SoundSource::getPositionRelativeToListener() const
{
  ALint relative = 0;

  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcei(this->buffer_->sound_id, AL_SOURCE_RELATIVE, &relative);
    AL_CHECK_ERR_VOID();
  }

  return (relative != 0);
}

real32 SoundSource::getMinDistance() const
{
  auto result = 0;
  return result;
}

real32 SoundSource::getAttenuation() const
{
  auto result = 0;
  return result;
}

// TOOD: REMOVE
uint32 SoundSource::buffer_id() const
{
  uint32 result = 0;
  return result;
}

// TOOD: REMOVE
real32 SoundSource::getPlayPosition() const
{
  real32 playback_position = 0.0f;

  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alGetSourcef(this->buffer_->sound_id, AL_SEC_OFFSET, &playback_position);
    AL_CHECK_ERR_VOID();
  }

  return playback_position;
}

// TOOD: REMOVE
uint32 SoundSource::state() const
{
  return 0;
}

// TOOD: REMOVE
void SoundSource::set_volume(real32 gain)
{
}

// TOOD: REMOVE
void SoundSource::set_min_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {

    // normalize the current gain level to a number between 0..1
    if( this->buffer_ != nullptr ) {
      AL_CLEAR_ERR();
      alSourcef(this->buffer_->sound_id, AL_MIN_GAIN, gain * 0.01f);
      AL_CHECK_ERR_VOID();
    }
  }
}

// TOOD: REMOVE
void SoundSource::set_max_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {

    // normalize the gain
    if( this->buffer_ != nullptr ) {
      AL_CLEAR_ERR();
      alSourcef(this->buffer_->sound_id, AL_MAX_GAIN, gain * 0.01f);
      AL_CHECK_ERR_VOID();
    }
  }
}

void SoundSource::setPitch(real32 pitch)
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSourcef(this->buffer_->sound_id, AL_PITCH, pitch);
    AL_CHECK_ERR_VOID();
  }
}

// TOOD: REMOVE
void SoundSource::setLooping ( bool loops )
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSourcei(this->buffer_->sound_id, AL_LOOPING, loops);
    AL_CHECK_ERR_VOID();
  }
}

// TOOD: REMOVE
void SoundSource::setPosition ( real32 x, real32 y, real32 z )
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSource3f(this->buffer_->sound_id, AL_POSITION, x, y, z);
    AL_CHECK_ERR_VOID();
  }
}

// TOOD: REMOVE
void SoundSource::setPosition ( const Point3f& position )
{
  this->setPosition ( position.x, position.y, position.z );
}

// TOOD: REMOVE
void SoundSource::set_velocity(real32 x, real32 y, real32 z)
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSource3f(this->buffer_->sound_id, AL_VELOCITY, x, y, z);
    AL_CHECK_ERR_VOID();
  }
}

// TOOD: REMOVE
void SoundSource::set_velocity(const Point3f& v)
{
  this->set_velocity(v.x, v.y, v.z);
}

void SoundSource::setPositionRelativeToListener ( bool relative )
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSourcei(this->buffer_->sound_id, AL_SOURCE_RELATIVE, relative);
    AL_CHECK_ERR_VOID();
  }
}

void SoundSource::setMinDistance(real32 distance)
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSourcef(this->buffer_->sound_id, AL_REFERENCE_DISTANCE, distance);
    AL_CHECK_ERR_VOID();
  }
}

void SoundSource::setAttenuation(real32 attenuation)
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSourcef(this->buffer_->sound_id, AL_ROLLOFF_FACTOR, attenuation);
    AL_CHECK_ERR_VOID();
  }
}

// TOOD: REMOVE
void SoundSource::setPlayPosition ( real32 seconds )
{
  if( this->buffer_ != nullptr ) {
    AL_CLEAR_ERR();
    alSourcef(this->buffer_->sound_id, AL_SEC_OFFSET, seconds);
    AL_CHECK_ERR_VOID();
  }
}

// TOOD: REMOVE
void SoundSource::play()
{
}

void SoundSource::stop()
{

}

void SoundSource::pause()
{
}

void SoundSource::resume()
{
  this->play();
}
#endif

// bool SoundSource::load_buffer(const ISoundBuffer& rhs)
// {
//   NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

//   this->buffer_ = &rhs;

//   if( this->buffer_ != nullptr ) {
//     // NOTE: Generate a unique source identifier for our audio buffer
//     AL_CLEAR_ERR();
//     alGenSources(1, &this->source_id_);
//     AL_CHECK_ERR_VOID();

//     auto buffer_id = this->buffer_->buffer_id;

//     // NOTE: Attach the audio buffer as the sound source
//     AL_CLEAR_ERR();
//     alSourcei(this->source_id_, AL_BUFFER, buffer_id);
//     AL_CHECK_ERR_VOID();
//   }

//   return true;
// }

// static
// uint32 generate_sound_id()
// {
//   uint32 source_id = 0;

//   AL_CLEAR_ERR();
//   alGenSources(1, &source_id);
//   AL_CHECK_ERR_VOID();

//   return source_id;
// }

#if 0
bool SoundSource::load_buffer(SoundBuffer& rhs)
{
  this->buffer_ = &rhs;

  if( this->buffer_ != nullptr ) {
    // NOTE: Generate a unique source identifier for our audio buffer
    this->buffer_->sound_id = generate_sound_id();

    auto buffer_id = this->buffer_->buffer_id;

    // NOTE: Attach the audio buffer as the sound source
    AL_CLEAR_ERR();
    alSourcei(this->buffer_->sound_id, AL_BUFFER, buffer_id);
    AL_CHECK_ERR_VOID();
  }

  return true;

}
#endif
#if 0
bool SoundSource::load_file(const std::string& filename)
{
  // NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  this->buffer_ = nom::create_audio_buffer(filename);
  if( this->buffer_ != nullptr) {

    // NOTE: Generate a unique source identifier for our audio buffer
    buffer_->sound_id = generate_sound_id();

    auto buffer_id = this->buffer_->buffer_id;

    // NOTE: Attach the audio buffer as the sound source
    AL_CLEAR_ERR();
    alSourcei(buffer_->sound_id, AL_BUFFER, buffer_id);
    AL_CHECK_ERR_VOID();

    this->buffer_created_ = true;
  } else {
    // ERR
    return false;
  }

  return true;
}
#endif

// buffer creation

SoundBuffer*
create_audio_buffer()
{
  SoundBuffer* buffer = new SoundBuffer();

  return buffer;
}

// static uint32 channel_format(SoundBuffer* buffer)
// {
//   // Derives from channel_count
//   auto channel_format = AL_FORMAT_STEREO16;
//   auto channel_count = 0;

//   if( buffer != nullptr ) {
//     channel_count = buffer->channel_count;

//     if( channel_count == 1 ) {
//       channel_format = alGetEnumValue("AL_FORMAT_MONO16");
//     } else if( channel_count == 2 ) {
//       channel_format = alGetEnumValue("AL_FORMAT_STEREO16");
//     } else if( channel_count == 4 ) {
//       channel_format = alGetEnumValue("AL_FORMAT_QUAD16");
//     } else if( channel_count == 5 ) {
//       channel_format = alGetEnumValue("AL_FORMAT_51CHN16");
//     } else if( channel_count == 6 ) {
//       channel_format = alGetEnumValue("AL_FORMAT_61CHN16");
//     } else if( channel_count == 7 ) {
//       channel_format = alGetEnumValue("AL_FORMAT_71CHN16");
//     }
//   }

//   return channel_format;
// }

SoundBuffer*
create_audio_buffer(const std::string& filename, IAudioDevice* target)
{
  SoundFile fp;
  // IMPORTANT: This is a pointer to SoundFile in disguise!
  file_handle fpp;
  // fpp->fp = new SNDFILE_tag;
  // fpp->info = nom::create_audio_buffer();
  // file_handle* fpp = nullptr;
  // SF_INFO info = {};
  // SoundBuffer* buffer = nom::create_audio_buffer();

  // TODO: CHANGE ME BACK!
  // SoundBuffer* buffer = fpp->info;
  std::vector<int16> samples;

  if( fp.open(&fpp, filename) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not load audio from file:", filename );
    // return buffer;
    return fpp.buffer;
  }

  if ( fp.read(&fpp, 4096) == false ) {
    NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
                  "Could not read audio samples from file:", filename );
    // return buffer;
    return fpp.buffer;
  }

  NOM_ASSERT(fpp.buffer != nullptr);
  if( fpp.buffer != nullptr ) {

    // buffer->buffer_id = buffer_id;
    // fpp.buffer->sound_id = 0;
    // fpp.buffer->samples = samples;
    // FIXME(?):
    // buffer->frame_count = fpp->fp->frame_count;
    // buffer->channel_count = fpp->fp->channel_count;
    // buffer->channel_format = channel_format(buffer);
    // buffer->sample_rate = fpp->fp->sample_rate;
    // buffer->sample_count = fpp->fp->frame_count * fpp->fp->channel_count;

    // buffer->audio_bytes = (buffer->sample_count * sizeof(int16) )

    // buffer->duration =
      // (1000.0f * buffer->sample_count / buffer->sample_rate / buffer->channel_count);
    // buffer->duration = floorf(buffer->duration);

    // NOTE: Fill the audio buffer with the sampled data
    if( target->caps()->fill_audio_buffer(fpp.buffer) == false ) {
      return nullptr;
    }
  }

  // return buffer;
  return fpp.buffer;
}

void free_buffer(SoundBuffer* buffer)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // Goodbye buffer!
  if( buffer != nullptr ) {

    // NOTE: De-attach the sound (OpenAL)
    if( buffer->sound_id > 0 ) {
      AL_CLEAR_ERR();
      alDeleteSources(1, &buffer->sound_id);
      AL_CHECK_ERR_VOID();
    }

    // NOTE: Remove the audio buffer (OpenAL)
    if( buffer->buffer_id > 0 ) {
      AL_CLEAR_ERR();
      alDeleteBuffers(1, &buffer->buffer_id);
      AL_CHECK_ERR_VOID();
    }

    NOM_DELETE_PTR(buffer);
  }
}

uint32 audio_id(SoundBuffer* buffer, AudioID type)
{
  uint32 audio_id = 0;

  if( buffer != nullptr ) {
    if( type == AudioID::AUDIO_BUFFER_ID ) {
      audio_id = buffer->buffer_id;
    } else if( type == AudioID::SOUND_ID ) {
      audio_id = buffer->sound_id;
    }
  }

  return audio_id;
}

real32 audio_duration(SoundBuffer* buffer)
{
  real32 duration = 0.0f;

  if( buffer != nullptr ) {
    duration = buffer->duration;
  }

  return duration;
}

nom::size_type sample_count(SoundBuffer* buffer)
{
  nom::size_type sample_count = 0;

  if( buffer != nullptr ) {
    sample_count = buffer->sample_count;
  }

  return sample_count;
}

nom::size_type frame_count(SoundBuffer* buffer)
{
  nom::size_type frame_count = 0;

  if( buffer != nullptr ) {
    frame_count = buffer->frame_count;
  }

  return frame_count;
}

uint32 channel_count(SoundBuffer* buffer)
{
  nom::size_type channel_count = 0;

  if( buffer != nullptr ) {
    channel_count = buffer->channel_count;
  }

  return channel_count;
}

uint32 sample_rate(SoundBuffer* buffer)
{
  nom::size_type sample_rate = 0;

  if( buffer != nullptr ) {
    sample_rate = buffer->sample_rate;
  }

  return sample_rate;
}

nom::size_type audio_bytes(SoundBuffer* buffer)
{
  nom::size_type number_bytes = 0;

  if( buffer != nullptr ) {
    number_bytes = buffer->audio_bytes;
  }

  return number_bytes;
}


// audio control

void
play_audio(SoundBuffer* buffer, IAudioDevice* target)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    target->caps()->play(buffer);
  }
}

void
stop_audio(SoundBuffer* buffer, IAudioDevice* target)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    target->caps()->stop(buffer);
  }
}

void
pause_audio(SoundBuffer* buffer, IAudioDevice* target)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {

    auto state = nom::audio_state(buffer, target);

    if( state != AudioState::AUDIO_STATE_STOPPED ) {
      target->caps()->pause(buffer);
    }
  }
}

void
resume_audio(SoundBuffer* buffer, IAudioDevice* target)
{
  nom::play_audio(buffer, target);
}

uint32
audio_state(SoundBuffer* buffer, IAudioDevice* target)
{
  uint32 result = AudioState::AUDIO_STATE_STOPPED;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    result = target->caps()->state(buffer);
  }

  return NOM_SCAST(AudioState, result);
}

real32
audio_pitch(SoundBuffer* buffer, IAudioDevice* target)
{
  auto result = 0.0f;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    result = target->caps()->pitch(buffer);
  }

  return result;
}

Point3f
audio_position(SoundBuffer* buffer, IAudioDevice* target)
{
  Point3f p(Point3f::zero);

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSource3f(sound_id, AL_POSITION, &p.x, &p.y, &p.z);
    AL_CHECK_ERR_VOID();
  }

  return p;
}

Point3f
audio_velocity(SoundBuffer* buffer, IAudioDevice* target)
{
  Point3f v(Point3f::zero);

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSource3f(sound_id, AL_VELOCITY, &v.x, &v.y, &v.z);
    AL_CHECK_ERR_VOID();
  }

  return v;
}

real32
audio_volume(SoundBuffer* buffer, IAudioDevice* target)
{
  real32 gain_level = nom::MIN_VOLUME;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_GAIN, &gain_level);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  gain_level = gain_level * 100.0f;

  return gain_level;
}

real32
audio_min_volume(SoundBuffer* buffer, IAudioDevice* target)
{
  real32 min_gain = nom::MIN_VOLUME;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_MIN_GAIN, &min_gain);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  min_gain = min_gain * 100.0f;

  return min_gain;
}

real32
audio_max_volume(SoundBuffer* buffer, IAudioDevice* target)
{
  real32 max_gain = nom::MIN_VOLUME;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_MAX_GAIN, &max_gain);
    AL_CHECK_ERR_VOID();
  }

  // De-normalize; 0..1 -> 0..100
  max_gain = max_gain * 100.0f;

  return max_gain;
}

real32
audio_playback_position(SoundBuffer* buffer, IAudioDevice* target)
{
  real32 result = 0.0f;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_SEC_OFFSET, &result);
    AL_CHECK_ERR_VOID();
  }

  return result;
}

real32
audio_playback_samples_position(SoundBuffer* buffer, IAudioDevice* target)
{
  real32 result = 0.0f;

  if( buffer != nullptr && target != nullptr && target->valid() ) {
    auto sound_id = buffer->sound_id;

    AL_CLEAR_ERR();
    alGetSourcef(sound_id, AL_SAMPLE_OFFSET, &result);
    AL_CHECK_ERR_VOID();
  }

  return result;
}

void
set_audio_state(SoundBuffer* buffer, IAudioDevice* target, AudioState state)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    target->caps()->set_state(buffer, state);
  }
}

void set_audio_volume(IAudioDevice* target, real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {
    if( target != nullptr && target->valid() ) {
      target->caps()->set_volume(gain);
    }
  }
}

void set_audio_volume(SoundBuffer* buffer, IAudioDevice* target, real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {

    // normalize the current gain level to a number between 0..1
    if( buffer != nullptr && target != nullptr && target->valid() ) {
      // auto sound_id = target->sound_id;

      target->caps()->set_volume(buffer, gain);
    }
  }
}

void set_min_volume(SoundBuffer* buffer, IAudioDevice* target, real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {

    // normalize the current gain level to a number between 0..1
    if( buffer != nullptr && target != nullptr && target->valid() ) {
      AL_CLEAR_ERR();
      alSourcef(buffer->sound_id, AL_MIN_GAIN, gain * 0.01f);
      AL_CHECK_ERR_VOID();
    }
  }
}

void set_max_volume(SoundBuffer* buffer, IAudioDevice* target, real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {

    // normalize the gain
    if( buffer != nullptr && target != nullptr && target->valid() ) {
      AL_CLEAR_ERR();
      alSourcef(buffer->sound_id, AL_MAX_GAIN, gain * 0.01f);
      AL_CHECK_ERR_VOID();
    }
  }
}

void set_audio_pitch(SoundBuffer* buffer, IAudioDevice* target, real32 pitch)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    target->caps()->set_pitch(buffer, pitch);
  }
}

void
set_audio_position(SoundBuffer* buffer, IAudioDevice* target, const Point3f& pos)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    AL_CLEAR_ERR();
    alSource3f(buffer->sound_id, AL_POSITION, pos.x, pos.y, pos.z);
    AL_CHECK_ERR_VOID();
  }
}

void
set_audio_velocity(SoundBuffer* buffer, IAudioDevice* target, const Point3f& v)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    target->caps()->set_velocity(buffer, v);
  }
}

void
set_audio_playback_position(SoundBuffer* buffer, IAudioDevice* target,
                            real32 offset_seconds)
{
  if( buffer != nullptr && target != nullptr && target->valid() ) {
    target->caps()->set_playback_position(buffer, offset_seconds);
  }
}

} // namespace nom
