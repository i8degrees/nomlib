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
#ifndef NOMLIB_AUDIO_IO_AUDIO_ENGINE_HPP
#define NOMLIB_AUDIO_IO_AUDIO_ENGINE_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"

namespace nom {
namespace audio {

// Forward declarations
struct SoundBuffer;

/// \brief Abstract base class for an instance of the audio hardware interface.
///
/// \ntoe An audio driver is required to subclass IOAudioDevice in order to
/// provide working audio to the system. A single driver instance will contain
/// a single instance of the driver's IOAudioDevice subclass. The subclass is
/// responsible for mapping all hardware device resources from the service
/// provider nub. It must control access to the hardware so that the hardware
/// doesn't fall into an inconsistent state.

// TODO(jeff): Add const to getters!
class IOAudioEngine
{
  protected:
    IOAudioEngine()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO,
                         NOM_LOG_PRIORITY_VERBOSE);
    }

  public:
    virtual ~IOAudioEngine()
    {
      NOM_LOG_TRACE_PRIO(NOM_LOG_CATEGORY_TRACE_AUDIO,
                         NOM_LOG_PRIORITY_VERBOSE);
    }

    virtual void init(void* driver) = 0;

    virtual bool valid() const = 0;

    // virtual int sample_rate() const = 0;
    // virtual int mono_sources() const = 0;
    // virtual int stereo_sources() const = 0;

    virtual uint32 caps() const = 0;
    virtual void set_cap(uint32 format) = 0;

    virtual bool connected() const = 0;

    virtual
    uint32 channel_format(uint32 num_channels, uint32 channel_format) = 0;

    virtual bool valid_buffer(SoundBuffer* buffer) = 0;
    virtual bool valid_source(SoundBuffer* buffer) = 0;

    virtual uint32 state(SoundBuffer* target) = 0;
    virtual real32 pitch(SoundBuffer* target) = 0;

    virtual real32 volume() const = 0;
    virtual Point3f position() const = 0;

    virtual real32 volume(SoundBuffer* buffer) const = 0;
    virtual real32 min_volume(SoundBuffer* buffer) = 0;
    virtual real32 max_volume(SoundBuffer* buffer) = 0;

    virtual Point3f velocity(SoundBuffer* buffer) = 0;
    virtual Point3f position(SoundBuffer* buffer) = 0;
    virtual real32 playback_position(SoundBuffer* buffer) = 0;
    virtual real32 playback_samples(SoundBuffer* buffer) = 0;

    // virtual void set_state(SoundBuffer* target, uint32 state) = 0;

    virtual void set_volume(real32 gain) = 0;
    virtual void set_position(const Point3f& p) = 0;

    virtual void set_volume(SoundBuffer* target, real32 gain) = 0;
    virtual void set_min_volume(SoundBuffer* target, real32 gain) = 0;
    virtual void set_max_volume(SoundBuffer* target, real32 gain) = 0;

    virtual void set_velocity(SoundBuffer* target, const Point3f& v) = 0;
    virtual void set_position(SoundBuffer* target, const Point3f& p) = 0;
    virtual void set_pitch(SoundBuffer* target, real32 pitch) = 0;
    virtual void set_playback_position(SoundBuffer* target, real32 offset_seconds) = 0;
    virtual void play(SoundBuffer* target) = 0;
    virtual void stop(SoundBuffer* target) = 0;
    virtual void pause(SoundBuffer* target) = 0;
    virtual void resume(SoundBuffer* target) = 0;

    virtual bool push_buffer(SoundBuffer* buffer) = 0;
    virtual bool queue_buffer(SoundBuffer* buffer) = 0;

    virtual void suspend() = 0;
    virtual void resume() = 0;
    virtual void close() = 0;

    virtual void free_buffer(SoundBuffer* buffer) = 0;
};

} // namespace audio
} // namespace nom

#endif // include guard defined
