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
