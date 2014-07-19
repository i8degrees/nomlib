/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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

******************************************************************************/
#include "nomlib/audio/AL/Sound.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"

// Forward declarations
#include "nomlib/audio/ISoundBuffer.hpp"

namespace nom {

Sound::Sound ( void ) : buffer ( nullptr )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

Sound::Sound ( const ISoundBuffer& copy )  : buffer ( nullptr )
{
  this->setBuffer ( copy );
}

Sound::~Sound ( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  this->Stop();

  if ( this->buffer )
    this->buffer->detach ( this );
}

void Sound::setBuffer ( const ISoundBuffer& copy )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  // First, detach previous buffer
  if ( this->buffer )
  {
    this->Stop();
    this->buffer->detach ( this );
  }

  // Assign new buffer & use it
  this->buffer = &copy;
  this->buffer->attach ( this );

AL_CHECK_ERR ( alSourcei ( source_id, AL_BUFFER, this->buffer->get() ) );
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

void Sound::reset( void )
{
  this->Stop();

  AL_CHECK_ERR( alSourcei( source_id, AL_BUFFER, 0 ) );

  buffer = nullptr;
}

} // namespace nom
