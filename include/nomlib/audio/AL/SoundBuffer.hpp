/******************************************************************************

    OpenAL SoundBuffer

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUNDBUFFER_HEADERS
#define NOMLIB_AL_SOUNDBUFFER_HEADERS

#include <memory>
#include <set>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio/AL/AudioDevice.hpp"
#include "nomlib/audio/AL/SoundFile.hpp"

class Sound; // forward declaration

namespace nom {
  namespace OpenAL {

class SoundBuffer
{
  public:
    SoundBuffer ( void );
    ~SoundBuffer ( void );

    /// Obtain buffer data
    ALuint get ( void ) const;

    /// Obtain buffer duration in milliseconds
    ///
    /// Default: zero (0)
    uint32 getDuration ( void ) const;

    // getSampleRate
    // getChannelCount
    // ...

    bool loadFromFile ( const std::string& filename );

  private:
    ALuint buffers[NUM_BUFFERS];
    /// We load our audio data into this buffer
    std::vector<int16> samples;
    std::set <std::shared_ptr<Sound>> sounds;
    /// Duration of sound buffer
    ///
    /// Default: zero (0)
    uint32 buffer_duration;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDBUFFER_HEADERS defined
