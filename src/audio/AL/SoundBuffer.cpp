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
#include "nomlib/audio/AL/SoundBuffer.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"
#include "nomlib/audio/AL/Sound.hpp"
#include "nomlib/audio/AL/SoundFile.hpp"

namespace nom {

SoundBuffer::SoundBuffer ( void ) : buffer ( 0 )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
  AL_CLEAR_ERR();
  alGenBuffers(1, &this->buffer);
  AL_CHECK_ERR_VOID();
}

SoundBuffer::~SoundBuffer( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  // First, release attached sound resources from this buffer
  for( auto it = this->sounds.begin(); it != this->sounds.end(); ++it ) {
    (*it)->reset();
  }

  // Goodbye buffer!
  if( this->buffer ) {
    AL_CLEAR_ERR();
    alDeleteBuffers(1, &this->buffer);
    AL_CHECK_ERR_VOID();
  }
}

uint32 SoundBuffer::get ( void ) const
{
  return this->buffer;
}

int64 SoundBuffer::getDuration( void ) const
{
  return this->buffer_duration;
}

bool SoundBuffer::load ( const std::string& filename )
{
  SoundFile fp;

  if ( ! fp.open ( filename ) )
  {
NOM_LOG_ERR ( NOM, "Could not load audio: " + filename );
    return false;
  }

  if ( ! fp.read ( this->samples ) )
  {
NOM_LOG_ERR ( NOM, "Could not read audio samples: " + filename );
    return false;
  }

  this->buffer_duration = ( 1000 * fp.getSampleCount() / fp.getSampleRate() / fp.getChannelCount() );

  AL_CLEAR_ERR();
  // Fill the audio buffer with loaded sample data
  alBufferData(this->buffer, fp.getChannelFormat(),
               &this->samples.front(), fp.getDataByteSize(),
               fp.getSampleRate() );
  AL_CHECK_ERR_VOID();

  return true;
}

void SoundBuffer::attach ( Sound* sound ) const
{
  sounds.insert ( sound );
}

void SoundBuffer::detach ( Sound* sound ) const
{
  sounds.erase ( sound );
}

} // namespace nom
