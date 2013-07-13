/******************************************************************************

    OpenAL Music Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Music.hpp"

namespace nom {
  namespace OpenAL {

Music::Music ( void )
{
NOMLIB_LOG_INFO;

  // Initialize here
}

Music::Music ( const SoundBuffer& copy )
{
  this->setBuffer ( copy );
}

Music::~Music ( void )
{
NOMLIB_LOG_INFO;

  //this->Stop();
}

void Music::setBuffer ( const SoundBuffer& copy )
{
NOMLIB_LOG_INFO;

AL_ERR ( alSourcei ( source_id, AL_BUFFER, copy.get() ) );
}

void Music::Play ( void )
{
AL_ERR ( alSourcePlay ( source_id ) );
}

void Music::Stop ( void )
{
AL_ERR ( alSourceStop ( source_id ) );
}

void Music::Pause ( void )
{
AL_ERR ( alSourcePause ( source_id ) );
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
NOMLIB_DUMP_VAR ( current_volume );
    }
    else
    {
      std::cout << "\nStopped\n";
      this->Pause();
    }

    current_volume = current_volume - fade_step;
    SDL_Delay ( 1000 );

  } // while getStatus loop
}


  } // namespace OpenAL
} // namespace nom
