/******************************************************************************

    OpenAL Sound Interface

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/audio/AL/Sound.hpp"

namespace nom {
  namespace OpenAL {

Sound::Sound ( void )
{
NOMLIB_LOG_INFO;
}

Sound::Sound ( const SoundBuffer& copy )
{
  this->setBuffer ( copy );
}

Sound::~Sound ( void )
{
NOMLIB_LOG_INFO;

  //this->Stop();
}

void Sound::setBuffer ( const SoundBuffer& copy )
{
NOMLIB_LOG_INFO;

AL_CHECK_ERR ( alSourcei ( source_id, AL_BUFFER, copy.get() ) );
}

void Sound::Play ( void )
{
AL_CHECK_ERR ( alSourcePlay ( source_id ) );
}

void Sound::Stop ( void )
{
AL_CHECK_ERR ( alSourceStop ( source_id ) );
}

void Sound::Pause ( void )
{
AL_CHECK_ERR ( alSourcePause ( source_id ) );
}

// TODO
/*
float Sound::getPlayPosition ( void ) const
{
  ALfloat playback_position;

  alGetSourcef ( source_id, AL_SEC_OFFSET, &playback_position );

  return playback_position;
}

void Sound::setPlayPosition ( float seconds )
{
  alSourcef ( source_id, AL_SEC_OFFSET, seconds );
}
*/

  } // namespace OpenAL
} // namespace nom
