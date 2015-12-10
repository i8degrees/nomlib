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
#ifndef NOMLIB_AL_SOUND_BUFFER_HPP
#define NOMLIB_AL_SOUND_BUFFER_HPP

#include <vector>

#include "nomlib/config.hpp"

namespace nom {

// Forward declarations
class IAudioDevice;

struct SoundBuffer
{
  /// \brief The unique identifier for the buffer.
  ///
  /// \remarks This is the internal referencing system used by OpenAL.
  uint32 buffer_id = 0;

  uint32 sound_id = 0;

  /// \brief The audio buffer.
  std::vector<int16> samples;

  /// \brief The total duration -- in seconds -- of the buffer.
  real32 duration = 0.0f;

  uint32 channel_count = 0;
  uint32 channel_format = 0;
  uint32 sample_rate = 0;
  nom::size_type sample_count = 0;
  nom::size_type audio_bytes = 0;

  /// \brief The total number of frames in this sample instance
  nom::size_type frame_count;
};

} // namespace nom

#endif // include guard defined
