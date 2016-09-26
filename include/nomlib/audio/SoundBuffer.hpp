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
#ifndef NOMLIB_AUDIO_SOUND_BUFFER_HPP
#define NOMLIB_AUDIO_SOUND_BUFFER_HPP

#include "nomlib/config.hpp"

namespace nom {
namespace audio {

// TODO(jeff): Verify that these fields match nom::SoundInfo data types!
struct SoundBuffer
{
  /// \brief The unique identifier for the buffer.
  ///
  /// \remarks This is the internal referencing system used by OpenAL.
  uint32 buffer_id = 0;

  /// \brief The unique identifier for the sound source.
  uint32 source_id = 0;

  /// \brief The audio buffer.
  void* samples = nullptr;

  /// \brief The total number of frames in this sample instance
  nom::size_type frame_count = 0;

  uint32 channel_count = 0;

  uint32 channel_format = 0;

  uint32 sample_rate = 0;

  int64 sample_count = 0;

  /// \brief The total time, in seconds, of the audio buffer.
  real32 duration = 0.0f;

  nom::size_type total_bytes = 0;

  bool seekable = false;

  /// \see nom::audio::ALAudioDeviceCaps
  /// \todo Consider using the AudioSourceType enumeration instead
  bool stream_source = false;

  nom::size_type samples_read = 0;
  // nom::size_type samples_output = 0;

  nom::size_type elapsed_seconds = 0;

  // TODO(jeff): Implement..? See API docs for alcGetContextsDevice(ctx)
  void* context = nullptr;
};

} // namespace audio
} // namespace nom

#endif // include guard defined
