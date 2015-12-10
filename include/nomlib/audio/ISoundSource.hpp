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
#ifndef NOMLIB_AUDIO_ISOUND_SOURCE_HPP
#define NOMLIB_AUDIO_ISOUND_SOURCE_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"

namespace nom {

// Forward declarations
// class ISoundBuffer;
// struct SoundBuffer;

class ISoundSource
{
  public:
    virtual ~ISoundSource( void )
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );
    }

    virtual real32 volume() const = 0;
    virtual real32 min_volume() const = 0;
    virtual real32 max_volume() const = 0;
    virtual real32 pitch() const = 0;
    virtual bool loop() const = 0;
    virtual Point3f position() const = 0;
    virtual Point3f velocity() const = 0;
    virtual bool getPositionRelativeToListener() const = 0;
    virtual real32 getMinDistance() const = 0;
    virtual real32 getAttenuation() const = 0;
    virtual uint32 buffer_id() const = 0;
    virtual real32 getPlayPosition() const = 0;
    virtual uint32 status() const = 0;
    virtual void set_volume(real32 gain) = 0;
    virtual void set_min_volume(real32 gain) = 0;
    virtual void set_max_volume(real32 gain) = 0;
    virtual void setPitch(real32 pitch) = 0;
    virtual void setLooping(bool loops) = 0;
    virtual void setPosition(real32 x, real32 y, real32 z) = 0;
    virtual void setPosition(const Point3f& position) = 0;
    virtual void set_velocity(real32 x, real32 y, real32 z) = 0;
    virtual void set_velocity(const Point3f& velocity) = 0;
    virtual void setPositionRelativeToListener(bool position) = 0;
    virtual void setMinDistance(real32 distance) = 0;
    virtual void setAttenuation(real32 attenuation) = 0;
    virtual void setPlayPosition(real32 seconds) = 0;

    virtual void play() = 0;
    // virtual void play(uint32 sound_id) = 0;
    virtual void stop() = 0;
    virtual void pause() = 0;
    virtual void resume() = 0;

    // virtual bool load_buffer(SoundBuffer& rhs) = 0;
    // virtual bool load_file(const std::string& filename) = 0;

    virtual uint32 sound_id() const = 0;
};

} // namespace nom

#endif // include guard defined
