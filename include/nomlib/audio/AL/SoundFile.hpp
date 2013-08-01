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

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"

namespace nom {
  namespace OpenAL {

class SoundFile
{
  public:
    SoundFile ( void );
    ~SoundFile ( void );

    /// Obtain number of samples in audio file
    size_t getSampleCount ( void ) const;

    /// Obtain number of channels used by audio file
    uint32 getChannelCount ( void ) const;

    /// Obtain audio sampling rate; this is the number of samples per second
    uint32 getSampleRate ( void ) const;

    /// Obtain channel format; used internally by OpenAL
    uint32 getChannelFormat ( void ) const;

    /// Obtain audio data size in bytes
    size_t getDataByteSize ( void ) const;

    bool open ( const std::string& filename );
    bool read ( std::vector<int16>& data );

  private:
    /// SNDFILE* file descriptor
    std::shared_ptr<SNDFILE> fp;
    /// Extracted audio stream from file
    std::vector<int16> samples;
    /// Total number of samples in the file
    std::size_t sample_count;
    /// Number of audio channels used by sound
    uint32 channel_count;
    /// Number of samples per second
    uint32 sample_rate;
    /// OpenAL compatible audio channels used by sound
    int32 channel_format;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDFILE_HEADERS defined
