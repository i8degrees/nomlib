/******************************************************************************

    OpenAL SoundBuffer

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUNDBUFFER_HEADERS
#define NOMLIB_AL_SOUNDBUFFER_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {
  namespace OpenAL {

class SoundBuffer
{
  public:
    SoundBuffer ( void );
    ~SoundBuffer ( void );

  private:
    // ...
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDBUFFER_HEADERS defined
