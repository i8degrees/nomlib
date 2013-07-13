/******************************************************************************

    OpenAL Audio Listener

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_LISTENER_HEADERS
#define NOMLIB_AL_LISTENER_HEADERS

#include <iostream>
#include <memory>

#include "math.hpp"
#include "OpenAL.hpp"

namespace nom {
  namespace OpenAL {

class Listener
{
  public:
    Listener ( void );
    ~Listener ( void );

    float getMasterVolume ( void );
    Vector3f getPosition ( void );
    Vector3f getVelocity ( void );
    Vector3f getDirection ( void );
    void setMasterVolume ( float gain );
    void setPosition ( const Vector3f& position );
    void setVelocity ( const Vector3f& velocity );
    void setDirection ( const Vector3f& direction );

  private:
    // ...
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_LISTENER_HEADERS defined
