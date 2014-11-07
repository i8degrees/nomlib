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
#include "nomlib/math/math_helpers.hpp"

// Forward declarations
#include "nomlib/math/Transformable.hpp"

namespace nom {

namespace priv {

std::random_device rd;
std::default_random_engine rd_generator;

} // namespace priv

void init_rand(uint32 seed_seq)
{
  priv::rd_generator = std::default_random_engine( priv::rd() );

  if(seed_seq != 0) {
    priv::rd_generator.seed(seed_seq);
  }
}

const Point2d rotate_points ( float angle, float x, float y, float pivot_x, float pivot_y )
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

void set_alignment( Transformable* obj, const Size2i& bounds,
                    uint32 align)
{
  // Resulting alignment calculation
  Point2i offset(Point2i::zero);

  // Object's rendered position
  Point2i pos(Point2i::zero);

  // Object's rendered width and height
  Size2i dims(Size2i::zero);

  NOM_ASSERT(obj != nullptr);

  if( obj != nullptr ) {
    pos = obj->position();
    dims = obj->size();
  } else {
    return;
  }

  // Reset alignment
  // if( align & Alignment::NONE ) {
  //   offset.x = pos.x;
  //   offset.y = pos.y;
  // }

  // Anchor::TopLeft, Anchor::Left, Anchor::BottomLeft
  if( align & Alignment::X_LEFT ) {
    offset.x = pos.x;
  }

  // Anchor::TopCenter, Anchor::MiddleCenter, Anchor::BottomCenter
  if( align & Alignment::X_CENTER ) {
    offset.x = pos.x + (bounds.w - dims.w) / 2;
  }

  // Anchor::TopRight, Anchor::MiddleRight, Anchor::BottomRight
  if( align & Alignment::X_RIGHT ) {
    offset.x = pos.x + (bounds.w - dims.w);
  }

  // Anchor::TopLeft, Anchor::TopCenter, Anchor::TopRight
  if( align & Alignment::Y_TOP ) {
    offset.y = pos.y;
  }

  // Anchor::MiddleLeft, Anchor::MiddleCenter, Anchor::MiddleRight
  if( align & Alignment::Y_CENTER ) {
    offset.y = pos.y + (bounds.h - dims.h) / 2;
  }

  // Anchor::BottomLeft, Anchor::BottomCenter, Anchor::BottomRight
  if( align & Alignment::Y_BOTTOM ) {
    offset.y = pos.y + (bounds.h - dims.h);
  }

  obj->set_position(offset);
}

} // namespace nom
