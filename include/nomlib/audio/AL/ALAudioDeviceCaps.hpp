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
#ifndef NOMLIB_AUDIO_AL_DEVICE_CAPS_HPP
#define NOMLIB_AUDIO_AL_DEVICE_CAPS_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/IOAudioEngine.hpp"

// FIXME(jeff): enums
#include "nomlib/audio/AL/SoundSource.hpp"

// Forward declarations
struct ALCdevice_struct;
struct ALCcontext_struct;

namespace nom {
namespace audio {

// Forward declarations
struct SoundBuffer;
// struct AudioSpec;
struct ALAudioDevice;

// Function declarations

void free_buffer(uint32 buffer_id);
void free_source(uint32 buffer_id);

uint32 buffer_id(const SoundBuffer* target);
uint32 source_id(const SoundBuffer* target);

/// \brief Register a buffer with the audio subsystem.
///
/// \returns A unique identifier, representing its allocation made with the
/// underlying audio system.
///
/// \see nom::audio::create_buffer
uint32 next_buffer_id();

/// \brief Register an array of buffers with the audio subsystem.
///
/// \returns When successful, a pointer to the beginning of an array the size
/// of the given number of sources with 32-bit integer offset alignment. On
/// failure to register the total given number of sources, a NULL pointer is
/// returned.
///
/// \see nom::audio::create_buffer
uint32* next_buffer_id(uint32 num_buffers);

/// \brief Register a buffer with the audio subsystem.
///
/// \returns A unique identifier, representing its allocation made with the
/// underlying audio system.
///
/// \see nom::audio::fill_audio_buffer
uint32 next_source_id();

/// \brief Register an array of sound sources with the audio subsystem.
///
/// \returns When successful, a pointer to the beginning of an array the size
/// of the given number of sources with 32-bit integer offset alignment. On
/// failure to register the total given number of sources, a NULL pointer is
/// returned.
///
/// \see nom::audio::create_buffer
uint32* next_source_id(uint32 num_sources);

// TODO(jeff): Add const to getters!
class ALAudioEngine: public IOAudioEngine
{
  public:
    ALAudioEngine();
    virtual ~ALAudioEngine();

    ALAudioEngine(ALAudioDevice* driver);
    virtual void init(void* driver) override;

    virtual bool valid() const override;

    virtual uint32 caps() const override;
    virtual void set_cap(uint32 format) override;

    virtual bool connected() const override;

    /// \brief Find a suitable data format for OpenAL.
    virtual
    uint32 channel_format(uint32 num_channels, uint32 channel_format) override;

    virtual bool valid_buffer(SoundBuffer* target) override;
    virtual bool valid_source(SoundBuffer* target) override;

    virtual uint32 state(SoundBuffer* target) override;
    virtual real32 pitch(SoundBuffer* target) override;

    virtual real32 volume() const override;
    virtual Point3f position() const override;

    virtual real32 volume(SoundBuffer* target) const override;
    virtual real32 min_volume(SoundBuffer* target) override;
    virtual real32 max_volume(SoundBuffer* target) override;

    virtual Point3f velocity(SoundBuffer* target) override;
    virtual Point3f position(SoundBuffer* target) override;
    virtual real32 playback_position(SoundBuffer* target) override;
    virtual real32 playback_samples(SoundBuffer* target) override;

    // virtual void set_state(SoundBuffer* target, uint32 state) override;

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

    virtual bool push_buffer(SoundBuffer* target) override;
    virtual bool queue_buffer(SoundBuffer* target) override;

    // IMPORTANT(jeff): The audio hardware capabilities, maximum sources,
    // limitations and so on are subject to change anytime that we touch the
    // audio context state!
    virtual void suspend() override;
    virtual void resume() override;
    virtual void close() override;

    virtual void free_buffer(SoundBuffer* target) override;

  private:
    bool fill_buffer(SoundBuffer* target);

    virtual void close_context();
    virtual void close_device();

    ALAudioDevice* impl_ = nullptr;
};

} // namespace audio
} // namespace nom

#endif // include guard defined
