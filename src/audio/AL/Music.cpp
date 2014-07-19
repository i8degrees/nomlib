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
#include "nomlib/audio/AL/Music.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"

// Forward declarations
#include "nomlib/audio/ISoundBuffer.hpp"

namespace nom {

Music::Music ( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  // Initialize here
}

Music::Music ( const ISoundBuffer& copy )
{
  this->setBuffer ( copy );
}

Music::~Music ( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

  this->Stop();
}

void Music::setBuffer ( const ISoundBuffer& copy )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );

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

} // namespace nom
