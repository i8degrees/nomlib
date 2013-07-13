/******************************************************************************

    OpenAL Headers

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_HEADERS
#define NOMLIB_OPENAL_HEADERS

#include "config.hpp"

#include <iostream>
#include <string>

#if defined NOMLIB_SYSTEM_WINDOWS
  #include <AL/al.h>
  #include <AL/alc.h>
#else // Assumes configuration to be alike OSX
  #include <OpenAL/al.h>
  #include <OpenAL/alc.h>
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

#ifdef NOMLIB_DEBUG
  #define AL_ERR(Function) \
    ( (Function ), nom::Logger::al_check_err ( __FILE__, __LINE__ ) )
#else
  #define AL_ERR(Function) ( Function )
#endif

#endif // NOMLIB_OPENAL_HEADERS defined
