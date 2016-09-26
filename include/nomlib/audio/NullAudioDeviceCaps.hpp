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
#ifndef NOMLIB_AUDIO_NULL_DEVICE_CAPS_HPP
#define NOMLIB_AUDIO_NULL_DEVICE_CAPS_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/IOAudioEngine.hpp"

namespace nom {
namespace audio {

// Forward declarations
struct SoundBuffer;

// TODO(jeff): Add const to getters!
class NullAudioEngineCaps: public IOAudioEngine
{
  public:
    NullAudioEngineCaps();
    virtual ~NullAudioEngineCaps();

    virtual
    uint32 channel_format(uint32 num_channels, uint32 channel_format) override;

    virtual bool valid_audio_buffer(SoundBuffer* buffer) override;
    virtual bool valid_sound_buffer(SoundBuffer* buffer) override;

    virtual uint32 state(SoundBuffer* target) override;
    virtual real32 pitch(SoundBuffer* target) override;

    virtual real32 volume() const override;
    virtual Point3f position() const override;

    virtual real32 volume(SoundBuffer* buffer) const override;
    virtual real32 min_volume(SoundBuffer* buffer) override;
    virtual real32 max_volume(SoundBuffer* buffer) override;

    virtual Point3f velocity(SoundBuffer* buffer) override;
    virtual Point3f position(SoundBuffer* buffer) override;
    virtual real32 playback_position(SoundBuffer* buffer) override;
    virtual real32 playback_samples(SoundBuffer* buffer) override;

    virtual void set_state(SoundBuffer* target, uint32 state) override;

    virtual void set_volume(real32 gain) override;
    virtual void set_position(const Point3f& p) override;

    virtual void set_min_volume(SoundBuffer* target, real32 gain) override;
    virtual void set_max_volume(SoundBuffer* target, real32 gain) override;
    virtual void set_volume(SoundBuffer* target, real32 gain) override;

    virtual void set_velocity(SoundBuffer* target, const Point3f& v) override;
    virtual void set_position(SoundBuffer* target, const Point3f& p) override;
    virtual void set_pitch(SoundBuffer* target, real32 pitch) override;
    virtual void set_playback_position(SoundBuffer* target, real32 offset_seconds) override;
    virtual void play(SoundBuffer* target) override;
    virtual void stop(SoundBuffer* target) override;
    virtual void pause(SoundBuffer* target) override;
    virtual void resume(SoundBuffer* target) override;

    virtual bool fill_audio_buffer(SoundBuffer* buffer) override;
    virtual void free_buffer(SoundBuffer* buffer) override;

    virtual void suspend() override;
    virtual void resume() override;
};

} // namespace audio
} // namespace nom

#endif // include guard defined
