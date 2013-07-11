/******************************************************************************

    OpenAL Sound Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUND_HEADERS
#define NOMLIB_AL_SOUND_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "config.hpp"

namespace nom {
  namespace OpenAL {

/// A sample is the amplitude of the sound signal at a given point of time, and
/// an array -- 16-bit signed integers -- of samples therefore represents a full
/// sound.
class Sound
{
  public:
    Sound ( void );
    ~Sound ( void );

  private:
    // ...
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUND_HEADERS defined
