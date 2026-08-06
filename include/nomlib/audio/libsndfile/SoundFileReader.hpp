/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014, 2015, 2016 Jeffrey Carpenter <i8degrees@gmail.com>
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
#ifndef NOMLIB_AUDIO_LIBSNDFILE_SOUND_FILE_READER_HPP
#define NOMLIB_AUDIO_LIBSNDFILE_SOUND_FILE_READER_HPP

#include <string>

#include "export.hpp"
#include "nomlib/config.hpp"
#include "nomlib/audio/ISoundFileReader.hpp"

// Forward declarations
typedef struct SF_INFO SF_INFO;
typedef struct sf_private_tag SNDFILE_tag;

namespace nom {
namespace audio {

NOM_EXPORT std::string libsndfile_version();

// TODO(jeff): Error checking
// TODO(jeff): Use nom::err
class NOM_EXPORT SoundFileReader: public ISoundFileReader
{
  public:
    SoundFileReader();

    ~SoundFileReader();

    virtual bool valid() const override;

    virtual bool open(const std::string& filename, SoundInfo& info) override;

    // The channel format is XXX -- see audio::ChannelFormat for the options
    // of this enumeration.
    // One frame is equal to one sample whereas on multi-channel formats, one
    // frame is equal to one sample of each channel.
    virtual int64
    read(void* data, uint32 channel_format, nom::size_type frames) override;

    /// \param offset The cursor offset position, depicted in audio frames.
    virtual int64 seek(int64 offset, SoundSeek dir) override;

    virtual void close() override;

  private:
    SoundInfo parse_header(SF_INFO& metadata);
    NOM_EXPORT static const char* parse_tags(SNDFILE_tag* fp, uint32 sound_tag);

    /// \brief A third-party file descriptor whose owned by the end-user.
    ///
    /// \see libsndfile
    SNDFILE_tag* fp_ = nullptr;
};

} // namespace audio
} // namespace nom

#endif // NOMLIB_AUDIO_LIBSNDFILE_SOUND_FILE_READER_HPP defined
