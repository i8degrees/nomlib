/******************************************************************************

    OpenAL SoundBuffer

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_SOUNDBUFFER_HEADERS
#define NOMLIB_OPENAL_SOUNDBUFFER_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {

class OpenAL_SoundBuffer
{
  public:
    OpenAL_SoundBuffer ( void );
    ~OpenAL_SoundBuffer ( void );

  private:
    // ...
};

} // namespace nom

#endif // NOMLIB_OPENAL_SOUNDBUFFER_HEADERS defined
