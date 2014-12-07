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
#ifndef NOMLIB_GRAPHICS_TRANSFORMABLE_HPP
#define NOMLIB_GRAPHICS_TRANSFORMABLE_HPP

#include "nomlib/config.hpp"
#include "nomlib/math/Size2.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

class Transformable: public virtual IDrawable
{
  public:
    Transformable( void );
    virtual ~Transformable( void );

    Transformable( const Point2i& pos, const Size2i& size );

    /// \brief Construct an object using a complete set of coordinates
    /// (bounds).
    Transformable( const IntRect& bounds );

    /// \brief Re-implements the IObject::type method.
    ///
    /// \remarks This uniquely identifies the object's type.
    ObjectTypeInfo type( void ) const;

    /// \brief Getter for stored positioning coordinates.
    const Point2i& position( void ) const;

    /// \brief Getter for stored size dimensions.
    const Size2i& size( void ) const;

    /// \brief Set the positioning -- X & Y rendering coordinates -- of the
    /// object.
    ///
    /// \remarks You will almost always want to call the base class --
    /// Transformable::set_position and Transformable::set_size -- when
    /// overriding any of the class methods. See nom::Gradient and nom::Text
    /// for examples of why you might need to override the methods here.
    virtual void set_position( const Point2i& coords );

    /// \brief Set the size dimensions-- width & height rendering coordinates
    /// of the object.
    ///
    /// \remarks You will almost always want to call the base class --
    /// Transformable::set_position and Transformable::set_size -- when
    /// overriding any of the class methods. See nom::Gradient and nom::Text
    /// for examples of why you might need to override the methods here.
    virtual void set_size( const Size2i& size );

    /// \brief Set the complete bounds of the object -- position (X, Y) and
    /// size (width and height) dimensions.
    virtual void set_bounds( const IntRect& bounds );

    /// \todo Remove this method; it causes trouble in paradise when we need to
    /// overload the virtual set_position method -- we unsuspectingly go through
    /// the wrong interface.
    void set_position( int x, int y );

    /// \todo Remove this method; it causes trouble in paradise when we need to
    /// overload the virtual set_position method -- we unsuspectingly go through
    /// the wrong interface.
    void set_size( int w, int h );

    /// \param pos The offset to add onto the stored position.
    virtual void translate(const Point2i& pos);

  private:
    Point2i position_;

    /// \brief The dimensions representing width and height.
    Size2i size_;
};

} // namespace nom

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
