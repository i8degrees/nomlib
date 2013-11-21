/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#ifndef NOMLIB_AL_LISTENER_HEADERS
#define NOMLIB_AL_LISTENER_HEADERS

#include <iostream>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/math/Point3.hpp"
#include "nomlib/audio/AL/AudioDevice.hpp"

namespace nom {
  namespace OpenAL {

class Listener
{
  public:
    Listener ( void );
    ~Listener ( void );

    /// Obtain master gain (volume)
    ///
    /// Volume is between 0 (muted) and 100 (max volume)
    ///
    /// Default: 100
    float getVolume ( void ) const;

    /// Obtain position
    const Point3f getPosition ( void ) const;

    /// Obtain velocity
    const Point3f getVelocity ( void ) const;

    /// Obtain direction
    const Point3f getDirection ( void ) const;

    /// Set position
    void setPosition ( float x, float y, float z );
    void setPosition ( const Point3f& position );

    /// Set velocity
    void setVelocity ( float x, float y, float z );
    void setVelocity ( const Point3f& velocity );

    /// Set direction
    void setDirection ( float x, float y, float z );
    void setDirection ( const Point3f& direction );

    /// Set master gain (volume)
    ///
    /// Volume is between 0 (muted) and 100 (max volume)
    ///
    /// Default: 100
    void setVolume ( float gain );
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_LISTENER_HEADERS defined
