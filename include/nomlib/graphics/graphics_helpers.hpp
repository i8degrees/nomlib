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
#ifndef NOMLIB_GRAPHICS_GRAPHICS_HELPERS_HPP
#define NOMLIB_GRAPHICS_GRAPHICS_HELPERS_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Point2.hpp"

namespace nom {

// Forward declarations
class Transformable;
class Texture;

/// \brief Calculate an object's alignment rectangle.
///
/// \returns Point2i::null on failure, such as when an invalid Transformable
/// object pointer is passed to this function.
///
/// \param obj    A valid nom::Transformable derived object pointer.
/// \param bounds The dimensions to calculate alignment for.
/// \param align  One of the nom::Alignment or nom::Anchor enumeration types.
///
/// \todo Add pos_offset argument
Point2i alignment_rect(Transformable* obj, const Size2i& bounds, uint32 align);

/// \brief Set an object's alignment.
///
/// \param obj    A valid nom::Transformable derived object pointer.
/// \param bounds The dimensions to be aligned in respect to.
/// \param align  One of the nom::Alignment or nom::Anchor enumeration types.
///
/// \todo Add pos_offset argument
void set_alignment(Transformable* obj, const Size2i& bounds, uint32 align);

/// \brief Calculate an object's alignment rectangle.
///
/// \returns Point2i::null on failure, such as when an invalid Texture object
/// pointer is passed to this function.
///
/// \param obj        A valid nom::Texture derived object pointer.
/// \param pos_offset Additional X, Y value (in pixels) to offset alignment by.
/// \param bounds     The dimensions to calculate alignment for.
/// \param align      One of the nom::Alignment or nom::Anchor enumeration
///                   types.
Point2i alignment_rect( Texture* obj, const Point2i& pos_offset,
                        const Size2i& bounds, uint32 align );

/// \brief Set an object's alignment.
///
/// \param obj        A valid nom::Texture derived object pointer.
/// \param pos_offset Additional X, Y value (in pixels) to offset alignment by.
/// \param bounds     The dimensions to be aligned in respect to.
/// \param align      One of the nom::Alignment or nom::Anchor enumeration
///                   types.
void set_alignment( Texture* obj, const Point2i& pos_offset,
                    const Size2i& bounds, uint32 align );

} // namespace nom

#endif // include guard defined
