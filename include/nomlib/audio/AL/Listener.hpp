/******************************************************************************

    OpenAL Audio Listener

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_LISTENER_HEADERS
#define NOMLIB_AL_LISTENER_HEADERS

#include <iostream>
#include <memory>

#include "nomlib/config.hpp"
#include "nomlib/system/Logger.hpp"
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/math/Vector3-inl.hpp"
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
    const Vector3f getPosition ( void ) const;

    /// Obtain velocity
    const Vector3f getVelocity ( void ) const;

    /// Obtain direction
    const Vector3f getDirection ( void ) const;

    /// Set position
    void setPosition ( float x, float y, float z );
    void setPosition ( const Vector3f& position );

    /// Set velocity
    void setVelocity ( float x, float y, float z );
    void setVelocity ( const Vector3f& velocity );

    /// Set direction
    void setDirection ( float x, float y, float z );
    void setDirection ( const Vector3f& direction );

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
