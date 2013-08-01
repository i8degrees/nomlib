/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Portions Copyright (c) 2004-2013 Lazy Foo' Productions [1]
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

1. http://lazyfoo.net/SDL_tutorials/lesson30/index.php

******************************************************************************/
#include "nomlib/audio/AL/Music.hpp"

namespace nom {
  namespace OpenAL {

Music::Music ( void )
{
NOM_LOG_CLASSINFO;

  // Initialize here
}

Music::Music ( const SoundBuffer& copy )
{
  this->setBuffer ( copy );
}

Music::~Music ( void )
{
NOM_LOG_CLASSINFO;

  //this->Stop();
}

void Music::setBuffer ( const SoundBuffer& copy )
{
NOM_LOG_CLASSINFO;

AL_CHECK_ERR ( alSourcei ( source_id, AL_BUFFER, copy.get() ) );
}

void Music::Play ( void )
{
AL_CHECK_ERR ( alSourcePlay ( source_id ) );
}

void Music::Stop ( void )
{
AL_CHECK_ERR ( alSourceStop ( source_id ) );
}

void Music::Pause ( void )
{
AL_CHECK_ERR ( alSourcePause ( source_id ) );
}

void Music::togglePause ( void )
{
  if ( this->getStatus() == SoundStatus::Paused )
  {
    this->Play();
  }
  else if ( this->getStatus() == SoundStatus::Playing )
  {
    this->Pause();
  }
}

void Music::fadeOut ( float seconds )
{
  float current_volume = this->getVolume();
  float fade_step = current_volume / seconds;

  while ( this->getStatus() != SoundStatus::Paused && this->getStatus() != SoundStatus::Stopped )
  {
    if ( current_volume > 0.0 )
    {
      std::cout << "\nFading out\n";
      this->setVolume ( current_volume );
NOM_DUMP_VAR ( current_volume );
    }
    else
    {
      std::cout << "\nStopped\n";
      this->Pause();
    }

    current_volume = current_volume - fade_step;
    sleep ( 1000 ); // FIXME

  } // while getStatus loop
}


  } // namespace OpenAL
} // namespace nom
