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
// #include "nomlib/audio/AL/SoundBuffer.hpp"
#include "SoundBuffer_priv.hpp"

// Forward declrations
#include "nomlib/audio/AL/AudioDevice.hpp"
// #include "nomlib/audio/AL/SoundSource.hpp"
#include "ALAudioDeviceCaps.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"

#include "nomlib/audio/AL/SoundFile.hpp"

namespace nom {

/*
SoundBuffer::SoundBuffer()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // Generate an audio buffer identifier from OpenAL
  AL_CLEAR_ERR();
  alGenBuffers(1, &this->buffer_id_);
  AL_CHECK_ERR_VOID();
}

SoundBuffer::~SoundBuffer()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // Goodbye buffer!
  AL_CLEAR_ERR();
  alDeleteBuffers(1, &this->buffer_id_);
  AL_CHECK_ERR_VOID();
}

uint32 SoundBuffer::buffer_id() const
{
  return this->buffer_id_;
}

const std::vector<int16>&
SoundBuffer::samples() const
{
  return this->samples_;
}

int64 SoundBuffer::duration() const
{
  return this->buffer_duration_;
}

bool SoundBuffer::load_file(const std::string& filename)
{
  SoundFile fp;
  SoundBufferB* buffer = nom::create_audio_buffer();
  std::vector<int16> samples;

  // if( fp.open(filename) == false ) {
  //   NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
  //                 "Could not load audio from file:", filename );
  //   return false;
  // }

  // if ( fp.read(samples) == false ) {
  //   NOM_LOG_ERR(  NOM_LOG_CATEGORY_APPLICATION,
  //                 "Could not read audio samples from file:", filename );
  //   return false;
  // }

  buffer = nom::create_audio_buffer(filename);

  buffer->buffer_id = this->buffer_id_;
  // buffer->data = samples;
  // buffer->channel_count = fp.getChannelCount();
  // buffer->channel_format = fp.getChannelFormat();
  // buffer->sample_rate = fp.getSampleRate();
  // buffer->sample_count = fp.getSampleCount();
  // buffer->audio_bytes = fp.getDataByteSize();
  // buffer->duration =
  //   (1000.0f * buffer->sample_count / buffer->sample_rate / buffer->channel_count);

  if( nom::fill_audio_buffer(buffer) == false ) {
    NOM_ASSERT_INVALID_PATH();
  }
  NOM_ASSERT(buffer != nullptr);

  // buffer->buffer_id = this->buffer_id_;
  this->channel_count_ = buffer->channel_count;
  this->channel_format_ = buffer->channel_format;
  this->sample_rate_ = buffer->sample_rate;
  this->sample_count_ = buffer->sample_count;
  this->audio_bytes_ = buffer->audio_bytes;
  this->samples_ = buffer->samples;
  this->buffer_duration_ = buffer->duration;

  // AL_CLEAR_ERR();
  // // Fill the audio buffer with file's sample data
  // alBufferData(this->buffer_id_, this->channel_format_,
  //              &this->samples_.front(), fp.getDataByteSize(),
  //              this->sample_rate_);
  // AL_CHECK_ERR_VOID();

  return true;
}

uint32 SoundBuffer::channel_count() const
{
  auto result = this->channel_count_;
  return(result);
}

uint32 SoundBuffer::channel_format() const
{
  auto result = this->channel_format_;
  return(result);
}

uint32 SoundBuffer::sample_rate() const
{
  auto result = this->sample_rate_;
  return(result);
}

nom::size_type SoundBuffer::sample_count() const
{
  auto result = this->sample_count_;
  return(result);
}

nom::size_type SoundBuffer::audio_bytes() const
{
  auto result = this->audio_bytes_;
  return(result);
}

void SoundBuffer::set_buffer(const std::vector<int16>& samples)
{
  this->samples_ = samples;
}
*/

// static
// uint32 generate_buffer_id()
// {
//   uint32 buffer_id = 0;

//   // Generate an audio buffer identifier from OpenAL
//   AL_CLEAR_ERR();
//   alGenBuffers(1, &buffer_id);
//   AL_CHECK_ERR_VOID();

//   return buffer_id;
// }



} // namespace nom
