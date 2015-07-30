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
#include "nomlib/audio/NullListener.hpp"

namespace nom {

// const nom::real32 MIN_VOLUME = 0.0f;
const nom::real32 MAX_VOLUME = 100.0f;

NullListener::NullListener()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

NullListener::~NullListener()
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_AUDIO );
}

real32 NullListener::volume() const
{
  auto gain_level = nom::MAX_VOLUME;

  return gain_level;
}

Point3f NullListener::position() const
{
  Point3f result(Point3f::zero);

  return result;
}

Point3f NullListener::velocity() const
{
  Point3f result(Point3f::zero);

  return result;
}

Point3f NullListener::direction() const
{
  auto result = Point3f(0.0f, 0.0f, -1.0f);

  return result;
}

void NullListener::set_position(const Point3f& position)
{
  // Do nothing
}

void NullListener::set_velocity(const Point3f& velocity)
{
  // Do nothing
}

void NullListener::set_direction(const Point3f& at, const Point3f& up)
{
  // Do nothing
}

void NullListener::set_volume(real32 gain)
{
  // Do nothing
}

} // namespace nom
