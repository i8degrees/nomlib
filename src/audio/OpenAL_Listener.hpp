/******************************************************************************

    OpenAL Audio Listener

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_LISTENER_HEADERS
#define NOMLIB_OPENAL_LISTENER_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {

class OpenAL_Listener
{
  public:
    OpenAL_Listener ( void );
    ~OpenAL_Listener ( void );

  private:
    // ...
};

} // namespace nom

#endif // NOMLIB_OPENAL_LISTENER_HEADERS defined
