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
#ifndef NOMLIB_SDL2_GRADIENT_HPP
#define NOMLIB_SDL2_GRADIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/math/Color.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/shapes/Rectangle.hpp"

namespace nom {

enum class FillDirection: int32
{
  Top = 0,    // Top down
  Bottom,     // Bottom's up!
  Left,       // Left to right
  Right       // Right to left
};

/// \brief Rectangle fill class with dithered, linear gradient colors
class Gradient:
                public IDrawable

{
  public:
    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Gradient ( void );

    /// Construct an object, fully initializing it.
    ///
    /// \DEPRECATED
    Gradient  (
                const Color& starting_color, const Color& ending_color,
                const Coords& bounds, int32 x_margin, int32 y_margin,
                enum FillDirection direction
              );

    /// Destructor; OK to inherit me.
    virtual ~Gradient ( void );

    /// Fully initialize this object
    void initialize (
                      const Color& starting_color, const Color& ending_color,
                      const Coords& bounds, int32 x_margin, int32 y_margin,
                      enum FillDirection direction
                    );

    const Coords getPosition ( void ) const;
    const Coords getSize ( void ) const;
    Color getStartColor ( void ) const;
    Color getEndColor ( void ) const;
    enum FillDirection getFillDirection ( void ) const;
    bool dithering ( void ) const;

    void setStartColor ( const Color& starting_color );
    void setEndColor ( const Color& ending_color );
    void reverseColors ( void );
    void setFillDirection ( enum FillDirection direction );
    void setPosition ( int32 x, int32 y );
    void setSize ( int32 width, int32 height );
    void setMargins ( int32 x, int32 y );
    void enableDithering ( bool toggle );

    void update ( void );
    void draw ( SDL_Renderer* target ) const;

  private:
    void strategyTopDown ( void );
    void strategyLeftToRight ( void );

    /// Drawables vector containing rectangle objects to be blit
    ///
    /// \todo Yes, you heard me right! We are using a rectangle function to draw
    /// what are one pixel wide / high line segments in a row single row
    /// iteration. Imaginably, performance would hardly measure a difference,
    /// but nevertheless.
    ///
    /// \todo Figure out how to get this vector of Drawables compiling as a
    /// std::unique_ptr (if this is even possible).
    /// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-03
    std::vector<std::shared_ptr<IDrawable>> rectangles;

    /// The starting & ending colors (nom::Color objects) used in the gradient.
    ///
    /// \todo Expand this into a dynamic array -- std::vector -- so we (should)
    /// be able to increase dithering capabilities higher. Perhaps increasing
    /// this threshold will also allow us to start thinking about other gradient
    /// types?
    /// Jeffrey Carpenter <jeffrey.carp@gmail.com> @ 2013-10-03
    Color gradient[2];

    /// Rendering coordinates (X, Y, width & height)
    Coords coords;

    /// x coordinate offset
    int32 x_margin;

    /// y coordinate offset
    int32 y_margin;

    /// Color fill axis -- X or Y increment
    enum FillDirection fill_direction;

    /// Toggle automatic dithering of colors
    bool enable_dithering;
};


} // namespace nom

#endif // include guard defined

/// \class nom::Gradient
/// \ingroup graphics
///
/// nom::Gradient is a beautiful class that paints as rectangle coordinates
/// would, but with dithered, linear gradient effects applied. Color range and
/// fill direction are the two likely features of your interest.
///
/// Usage example:
/// \code
/// #include <nomlib/graphics.hpp>
///
/// nom::Gradient linear;
///
/// this->linear.setSize ( 64, 64 );
/// this->linear.setMargins ( 4, 4 );
/// this->linear.setFillDirection ( nom::FillDirection::Top );

/// this->linear.setStartColor ( nom::Color ( 208, 223, 255 ) );
/// this->linear.setEndColor ( nom::Color ( 50, 59, 114 ) );

/// this->linear.setPosition ( 96, 16 );
/// this->linear.Update();
/// this->linear.Draw ( video_buffer );
///
/// \endcode
