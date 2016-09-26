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
#ifndef NOMLIB_AL_OSX_APPLE_EXTENSIONS_HPP
#define NOMLIB_AL_OSX_APPLE_EXTENSIONS_HPP

#include "nomlib/config.hpp"

// Forward declarations
struct ALCdevice_struct;

namespace nom {
namespace audio {

/// \brief Get the sampling rate of audio playback.
///
/// \see http://www.opensource.apple.com/source/OpenAL/OpenAL-54/Source/OpenAL/READ_ME
real64 sample_rate(ALCdevice_struct* target);

/// \brief Set the sampling rate of audio playback.
///
/// \remarks This allows us to optimize the audio mixing performance based on
/// the sampling rate of its audio assets.
///
/// \see http://www.opensource.apple.com/source/OpenAL/OpenAL-54/Source/OpenAL/READ_ME
void set_sample_rate(real64 sample_rate);

/// \brief Get the maximum number of simultaneous playing audio buffers.
///
/// \see http://www.opensource.apple.com/source/OpenAL/OpenAL-54/Source/OpenAL/READ_ME
int max_sources();

/// \brief Set the maximum number of simultaneous playing audio buffers.
///
/// \see http://www.opensource.apple.com/source/OpenAL/OpenAL-54/Source/OpenAL/READ_ME
void set_max_sources(int num_sources);

} // namespace audio
} // namespace nom

#endif // include guard defined
