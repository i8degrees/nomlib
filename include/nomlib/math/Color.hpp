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

#include "SDL2/SDL.h"

#include "nomlib/config.hpp"

namespace nom {

const std::string color_delimiter = ", ";

/// \brief Utility class container for RGBA colors
///
/// \todo Convert to a template class
class Color
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color ( void );

    /// Destructor
    ~Color ( void );

    /// Copy constructor
    Color ( const Color& color );

    /// Constructor variant for setting a color using RGB values
    Color ( uint8 red, uint8 green, uint8 blue );

    /// Constructor variant for setting a color using RGBA values
    Color ( uint8 red, uint8 green, uint8 blue, uint8 alpha );

    /// Convenience method for determining if this object is initialized or not;
    /// an initialized object should never be equal to nom::Color::null
    ///
    /// \todo Fully implement me
    bool isNull ( void ) const;

    /// Convenience getter for obtaining a copy of this object
    const Color& get ( void ) const;

    /// Copy assignment operator
    Color& operator = ( const Color& other );

    /// Convenience object that will always contain a value of -1
    static const Color null;

    /// Predefined color constants for convenience sake
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;

    /// Additional predefined color constants
    static const Color LightGray;
    static const Color Gray;

    /// Sky blue color key
    static const Color NomPrimaryColorKey;

    /// Light magenta color key
    static const Color NomSecondaryColorKey;

  public:
    uint8 red;
    uint8 green;
    uint8 blue;
    uint8 alpha;

    /// \todo Test me
    ///uint8& r = red;
    /// \todo Test me
    ///uint8& g = green;

    /// \todo Test me
    ///uint8& b = blue;

    /// \todo Test me
    ///uint8& a = alpha;
};

class RGBA
{
  public:
    RGBA ( void ) {};
    ~RGBA ( void ) {};

    /// Returns a SDL color structure of a nom::Color object
    static inline SDL_Color asSDLCOLOR ( const Color& color )
    {
      SDL_Color c;

      c.r = color.red;
      c.g = color.green;
      c.b = color.blue;
      //      SDLv2 SDL_Color struct
      //
      //  c.a = color.alpha;

      return c;
    }

    /// Returns RGBA components via nom::Color object, holding the red, green, blue
    /// and alpha values.
    static inline void asRGB ( uint32 pixel, SDL_PixelFormat* fmt, Color& color )
    {
      SDL_GetRGB ( pixel, fmt, &color.red, &color.green, &color.blue );
    }

    /// Convenience helper for obtaining a color as an integer, respective to
    /// the video surface pixel format (color bit per pixel)
    ///
    /// Returns RGBA components via nom::Color object, holding the red, green, blue
    /// and alpha values.
    static inline uint32 asInt32 ( SDL_PixelFormat* fmt, const Color& color )
    {
      return SDL_MapRGB ( fmt, color.red, color.green, color.blue );
    }
};

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
