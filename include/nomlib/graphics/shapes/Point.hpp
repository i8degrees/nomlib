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
#ifndef NOMLIB_GRAPHICS_SHAPES_POINT_HPP
#define NOMLIB_GRAPHICS_SHAPES_POINT_HPP

#include <iostream>

#include "SDL.h" // SDL2

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Point2.hpp"
#include "nomlib/graphics/shapes/Shape.hpp"

namespace nom {

/// \brief 2D Point (pixel representation)
///
/// \todo Use SDL2's new multi-point API; see SDL_RenderDrawPoints.
class Point: public Shape
{
  public:
    /// \brief Default constructor.
    Point ( void );

    /// \brief Destructor; should be fine to inherit from.
    virtual ~Point ( void );

    /// \brief Construct a Point from parameters.
    ///
    /// \param pos nom::Point2i object containing the coordinates.
    /// \param fill nom::Color4i color to render the point as.
    Point ( const Point2i& pos, const Color4i& fill );

    void update ( void );

    /// \brief Render the point coordinate.
    ///
    /// \param target nom::RenderWindow object to render to.
    void draw ( RenderTarget& target ) const;
};

} // namespace nom

#endif // include guard defined
