/******************************************************************************

    OpenAL SoundSource

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_SOUNDSOURCE_HEADERS
#define NOMLIB_AL_SOUNDSOURCE_HEADERS

#include <memory>
#include <algorithm>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "OpenAL.hpp"
#include "math/Vector3-inl.hpp"

namespace nom {

/// Sound source is one of the three states: stopped, paused or playing
enum SoundStatus
{
  Stopped = 0,
  Paused = 1,
  Playing = 2
};

  namespace OpenAL {

// This is an inheritance-only class
class SoundSource
{
  public:
    virtual ~SoundSource ( void );

    /// Get source volume level
    ///
    /// Volume is between 0 (muted) and 100 (max volume)
    ///
    /// Default: 100
    float getVolume ( void ) const;
    float getMinVolume ( void ) const;
    float getMaxVolume ( void ) const;
    float getPitch ( void ) const;
    bool getLooping ( void ) const;

    Vector3f getPosition ( void ) const;
    Vector3f getVelocity ( void ) const;

    bool getPositionRelativeToListener ( void ) const;
    float getMinDistance ( void ) const;
    float getAttenuation ( void ) const;

    /// Obtain buffer identifier of source
    int32 getBufferID ( void ) const;
    /// Obtain the current playback position of source in seconds
    float getPlayPosition ( void ) const;
    /// Obtain current state of sound
    SoundStatus getStatus ( void ) const;

    /// Set source volume level
    ///
    /// Volume is between 0 (muted) and 100 (max volume)
    ///
    /// Default: 100
    void setVolume ( float gain );
    void setMinVolume ( float gain );
    void setMaxVolume ( float gain );
    void setPitch ( float pitch );
    void setLooping ( bool loops );

    void setPosition ( float x, float y, float z );
    void setPosition ( const Vector3f& position );

    void setVelocity ( float x, float y, float z );
    void setVelocity ( const Vector3f& velocity );

    void setPositionRelativeToListener ( bool position );
    void setMinDistance ( float distance );
    void setAttenuation ( float attenuation );
    /// Set playback position of source in seconds
    void setPlayPosition ( float seconds );

  protected:
    /// Constructor can only be called from deriving classes
    SoundSource ( void );
    /// Source identification; used by OpenAL
    uint32 source_id;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_SOUNDSOURCE_HEADERS defined
