/******************************************************************************

    OpenAL SoundBuffer

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/audio/AL/SoundBuffer.hpp"

namespace nom {
  namespace OpenAL {

SoundBuffer::SoundBuffer ( void )
{
NOM_LOG_CLASSINFO;

  // Initialize here
}

SoundBuffer::~SoundBuffer ( void )
{
NOM_LOG_CLASSINFO;

//AL_CHECK_ERR ( alDeleteBuffers ( 1, &this->buffers[0] ) );
AL_CHECK_ERR ( alDeleteBuffers ( NUM_BUFFERS, this->buffers ) );
}

ALuint SoundBuffer::get ( void ) const
{
  return this->buffers[0];
}

uint32 SoundBuffer::getDuration ( void ) const
{
  return this->buffer_duration;
}

bool SoundBuffer::loadFromFile ( const std::string& filename )
{
  SoundFile fp;

  if ( ! fp.open ( filename ) )
  {
NOM_LOG_ERR ( "Could not load audio: " + filename );
  return false;
  }

  if ( ! fp.read ( this->samples ) )
  {
NOM_LOG_ERR ( "Could not read audio samples: " + filename );
  return false;
  }

  this->buffer_duration = ( 1000 * fp.getSampleCount() / fp.getSampleRate() / fp.getChannelCount() );

//AL_CHECK_ERR ( alGenBuffers ( 1, &this->buffers[0] ) );
//AL_CHECK_ERR ( alGenBuffers ( NUM_BUFFERS, &this->buffers[0] ) );
AL_CHECK_ERR ( alGenBuffers ( NUM_BUFFERS, this->buffers ) );

AL_CHECK_ERR  ( alBufferData (  this->buffers[0], fp.getChannelFormat(),
                          &this->samples.front(), fp.getDataByteSize(),
                          fp.getSampleRate() )
        );

  return true;
}


  } // namespace OpenAL
} // namespace nom
