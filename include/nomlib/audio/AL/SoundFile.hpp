/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#ifndef NOMLIB_AL_SOUNDFILE_HEADERS
#define NOMLIB_AL_SOUNDFILE_HEADERS

#include <iostream>
#include <string>
#include <memory>
#include <vector>

#include "nomlib/config.hpp"

// Forward declarations
struct SNDFILE_tag {};

namespace nom {

class SoundFile
{
  public:
    SoundFile ( void );
    ~SoundFile ( void );

    /// Obtain number of samples in audio file
    int64 getSampleCount ( void ) const;

    /// Obtain number of channels used by audio file
    uint32 getChannelCount ( void ) const;

    /// Obtain audio sampling rate; this is the number of samples per second
    uint32 getSampleRate ( void ) const;

    /// Obtain channel format; used internally by OpenAL
    uint32 getChannelFormat ( void ) const;

    /// Obtain audio data size in bytes
    int64 getDataByteSize ( void ) const;

    bool open ( const std::string& filename );
    bool read ( std::vector<int16>& data );

  private:
    /// SNDFILE* file descriptor
    /// \todo Change me to a std::unique_ptr
    std::shared_ptr<SNDFILE_tag> fp;
    /// Extracted audio stream from file
    std::vector<int16> samples;
    /// Total number of samples in the file
    int64 sample_count;
    /// Number of audio channels used by sound
    uint32 channel_count;
    /// Number of samples per second
    uint32 sample_rate;
    /// OpenAL compatible audio channels used by sound
    int32 channel_format;
};

std::string libsndfile_version();

} // namespace nom

#endif // NOMLIB_AL_SOUNDFILE_HEADERS defined
