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
#ifndef NOMLIB_COLORS_HEADERS
#define NOMLIB_COLORS_HEADERS

#include <algorithm>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Utility class container for RGBA colors
class Color
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color ( void );
    /// Constructor variant for setting a color using RGBA values
    Color ( int16 red, int16 green, int16 blue, int16 alpha = -1 ); // SDL_ALPHA_OPAQUE (255)
    /// Copy constructor
    Color ( const Color& color );
    ~Color ( void );

    /// Convenience getter helper for obtaining a color by object
    const Color& getColor ( void ) const;

    /// Copy assignment constructor
    Color& operator = ( const Color& other );

    /// Predefined color constants for convenience sake
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Transparent;

    /// Additional predefined color constants
    static const Color Gray;

  public:
    int16 red;
    int16 green;
    int16 blue;
    int16 alpha;

}; // class Color

bool operator == ( const Color& left, const Color& right );
bool operator != ( const Color& left, const Color& right );

/// Values that exceed 255 are clamped to 255
Color operator + ( const Color& left, const Color& right );

/// Values that exceed 255 are clamped to 255
Color operator ++ ( Color& left );

/// Values that exceed 255 are clamped to 255
Color operator - ( const Color& left, const Color& right );

/// Values that exceed 255 are clamped to 255
Color operator -- ( Color& left );

/// Values that exceed 255 are clamped to 255
Color operator * ( const Color& left, const Color& right);

Color& operator += ( Color& left, const Color& right );
Color& operator -= ( Color& left, const Color& right );
Color& operator *= ( Color& left, const Color& right );


} // namespace nom

#endif // NOMLIB_COLORS_HEADERS defined
