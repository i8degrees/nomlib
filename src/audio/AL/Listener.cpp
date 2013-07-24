/******************************************************************************

    OpenAL Listener

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/audio/AL/Listener.hpp"

namespace nom {
  namespace OpenAL {

Listener::Listener ( void )
{
NOMLIB_LOG_INFO;

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
NOMLIB_LOG_INFO;

  // Clean up instance variables
}

float Listener::getVolume ( void ) const
{
  ALfloat master_volume;

AL_CHECK_ERR ( alGetListenerf ( AL_GAIN, &master_volume ) );

  return master_volume * 100.f;
}

const Vector3f Listener::getPosition ( void ) const
{
  Vector3f position;

AL_CHECK_ERR ( alGetListener3f ( AL_POSITION, &position.x, &position.y, &position.z ) );

  return position;
}

const Vector3f Listener::getVelocity ( void ) const
{
  Vector3f velocity;

AL_CHECK_ERR ( alGetListener3f ( AL_VELOCITY, &velocity.x, &velocity.y, &velocity.z ) );

  return velocity;
}

const Vector3f Listener::getDirection ( void ) const
{
  Vector3f direction;

AL_CHECK_ERR ( alGetListener3f ( AL_ORIENTATION, &direction.x, &direction.y, &direction.z ) );

  return direction;
}

void Listener::setPosition ( float x, float y, float z )
{
AL_CHECK_ERR ( alListener3f ( AL_POSITION, x, y, z ) );
}

void Listener::setPosition ( const Vector3f& position )
{
  this->setPosition ( position.x, position.y, position.z );
}

void Listener::setVelocity ( float x, float y, float z )
{
AL_CHECK_ERR ( alListener3f ( AL_VELOCITY, x, y, z ) );
}

void Listener::setVelocity ( const Vector3f& velocity )
{
  this->setVelocity ( velocity.x, velocity.y, velocity.z );
}

void Listener::setDirection ( float x, float y, float z )
{
AL_CHECK_ERR ( alListener3f ( AL_ORIENTATION, x, y, z ) );
}

void Listener::setDirection ( const Vector3f& direction )
{
  this->setDirection ( direction.x, direction.y, direction.z );
}

void Listener::setVolume ( float gain )
{
AL_CHECK_ERR ( alListenerf ( AL_GAIN, gain * 0.01f ) );
}


  } // namespace OpenAL
} // namespace nom
