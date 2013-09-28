/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
#include "nomlib/audio/AL/Listener.hpp"

namespace nom {
  namespace OpenAL {

Listener::Listener ( void )
{
NOM_LOG_TRACE ( NOM );

  // Defaults as per OpenAL/al.h
  ALfloat position[] = { 0.0, 0.0, 0.0 };
  ALfloat velocity[] = { 0.0, 0.0, 0.0 };
  // Listener is facing into the screen
  ALfloat direction[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
  ALfloat gain = 1.0;

  // Initialize with sane defaults to be on the safe side; note that you must
  // have the audio card initialized before-hand or these will be invalid
  // presets
AL_CHECK_ERR ( alListenerf ( AL_GAIN, gain ) );
AL_CHECK_ERR ( alListenerfv ( AL_POSITION, position ) );
AL_CHECK_ERR ( alListenerfv ( AL_VELOCITY, velocity ) );
AL_CHECK_ERR ( alListenerfv ( AL_ORIENTATION, direction ) );
}

Listener::~Listener ( void )
{
NOM_LOG_TRACE ( NOM );

  // Clean up instance variables
}

float Listener::getVolume ( void ) const
{
  ALfloat master_volume;

AL_CHECK_ERR ( alGetListenerf ( AL_GAIN, &master_volume ) );

  return master_volume * 100.f;
}

const Point3f Listener::getPosition ( void ) const
{
  Point3f position;

AL_CHECK_ERR ( alGetListener3f ( AL_POSITION, &position.x, &position.y, &position.z ) );

  return position;
}

const Point3f Listener::getVelocity ( void ) const
{
  Point3f velocity;

AL_CHECK_ERR ( alGetListener3f ( AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z ) );

  return velocity;
}

const Point3f Listener::getDirection ( void ) const
{
  Point3f direction;

AL_CHECK_ERR ( alGetListener3f ( AL_ORIENTATION, &direction.x, &direction.y, &direction.z ) );

  return direction;
}

void Listener::setPosition ( float x, float y, float z )
{
AL_CHECK_ERR ( alListener3f ( AL_POSITION, x, y, z ) );
}

void Listener::setPosition ( const Point3f& position )
{
  this->setPosition ( position.x, position.y, position.z );
}

void Listener::setVelocity ( float x, float y, float z )
{
AL_CHECK_ERR ( alListener3f ( AL_VELOCITY, x, y, z ) );
}

void Listener::setVelocity ( const Point3f& velocity )
{
  this->setVelocity ( velocity.x, velocity.y, velocity.z );
}

void Listener::setDirection ( float x, float y, float z )
{
AL_CHECK_ERR ( alListener3f ( AL_ORIENTATION, x, y, z ) );
}

void Listener::setDirection ( const Point3f& direction )
{
  this->setDirection ( direction.x, direction.y, direction.z );
}

void Listener::setVolume ( float gain )
{
AL_CHECK_ERR ( alListenerf ( AL_GAIN, gain * 0.01f ) );
}


  } // namespace OpenAL
} // namespace nom
