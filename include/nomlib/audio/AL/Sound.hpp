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
#ifndef NOMLIB_AL_SOUND_HEADERS
#define NOMLIB_AL_SOUND_HEADERS

#include <iostream>

#include "nomlib/config.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio/AL/SoundBuffer.hpp"
#include "nomlib/audio/AL/SoundSource.hpp"

namespace nom {
  namespace OpenAL {

// forward declarations
class SoundBuffer;

/// \brief Audio interface for sound samples
class Sound: public SoundSource // "is-a relationship"
{
  public:
    Sound ( void );
    Sound ( const SoundBuffer& copy );
    ~Sound ( void );

    void setBuffer ( const SoundBuffer& copy );

    void Play ( void );
    void Stop ( void );
    void Pause ( void );

    /// Obtain the current playback position of source in seconds
    //float getPlayPosition ( void ) const;

    /// Set playback position of source in seconds
    //void setPlayPosition ( float seconds );

    /// Internally used by SoundBuffer class for properly freeing a sound from
    /// its attached buffer
    void reset ( void );
  private:
    /// Buffer that this sound is attached to
    const SoundBuffer* buffer;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUND_HEADERS defined

/// \class nom::Sound
/// \ingroup Audio
///
/// A sample is the amplitude of the sound signal at a given point of time, and
/// an array -- 16-bit signed integers -- of samples therefore represents a full
/// sound.
