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

namespace nom {

// Forward declarations
class ISoundBuffer;

/// Sound source is one of the three states: stopped, paused or playing
enum SoundStatus
{
  Stopped = 0,
  Paused = 1,
  Playing = 2
};

class ISoundSource
{
  public:
    virtual ~ISoundSource( void )
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, nom::LogPriority::NOM_LOG_PRIORITY_VERBOSE );
    }

    virtual float getVolume ( void ) const = 0;
    virtual float getMinVolume ( void ) const = 0;
    virtual float getMaxVolume ( void ) const = 0;
    virtual float getPitch ( void ) const = 0;
    virtual bool getLooping ( void ) const = 0;
    virtual Point3f getPosition ( void ) const = 0;
    virtual Point3f getVelocity ( void ) const = 0;
    virtual bool getPositionRelativeToListener ( void ) const = 0;
    virtual float getMinDistance ( void ) const = 0;
    virtual float getAttenuation ( void ) const = 0;
    virtual int32 getBufferID ( void ) const = 0;
    virtual float getPlayPosition ( void ) const = 0;
    virtual SoundStatus getStatus ( void ) const = 0;
    virtual void setVolume ( float gain ) = 0;
    virtual void setMinVolume ( float gain ) = 0;
    virtual void setMaxVolume ( float gain ) = 0;
    virtual void setPitch ( float pitch ) = 0;
    virtual void setLooping ( bool loops ) = 0;
    virtual void setPosition ( float x, float y, float z ) = 0;
    virtual void setPosition ( const Point3f& position ) = 0;
    virtual void setVelocity ( float x, float y, float z ) = 0;
    virtual void setVelocity ( const Point3f& velocity ) = 0;
    virtual void setPositionRelativeToListener ( bool position ) = 0;
    virtual void setMinDistance ( float distance ) = 0;
    virtual void setAttenuation ( float attenuation ) = 0;
    virtual void setPlayPosition ( float seconds ) = 0;

    virtual void setBuffer( const ISoundBuffer& copy ) = 0;
    virtual void Play( void ) = 0;
    virtual void Stop( void ) = 0;
    virtual void Pause( void ) = 0;
    virtual void togglePause( void ) = 0;
    virtual void fadeOut( float seconds ) = 0;
};

} // namespace nom

#endif // include guard defined
