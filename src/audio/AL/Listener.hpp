/******************************************************************************

    OpenAL Audio Listener

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_LISTENER_HEADERS
#define NOMLIB_AL_LISTENER_HEADERS

#include <iostream>
#include <memory>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {
  namespace OpenAL {

class Listener
{
  public:
    Listener ( void );
    ~Listener ( void );

  private:
    // ...
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_LISTENER_HEADERS defined
