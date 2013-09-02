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

#include <SDL/SDL.h>

#include "nomlib/config.hpp"

namespace nom {

const std::string color_delimiter = ", ";

/// \brief Utility class container for RGBA colors
class Color
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color ( void );

    /// Constructor variant for setting a color using RGB values
    Color ( uint8 red, uint8 green, uint8 blue );

    /// Constructor variant for setting a color using RGBA values
    Color ( uint8 red, uint8 green, uint8 blue, uint8 alpha );

    /// Copy constructor
    Color ( const Color& color );

    /// Destructor
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
    uint8 red;
    uint8 green;
    uint8 blue;
    uint8 alpha;
}; // class Color

/// Pretty print the color using the following format string:
///
/// <color.red>, <color.green>, <color.blue>, <color.alpha>
///
/// This will look like:
///
/// 99, 144, 255, 128
std::ostream& operator << ( std::ostream& os, const Color& color );

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
