/******************************************************************************

    OpenAL SoundBuffer

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SoundBuffer.hpp"

namespace nom {
  namespace OpenAL {

SoundBuffer::SoundBuffer ( void )
{
NOMLIB_LOG_INFO;

  // Initialize here
}

SoundBuffer::~SoundBuffer ( void )
{
NOMLIB_LOG_INFO;

  // Clean up instance variables
}

ALuint SoundBuffer::get ( void )
{
  return this->buffers[0];
}

uint32 SoundBuffer::getDuration ( void )
{
  return this->buffer_duration;
}

bool SoundBuffer::loadFromFile ( const std::string& filename )
{
  SoundFile fp;
  std::vector<int16> samples;

  fp.open ( filename );
  fp.read ( samples );

  this->buffer_duration = ( 1000 * fp.getSampleCount() / fp.getSampleRate() / fp.getChannelCount() );

  alGenBuffers ( 1, &this->buffers[0] );

  assert ( this->buffers[0] != AL_INVALID_VALUE );

  alBufferData  ( this->buffers[0], fp.getChannelFormat(), &samples.front(),
                  fp.getSampleCount() * sizeof ( uint16_t ), fp.getSampleRate()
                );

  assert ( alGetError() == AL_NO_ERROR );

  return true;
}


  } // namespace OpenAL
} // namespace nom
