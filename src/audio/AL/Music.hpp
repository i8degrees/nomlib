/******************************************************************************

    OpenAL Music Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_AL_MUSIC_HEADERS
#define NOMLIB_AL_MUSIC_HEADERS

#include <iostream>
#include <memory>

#include "config.hpp"
#include "sys/Logger.hpp"
#include "OpenAL.hpp"
#include "sys/Sleep.hpp"
#include "SoundBuffer.hpp"
#include "SoundSource.hpp"

namespace nom {
  namespace OpenAL {

class Music: public SoundSource // "is-a relationship"
{
  public:
    Music ( void );
    Music ( const SoundBuffer& copy );
    ~Music ( void );

    void setBuffer ( const SoundBuffer& copy );

    void Play ( void );
    void Stop ( void );
    void Pause ( void );
    void togglePause ( void );

    void fadeOut ( float seconds );

  private:
    //SoundFile *fp;
};


  } // namespace OpenAL
} // namespace nom

#endif // NOMLIB_AL_MUSIC_HEADERS defined
