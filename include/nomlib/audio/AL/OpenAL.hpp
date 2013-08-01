/******************************************************************************

    OpenAL Headers

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_HEADERS
#define NOMLIB_OPENAL_HEADERS

#include <iostream>
#include <string>
#include <ctime>

#include "nomlib/config.hpp"
#include "nomlib/sys/Clock.hpp"

#if defined NOM_PLATFORM_OSX
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
#else // Assumes configuration to be alike (Ubuntu) Linux & Windows platforms
  #include <AL/al.h>
  #include <AL/alc.h>
#endif

/// Seven (7) buffers appears to be the highest my MacBook Air mid-2011 model
/// can achieve; using Intel 6 Series/C200 Audio Chipset
///
/// Minimum value is 1
const nom::uint32 NUM_BUFFERS = 4;

/// Optimal sound frame size (in bytes); used by libsndfile
const nom::uint32 BUFFER_SIZE = 4096;
const nom::uint32 NUM_SOURCES = 16; // not implemented

const float MIN_VOLUME = 0.0;
const float MAX_VOLUME = 100.0;

#ifdef NOM_DEBUG
  #define AL_CHECK_ERR(Function) \
    ( (Function ), nom::priv::al_err ( __FILE__, __LINE__ ) )
#else
  #define AL_CHECK_ERR(Function) ( Function )
#endif

namespace nom {
  namespace priv {

void al_err ( const std::string& file, nom::uint32 line );

  } // namespace priv
} // namespace nom


#endif // NOMLIB_OPENAL_HEADERS defined
