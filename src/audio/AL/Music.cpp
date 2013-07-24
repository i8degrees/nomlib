/******************************************************************************

    OpenAL Music Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

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
