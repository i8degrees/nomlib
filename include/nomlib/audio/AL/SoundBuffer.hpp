/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#ifndef NOMLIB_AL_SOUNDBUFFER_HEADERS
#define NOMLIB_AL_SOUNDBUFFER_HEADERS

#include <memory>
#include <set>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio/AL/AudioDevice.hpp"
#include "nomlib/audio/AL/SoundFile.hpp"

class Sound; // forward declaration

namespace nom {
  namespace OpenAL {

class SoundBuffer
{
  public:
    SoundBuffer ( void );
    ~SoundBuffer ( void );

    /// Obtain buffer data
    ALuint get ( void ) const;

    /// Obtain buffer duration in milliseconds
    ///
    /// Default: zero (0)
    uint32 getDuration ( void ) const;

    // getSampleRate
    // getChannelCount
    // ...

    bool loadFromFile ( const std::string& filename );

  private:
    ALuint buffers[NUM_BUFFERS];
    /// We load our audio data into this buffer
    std::vector<int16> samples;
    std::set <std::shared_ptr<Sound>> sounds;
    /// Duration of sound buffer
    ///
    /// Default: zero (0)
    uint32 buffer_duration;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDBUFFER_HEADERS defined
