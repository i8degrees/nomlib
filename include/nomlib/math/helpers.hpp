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
#ifndef NOMLIB_MATH_HELPERS_HPP
#define NOMLIB_MATH_HELPERS_HPP

#include <chrono>
#include <random>
#include <cmath>

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"

namespace nom {

const double PI = 4.0 * atan ( 1.0 );

/// Returns a random number between the specified start and end numbers.
inline int32 rand ( int32 start, int32 end )
{
  auto seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::default_random_engine rand_generator ( seed );
  std::uniform_int_distribution<int32> distribution ( start, end );

  return distribution ( rand_generator );
}

/// Rotates a given X & Y coordinate point along a given pivot axis
/// (rotation point) at the given angle (in degrees), clockwise.
inline const Point2d rotate_points ( float angle, float x, float y, float pivot_x, float pivot_y )
{
  Point2d p;
  double  rotated_x = 0;
  double rotated_y = 0;
  float translated_x = 0;
  float translated_y = 0;

  float center_x = pivot_x / 2.0f;
  float center_y = pivot_y / 2.0f;

  translated_x = x - center_x;
  translated_y = y - center_y;

  rotated_x = ( translated_x * cos ( -angle * PI / 180 ) - translated_y * sin ( -angle * PI / 180 ) );
  rotated_y = ( translated_x * sin ( -angle * PI / 180 ) + translated_y * cos ( -angle * PI / 180 ) );

  rotated_x += center_x;
  rotated_y += center_y;

  p.x = rotated_x;
  p.y = rotated_y;

  return p;
}

/// ...
inline double round ( double number )
{
  return number < 0.0 ? ceil ( number - 0.5 ) : floor ( number + 0.5 );
}

} // namespace nom

#endif // include guard defined
