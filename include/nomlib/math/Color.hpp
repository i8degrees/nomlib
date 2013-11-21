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
/// \todo Implement lesser than, greater than and so on operators?
template <typename T>
class Color
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( Color::ALPHA_OPAQUE ) {}

    /// Destructor
    ~Color ( void ) {}

    /// Copy constructor
    template <typename U>
    Color ( const Color<U>& copy )
    {
      this->red = static_cast<T> ( copy.red );
      this->green = static_cast<T> ( copy.green );
      this->blue = static_cast<T> ( copy.blue );
      this->alpha = static_cast<T> ( copy.alpha );
    }

    /// Constructor variant for setting a color using RGB values
    Color ( T r, T g, T b ) : red ( r ), green ( g ),
      blue ( b ), alpha ( Color::ALPHA_OPAQUE ) {}

    /// Constructor variant for setting a color using RGBA values
    Color ( T red, T green, T blue, T alpha ) : red ( red ),
      green ( green ), blue ( blue ), alpha ( alpha ) {}

    /// Copy assignment operator
    inline Color<T>& operator = ( const Color<T>& other )
    {
      this->red = other.red;
      this->green = other.green;
      this->blue = other.blue;
      this->alpha = other.alpha;

      return *this;
    }

    /// Convenience getter for obtaining a copy of this object
    inline const Color<T>& get ( void ) const
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
    T red;
    T green;
    T blue;
    T alpha;

    /// Reference alias for red instance variable
    T& r = red;

    /// Reference alias for green instance variable
    T& g = green;

    /// Reference alias for blue instance variable
    T& b = blue;

    /// Reference alias for alpha instance variable
    T& a = alpha;
};

/// Pretty print the color using the following format string:
///
/// <color.red>, <color.green>, <color.blue>, <color.alpha>
///
/// This will look like:
///
/// 99, 144, 255, 128
template <typename T>
static inline std::ostream& operator << ( std::ostream& os,
                                          const Color<T>& color
                                        )
{
  os << static_cast<T> ( color.red ) << COLOR_DELIMITER
     << static_cast<T> ( color.green ) << COLOR_DELIMITER
     << static_cast<T> ( color.blue ) << COLOR_DELIMITER
     << static_cast<T> ( color.alpha );
  return os;
}

template <typename T>
static inline bool operator == ( const Color<T>& left, const Color<T>& right )
{
  return ( left.red == right.red ) &&
         ( left.green == right.green ) &&
         ( left.blue == right.blue ) &&
         ( left.alpha == right.alpha );
}

template <typename T>
static inline bool operator != ( const Color<T>& left, const Color<T>& right )
{
  return ! ( left == right );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color<T> operator + ( const Color<T>& left, const Color<T>& right )
{
  return Color<T> ( static_cast<T> ( std::min ( left.red + right.red, 255 ) ),
                    static_cast<T> ( std::min ( left.green + right.green, 255 ) ),
                    static_cast<T> ( std::min ( left.blue + right.blue, 255 ) ),
                    static_cast<T> ( std::min ( left.alpha + right.alpha, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color<T> operator ++ ( Color<T>& left )
{
  return Color<T> ( static_cast<T> ( left.red-- ),
                    static_cast<T> ( left.green-- ),
                    static_cast<T> ( left.blue-- ),
                    static_cast<T> ( left.alpha-- )
                );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color<T> operator - ( const Color<T>& left, const Color<T>& right )
{
  return Color<T> ( static_cast<T> ( std::min ( left.red - right.red, 255 ) ),
                    static_cast<T> ( std::min ( left.green - right.green, 255 ) ),
                    static_cast<T> ( std::min ( left.blue - right.blue, 255 ) ),
                    static_cast<T> ( std::min ( left.alpha - right.alpha, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color<T> operator -- ( Color<T>& left )
{
  return Color<T> ( static_cast<T> ( left.red-- ),
                    static_cast<T> ( left.green-- ),
                    static_cast<T> ( left.blue-- ),
                    static_cast<T> ( left.alpha-- )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color<T> operator * ( const Color<T>& left, const Color<T>& right )
{
  return Color<T> ( static_cast<T> ( left.red * right.red / 255 ),
                    static_cast<T> ( left.green * right.green / 255 ),
                    static_cast<T> ( left.blue * right.blue / 255 ),
                    static_cast<T> ( left.alpha * right.alpha / 255 )
                  );
}

template <typename T>
static inline Color<T>& operator += ( Color<T>& left, const Color<T>& right )
{
  return left = left + right;
}

template <typename T>
static inline Color<T>& operator -= ( Color<T>& left, const Color<T>& right )
{
  return left = left - right;
}

template <typename T>
static inline Color<T>& operator *= ( Color<T>& left, const Color<T>& right )
{
  return left = left * right;
}

typedef Color<uint8> Color4u;
typedef Color<float> Color4f;

} // namespace nom

#endif // include guard defined
