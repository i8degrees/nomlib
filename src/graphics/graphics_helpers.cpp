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
#include "nomlib/graphics/graphics_helpers.hpp"

// Forward declarations
#include "nomlib/math/Transformable.hpp"

namespace nom {

Point2i alignment(Transformable* obj, const Size2i& bounds, uint32 align)
{
  // Resulting alignment calculation
  Point2i offset(Point2i::zero);

  // Object's rendered position
  Point2i pos(Point2i::zero);

  // Object's rendered width and height
  Size2i dims(Size2i::zero);

  if( obj != nullptr ) {
    pos = obj->position();
    dims = obj->size();
  } else {
    // Err: invalid pointer given
    return Point2i::null;
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

  return offset;
}

void set_alignment(Transformable* obj, const Size2i& bounds, uint32 align)
{
  // Resulting alignment calculation
  Point2i offset(Point2i::zero);

  NOM_ASSERT(obj != nullptr);
  if( obj == nullptr ) {
    return; // Err
  }

  offset = alignment(obj, bounds, align);

  if(offset != Point2i::null) {
    obj->set_position(offset);
  }
}

} // namespace nom
