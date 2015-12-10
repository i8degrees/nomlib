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
#include <fstream>

#include "nomlib/config.hpp"

// Forward declarations
namespace nom {

struct SoundBuffer;

} // namespace nom

struct SNDFILE_tag
{
  SNDFILE_tag* fp = nullptr;
  // nom::SoundBuffer* info = nullptr;
  nom::SoundBuffer* buffer = nullptr;
};

namespace nom {

// typedef std::ifstream file_handle;
typedef SNDFILE_tag file_handle;
// typedef SoundBuffer file_handle;
// struct SoundBuffer;

class IStream
{
  public:
    //
    IStream();
    ~IStream();

    virtual bool valid() = 0;
    virtual bool good();
    // virtual bool eof() = 0;

    virtual bool open(file_handle* in, const std::string& filename) = 0;
    virtual bool read(file_handle* in, nom::size_type read_size) = 0;
    virtual bool seek(file_handle* in, int offset) = 0;
    virtual void close() = 0;
};

class SoundFile: public IStream
{
  public:
    SoundFile();
    virtual ~SoundFile();

    // TODO: Remove!
    /// Obtain number of samples in audio file
    int64 getSampleCount() const;

    // TODO: Remove!
    /// Obtain number of channels used by audio file
    uint32 getChannelCount() const;

    // TODO: Remove!
    /// Obtain audio sampling rate; this is the number of samples per second
    uint32 getSampleRate() const;

    // TODO: Remove!
    /// Obtain audio data size in bytes
    int64 getDataByteSize() const;

    virtual bool good() override;
    virtual bool valid() override;
    // virtual bool eof() const override;

    // WAVFileStream (i.e.: WAVMemoryStream, WAVCompressedStream, etc.)
    // file_handle = WAVFileStream
    virtual bool open(file_handle* in, const std::string& filename) override;
    virtual bool read(file_handle* in, nom::size_type read_size) override;
    virtual bool seek(file_handle* in, int offset) override;
    virtual void close() override;

  // private:
    /// \brief A third-party file descriptor whose owned by the end-user.
    ///
    /// \see libsndfile
    // TODO: Remove stored pointer; pass in as file_handle, i.e.: external
    SNDFILE_tag* fp_;
    // SoundBuffer* fp_;

    // TODO: Use nom::SoundBuffer from **here** on out
    /// Extracted audio stream from file
    std::vector<int16> samples;
    /// Total number of samples in the file
    int64 sample_count;
    /// Number of audio channels used by sound
    uint32 channel_count;
    /// Number of samples per second
    uint32 sample_rate;

    uint32 frame_count = 0;
};

std::string libsndfile_version();

} // namespace nom

#endif // NOMLIB_AL_SOUNDFILE_HEADERS defined
