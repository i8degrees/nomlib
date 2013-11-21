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
#ifndef NOMLIB_RGBA_COLOR_HPP
#define NOMLIB_RGBA_COLOR_HPP

#include <algorithm>

#include "nomlib/config.hpp"

namespace nom {

/// Delimiter character to use with << operator
///
/// \todo Fix style; all uppercase letters
const std::string COLOR_DELIMITER = ", ";

/// \brief RGBA color
///
/// \todo Convert to a template class
/// \todo Implement lesser than, greater than and so on operators?
class Color
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( Color::ALPHA_OPAQUE ) {}

    /// Destructor
    ~Color ( void ) {}

    /// Copy constructor
    Color ( const Color& other )
    {
      this->red = other.red;
      this->green = other.green;
      this->blue = other.blue;
      this->alpha = other.alpha;
    }

    /// Constructor variant for setting a color using RGB values
    Color ( uint8 r, uint8 g, uint8 b ) : red ( r ), green ( g ),
      blue ( b ), alpha ( Color::ALPHA_OPAQUE ) {}

    /// Constructor variant for setting a color using RGBA values
    Color ( uint8 red, uint8 green, uint8 blue, uint8 alpha ) : red ( red ),
      green ( green ), blue ( blue ), alpha ( alpha ) {}

    /// Copy assignment operator
    inline Color& operator = ( const Color& other )
    {
      this->red = other.red;
      this->green = other.green;
      this->blue = other.blue;
      this->alpha = other.alpha;

      return *this;
    }

    /// Convenience getter for obtaining a copy of this object
    inline const Color& get ( void ) const
    {
      return *this;
    }

    /// 100% transparent alpha channel value
    static const int ALPHA_TRANSPARENT;

    /// 100% opaque alpha channel value
    static const int ALPHA_OPAQUE;

    /// Primary colors
    static const Color Black;
    static const Color White;
    static const Color Red;
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;

    /// Additional colors
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

    /// Reference alias for red instance variable
    uint8& r = red;

    /// Reference alias for green instance variable
    uint8& g = green;

    /// Reference alias for blue instance variable
    uint8& b = blue;

    /// Reference alias for alpha instance variable
    uint8& a = alpha;
};

/// Pretty print the color using the following format string:
///
/// <color.red>, <color.green>, <color.blue>, <color.alpha>
///
/// This will look like:
///
/// 99, 144, 255, 128
static inline std::ostream& operator << ( std::ostream& os,
    const Color& color )
{
  os << static_cast<uint8> ( color.red ) << COLOR_DELIMITER
     << static_cast<uint8> ( color.green ) << COLOR_DELIMITER
     << static_cast<uint8> ( color.blue ) << COLOR_DELIMITER
     << static_cast<uint8> ( color.alpha );
  return os;
}

static inline bool operator == ( const Color& left, const Color& right )
{
  return ( left.red == right.red ) &&
         ( left.green == right.green ) &&
         ( left.blue == right.blue ) &&
         ( left.alpha == right.alpha );
}

static inline bool operator != ( const Color& left, const Color& right )
{
  return ! ( left == right );
}

/// Values that exceed 255 are clamped to 255
static inline Color operator + ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8> ( std::min ( left.red + right.red, 255 ) ),
                  static_cast<uint8> ( std::min ( left.green + right.green, 255 ) ),
                  static_cast<uint8> ( std::min ( left.blue + right.blue, 255 ) ),
                  static_cast<uint8> ( std::min ( left.alpha + right.alpha, 255 ) )
                );
}

/// Values that exceed 255 are clamped to 255
static inline Color operator ++ ( Color& left )
{
  return Color  ( static_cast<uint8> ( left.red-- ),
                  static_cast<uint8> ( left.green-- ),
                  static_cast<uint8> ( left.blue-- ),
                  static_cast<uint8> ( left.alpha-- )
                );
}

/// Values that exceed 255 are clamped to 255
static inline Color operator - ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8> ( std::min ( left.red - right.red, 255 ) ),
                  static_cast<uint8> ( std::min ( left.green - right.green, 255 ) ),
                  static_cast<uint8> ( std::min ( left.blue - right.blue, 255 ) ),
                  static_cast<uint8> ( std::min ( left.alpha - right.alpha, 255 ) )
                );
}

/// Values that exceed 255 are clamped to 255
static inline Color operator -- ( Color& left )
{
  return Color  ( static_cast<uint8> ( left.red-- ),
                  static_cast<uint8> ( left.green-- ),
                  static_cast<uint8> ( left.blue-- ),
                  static_cast<uint8> ( left.alpha-- )
                );
}

/// Values that exceed 255 are clamped to 255
static inline Color operator * ( const Color& left, const Color& right )
{
  return Color  ( static_cast<uint8> ( left.red * right.red / 255 ),
                  static_cast<uint8> ( left.green * right.green / 255 ),
                  static_cast<uint8> ( left.blue * right.blue / 255 ),
                  static_cast<uint8> ( left.alpha * right.alpha / 255 )
                );
}

static inline Color& operator += ( Color& left, const Color& right )
{
  return left = left + right;
}

static inline Color& operator -= ( Color& left, const Color& right )
{
  return left = left - right;
}

static inline Color& operator *= ( Color& left, const Color& right )
{
  return left = left * right;
}

} // namespace nom

#endif // include guard defined
