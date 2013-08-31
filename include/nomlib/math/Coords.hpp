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
#ifndef NOMLIB_COORDS_HEADERS
#define NOMLIB_COORDS_HEADERS

#include "nomlib/config.hpp"

namespace nom {

/// \brief Coordinate values (x, y, z, width, height ) wrapper
class Coords
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Coords ( void );
    /// Constructor variant for setting coords by x, y, width, height values
    Coords ( int32_t x, int32_t y, int32_t width = 0, int32_t height = 0 );
    /// Copy constructor
    Coords ( const Coords& coords );
    ~Coords ( void );

    Coords getPosition ( void ) const;
    void setPosition ( int32_t x, int32_t y );

    Coords getSize ( void ) const;
    void setSize ( int32_t width, int32_t height );

    /// \todo TEST ME
    bool contains ( int32_t x, int32_t y ) const;

    /// \todo TEST ME
    bool contains ( const Coords& pos ) const;

    /// Checks to see if our rectangle overlaps with another
    /// \todo TEST ME
    bool intersects ( Coords& rectangle ) const;

    /// Copy assignment constructor
    Coords& operator = ( const Coords& other );

  public:
    signed int x;
    signed int y;
    signed int z; // reserved
    signed int width;
    signed int height;

}; // class Coords

bool operator == ( const Coords& left, const Coords& right );
bool operator != ( const Coords& left, const Coords& right );
Coords operator + ( const Coords& left, const Coords& right );
Coords operator ++ ( Coords& left );
Coords operator - ( const Coords& left, const Coords& right );
Coords operator -- ( Coords& left );
Coords operator * ( const Coords& left, const Coords& right );
Coords operator / ( const Coords& left, const Coords& right );
Coords& operator += ( Coords& left, const Coords& right );
Coords& operator -= ( Coords& left, const Coords& right );
Coords& operator *= ( Coords& left, const Coords& right );


} // namespace nom

#endif // NOMLIB_COORDS_HEADERS defined
