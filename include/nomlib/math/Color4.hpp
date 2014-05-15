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
#ifndef NOMLIB_MATH_COLOR4_HPP
#define NOMLIB_MATH_COLOR4_HPP

#include <algorithm>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string COLOR_DELIMITER = ", ";

/// \brief RGBA color container
///
/// \todo Implement lesser than, greater than and so on operators?
/// \todo Implement specialized Color4<float> operators
template <typename T>
struct Color4
{
    /// Default constructor; initialize values to Color<T>::null
    Color4 ( void ) :
      r ( -1 ),
      g ( -1 ),
      b ( -1 ),
      a ( Color4<T>::ALPHA_OPAQUE )
    {
      //NOM_LOG_TRACE(NOM);
    }

    /// Destructor
    ~Color4 ( void )
    {
      //NOM_LOG_TRACE(NOM);
    }

    /// \brief Copy constructor
    ///
    /// \remarks The explicit keyword here will result in compile-time errors
    /// in any instance that it finds incompatible casting occurring, such as if
    /// you try to down-cast a Color4<int16> to a Color4<uint8>.
    template <typename U>
    explicit Color4 ( const Color4<U>& copy ) :
      r { static_cast<T> ( copy.r ) },
      g { static_cast<T> ( copy.g ) },
      b { static_cast<T> ( copy.b ) },
      a { static_cast<T> ( copy.a ) }
    {
      //NOM_LOG_TRACE(NOM);
    }

    /// Constructor variant for setting a color using RGB values
    Color4 ( T r, T g, T b )  :
      r ( r ),
      g ( g ),
      b ( b ),
      a ( Color4<T>::ALPHA_OPAQUE )
    {
      //NOM_LOG_TRACE(NOM);
    }

    /// Constructor variant for setting a color using RGBA values
    Color4 ( T red, T green, T blue, T alpha )  :
      r ( red ),
      g ( green ),
      b ( blue ),
      a ( alpha )
    {
      //NOM_LOG_TRACE(NOM);
    }

    /// Convenience getter for obtaining a copy of this object
    inline const Color4<T>& get ( void ) const
    {
      return *this;
    }

    /// \brief 100% transparent alpha channel value
    static const T ALPHA_TRANSPARENT;

    /// \brief 100% opaque alpha channel value
    static const T ALPHA_OPAQUE;

    /// \brief Null value
    ///
    /// \remarks  Null value implementation depends on signed (negative) numbers.
    static const Color4 null;

    /// Primary colors
    static const Color4 Black;
    static const Color4 White;
    static const Color4 Red;
    static const Color4 Green;
    static const Color4 Blue;
    static const Color4 Yellow;
    static const Color4 Magenta;
    static const Color4 Cyan;

    /// Additional colors
    static const Color4 Silver;
    static const Color4 Purple;
    static const Color4 Orange;
    static const Color4 LightGray;
    static const Color4 Gray;
    static const Color4 SkyBlue;

    /// Red component
    T r;

    /// Green component
    T g;

    /// Blue component
    T b;

    /// Alpha component
    T a;
};

/// Pretty prints nom::Color4 object
///
/// \remarks Uses the following formatting string:
/// \code
/// <color.r>, <color.g>, <color.b>, <color.a>
/// \endcode
///
/// \note The color values are static casted to nom::sint before outputting the
/// string in case a nom::Color4<uint8> object is used.
template <typename T>
inline std::ostream& operator << ( std::ostream& os, const Color4<T>& color )
{
  os
  << static_cast<sint> ( color.r )
  << COLOR_DELIMITER
  << static_cast<sint> ( color.g )
  << COLOR_DELIMITER
  << static_cast<sint> ( color.b )
  << COLOR_DELIMITER
  << static_cast<sint> ( color.a );

  return os;
}

/// \brief Equality comparison operator.
template <typename T>
inline bool operator == ( const Color4<T>& left, const Color4<T>& right )
{
  return ( left.r == right.r ) &&
         ( left.g == right.g ) &&
         ( left.b == right.b ) &&
         ( left.a == right.a );
}

/// \brief In-equality comparison operator.
template <typename T>
inline bool operator != ( const Color4<T>& left, const Color4<T>& right )
{
  return ! ( left == right );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
inline Color4<T> operator + ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T> (  static_cast<T> ( std::min ( left.r + right.r, 255 ) ),
                    static_cast<T> ( std::min ( left.g + right.g, 255 ) ),
                    static_cast<T> ( std::min ( left.b + right.b, 255 ) ),
                    static_cast<T> ( std::min ( left.a + right.a, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
inline Color4<T> operator ++ ( Color4<T>& left )
{
  return Color4<T> (  static_cast<T> ( left.r-- ),
                    static_cast<T> ( left.g-- ),
                    static_cast<T> ( left.b-- ),
                    static_cast<T> ( left.a-- )
                );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
inline Color4<T> operator - ( const Color4<int16>& left, const Color4<int16>& right )
{
  return Color4<T> (  static_cast<T> ( std::min ( left.r - right.r, 255 ) ),
                    static_cast<T> ( std::min ( left.g - right.g, 255 ) ),
                    static_cast<T> ( std::min ( left.b - right.b, 255 ) ),
                    static_cast<T> ( std::min ( left.a - right.a, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
inline Color4<T> operator -- ( Color4<T>& left )
{
  return Color4<T> ( static_cast<T> ( left.r-- ),
                    static_cast<T> ( left.g-- ),
                    static_cast<T> ( left.b-- ),
                    static_cast<T> ( left.a-- )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
inline Color4<T> operator * ( const Color4<int16>& left, const Color4<int16>& right )
{
  return Color4<T> ( static_cast<T> ( left.r * right.r / 255 ),
                    static_cast<T> ( left.g * right.g / 255 ),
                    static_cast<T> ( left.b * right.b / 255 ),
                    static_cast<T> ( left.a * right.a / 255 )
                  );
}

template <typename T>
inline Color4<T>& operator += ( Color4<T>& left, const Color4<T>& right )
{
  return left = left + right;
}

template <typename T>
inline Color4<T>& operator -= ( Color4<T>& left, const Color4<T>& right )
{
  return left = left - right;
}

template <typename T>
inline Color4<T>& operator *= ( Color4<T>& left, const Color4<T>& right )
{
  return left = left * right;
}

/// Color4 object defined using signed 16-bit integers
typedef Color4<int16> Color4i;

/// Color4 object defined using floating point numbers
typedef Color4<float> Color4f;

/// \brief Color4 object defined using signed 8-bit integers
///
/// \remarks Color4<T>::null does not work with this Color4 type, nor are there
/// any predefined colors for the type. It is intended solely for use with
/// conversions (such as in feeding to SDL2).
typedef Color4<uint8> Color4u;

} // namespace nom

#endif // include guard defined
