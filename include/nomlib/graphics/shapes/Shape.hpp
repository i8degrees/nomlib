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
#ifndef NOMLIB_GRAPHICS_SHAPES_SHAPE_HPP
#define NOMLIB_GRAPHICS_SHAPES_SHAPE_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Transformable.hpp"
#include "nomlib/math/Color4.hpp"

namespace nom {

class Shape: public Transformable
{
  public:
    typedef Shape self_type;

    /// Destructor.
    virtual ~Shape ( void );

    /// \brief Copy constructor
    Shape ( const Shape& copy );

    /// \brief Copy assignment operator
    //Shape& operator = ( const Shape& other );

    virtual std::unique_ptr<self_type> clone() const = 0;

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Obtain the outline color used in rendering a shape.
    const Color4i& outline_color ( void ) const;

    /// \brief Obtain the inside fill color used in rendering a shape.
    const Color4i& fill_color ( void ) const;

    /// \brief Set the outline color of the shape to be rendered.
    void set_outline_color ( const Color4i& color );

    /// \brief Set the inside (fill) color of the shape to be rendered.
    void set_fill_color ( const Color4i& color );

  protected:
    /// Default constructor; initialize the outline rendering color to
    /// nom::Color4i::White & the fill rendering color to nom::Color4i::Black.
    Shape ( void );

  private:
    /// RGBA color used in rendering the outline of a shape
    Color4i outline_color_;

    /// RGBA color used in rendering the inside (fill) of a shape
    Color4i fill_color_;
};

} // namespace nom

#endif // include guard defined
