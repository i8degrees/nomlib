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
#ifndef NOMLIB_GRAPHICS_SHAPES_LINE_HPP
#define NOMLIB_GRAPHICS_SHAPES_LINE_HPP

#include <iostream>

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/shapes/Shape.hpp"

namespace nom {

/// \brief 2D line segment
///
/// \todo Use SDL2's new multi-line API; see SDL_RenderDrawLines.
///
/// Jeffrey Carpenter <i8degrees@gmail.com> @ 2013-10-03
class Line: public Shape
{
  public:
    typedef Line self_type;
    typedef Shape derived_class;

    /// \brief Default constructor.
    Line ( void );

    /// \brief Destructor; should be fine to inherit from.
    virtual ~Line ( void );

    /// \brief Construct a Line object from parameters
    ///
    /// \param bounds nom::IntRect object containing the starting & ending
    /// coordinates.
    /// \param color nom::Color4i color to render.
    Line ( const IntRect& bounds, const Color4i& outline );

    /// \brief Implements the required Shape::clone method.
    virtual std::unique_ptr<derived_class> clone() const override;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// Do nothing method; we have it only because it is required by interface
    /// contract with IDrawable (which is fine).
    ///
    /// \todo Measure performance (CPU cycles, ticks/FPS, ...) difference with
    /// using a return / abort on rendering when our line object is up-to-date
    /// and determine if it is worth the implementation VS risk of inconsistency.
    /// Jeffrey Carpenter <i8degrees@gmail.com> @ 2013-10-03
    void update ( void );

    /// \brief Render the line segments.
    ///
    /// \param target nom::RenderWindow object to render to.
    void draw ( RenderTarget& target ) const;
};

} // namespace nom

#endif // include guard defined
