#pragma once

#include "nomlib/types.hpp"

namespace nom {
namespace audio {

// Pure abstract interface class
class IAudioContext
{
  public:
    // typedef void void_ptr;
    // typedef void value_type;

    virtual ~IAudioContext()
    {
    }

    //virtual value_type* context() = 0;
    //virtual void set_context(value_type* ctx) = 0;
    virtual void release() = 0;

    virtual void resume() = 0; // unwind (begin)
    virtual void pause() = 0; // suspsned
                              //
    virtual bool connected() = 0;
    virtual bool valid() = 0;

    virtual nom::real32 master_volume() = 0;
    virtual void set_master_volume(nom::real32 gain) = 0;

    virtual void update(nom::real32 delta) = 0;
    //virtual void render(nom::real32 delta) = 0;

    static const nom::uint32 num_mono_sources_ = 128;
    static const nom::uint32 num_stereo_sources_ = 128;
    static const nom::uint32 max_mono_sources_ = 128;
    static const nom::uint32 max_stereo_sources_ = 128;

  protected:
    IAudioContext() {

    }

    nom::real32 master_gain_;
    int refresh_hz_ = 0;
    int sync_ = 0;
    int sample_rate_ = 0;
};

} // namespace audio
} // namespace nom
