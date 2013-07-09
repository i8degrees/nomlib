/******************************************************************************

    OpenAL SoundFile

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUNDFILE_HEADERS
#define NOMLIB_AL_SOUNDFILE_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <sndfile.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "config.hpp"

namespace nom {
  namespace OpenAL {

class SoundFile
{
  public:
    SoundFile ( void );
    ~SoundFile ( void );

    size_t getSampleCount ( void ) const;
    uint32 getChannelCount ( void ) const;
    uint32 getSampleRate ( void ) const;

    bool loadFromFile ( const std::string& filename );

  private:
    /// SNDFILE* file descriptor
    std::shared_ptr<SNDFILE> fp;
    /// Total number of samples in the file
    std::size_t sample_count;
    /// Number of channels used by sound
    uint32 channel_count;
    /// Number of samples per second
    uint32 sample_rate;
    std::vector<uint16_t> snd;
    /// Read buffer
    std::vector<int16_t> read_buffer;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDFILE_HEADERS defined
