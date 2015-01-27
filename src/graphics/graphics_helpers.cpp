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
#include "nomlib/graphics/Texture.hpp"

namespace nom {

Point2i alignment_rect(Transformable* obj, const Point2i& pos_offset, const Size2i& bounds, uint32 align)
{
  // Resulting alignment calculation
  Point2i align_offset(Point2i::zero);

  // Object's rendered width and height
  Size2i dims(Size2i::zero);

  if( obj != nullptr ) {
    dims = obj->size();
  } else {
    // Err: invalid pointer given
    return Point2i::null;
  }

  // Anchor::TopLeft, Anchor::Left, Anchor::BottomLeft
  if( align & Alignment::X_LEFT ) {
    align_offset.x = pos_offset.x;
  }

  // Anchor::TopCenter, Anchor::MiddleCenter, Anchor::BottomCenter
  if( align & Alignment::X_CENTER ) {
    align_offset.x = pos_offset.x + (bounds.w - dims.w) / 2;
  }

  // Anchor::TopRight, Anchor::MiddleRight, Anchor::BottomRight
  if( align & Alignment::X_RIGHT ) {
    align_offset.x = pos_offset.x + (bounds.w - dims.w);
  }

  // Anchor::TopLeft, Anchor::TopCenter, Anchor::TopRight
  if( align & Alignment::Y_TOP ) {
    align_offset.y = pos_offset.y;
  }

  // Anchor::MiddleLeft, Anchor::MiddleCenter, Anchor::MiddleRight
  if( align & Alignment::Y_CENTER ) {
    align_offset.y = pos_offset.y + (bounds.h - dims.h) / 2;
  }

  // Anchor::BottomLeft, Anchor::BottomCenter, Anchor::BottomRight
  if( align & Alignment::Y_BOTTOM ) {
    align_offset.y = pos_offset.y + (bounds.h - dims.h);
  }

  return align_offset;
}

void set_alignment(Transformable* obj, const Point2i& pos_offset, const Size2i& bounds, uint32 align)
{
  // Resulting alignment calculation
  Point2i offset(Point2i::zero);

  NOM_ASSERT(obj != nullptr);
  if( obj == nullptr ) {
    return; // Err
  }

  offset = nom::alignment_rect(obj, pos_offset, bounds, align);

  if(offset != Point2i::null) {
    obj->set_position(offset);
  }
}

Point2i alignment_rect( Texture* obj, const Point2i& pos_offset,
                        const Size2i& bounds, uint32 align )
{
  // Resulting alignment calculation
  Point2i align_offset(Point2i::zero);

  // Object's rendered width and height
  Size2i dims(Size2i::zero);

  if( obj != nullptr ) {
    dims = obj->size();
  } else {
    // Err: invalid pointer given
    return Point2i::null;
  }

  // Anchor::TopLeft, Anchor::Left, Anchor::BottomLeft
  if( align & Alignment::X_LEFT ) {
    align_offset.x = pos_offset.x;
  }

  // Anchor::TopCenter, Anchor::MiddleCenter, Anchor::BottomCenter
  if( align & Alignment::X_CENTER ) {
    align_offset.x = pos_offset.x + (bounds.w - dims.w) / 2;
  }

  // Anchor::TopRight, Anchor::MiddleRight, Anchor::BottomRight
  if( align & Alignment::X_RIGHT ) {
    align_offset.x = pos_offset.x + (bounds.w - dims.w);
  }

  // Anchor::TopLeft, Anchor::TopCenter, Anchor::TopRight
  if( align & Alignment::Y_TOP ) {
    align_offset.y = pos_offset.y;
  }

  // Anchor::MiddleLeft, Anchor::MiddleCenter, Anchor::MiddleRight
  if( align & Alignment::Y_CENTER ) {
    align_offset.y = pos_offset.y + (bounds.h - dims.h) / 2;
  }

  // Anchor::BottomLeft, Anchor::BottomCenter, Anchor::BottomRight
  if( align & Alignment::Y_BOTTOM ) {
    align_offset.y = pos_offset.y + (bounds.h - dims.h);
  }

  return align_offset;
}

void set_alignment( Texture* obj, const Point2i& pos_offset,
                    const Size2i& bounds, uint32 align )
{
  // Resulting alignment calculation
  Point2i align_offset(Point2i::zero);

  NOM_ASSERT(obj != nullptr);
  if( obj == nullptr ) {
    return; // Err
  }

  align_offset = nom::alignment_rect(obj, pos_offset, bounds, align);

  if(align_offset != Point2i::null) {
    obj->set_position(align_offset);
  }
}

} // namespace nom
