#pragma once

#include "nomlib/audio2/Sound.hpp"
#include "nomlib/types.hpp"

namespace nom {
namespace audio {

// OggSound
class StreamSound: public Sound
{
  private:
    void init(const std::string& filename);
    void init(nom::int16* samples, nom::uint32 format, int num_channels);

  public:
    typedef Sound super;
    virtual ~StreamSound();

    void close();

    // NO-OP init
    StreamSound();
    // init(filename);
    StreamSound(const std::string& filename);
    // init(samples, fmt, num_channels);
    StreamSound(nom::int16* samples, nom::uint32 format, int channels);

    // TODO(JEFF): Change return type of bool to void?
    virtual bool open(const std::string& filename);

    virtual nom::real64 stream(nom::uint32 buffer, nom::real64 pos);
};

} // namespace audio
} // namespace nom
