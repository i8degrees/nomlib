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
#ifndef NOMLIB_AL_SOUNDSOURCE_HEADERS
#define NOMLIB_AL_SOUNDSOURCE_HEADERS

#include <memory>
#include <algorithm>

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"

namespace nom {
namespace audio {

// Forward declarations
class IOAudioEngine;
struct SoundBuffer;
class ISoundFileReader;
struct SoundInfo;

/// Sound source is one of the three states: stopped, paused or playing
enum AudioState
{
  AUDIO_STATE_INITIAL = 0,
  AUDIO_STATE_STOPPED,
  AUDIO_STATE_PAUSED,
  AUDIO_STATE_PLAYING,
  AUDIO_STATE_STREAMING,

  // TODO: Bit mask..?
  AUDIO_STATE_LOOPING
};

enum AudioSourceType
{
  AUDIO_TYPE_UNKNOWN = 0,
  AUDIO_TYPE_STATIC,
  AUDIO_TYPE_STREAMING,
};

// buffer creation

uint32 channel_format(uint32 num_channels, uint32 channel_format,
                      IOAudioEngine* target);

bool write_info(SoundBuffer* buffer, const SoundInfo& metadata);

void* create_samples(nom::size_type alloc_bytes, uint32 num_channels,
                     uint32 channel_format);

void free_samples(uint32 channel_format, void* data);

SoundBuffer* create_buffer_memory();

SoundBuffer*
create_buffer_memory(nom::size_type total_sample_bytes, uint32 num_channels,
                     uint32 channel_format);

SoundBuffer*
create_buffer(void* samples, const SoundInfo& metadata, IOAudioEngine* target);

// TODO(jeff): Restructure this function..? Perhaps split it up.
SoundBuffer*
create_buffer(const std::string& filename, ISoundFileReader* fp,
                    IOAudioEngine* target);

SoundBuffer*
create_buffer(const std::string& filename, IOAudioEngine* target);

bool valid_buffer(SoundBuffer* buffer, IOAudioEngine* target);
bool valid_source(SoundBuffer* buffer, IOAudioEngine* target);

SoundInfo info(SoundBuffer* buffer);

void free_buffer(SoundBuffer* buffer, IOAudioEngine* target);

// audio control

// void queue(SoundBuffer* buffer, IOAudioEngine* target);

void play(SoundBuffer* buffer, IOAudioEngine* target);

void stop(SoundBuffer* buffer, IOAudioEngine* target);

void pause(SoundBuffer* buffer, IOAudioEngine* target);

void resume(SoundBuffer* buffer, IOAudioEngine* target);

uint32 state(SoundBuffer* buffer, IOAudioEngine* target);

real32 pitch(SoundBuffer* buffer, IOAudioEngine* target);

Point3f
position(SoundBuffer* buffer, IOAudioEngine* target);

Point3f
velocity(SoundBuffer* buffer, IOAudioEngine* target);

/// \brief Get the volume level of an audio device.
///
/// \returns A number between 0..100 (min/max).
real32
volume(IOAudioEngine* target);

Point3f
position(IOAudioEngine* target);

/// \brief Get the volume level of an audio buffer.
///
/// \returns A number between 0..100 (min/max).
real32
volume(SoundBuffer* buffer, IOAudioEngine* target);

/// \brief Get the minimum volume level of an audio buffer.
///
/// \returns A number between 0..100 (min/max).
real32
min_volume(SoundBuffer* buffer, IOAudioEngine* target);

/// \brief Get the maximum volume level of an audio buffer.
///
/// \returns A number between 0..100 (min/max).
real32
max_volume(SoundBuffer* buffer, IOAudioEngine* target);

real32
playback_position(SoundBuffer* buffer, IOAudioEngine* target);

real32
playback_samples(SoundBuffer* buffer, IOAudioEngine* target);

// TODO(jeff): Update me
SoundBuffer* clone_buffer(const SoundBuffer* rhs);

// void
// set_state(SoundBuffer* buffer, IOAudioEngine* target, uint32 state);

/// \brief Set the master volume level of an audio output.
///
/// \param gain A 32-bit floating-point number between 0..100 (min/max),
/// representing the volume level.
///
/// \remarks This overrides an audio buffer's local volume level.
void set_volume(real32 gain, IOAudioEngine* target);

void
set_volume(SoundBuffer* buffer, IOAudioEngine* target, real32 gain);

/// \brief Set the minimum volume level for an audio buffer.
///
/// \param gain A 32-bit floating-point number between 0..100 (min/max).
void set_min_volume(SoundBuffer* buffer, IOAudioEngine* target, real32 gain);

/// \brief Set the maximum volume level for an audio buffer.
///
/// \param gain A 32-bit floating-point number between 0..100 (min/max).
void set_max_volume(SoundBuffer* buffer, IOAudioEngine* target, real32 gain);

void set_pitch(SoundBuffer* buffer, IOAudioEngine* target, real32 pitch);

void
set_velocity(SoundBuffer* buffer, IOAudioEngine* target, const Point3f& v);

void
set_position(SoundBuffer* buffer, IOAudioEngine* target, const Point3f& pos);

void
set_playback_position(SoundBuffer* buffer, IOAudioEngine* target,
                            real32 offset_seconds);

void suspend(IOAudioEngine* target);
void resume(IOAudioEngine* target);

} // namespace audio
} // namespace nom

#endif // NOMLIB_AL_SOUNDSOURCE_HEADERS defined

/// \class nom::SoundFileReader
/// \ingroup audio
///
/// **NOTE:** This documentation section is a *work in progress*!
///
/// ### Using std::vector with nom::SoundFileReader
///
/// \code
///
///  ISoundFileReader fp = new SoundFileReader();
///  NOM_ASSERT(fp != nullptr);
///  if(fp == nullptr) {
///    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
///                "Failed to create audio buffer: out of memory!");
///    return;
///  }
///
///  if(fp->open(filename, metadata) == false) {
///    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION,
///               "Could not load audio from input file:", filename);
///    return;
///  }
///
///  NOM_ASSERT(fp->valid() == true);
///
///  // The number of bytes to read into our temporary read buffer in a single
///  // update cycle
///  const nom::size_type CHUNK_SIZE = metadata.total_bytes;
///
///  // Temporary input buffer
///  std::vector<int16> read_buffer;
///
///  // Output buffer; the audio samples for audio playback with one of nomlib's
///  // audio engines, i.e.: OpenAL
///  std::vector<int16> buffer = nullptr;
///
///  nom::size_type total_samples_read = 0;
///  int64 samples_read = -1;
///  while(samples_read != 0) {
///    samples_read = fp->read(read_buffer.data(), CHUNK_SIZE);
///
///    auto& samples = buffer;
///    samples.insert(samples.end(), read_buffer.begin(),
///                   read_buffer.begin() + samples_read);
///
///    total_samples_read += samples_read;
///
///    if(total_samples_read >= CHUNK_SIZE) {
///      break;
///     }
///  }
///
///  delete NOM_SCAST(int16*, buffer);
///  buffer = nullptr;
///
/// \endcode
///
