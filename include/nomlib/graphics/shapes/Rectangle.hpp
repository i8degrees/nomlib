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
#ifndef NOMLIB_GRAPHICS_SHAPES_RECTANGLE_HPP
#define NOMLIB_GRAPHICS_SHAPES_RECTANGLE_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/shapes/Shape.hpp"
#include "nomlib/system/SDL_helpers.hpp"

namespace nom {

// Forward declarations
class Texture;

/// \brief 2D Rectangle shape
///
/// \todo Use SDL2's new multi-rectangle API; see SDL_RenderFillRects.
///
/// Jeffrey Carpenter <i8degrees@gmail.com> @ 2013-10-03
class Rectangle: public Shape
{
  public:
    typedef Rectangle self_type;
    typedef Shape derived_type;

    /// \brief Default constructor.
    Rectangle();

    /// \brief Destructor; should be fine to inherit from.
    virtual ~Rectangle();

    /// \brief Construct a Rectangle object from parameters.
    ///
    /// \param rect nom::IntRect object containing the coordinates.
    /// \param color nom::Color4i color to fill with.
    Rectangle(const IntRect& rect, const Color4i& fill);

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type() const;

    /// \note Implements the required Shape::clone method.
    virtual Shape* clone() const override;

    /// \brief Get a texture representation of the rectangle.
    ///
    /// \returns A pointer to a new nom::Texture instance containing the
    /// rendering of the rectangle. The returned pointer is owned by the
    /// caller.
    ///
    /// \remarks This is an expensive function call.
    Texture* texture() const;

    /// \note Implements the required IDrawable::update method.
    void update() override;

    /// \brief Render the rectangle shape.
    ///
    /// \param target nom::RenderWindow object to render to.
    void draw(RenderTarget& target) const;
};

} // namespace nom

#endif // include guard defined
