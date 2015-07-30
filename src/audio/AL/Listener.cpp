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
#include "nomlib/audio/AL/Listener.hpp"

// Private headers
#include "nomlib/audio/AL/OpenAL.hpp"

namespace nom {

const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;

Listener::Listener()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);

  // Defaults as per OpenAL/al.h
  Point3f position = {0.0f, 0.0f, 0.0f};
  Point3f velocity = {0.0f, 0.0f, 0.0f};

  // Listener is facing into the screen
  Point3f direction_at(0.0f, 0.0f, -1.0f);
  Point3f direction_up(0.0f, 1.0f, 0.0f);

  auto gain = nom::MAX_VOLUME;

  this->set_volume(gain);
  this->set_position(position);
  this->set_velocity(velocity);
  this->set_direction(direction_at, direction_up);
}

Listener::~Listener()
{
  NOM_LOG_TRACE(NOM_LOG_CATEGORY_TRACE_AUDIO);
}

real32 Listener::volume() const
{
  auto gain_level = nom::MAX_VOLUME;

  AL_CLEAR_ERR();
  alGetListenerf(AL_GAIN, &gain_level);
  AL_CHECK_ERR_VOID();

  // de-normalized gain level; 0..1 -> 0..100
  return(gain_level * 100.0f);
}

// static
real32 Listener::min_volume()
{
  auto gain_level = nom::MIN_VOLUME;

  return(gain_level);
}

// static
real32 Listener::max_volume()
{
  auto gain_level = nom::MAX_VOLUME;

  return(gain_level);
}

Point3f Listener::position() const
{
  Point3f position;

  AL_CLEAR_ERR();
  alGetListener3f(AL_POSITION, &position.x, &position.y, &position.z);
  AL_CHECK_ERR_VOID();

  return position;
}

Point3f Listener::velocity() const
{
  Point3f v(Point3f::zero);

  AL_CLEAR_ERR();
  alGetListener3f(AL_VELOCITY, &v.x, &v.y, &v.z);
  AL_CHECK_ERR_VOID();

  return v;
}

Point3f Listener::direction() const
{
  Point3f direction;
  real32 d[] = {};

  AL_CLEAR_ERR();
  alGetListenerfv(AL_ORIENTATION, d);
  AL_CHECK_ERR_VOID();

  direction.x = d[0];
  direction.y = d[1];
  direction.z = d[2];

  return direction;
}

void Listener::set_position(const Point3f& position)
{
  AL_CLEAR_ERR();
  alListener3f(AL_POSITION, position.x, position.y, position.z);
  AL_CHECK_ERR_VOID();
}

void Listener::set_velocity(const Point3f& velocity)
{
  AL_CLEAR_ERR();
  alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
  AL_CHECK_ERR_VOID();
}

void Listener::set_direction(const Point3f& at, const Point3f& up)
{
  real32 direction[] = {at.x, at.y, at.z, up.x, up.y, up.z};

  AL_CLEAR_ERR();
  alListenerfv(AL_ORIENTATION, direction);
  AL_CHECK_ERR_VOID();
}

void Listener::set_volume(real32 gain)
{
  if( gain >= nom::MIN_VOLUME && gain <= nom::MAX_VOLUME ) {
    AL_CLEAR_ERR();

    // normalize the gain
    alListenerf(AL_GAIN, gain * 0.01f);
    AL_CHECK_ERR_VOID();
  }
}

} // namespace nom
