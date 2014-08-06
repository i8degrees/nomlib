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

#include "nomlib/config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Color4.hpp"
#include "nomlib/graphics/IDrawable.hpp"
#include "nomlib/graphics/shapes/Rectangle.hpp"
#include "nomlib/core/helpers.hpp"

namespace nom {

/// \brief Rectangle fill class with dithered, linear gradient colors.
class Gradient: public Transformable

{
  public:
    typedef Gradient self_type;
    typedef Transformable derived_class;

    typedef self_type* raw_ptr;
    typedef std::unique_ptr<self_type> unique_ptr;
    typedef std::shared_ptr<self_type> shared_ptr;

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

    /// \brief Default constructor; initialize the object to an invalid state.
    ///
    /// \remarks At the very minimum, you must set the position & size to get a
    /// rendered object on screen. The default gradient colors is an opaque
    /// Color4i::Blue set as both the starting and ending color fill to signify
    /// an invalid object state.
    Gradient( void );

    /// \brief Construct a complete object state.
    ///
    /// \param colors The list of Color4i objects to be used in the gradient.
    Gradient  (
                const Color4iColors& colors,
                const Point2i& pos,
                const Size2i& size,
                const Point2i& margin,
                Gradient::FillDirection direction
              );

    /// \brief Destructor.
    virtual ~Gradient( void );

    /// \brief Copy constructor.
    Gradient( const self_type& copy );

    /// \brief Implements the required IDrawable::clone method.
    IDrawable::raw_ptr clone( void ) const;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Query the validity of the object
    ///
    /// \remarks A valid object must have both the positioning & size bounding
    /// coordinates be set to a non-null object value. See nom::Point2i::null
    /// and nom::Size2i::null for their respective values.
    ///
    /// \note A default constructed object is in an invalid state.
    bool valid( void ) const;

    /// \brief Get the first (starting) color used in the gradient fill.
    const Color4i& start_color( void ) const;

    /// \brief Get the last (ending) color used in the gradient fill.
    const Color4i& end_color( void ) const;

    Gradient::FillDirection fill_direction ( void ) const;
    bool dithering( void ) const;
    const Point2i& margins ( void ) const;

    /// \brief Get the gradient colors container.
    const Color4iColors& colors( void ) const;

    /// \brief Re-implements the Transformable::set_position method.
    ///
    /// \remarks Thie method call is necessary in order to ensure the internal
    /// object state remains consistent.
    void set_position( const Point2i& coords );

    /// \brief Re-implements the Transformable::set_size method.
    ///
    /// \remarks Thie method call is necessary in order to ensure the internal
    /// object state remains consistent.
    void set_size( const Size2i& size );

    /// \brief Set the gradient colors used in the rendering of the gradient
    /// fill.
    void set_colors( const Color4iColors& colors );

    /// \brief Swap the first and last color used in the gradient fill.
    void reverse_colors( void );

    void set_fill_direction ( Gradient::FillDirection direction );

    //void set_size ( const Size2i& size );
    void set_margins ( const Point2i& margin );
    void set_dithering( bool state );

    /// \brief Implements the IDrawable::draw method.
    void draw( RenderTarget& target ) const;

  private:
    /// \brief Implements the IDrawable::update method.
    void update( void );

    void strategy_top_down( void );
    void strategy_left_right( void );

    /// Drawables vector containing rectangle objects to be blit
    ///
    /// \todo Yes, you heard me right! We are using a rectangle function to draw
    /// what are one pixel wide / high line segments in a row single row
    /// iteration. Imaginably, performance would hardly measure a difference,
    /// but nevertheless.
    std::vector<Rectangle::shared_ptr> rectangles_;

    /// \brief The starting & ending colors used in the gradient fill.
    ///
    /// \todo Finish implementing vectorized color container support; we only
    /// use the first and last color of whatever is set in the container at the
    /// moment.
    Color4iColors gradient_;

    /// \brief Additional offset coordinates, in pixels.
    Point2i margins_;

    //Size2i size_;

    /// Color fill axis -- X or Y increment.
    enum Gradient::FillDirection fill_direction_;

    /// \brief Dithering (of gradient colors) state.
    bool dithering_;
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
/// linear.draw ( window );
///
/// \endcode
