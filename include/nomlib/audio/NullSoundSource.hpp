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
#ifndef NOMLIB_AUDIO_NULL_SOUND_SOURCE_HPP
#define NOMLIB_AUDIO_NULL_SOUND_SOURCE_HPP

#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/ISoundSource.hpp"

namespace nom {

class NullSoundSource: public ISoundSource
{
  public:
    virtual ~NullSoundSource( void );

    real64 duration() const override;
    real32 volume() const;
    real32 min_volume() const;
    real32 max_volume() const;
    real32 getPitch() const;
    bool getLooping() const;

    Point3f position() const;
    Point3f velocity() const;

    bool getPositionRelativeToListener() const;
    real32 getMinDistance() const;
    real32 getAttenuation() const;

    int32 getBufferID() const;

    real32 getPlayPosition() const;

    SoundStatus getStatus() const;

    void set_volume(real32 gain);
    void set_min_volume(real32 gain);
    void set_max_volume(real32 gain);
    void setPitch ( real32 pitch );
    void setLooping ( bool loops );

    void setPosition ( real32 x, real32 y, real32 z );
    void setPosition ( const Point3f& position );

    void set_velocity(real32 x, real32 y, real32 z);
    void set_velocity(const Point3f& velocity);

    void setPositionRelativeToListener ( bool position );
    void setMinDistance ( real32 distance );
    void setAttenuation ( real32 attenuation );
    void setPlayPosition ( real32 seconds );

    virtual void play() override;
    virtual void stop() override;
    virtual void pause() override;
    virtual void resume() override;

    virtual bool load_buffer(SoundBuffer& rhs) override;
    virtual bool load_file(const std::string& filename) override;

  protected:
    /// Constructor can only be called from deriving classes
    NullSoundSource( void );
};

} // namespace nom

#endif // include guard defined
