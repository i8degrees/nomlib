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
