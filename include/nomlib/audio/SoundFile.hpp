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
#ifndef NOMLIB_AUDIO_SOUND_FILE_HPP
#define NOMLIB_AUDIO_SOUND_FILE_HPP

#include <string>

#include "export.hpp"
#include "nomlib/config.hpp"

namespace nom {
namespace audio {

// This enumeration is filled in by libsndfile
enum NOM_EXPORT AudioTags
{
  SOUND_TAG_UNDEFINED = 0,
  SOUND_TAG_TITLE,
  SOUND_TAG_COPYRIGHT,
  SOUND_TAG_SOFTWARE,
  SOUND_TAG_ARTIST,
  SOUND_TAG_COMMENT,
  SOUND_TAG_DATE,
  SOUND_TAG_ALBUM,
  SOUND_TAG_LICENSE,
  SOUND_TAG_TRACK_NUMBER,
  SOUND_TAG_GENRE,
};

// This enumeration is filled in by libsndfile
enum NOM_EXPORT AudioFormat
{
  AUDIO_FORMAT_UNKNOWN = 0,

  /// \brief Signed 8-bit integer sample data
  AUDIO_FORMAT_S8,

  /// \brief Unsigned 8-bit integer sample data
  AUDIO_FORMAT_U8,

  /// \brief Signed 16-bit integer sample data
  AUDIO_FORMAT_S16,

  /// \brief Signed 24-bit integer sample data
  AUDIO_FORMAT_S24,

  /// \brief Signed 32-bit integer sample data
  AUDIO_FORMAT_S32,

  /// \brief 32-bit IEEE floating-point sample data
  AUDIO_FORMAT_R32,

  /// \brief 64-bit IEEE floating-point sample data
  AUDIO_FORMAT_R64,
};

#if 0
enum AudioChannels
{
  // Mono
  ONE_CHANNELS = 1,
  TWO_CHANNELS,
  FOUR_CHANNELS,
  FIVE_CHANNELS,
  SIX_CHANNELS,
  SEVEN_CHANNELS
};
#endif

// This struct is filled in by libsndfile
// TODO(JEFF): Rename struct to SoundFile?
struct NOM_EXPORT SoundInfo
{
  /// \brief The total number of frames.
  ///
  /// \remarks One audio frame is the ...
  nom::size_type frame_count = 0;

  /// \brief The total number of samples.
  int64 sample_count = 0;

  /// \brief The number of samples per second.
  uint32 sample_rate = 0;

  /// \brief The number of audio channels.
  int channel_count = 0;

  /// \brief The total time, in seconds, of the audio samples.
  real32 duration = 0.0f;

  /// \brief The total number of bytes represented by the audio samples.
  nom::int64 total_bytes = 0;

  /// \see nom::audio::AudioFormat
  uint32 channel_format = 0;

  bool seekable = false;

  // TODO(JEFF): Rename struct to SoundFileMetadata and relocate outside of the main
  // struct?
  struct Metadata
  {
    const char* title = "\0";
    const char* copyright = "\0";
    const char* software = "\0";
    const char* artist = "\0";
    const char* comment = "\0";
    const char* date = "\0";
    const char* album = "\0";
    const char* license = "\0";
    const char* track_number = "\0";
    const char* genre = "\0";
  };

  Metadata tags;
};

} // namespace audio
} // namespace nom

#endif // NOMLIB_AUDIO_SOUND_FILE_HPP defined
