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

Sound::Sound() :
buffer(nullptr)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
}

Sound::Sound(const ISoundBuffer& copy)  :
  buffer(nullptr)
{
  this->setBuffer(copy);
}

Sound::~Sound()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  this->Stop();

  AL_CLEAR_ERR();
  alDeleteSources(1, &this->source_id_);
  AL_CHECK_ERR_VOID();

  if( this->buffer != nullptr ) {
    this->buffer->detach(this);
  }
}

void Sound::setBuffer(const ISoundBuffer& copy)
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // First, detach previous buffer
  if ( this->buffer != nullptr ) {
    this->Stop();
    this->buffer->detach(this);
  }

  // Assign new buffer & use it
  this->buffer = &copy;
  this->buffer->attach(this);

  // FIXME: Rethink where we should be doing this!
  AL_CLEAR_ERR();
  alGenSources(1, &this->source_id_);
  AL_CHECK_ERR_VOID();

  AL_CLEAR_ERR();
  alSourcei(this->source_id_, AL_BUFFER, this->buffer->get() );
  AL_CHECK_ERR_VOID();
}

void Sound::Play()
{
  AL_CLEAR_ERR();
  alSourcePlay(this->source_id_);
  AL_CHECK_ERR_VOID();
}

void Sound::Stop()
{
  if( this->getStatus() != nom::SoundStatus::Stopped ) {
    AL_CLEAR_ERR();
    alSourceStop(this->source_id_);
    AL_CHECK_ERR_VOID();
  }
}

void Sound::Pause()
{
  AL_CLEAR_ERR();
  alSourcePause(this->source_id_);
  AL_CHECK_ERR_VOID();
}

void Sound::reset()
{
  this->Stop();

  AL_CLEAR_ERR();
  AL_CHECK_ERR_VOID();
  alSourcei(this->source_id_, AL_BUFFER, 0);

  buffer = nullptr;
}

} // namespace nom
