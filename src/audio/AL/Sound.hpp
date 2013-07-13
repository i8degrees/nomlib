/******************************************************************************

    OpenAL Sound Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUND_HEADERS
#define NOMLIB_AL_SOUND_HEADERS

#include <iostream>
#include <memory>

#include "OpenAL.hpp"
#include "SoundBuffer.hpp"
#include "SoundSource.hpp"

namespace nom {
  namespace OpenAL {

/// A sample is the amplitude of the sound signal at a given point of time, and
/// an array -- 16-bit signed integers -- of samples therefore represents a full
/// sound.
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

  private:
    //SoundBuffer buffer;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUND_HEADERS defined
