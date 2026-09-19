#pragma once

#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisenc.h>
#include <vorbis/vorbisfile.h>

#include "nomlib/audio2/Sound.hpp"
#include "nomlib/types.hpp"

typedef FILE file_handle;
typedef OggVorbis_File OggFile;

#ifndef BUFFERLENGTH
  #define BUFFERLENGTH 65536 // 64 kilobytes
#endif

namespace nom {
namespace audio {

class OggSound: public Sound
{
  protected:
    file_handle* file_handle_;
    OggFile sfp_;
  private:
    // See also
    // https://xiph.org/vorbis/doc/libvorbis/return.html
    void check_error(const std::string& filename, int error_code);

    void init(const std::string& filename);
    void init(nom::int16* samples, nom::uint32 format, int num_channels);
    //bool seek_stream(nom::real64 pos);
    void close();

  public:
    typedef Sound super;

    virtual ~OggSound();

    // NO-OP init
    OggSound();
    // init(filename);
    OggSound(const std::string& filename);
    // init(samples, fmt, num_channels);
    OggSound(nom::int16* samples, nom::uint32 format, int channels);

    // TODO(JEFF): Change return type of bool to void?
    virtual bool open(const std::string& filename);

    virtual nom::real64 stream(nom::uint32 buffer, nom::real64 pos);
    virtual bool stream__(nom::uint32 buffer, nom::real64 pos);
};

} // namespace audio
} // namespace nom
