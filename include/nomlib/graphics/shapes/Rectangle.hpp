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
#ifndef NOMLIB_SDL2_RECTANGLE_HEADERS
#define NOMLIB_SDL2_RECTANGLE_HEADERS

#include "SDL.h"

#include "nomlib/config.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Rect.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/SDL_helpers.hpp"

namespace nom {

/// \brief 3D Rectangle shape
///
/// \todo Perhaps look at implementing the multi-rectangle variant; SDL_RenderFillRects.
/// I bet, in terms of efficiency, gains are to be seen!
/// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-03
class Rectangle:
                  public IDrawable

{
  public:
    /// Default constructor; construct component objects with their sane defaults.
    Rectangle ( void );

    /// Lazy destructor with nothing to do. Please inherit me!
    virtual ~Rectangle ( void );

    /// Construct a Rectangle object using an existing Rectangle instance.
    /// (This is a typical copy constructor).
    Rectangle ( const Rectangle& rect );

    /// Construct a Rectangle object using a nom::Coords (X, Y, width & height)
    /// coordinates and a chosen color.
    Rectangle ( const Coords& coords, const Color4u& color );

    /// Construct a Rectangle object using a nom::Coords (X, Y, width & height)
    /// coordinates and a chosen color.
    Rectangle ( int32 x1, int32 y1, int32 width, int32 height, const Color4u& color );

    /// Do nothing method; we have it only because it is required by interface
    /// contract with IDrawable (which is fine).
    ///
    /// \todo Measure performance (CPU cycles, ticks/FPS, ...) difference with
    /// using a return / abort on rendering when our line object is up-to-date
    /// and determine if it is worth the implementation VS risk of inconsistency.
    /// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-03
    void update ( void );

    /// Draw a 3D four sided cube shape (rectangle) onto the the attached
    /// renderer
    void draw ( RenderTarget target ) const;

  private:
    /// X, Y signed integer rendering coordinates
    Coords coords;
    /// Rendering color (RGBA).
    Color4u color;
};


} // namespace nom

#endif // include guard defined
