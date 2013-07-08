/******************************************************************************

    OpenAL SoundFile

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_OPENAL_SOUNDFILE_HEADERS
#define NOMLIB_OPENAL_SOUNDFILE_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include <sndfile.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#include "nomlib_config.hpp"

namespace nom {

class OpenAL_SoundFile
{
  public:
    OpenAL_SoundFile ( void );
    ~OpenAL_SoundFile ( void );

    std::size_t getSampleCount ( void ) const;
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

} // namespace nom

#endif // NOMLIB_OPENAL_SOUNDFILE_HEADERS defined
