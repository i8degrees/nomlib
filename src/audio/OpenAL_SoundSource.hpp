/******************************************************************************

    OpenAL SoundSource

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_SOUNDSOURCE_HEADERS
#define NOMLIB_OPENAL_SOUNDSOURCE_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {

class OpenAL_SoundSource
{
  public:
    OpenAL_SoundSource ( void );
    ~OpenAL_SoundSource ( void );

  private:
    // ...
};

} // namespace nom

#endif // NOMLIB_OPENAL_SOUNDSOURCE_HEADERS defined
