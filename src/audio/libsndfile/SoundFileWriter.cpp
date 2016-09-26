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
#include "nomlib/audio/libsndfile/SoundFileWriter.hpp"

// Private headers
#include "nomlib/math/math_helpers.hpp"

// Forward declarations (third-party)
#include <sndfile.h>

namespace nom {

// static std::string libsndfile_version()
// {
//   return sf_version_string();
// }

static uint64 sample_bytes(SoundFormat channel_format, int64 sample_count)
{
  uint64 total_bytes = 0;

  // IMPORTANT(jeff): This is dependent upon the internal data type of the
  // samples when they were read from the input file.
  uint8 bit_size = 0;

  if(channel_format == FORMAT_PCM_S8) {
    bit_size = sizeof(int8);
  } else if(channel_format == FORMAT_PCM_S16) {
    bit_size = sizeof(int16);
  } else if(channel_format == FORMAT_PCM_S32) {
    bit_size = sizeof(int32);
  } else if(channel_format == FORMAT_PCM_R32) {
    bit_size = sizeof(real32);
  } else if(channel_format == FORMAT_PCM_R32) {
    bit_size = sizeof(real64);
  }

  // Clamp negative values to zero
  sample_count = nom::maximum<int64>(0, sample_count);

  total_bytes = (sample_count * bit_size);

  return total_bytes;
}

static real32 duration_seconds(SF_INFO& metadata)
{
  auto sample_count = metadata.frames * metadata.channels;
  auto sample_rate = metadata.samplerate;
  auto channel_count = metadata.channels;
  real32 duration = 0.0f;

  // IMPORTANT(jeff): When sample_count, an integer, is smaller than
  // sample_rate, it must be seen as a fractional value, or else when we
  // divide by it, we get a value of zero!
  duration =
    ( (real32)sample_count / sample_rate) / channel_count;

  return duration;
}

static bool libsndfile_check_error(SNDFILE_tag* fp)
{
  int err = sf_error(fp);
  if(err != SF_ERR_NO_ERROR) {
    const char* err_string = sf_strerror(fp);

    NOM_LOG_ERR(NOM_LOG_CATEGORY_APPLICATION, err_string);
    return false;
  }

  return true;
}

SoundFileWriter::SoundFileWriter()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);
}

SoundFileWriter::~SoundFileWriter()
{
  NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO, NOM_LOG_PRIORITY_DEBUG);

  this->close();
}

bool SoundFileWriter::valid() const
{
  return(this->fp_ != nullptr);
}

// TODO:
// bool SoundFileWriter::open(void* data, SoundInfo& info)
bool SoundFileWriter::open(const std::string& filename, SoundInfo& info)
{
  SF_INFO metadata = {};

  // If this object instance has been used before; we must free its memory
  // first!
  if(this->fp_ != nullptr) {
    this->close();
  }

  // NOTE(jeff): As per the libsndfile API documentation [1], we *should* set
  // this field to zero before calling sf_open. Less and except when we are
  // opening a RAW file (i.e.: PCM?), where the caller must set the sample rate,
  // channels and format fields to their appropriate values.
  //
  // 1. http://www.mega-nerd.com/libsndfile/api.html#open
  metadata.format = 0;

  this->fp_ = sf_open(filename.c_str(), SFM_READ, &metadata);

  if(libsndfile_check_error(this->fp_) == false) {
    return false;
  }

  info = this->parse_header(metadata);

  return true;
}

int64 SoundFileWriter::write(void* data, nom::size_type byte_size)
{
  sf_count_t samples_written = 0;

  NOM_ASSERT(this->fp_ != nullptr);
  if(this->fp_ == nullptr) {
    return samples_written;
  }

  samples_written = sf_write_short(this->fp_, (int16*)data, byte_size);

  if(libsndfile_check_error(this->fp_) == false) {
    return samples_written;
  }

  return(samples_written);
}

int64 SoundFileWriter::seek(int64 offset, SoundSeek dir)
{
  sf_count_t cursor_pos = 0;
  int whence = 0;

  switch(dir)
  {
    default:
    case SOUND_SEEK_SET: {
      whence = SEEK_SET;
    } break;

    case SOUND_SEEK_CUR: {
      whence = SEEK_CUR;
    } break;

    case SOUND_SEEK_END: {
      whence = SEEK_END;
    } break;
  }

  cursor_pos = sf_seek(this->fp_, offset, whence);

  return cursor_pos;
}

void SoundFileWriter::close()
{
  if(this->fp_ != nullptr) {
    sf_close(this->fp_);
  }

  this->fp_ = nullptr;
}

SoundInfo SoundFileWriter::parse_header(SF_INFO& metadata)
{
  SoundInfo info;

  // IMPORTANT(jeff): Take heed that these may be order-dependent!
  info.frame_count = metadata.frames;
  info.sample_count = metadata.frames * metadata.channels;
  info.sample_rate = metadata.samplerate;
  info.channel_count = metadata.channels;
  info.total_bytes = nom::sample_bytes(AUDIO_FORMAT_S16, info.sample_count);
  info.duration = nom::duration_seconds(metadata);
  info.seekable = metadata.seekable;

  // Audio metadata
  info.tags.title = this->parse_tags(this->fp_, SOUND_TAG_TITLE);
  info.tags.copyright = this->parse_tags(this->fp_, SOUND_TAG_COPYRIGHT);
  info.tags.software = this->parse_tags(this->fp_, SOUND_TAG_SOFTWARE);
  info.tags.artist = this->parse_tags(this->fp_, SOUND_TAG_ARTIST);
  info.tags.comment = this->parse_tags(this->fp_, SOUND_TAG_COMMENT);
  info.tags.date = this->parse_tags(this->fp_, SOUND_TAG_DATE);
  info.tags.album = this->parse_tags(this->fp_, SOUND_TAG_ALBUM);
  info.tags.license = this->parse_tags(this->fp_, SOUND_TAG_LICENSE);
  info.tags.track_number = this->parse_tags(this->fp_, SOUND_TAG_TRACK_NUMBER);
  info.tags.genre = this->parse_tags(this->fp_, SOUND_TAG_GENRE);

  return info;
}

const char* SoundFileWriter::parse_tags(SNDFILE_tag* fp, uint32 sound_tag)
{
  const char* tag = nullptr;

  switch(sound_tag) {
    default: {
      tag = "";
    } break;

    case SOUND_TAG_TITLE: {
      tag = sf_get_string(fp, SF_STR_TITLE);
    } break;

    case SOUND_TAG_COPYRIGHT: {
      tag = sf_get_string(fp, SF_STR_COPYRIGHT);
    } break;

    case SOUND_TAG_SOFTWARE: {
      tag = sf_get_string(fp, SF_STR_SOFTWARE);
    } break;

    case SOUND_TAG_ARTIST: {
      tag = sf_get_string(fp, SF_STR_ARTIST);
    } break;

    case SOUND_TAG_COMMENT: {
      tag = sf_get_string(fp, SF_STR_COMMENT);
    } break;

    case SOUND_TAG_DATE: {
      tag = sf_get_string(fp, SF_STR_DATE);
    } break;

    case SOUND_TAG_ALBUM: {
      tag = sf_get_string(fp, SF_STR_ALBUM);
    } break;

    case SOUND_TAG_LICENSE: {
      tag = sf_get_string(fp, SF_STR_LICENSE);
    } break;

    case SOUND_TAG_TRACK_NUMBER: {
      tag = sf_get_string(fp, SF_STR_TRACKNUMBER);
    } break;

    case SOUND_TAG_GENRE: {
      tag = sf_get_string(fp, SF_STR_GENRE);
    } break;
  }

  if(tag == nullptr) {
    tag = "";
  }

  return tag;
}

} // namespace nom
