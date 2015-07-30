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

/// \brief Base class for audio inputs
class SoundSource: public ISoundSource
{
  public:
    virtual ~SoundSource();

    /// \brief Get the gain level of this audio source.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 volume() const;

    /// \brief Get the minimum gain level of this audio source.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 min_volume() const;

    /// \brief Get the maximum gain level of this audio source.
    ///
    /// \returns A number between 0..100 (min/max).
    real32 max_volume() const;

    real32 getPitch() const;
    bool getLooping() const;

    Point3f position() const;
    Point3f velocity() const;

    bool getPositionRelativeToListener() const;
    real32 getMinDistance() const;
    real32 getAttenuation() const;

    /// Obtain buffer identifier of source
    int32 getBufferID() const;
    /// Obtain the current playback position of source in seconds
    real32 getPlayPosition() const;
    /// Obtain current state of sound
    SoundStatus getStatus() const;

    /// \brief Set a new gain level for this audio source.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max).
    ///
    /// \remarks This controls the gain level of this audio source.
    void set_volume(real32 gain);

    /// \brief Set a new minimum gain level for this audio source.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max).
    ///
    /// \remarks This controls the minimum gain level of this audio source.
    void set_min_volume(real32 gain);

    /// \brief Set a new maximum gain level for this audio source.
    ///
    /// \param gain A 32-bit floating-point number between 0..100 (min/max).
    ///
    /// \remarks This controls the maximum gain level of this audio source.
    void set_max_volume(real32 gain);

    void setPitch(real32 pitch);
    void setLooping(bool loops);

    void setPosition ( real32 x, real32 y, real32 z );
    void setPosition ( const Point3f& position );

    void set_velocity(real32 x, real32 y, real32 z);
    void set_velocity(const Point3f& velocity);

    void setPositionRelativeToListener ( bool position );
    void setMinDistance ( real32 distance );
    void setAttenuation ( real32 attenuation );
    /// Set playback position of source in seconds
    void setPlayPosition ( real32 seconds );

    virtual void togglePause();

  protected:
    /// Constructor can only be called from deriving classes
    SoundSource();

    /// Source identification; used by OpenAL
    uint32 source_id_ = 0;
};

} // namespace nom

#endif // NOMLIB_AL_SOUNDSOURCE_HEADERS defined
