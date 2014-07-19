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
#ifndef NOMLIB_AUDIO_LISTENER_HPP
#define NOMLIB_AUDIO_LISTENER_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Point3.hpp"

namespace nom {

/// \brief Abstract audio volume interface
class IListener
{
  public:
    virtual ~IListener( void )
    {
      NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_AUDIO, SDL_LOG_PRIORITY_VERBOSE );
    }

    virtual float getVolume ( void ) const = 0;
    virtual const Point3f getPosition ( void ) const = 0;
    virtual const Point3f getVelocity ( void ) const = 0;
    virtual const Point3f getDirection ( void ) const = 0;
    virtual void setPosition ( float x, float y, float z ) = 0;
    virtual void setPosition ( const Point3f& position ) = 0;
    virtual void setVelocity ( float x, float y, float z ) = 0;
    virtual void setVelocity ( const Point3f& velocity ) = 0;
    virtual void setDirection ( float x, float y, float z ) = 0;
    virtual void setDirection ( const Point3f& direction ) = 0;
    virtual void setVolume ( float gain ) = 0;
};

} // namespace nom

#endif // include guard defined
