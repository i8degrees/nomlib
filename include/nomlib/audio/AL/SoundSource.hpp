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
#ifndef NOMLIB_AL_SOUNDSOURCE_HEADERS
#define NOMLIB_AL_SOUNDSOURCE_HEADERS

#include <memory>
#include <algorithm>

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/ISoundSource.hpp"

namespace nom {

// Forward declarations
class IAudioDevice;
struct SoundBuffer;

/// Sound source is one of the three states: stopped, paused or playing
enum AudioState
{
  AUDIO_STATE_STOPPED = 0,
  AUDIO_STATE_PAUSED,
  AUDIO_STATE_PLAYING,

  // TODO: Bit mask..?
  AUDIO_STATE_LOOPING
};

// enum AudioFormat
// {
//   AUDIO_FORMAT_MONO16,
//   AUDIO_FORMAT_STEREO16,
//   AUDIO_FORMAT_QUAD16,
//   AUDIO_FORMAT_51CHN16,
//   AUDIO_FORMAT_61CHN16
// };

/// \brief Base class for audio inputs
/*
class SoundSource: public ISoundSource
{
  public:
    virtual ~SoundSource();

    // real64 duration() const override;

    /// \brief Get the gain level of this audio source.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 volume() const override;

    /// \brief Get the minimum gain level of this audio source.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 min_volume() const override;

    /// \brief Get the maximum gain level of this audio source.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 max_volume() const override;

    real32 pitch() const override;
    bool loop() const override;

    Point3f position() const override;
    Point3f velocity() const override;

    bool getPositionRelativeToListener() const override;
    real32 getMinDistance() const override;
    real32 getAttenuation() const override;

    /// Obtain buffer identifier of source
    uint32 buffer_id() const override;

    /// Obtain the current playback position of source in seconds
    real32 getPlayPosition() const override;

    /// Obtain current state of sound
    uint32 state() const override;

    /// \brief Set a new gain level for this audio source.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max).
    ///
    /// \remarks This controls the gain level of this audio source.
    void set_volume(real32 gain) override;

    /// \brief Set a new minimum gain level for this audio source.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max).
    ///
    /// \remarks This controls the minimum gain level of this audio source.
    void set_min_volume(real32 gain) override;

    /// \brief Set a new maximum gain level for this audio source.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max).
    ///
    /// \remarks This controls the maximum gain level of this audio source.
    void set_max_volume(real32 gain) override;

    void setPitch(real32 pitch) override;
    void setLooping(bool loops) override;

    void setPosition(real32 x, real32 y, real32 z) override;
    void setPosition(const Point3f& position) override;

    void set_velocity(real32 x, real32 y, real32 z) override;
    void set_velocity(const Point3f& velocity) override;

    void setPositionRelativeToListener(bool position) override;
    void setMinDistance(real32 distance) override;
    void setAttenuation(real32 attenuation) override;
    /// Set playback position of source in seconds
    void setPlayPosition(real32 seconds) override;

    virtual void play() override;
    // virtual void play(uint32 sound_id) override;
    virtual void stop() override;
    virtual void pause() override;
    virtual void resume() override;

    // virtual bool load_buffer(SoundBuffer& rhs) override;
    // virtual bool load_file(const std::string& filename) override;

    uint32 sound_id() const override;

SoundSource();
  protected:
    /// Constructor can only be called from deriving classes
    // SoundSource();

    /// \brief A handle to the stored audio buffer data.
    ///
    /// \remarks Temporary, non-owned handle pointer.
    SoundBuffer* buffer_ = nullptr;

  private:
    bool buffer_created_ = false;

    /// Source identification; used by OpenAL
    // uint32 source_id_ = 0;
};
*/

// buffer creation

SoundBuffer*
create_audio_buffer();

SoundBuffer*
create_audio_buffer(const std::string& filename, IAudioDevice* target);

bool
fill_audio_buffer(SoundBuffer* buffer, IAudioDevice* target);

void free_buffer(SoundBuffer* buffer);

real32 audio_duration(SoundBuffer*);

enum AudioID
{
  AUDIO_BUFFER_ID = 0,
  SOUND_ID
};

uint32 audio_id(SoundBuffer*, AudioID);
nom::size_type sample_count(SoundBuffer*);
nom::size_type frame_count(SoundBuffer* buffer);
uint32 channel_count(SoundBuffer*);
uint32 sample_rate(SoundBuffer*);
nom::size_type audio_bytes(SoundBuffer*);

// audio control

void
play_audio(SoundBuffer* buffer, IAudioDevice* target);

void
stop_audio(SoundBuffer* buffer, IAudioDevice* target);

void
pause_audio(SoundBuffer* buffer, IAudioDevice* target);

void
resume_audio(SoundBuffer* buffer, IAudioDevice* target);

uint32
audio_state(SoundBuffer* buffer, IAudioDevice* target);

real32 audio_pitch(SoundBuffer* buffer, IAudioDevice* target);

Point3f
audio_position(SoundBuffer* buffer, IAudioDevice* target);

Point3f
audio_velocity(SoundBuffer* buffer, IAudioDevice* target);

real32
audio_volume(SoundBuffer* buffer, IAudioDevice* target);

real32
audio_min_volume(SoundBuffer* buffer, IAudioDevice* target);

real32
audio_max_volume(SoundBuffer* buffer, IAudioDevice* target);

real32
audio_playback_position(SoundBuffer* buffer, IAudioDevice* target);

real32
audio_playback_samples_position(SoundBuffer* buffer, IAudioDevice* target);

void
set_audio_state(SoundBuffer* buffer, IAudioDevice* target, AudioState);

void set_audio_volume(IAudioDevice* target, real32 gain);

void
set_audio_volume(SoundBuffer* buffer, IAudioDevice* target, real32 gain);

void set_min_volume(SoundBuffer* buffer, IAudioDevice* target, real32 gain);

void set_max_volume(SoundBuffer* buffer, IAudioDevice* target, real32 gain);

void set_audio_pitch(SoundBuffer* buffer, IAudioDevice* target, real32 pitch);

void
set_audio_velocity(SoundBuffer* buffer, IAudioDevice* target, const Point3f& v);

void
set_audio_position(SoundBuffer* buffer, IAudioDevice* target, const Point3f& pos);

void
set_audio_playback_position(SoundBuffer* buffer, IAudioDevice* target,
                            real32 offset_seconds);

} // namespace nom

#endif // NOMLIB_AL_SOUNDSOURCE_HEADERS defined
