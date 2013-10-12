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

#include <algorithm>

#include "SDL.h"

#include "nomlib/config.hpp"

namespace nom {

const std::string coords_delimiter = ", ";

/// \brief Coordinate values (x, y, z, width, height ) wrapper
///
/// \deprecated This class is likely to be phased out & replaced by nom::Point2,
/// nom::Point3 and nom::Rect at some point in the near future!
class Coords
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Coords ( void );

    /// Destructor
    ~Coords ( void );

    /// Copy constructor
    Coords ( const Coords& coords );

    /// Constructor variant for creating coords
    Coords ( int32 x, int32 y, int32 width, int32 height );

    /// Constructor variant for creating coords
    Coords ( int32 x, int32 y );

    /// Convenience method for determining if this object is initialized or not;
    /// an initialized object should never be equal to nom::Coords::null
    ///
    /// \todo Fully implement me
    bool isNull ( void ) const;

    /// Convenience getter for obtaining a copy of this object
    const Coords& get ( void ) const;

    Coords getPosition ( void ) const;
    void setPosition ( int32 x, int32 y );

    Coords getSize ( void ) const;
    void setSize ( int32 width, int32 height );

    /// Check to see if input X, Y coordinates are within the bounds of this
    /// object -- or in other words, a collision!
    bool contains ( int32 x, int32 y ) const;

    /// Check to see if input X, Y coordinates are within the bounds of this
    /// object -- or in other words, a collision!
    ///
    /// ...
    bool contains ( const Coords& pos ) const;

    /// Checks to see if our rectangle overlaps with another
    bool intersects ( Coords& rectangle ) const;

    /// Copy assignment operator
    Coords& operator = ( const Coords& other );

    /// Convenience object that will always contain a value of -1
    static const Coords null;

    /// SDL backwards-compatibility wrappers for nomlib
    ///
    /// Returns a SDL_Rect structure of a nom::Coords object
    inline SDL_Rect SDL ( void ) const
    {
      SDL_Rect r;

      r.x = this->x;
      r.y = this->y;
      r.w = this->width;
      r.h = this->height;
      return r;
    }

  public:
    int32 x;
    int32 y;
    int32 width;
    int32 height;

    /// Reference alias for width instance variable
    int32& w = width;

    /// Reference alias for height instance variable
    int32& h = height;
};

/// Pretty print the color using the following format string:
///
/// <coords.x>, <coords.y>, <coords.width>, <coords.height>
///
/// This will look like:
///
/// 128, 144, 64, 64
std::ostream& operator << ( std::ostream& os, const Coords& coords );

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
