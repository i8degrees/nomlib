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

#include "OpenAL.hpp"

namespace nom {
  namespace OpenAL {

enum Channels
{
  Mono = AL_FORMAT_MONO16,
  Stereo = AL_FORMAT_STEREO16
};

class SoundFile
{
  public:
    SoundFile ( void );
    ~SoundFile ( void );

    size_t getSampleCount ( void ) const;
    uint32 getChannelCount ( void ) const;
    uint32 getSampleRate ( void ) const;
    uint32 getChannelFormat ( void ) const;

    bool open ( const std::string& filename );
    bool read ( std::vector<int16>& data );

  private:
    /// SNDFILE* file descriptor
    std::shared_ptr<SNDFILE> fp;
    /// Extracted audio stream from file
    std::vector<int16> samples;
    /// Total number of samples in the file
    std::size_t sample_count;
    /// Number of channels used by sound; mono if one, stereo if two
    uint32 channel_count;
    /// Number of samples per second
    uint32 sample_rate;
    /// Mono or stereo channels
    int32 channel_format;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDFILE_HEADERS defined
