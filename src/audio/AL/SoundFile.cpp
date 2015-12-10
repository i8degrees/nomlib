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
#include "nomlib/audio/AL/SoundFile.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"

// Forward declarations
#include "SoundBuffer_priv.hpp"

// Forward declarations (third-party)
#include <sndfile.h>

namespace nom {

IStream::IStream()
{
  //
}

IStream::~IStream()
{
  //
}

bool IStream::good()
{
  return valid();
}

SoundFile::SoundFile()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
#if 1
  if( this->fp_ != nullptr ) {
    // sf_close(this->fp_);
    this->fp_ = nullptr;
  }

  this->fp_ = new SNDFILE;
#endif
}

SoundFile::~SoundFile()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
#if 0
  if( this->fp_ ) {
    sf_close(this->fp_);
  }
#endif
}

int64 SoundFile::getSampleCount() const
{
  return this->sample_count;
}

uint32 SoundFile::getChannelCount() const
{
  return this->channel_count;
}

uint32 SoundFile::getSampleRate() const
{
  return this->sample_rate;
}

int64 SoundFile::getDataByteSize() const
{
  return( this->getSampleCount() * sizeof(int16) );
}

bool SoundFile::good()
{
  return valid();
}

bool SoundFile::valid()
{
  if( fp_ != nullptr ) {
    // return(fp->buffer_id > 0 && fp->sound_id > 0 &&
           // fp->samples.size() > 0 && fp->channel_count > 0);
    return true;
  }

  return false;
}

#if 0
bool SoundFile::eof()
{
  return();
}
#endif

static uint32 channel_format(uint32 num_channels)
{
  // Derives from channel_count
  auto channel_format = AL_FORMAT_STEREO16;
  auto channel_count = num_channels;

  if( channel_count == 1 ) {
    channel_format = alGetEnumValue("AL_FORMAT_MONO16");
  } else if( channel_count == 2 ) {
    channel_format = alGetEnumValue("AL_FORMAT_STEREO16");
  } else if( channel_count == 4 ) {
    channel_format = alGetEnumValue("AL_FORMAT_QUAD16");
  } else if( channel_count == 5 ) {
    channel_format = alGetEnumValue("AL_FORMAT_51CHN16");
  } else if( channel_count == 6 ) {
    channel_format = alGetEnumValue("AL_FORMAT_61CHN16");
  } else if( channel_count == 7 ) {
    channel_format = alGetEnumValue("AL_FORMAT_71CHN16");
  }

  return channel_format;
}

bool SoundFile::open(file_handle* in, const std::string& filename)
{
  SF_INFO metadata = {};
// #if 1
  in->fp = sf_open(filename.c_str(), SFM_READ, &metadata);
  if( in->fp == nullptr ) {
// #else
//   fp_ = sf_open(filename.c_str(), SFM_READ, &metadata);
//   // fp_->info = new SoundBuffer;
//   if( fp_ == nullptr ) {
// #endif
    NOM_LOG_ERR(NOM, "Could not open audio file:", filename);
    return false;
  }

  in->buffer = new SoundBuffer();

  // this->frame_count = metadata.frames;
  in->buffer->frame_count = metadata.frames;
  // NOTE: the sample count should be the same size as the samples
  in->buffer->sample_count = metadata.frames * metadata.channels;

  in->buffer->sample_rate = metadata.samplerate;
  in->buffer->channel_count = metadata.channels;
  in->buffer->channel_format = channel_format(in->buffer->channel_count);

  in->buffer->duration =
    (1000.0f * in->buffer->sample_count / in->buffer->sample_rate / in->buffer->channel_count);
  in->buffer->duration = floorf(in->buffer->duration);

  in->buffer->audio_bytes = (in->buffer->sample_count * sizeof(int16) );

  return true;
}

bool SoundFile::read(file_handle* in, nom::size_type read_size)
{
  sf_count_t chunk_size = read_size;
  std::vector<int16> read_buffer;
  read_buffer.resize(BUFFER_SIZE);

  while(chunk_size != 0) {
    chunk_size =
      sf_read_short(in->fp, read_buffer.data(), read_buffer.size() );

    auto& samples = in->buffer->samples;

    samples.insert(samples.end(), read_buffer.begin(),
                   read_buffer.begin() + chunk_size);
  }

  return true;
}

bool SoundFile::seek(file_handle* in, int offset)
{
  if( in != nullptr ) {
    // ...
  }

  return false;
}

void SoundFile::close()
{
  if( this->fp_ ) {
    sf_close(this->fp_);
  }
}

std::string libsndfile_version()
{
  return sf_version_string();
}

} // namespace nom
