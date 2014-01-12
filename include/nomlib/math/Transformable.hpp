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
#ifndef NOMLIB_TRANSFORMABLE_HEADERS
#define NOMLIB_TRANSFORMABLE_HEADERS

#include "nomlib/config.hpp"
#include "nomlib/math/Coords.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

class Transformable: public virtual IDrawable
{
  public:
    Transformable ( void );
    virtual ~Transformable ( void );

    Transformable ( int x, int y, int w, int h );
    Transformable ( const Coords& coords );

    const Coords position ( void ) const;
    const Coords size ( void ) const;
    void set_position ( int32 x, int32 y );
    void set_position ( int32 x, int32 y, int32 width, int32 height );

    /// \deprecated Use nom::Transformable::set_position(const IntRect& coords)
    void set_position ( const Coords& coords );

    void set_position ( const IntRect& coords );

    void setSize ( int32 width, int32 height );
    void setSize ( const Coords& size );

    void move ( uint32 x = 0, uint32 y = 0 );
    void move ( const Coords& offsets = Coords ( 0, 0 ) );

  protected:
    /// \deprecated Phasing nom::Coords class out -- use IntRects instead.
    Coords position_;
};


} // namespace nom

#endif // NOMLIB_TRANSFORMABLE_HEADERS defined
