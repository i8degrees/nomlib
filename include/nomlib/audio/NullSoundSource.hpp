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

    float getVolume ( void ) const;
    float getMinVolume ( void ) const;
    float getMaxVolume ( void ) const;
    float getPitch ( void ) const;
    bool getLooping ( void ) const;

    Point3f getPosition ( void ) const;
    Point3f getVelocity ( void ) const;

    bool getPositionRelativeToListener ( void ) const;
    float getMinDistance ( void ) const;
    float getAttenuation ( void ) const;

    int32 getBufferID ( void ) const;

    float getPlayPosition ( void ) const;

    SoundStatus getStatus ( void ) const;

    void setVolume ( float gain );
    void setMinVolume ( float gain );
    void setMaxVolume ( float gain );
    void setPitch ( float pitch );
    void setLooping ( bool loops );

    void setPosition ( float x, float y, float z );
    void setPosition ( const Point3f& position );

    void setVelocity ( float x, float y, float z );
    void setVelocity ( const Point3f& velocity );

    void setPositionRelativeToListener ( bool position );
    void setMinDistance ( float distance );
    void setAttenuation ( float attenuation );
    void setPlayPosition ( float seconds );

    void togglePause( void );
    void fadeOut( float seconds );

  protected:
    /// Constructor can only be called from deriving classes
    NullSoundSource( void );
};

} // namespace nom

#endif // include guard defined
