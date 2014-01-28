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
#ifndef NOMLIB_SDL2_GRADIENT_HPP
#define NOMLIB_SDL2_GRADIENT_HPP

#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <utility>

#include "nomlib/config.hpp"
//#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/shapes/Rectangle.hpp"
#include "nomlib/system/make_unique.hpp"

namespace nom {

/// \brief Rectangle fill class with dithered, linear gradient colors
class Gradient: public Transformable

{
  public:
    typedef std::shared_ptr<Gradient> SharedPtr;
    typedef std::unique_ptr<Gradient> UniquePtr;
    typedef Gradient* RawPtr;

    /// \todo
    /// Rename me -- Direction seems fitting?
    /// Jeffrey Carpenter <i8degrees@gmail.com> @ 2013-10-05
    enum FillDirection
    {
      Top = 0,    // Top down
      Bottom,     // Bottom's up!
      Left,       // Left to right
      Right       // Right to left
    };

    /// Default construct for initializing instance variables to their
    /// respective defaults.
    Gradient ( void );

    /// Construct an object, fully initializing it.
    ///
    /// \DEPRECATED
    Gradient  (
                Color4i gradient_color[2],
                const Point2i& pos, const Size2i& size,
                const Point2i& margin,
                Gradient::FillDirection direction
              );

    /// Destructor; OK to inherit me.
    virtual ~Gradient ( void );

    /// Fully initialize this object
    void initialize (
                      Color4i gradient_color[2],
                      const Point2i& pos, const Size2i& size,
                      const Point2i& margin,
                      Gradient::FillDirection direction
                    );

    //const Size2i& size ( void ) const;
    Color4i start_color ( void ) const;
    Color4i end_color ( void ) const;
    Gradient::FillDirection fill_direction ( void ) const;
    bool dithering ( void ) const;
    const Point2i& margins ( void ) const;

    void set_start_color ( const Color4i& starting_color );
    void set_end_color ( const Color4i& ending_color );
    void reverse_colors ( void );
    void set_fill_direction ( Gradient::FillDirection direction );
    //void set_size ( const Size2i& size );
    void set_margins ( const Point2i& margin );
    void enable_dithering ( bool toggle );

    void update ( void );
    void draw ( RenderTarget target ) const;

  private:
    void strategy_top_down ( void );
    void strategy_left_right ( void );

    /// Drawables vector containing rectangle objects to be blit
    ///
    /// \todo Yes, you heard me right! We are using a rectangle function to draw
    /// what are one pixel wide / high line segments in a row single row
    /// iteration. Imaginably, performance would hardly measure a difference,
    /// but nevertheless.
    IDrawable::UniqueDrawables rectangles_;

    /// The starting & ending colors (nom::Color objects) used in the gradient.
    ///
    /// \todo Expand this into a dynamic array -- std::vector -- so we (should)
    /// be able to increase dithering capabilities higher. Perhaps increasing
    /// this threshold will also allow us to start thinking about other gradient
    /// types?
    /// Jeffrey Carpenter <i8degrees@gmail.com> @ 2013-10-03
    Color4i gradient_[2];

    /// X, Y offset coordinates
    Point2i margins_;

    //Size2i size_;

    /// Color fill axis -- X or Y increment
    enum Gradient::FillDirection fill_direction_;

    /// Toggle automatic dithering of colors
    bool enable_dithering_;
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
/// linear.set_size ( 64, 64 );
/// linear.set_margins ( 4, 4 );
/// linear.set_fill_direction ( nom::Gradient::FillDirection::Top );

/// linear.set_start_color ( nom::Color4i ( 208, 223, 255 ) );
/// linear.set_end_color ( nom::Color4i ( 50, 59, 114 ) );

/// linear.set_position ( 96, 16 );
/// linear.update();
/// linear.draw ( window );
///
/// \endcode
