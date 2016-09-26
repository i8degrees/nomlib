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
#ifndef NOMLIB_AUDIO_DEFINITIONS_HPP
#define NOMLIB_AUDIO_DEFINITIONS_HPP

#include <nomlib/types.hpp>

namespace nom {
namespace audio {

// Forward declarations
// struct IAudioDevice;
class IOAudioEngine;

// exposed to the end-user as 0.0f .. 100.0f
// internally stored as a 32-bit floating-point value that is normalized to 0..1
const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;

/// Optimal sound frame size (in bytes); used by libsndfile
const nom::uint32 BUFFER_SIZE = kilobyte(32);
const nom::size_type TOTAL_NUM_BUFFERS = 32;
const nom::size_type TOTAL_NUM_MONO_SOURCES = 128;
const nom::size_type TOTAL_NUM_STEREO_SOURCES = 127;
const nom::size_type TOTAL_NUM_SOURCES =
  TOTAL_NUM_MONO_SOURCES + TOTAL_NUM_STEREO_SOURCES;

// TODO(jeff): Collapse with nom::audio::AudioFormat..?
enum AudioCaps
{
  CAPS_UNDEFINED = 0x0000,

  CAPS_FORMAT_MONO_S8 = 0x1000,
  CAPS_FORMAT_STEREO_S8 = 0x1002,
  CAPS_FORMAT_QUAD_S8 = 0x1004,
  CAPS_FORMAT_51CHN_S8 = 0x1006,
  CAPS_FORMAT_61CHN_S8 = 0x1008,
  CAPS_FORMAT_71CHN_S8 = 0x1010,

  CAPS_FORMAT_MONO_U8 = 0x1100,
  CAPS_FORMAT_STEREO_U8 = 0x1102,
  CAPS_FORMAT_QUAD_U8 = 0x1104,
  CAPS_FORMAT_51CHN_U8 = 0x1106,
  CAPS_FORMAT_61CHN_U8 = 0x1108,
  CAPS_FORMAT_71CHN_U8 = 0x1110,

  CAPS_FORMAT_MONO_S16 = 0x1200,
  CAPS_FORMAT_STEREO_S16 = 0x1202,
  CAPS_FORMAT_QUAD_S16 = 0x1204,
  CAPS_FORMAT_51CHN_S16 = 0x1206,
  CAPS_FORMAT_61CHN_S16 = 0x1208,
  CAPS_FORMAT_71CHN_S16 = 0x1210,

  CAPS_FORMAT_MONO_S24 = 0x1300,
  CAPS_FORMAT_STEREO_S24 = 0x1302,
  CAPS_FORMAT_QUAD_S24 = 0x1304,
  CAPS_FORMAT_51CHN_S24 = 0x1306,
  CAPS_FORMAT_61CHN_S24 = 0x1308,
  CAPS_FORMAT_71CHN_S24 = 0x1310,

  CAPS_FORMAT_MONO_S32 = 0x1400,
  CAPS_FORMAT_STEREO_S32 = 0x1402,
  CAPS_FORMAT_QUAD_S32 = 0x1404,
  CAPS_FORMAT_51CHN_S32 = 0x1406,
  CAPS_FORMAT_61CHN_S32 = 0x1408,
  CAPS_FORMAT_71CHN_S32 = 0x1410,

  CAPS_FORMAT_MONO_FLOAT32 = 0x1500,
  CAPS_FORMAT_STEREO_FLOAT32 = 0x1502,
  CAPS_FORMAT_QUAD_R32 = 0x1504,

  CAPS_FORMAT_MONO_FLOAT64 = 0x1600,
  CAPS_FORMAT_STEREO_FLOAT64 = 0x1602,
  CAPS_FORMAT_QUAD_R64 = 0x1604,

  // CAPS_MAC_STATIC_BUFFER,
  // CAPS_MAC_EXT,
  // CAPS_EAX,
  // CAPS_EFX,
  // CAPS_XRAM,
};

enum AudioSpecFlags: uint32
{
  AUDIO_SPEC_UNDEFINED = 0x00000,

  /// \brief A request to the specified audio context that context processing
  /// occurs asynchronously where possible.
  // AUDIO_SPEC_ASYNC = 0x00100,
  AUDIO_SPEC_CAPTURE = 0x00200,
  // AUDIO_SPEC_STATIC_BUFFER,
  // AUDIO_SPEC_SOURCE_NOTIFICATION,
};

struct AudioSpec
{
  // audio front-end
  // IAudioDevice* dev = nullptr;

  // audio back-end
  // IOAudioEngine* engine = nullptr;

  // audio device name
  const char* name = nullptr;

  // audio back-end
  const char* engine = nullptr;

  /// \brief The final output audio frequency, specified in Hertz (Hz).
  int sample_rate = 44100;

  /// \brief A request for a specific number of mono (one channel) sources to
  /// be allocated at the time of an audio context creation.
  int num_mono_sources = audio::TOTAL_NUM_MONO_SOURCES;

  /// \brief A request for a specified number of stereo (two channel) sources
  /// to be allocated at the time of an audio context creation.
  int num_stereo_sources = audio::TOTAL_NUM_STEREO_SOURCES;

  /// \brief The update frequency, in Hertz (Hz), of audio context processing.
  int refresh_rate = 0;

  /// \brief A request to the specified audio context that context processing
  /// occurs asynchronously where possible.
  bool sync_context = false;

  /// \brief One or more bit-masked enumeration values from
  /// audio::AudioSpecFlags.
  uint32 flags = 0;

  // uint32 channel_format = 0;
};

} // namespace audio
} // namespace nom

#endif // include guard defined
