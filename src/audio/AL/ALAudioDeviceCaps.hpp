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
#ifndef NOMLIB_AUDIO_AL_DEVICE_CAPS_HPP
#define NOMLIB_AUDIO_AL_DEVICE_CAPS_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/AL/SoundSource.hpp"

namespace nom {

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
class IOAudioEngine
{
  public:
    // IOAudioEngine();
    virtual ~IOAudioEngine();

    virtual bool fill_audio_buffer(SoundBuffer* target) = 0;
    virtual uint32 state(SoundBuffer* target) = 0;
    virtual real32 pitch(SoundBuffer* target) = 0;
    virtual void set_state(SoundBuffer* target, AudioState) = 0;
    virtual void set_volume(real32 gain) = 0;
    virtual void set_volume(SoundBuffer* target, real32 gain) = 0;
    virtual void set_velocity(SoundBuffer* target, const Point3f& v) = 0;
    virtual void set_pitch(SoundBuffer* target, real32 pitch) = 0;
    virtual void set_playback_position(SoundBuffer* target, real32 offset_seconds) = 0;
    virtual void play(SoundBuffer* target) = 0;
    virtual void stop(SoundBuffer* target) = 0;
    virtual void pause(SoundBuffer* target) = 0;
    virtual void resume(SoundBuffer* target) = 0;

  protected:
    IOAudioEngine();

  private:
    //
};

class ALAudioEngine: public IOAudioEngine
{
  public:
    ALAudioEngine();
    virtual ~ALAudioEngine();

    virtual bool fill_audio_buffer(SoundBuffer* target) override;
    virtual uint32 state(SoundBuffer* target) override;
    virtual real32 pitch(SoundBuffer* target) override;
    virtual void set_state(SoundBuffer* target, AudioState) override;
    virtual void set_volume(real32 gain) override;
    virtual void set_volume(SoundBuffer* target, real32 gain) override;
    virtual void set_velocity(SoundBuffer* target, const Point3f& v) override;
    virtual void set_pitch(SoundBuffer* target, real32 pitch) override;
    virtual void set_playback_position(SoundBuffer* target, real32 offset_seconds) override;
    virtual void play(SoundBuffer* target) override;
    virtual void stop(SoundBuffer* target) override;
    virtual void pause(SoundBuffer* target) override;
    virtual void resume(SoundBuffer* target) override;

  private:
    // ...
};

} // namespace nom

#endif // include guard defined
