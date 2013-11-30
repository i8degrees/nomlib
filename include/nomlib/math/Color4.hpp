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
#ifndef NOMLIB_RGBA_COLOR4_HPP
#define NOMLIB_RGBA_COLOR4_HPP

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
class Color4
{
  public:
    /// Default constructor; sets the color to their respective defaults
    Color4 ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( Color4::ALPHA_OPAQUE ) {}

    /// Destructor
    ~Color4 ( void ) {}

    /// Copy constructor
    template <typename U>
    Color4 ( const Color4<U>& copy )
    {
      this->red = static_cast<T> ( copy.red );
      this->green = static_cast<T> ( copy.green );
      this->blue = static_cast<T> ( copy.blue );
      this->alpha = static_cast<T> ( copy.alpha );
    }

    /// Constructor variant for setting a color using RGB values
    Color4 ( T r, T g, T b ) : red ( r ), green ( g ),
      blue ( b ), alpha ( Color4::ALPHA_OPAQUE ) {}

    /// Constructor variant for setting a color using RGBA values
    Color4 ( T red, T green, T blue, T alpha ) : red ( red ),
      green ( green ), blue ( blue ), alpha ( alpha ) {}

    /// Copy assignment operator
    inline Color4<T>& operator = ( const Color4<T>& other )
    {
      this->red = other.red;
      this->green = other.green;
      this->blue = other.blue;
      this->alpha = other.alpha;

      return *this;
    }

    /// Convenience getter for obtaining a copy of this object
    inline const Color4<T>& get ( void ) const
    {
      return *this;
    }

    /// 100% transparent alpha channel value
    static const int ALPHA_TRANSPARENT;

    /// 100% opaque alpha channel value
    static const int ALPHA_OPAQUE;

    /// Primary colors
    static const Color4 Black;
    static const Color4 White;
    static const Color4 Red;
    static const Color4 Green;
    static const Color4 Blue;
    static const Color4 Yellow;
    static const Color4 Magenta;
    static const Color4 Cyan;
    static const Color4 Silver;
    static const Color4 Purple;
    static const Color4 Orange;

    /// Additional colors
    static const Color4 LightGray;
    static const Color4 Gray;

    /// Sky blue color key
    static const Color4 NomPrimaryColorKey;

    /// Light magenta color key
    static const Color4 NomSecondaryColorKey;

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
                                          const Color4<T>& color
                                        )
{
  os << static_cast<T> ( color.red ) << COLOR_DELIMITER
     << static_cast<T> ( color.green ) << COLOR_DELIMITER
     << static_cast<T> ( color.blue ) << COLOR_DELIMITER
     << static_cast<T> ( color.alpha );
  return os;
}

template <typename T>
static inline bool operator == ( const Color4<T>& left, const Color4<T>& right )
{
  return ( left.red == right.red ) &&
         ( left.green == right.green ) &&
         ( left.blue == right.blue ) &&
         ( left.alpha == right.alpha );
}

template <typename T>
static inline bool operator != ( const Color4<T>& left, const Color4<T>& right )
{
  return ! ( left == right );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color4<T> operator + ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T> ( static_cast<T> ( std::min ( left.red + right.red, 255 ) ),
                    static_cast<T> ( std::min ( left.green + right.green, 255 ) ),
                    static_cast<T> ( std::min ( left.blue + right.blue, 255 ) ),
                    static_cast<T> ( std::min ( left.alpha + right.alpha, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color4<T> operator ++ ( Color4<T>& left )
{
  return Color4<T> ( static_cast<T> ( left.red-- ),
                    static_cast<T> ( left.green-- ),
                    static_cast<T> ( left.blue-- ),
                    static_cast<T> ( left.alpha-- )
                );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color4<T> operator - ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T> ( static_cast<T> ( std::min ( left.red - right.red, 255 ) ),
                    static_cast<T> ( std::min ( left.green - right.green, 255 ) ),
                    static_cast<T> ( std::min ( left.blue - right.blue, 255 ) ),
                    static_cast<T> ( std::min ( left.alpha - right.alpha, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color4<T> operator -- ( Color4<T>& left )
{
  return Color4<T> ( static_cast<T> ( left.red-- ),
                    static_cast<T> ( left.green-- ),
                    static_cast<T> ( left.blue-- ),
                    static_cast<T> ( left.alpha-- )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
static inline Color4<T> operator * ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T> ( static_cast<T> ( left.red * right.red / 255 ),
                    static_cast<T> ( left.green * right.green / 255 ),
                    static_cast<T> ( left.blue * right.blue / 255 ),
                    static_cast<T> ( left.alpha * right.alpha / 255 )
                  );
}

template <typename T>
static inline Color4<T>& operator += ( Color4<T>& left, const Color4<T>& right )
{
  return left = left + right;
}

template <typename T>
static inline Color4<T>& operator -= ( Color4<T>& left, const Color4<T>& right )
{
  return left = left - right;
}

template <typename T>
static inline Color4<T>& operator *= ( Color4<T>& left, const Color4<T>& right )
{
  return left = left * right;
}

typedef Color4<uint8> Color4u;
typedef Color4<float> Color4f;

} // namespace nom

#endif // include guard defined
