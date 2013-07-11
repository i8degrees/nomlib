/******************************************************************************

    OpenAL SoundBuffer

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUNDBUFFER_HEADERS
#define NOMLIB_AL_SOUNDBUFFER_HEADERS

#include <vector>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "SoundFile.hpp"
#include "config.hpp"

namespace nom {
  namespace OpenAL {

#define NUM_BUFFERS 4
#define BUFFER_SIZE 4096

class SoundBuffer
{
  public:
    SoundBuffer ( void );
    ~SoundBuffer ( void );

    ALuint get ( void );
    uint32 getDuration ( void );

    bool loadFromFile ( const std::string& filename );

  private:
    ALuint buffers[NUM_BUFFERS];
    /// Duration of sound buffer
    /// Default: zero (0)
    uint32 buffer_duration;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDBUFFER_HEADERS defined
