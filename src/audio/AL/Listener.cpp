/******************************************************************************

    OpenAL Listener

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Listener.hpp"

namespace nom {
  namespace OpenAL {

Listener::Listener ( void )
{
//NOMLIB_LOG_INFO;

  // Defaults as per OpenAL/al.h
  ALfloat position[] = { 0.0, 0.0, 0.0 };
  ALfloat velocity[] = { 0.0, 0.0, 0.0 };
  ALfloat orientation[] = { 0.0, 0.0, -1.0, 0.0, 1.0, 0.0 };
  ALfloat gain = 1.0;

  // Initialize with sane defaults to be on the safe side; note that you must
  // have the audio card initialized before-hand or these will be invalid
  // presets
  alListenerf ( AL_GAIN, gain );
  alListenerfv ( AL_POSITION, position );
  alListenerfv ( AL_VELOCITY, velocity );
  alListenerfv ( AL_ORIENTATION, orientation );
}

Listener::~Listener ( void )
{
//NOMLIB_LOG_INFO;

  // Clean up instance variables
}

float Listener::getMasterVolume ( void )
{
  ALfloat master_volume;

  alGetListenerf ( AL_GAIN, &master_volume );

  return master_volume;
}

Vector3f Listener::getPosition ( void )
{
  ALfloat position_left;
  ALfloat position_center;
  ALfloat position_right;

  alGetListener3f ( AL_POSITION, &position_left, &position_center, &position_right );

  return Vector3f ( position_left, position_center, position_right );
}

Vector3f Listener::getVelocity ( void )
{
  ALfloat velocity_left;
  ALfloat velocity_center;
  ALfloat velocity_right;

  alGetListener3f ( AL_VELOCITY, &velocity_left, &velocity_center, &velocity_right );

  return Vector3f ( velocity_left, velocity_center, velocity_right );
}

Vector3f Listener::getDirection ( void )
{
  ALfloat direction_left;
  ALfloat direction_center;
  ALfloat direction_right;

  alGetListener3f ( AL_DIRECTION, &direction_left, &direction_center, &direction_right );

  return Vector3f ( direction_left, direction_center, direction_right );
}

void Listener::setMasterVolume ( float gain )
{
  alListenerf ( AL_GAIN, gain );
}

void Listener::setPosition ( const Vector3f& position )
{
  //
}

void Listener::setVelocity ( const Vector3f& velocity )
{
  //
}

void Listener::setDirection ( const Vector3f& direction )
{
  //
}


  } // namespace OpenAL
} // namespace nom
