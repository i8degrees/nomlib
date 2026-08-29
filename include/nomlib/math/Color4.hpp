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
#include <vector>

#include "export.hpp"
#include "nomlib/config.hpp"

// FIXME: The following declaration is necessary in order to avoid a very
// nasty compiling conflict that can happen under Windows anytime the
// windef.h header file is included (commonly from windows.h), due to min and
// max macros being declared there. This is why macros are evil.
//
// http://support.microsoft.com/kb/143208
// http://stackoverflow.com/questions/5004858/stdmin-gives-error
#if defined( NOM_PLATFORM_WINDOWS )
  #undef min
  #undef max
#endif

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string COLOR_DELIMITER = ", ";

/// \brief RGBA color container
///
/// \todo Implement specialized Color4<float> operators
/// \todo Explicitly initialize alpha component of static colors to 255.
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

    /// \brief Copy constructor.
    ///
    /// \remarks The explicit keyword here will result in compile-time errors
    /// in any instance that it finds incompatible casting occurring, such as if
    /// you try to down-cast a Color4<int16> to a Color4<uint8>.
    template <typename U>
    Color4( const Color4<U>& rhs ) :
      r{ static_cast<T> ( rhs.r ) },
      g{ static_cast<T> ( rhs.g ) },
      b{ static_cast<T> ( rhs.b ) },
      a{ static_cast<T> ( rhs.a ) }
    {
      //NOM_LOG_TRACE(NOM);
    }

    /// \brief Copy constructor.
    ///
    /// \remarks The explicit keyword here will result in compile-time errors
    /// in any instance that it finds incompatible casting occurring, such as if
    /// you try to down-cast a Color4<int16> to a Color4<uint8>.
    explicit Color4( const Color4<float>& rhs ) :
      r{ static_cast<float> ( rhs.r ) },
      g{ static_cast<float> ( rhs.g ) },
      b{ static_cast<float> ( rhs.b ) },
      a{ static_cast<float> ( rhs.a ) }
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

    /// \brief 100% transparent alpha channel value
    static const T ALPHA_TRANSPARENT;

    /// \brief 100% opaque alpha channel value
    static const T ALPHA_OPAQUE;

    /// \brief Null value
    ///
    /// \remarks  Null value implementation depends on signed (negative) numbers.
    static const Color4 null;

    /// \brief Primary colors
    ///
    /// \todo Consider using Color4::Transparent instead of Color4::null.
    /// \todo Rename constants to ALL UPPERCASE.
    static const Color4 Transparent;
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

    // static const Color4 ButtonGradient;

    /// Red component
    T r;

    /// Green component
    T g;

    /// Blue component
    T b;

    /// Alpha component
    T a;
};


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

/// \brief A container of Color4i objects.
typedef std::vector<Color4i> Color4iColors;

/// \brief A container of Color4f objects.
typedef std::vector<Color4f> Color4fColors;

/// \brief A container of Color4u objects.
typedef std::vector<Color4u> Color4uColors;
#if 0
template <> const uint8 Color4u::ALPHA_OPAQUE;
template <> const int16 Color4i::ALPHA_TRANSPARENT;
template <> const int16 Color4i::ALPHA_OPAQUE;
template <> const float Color4f::ALPHA_TRANSPARENT;
template <> const float Color4f::ALPHA_OPAQUE;
template <> const Color4i Color4i::null;
/// Null value for a nom::Color4 using floating point numbers
template <> const Color4f Color4f::null;
template <> const Color4i Color4i::Transparent;
template <> const Color4i Color4i::Black;
template <> const Color4i Color4i::White;
template <> const Color4i Color4i::Red;
template <> const Color4i Color4i::Green;
template <> const Color4i Color4i::Blue;
template <> const Color4i Color4i::Yellow;
template <> const Color4i Color4i::Magenta;
template <> const Color4i Color4i::Cyan;
template <> const Color4i Color4i::Silver;
template <> const Color4i Color4i::Purple;
template <> const Color4i Color4i::Orange;
template <> const Color4i Color4i::LightGray;
template <> const Color4i Color4i::Gray;
template <> const Color4i Color4i::SkyBlue;

template <> const Color4f Color4f::Transparent;
template <> const Color4f Color4f::Black;
template <> const Color4f Color4f::White;
template <> const Color4f Color4f::Red;
template <> const Color4f Color4f::Green;
template <> const Color4f Color4f::Blue;
template <> const Color4f Color4f::Yellow;
template <> const Color4f Color4f::Magenta;
template <> const Color4f Color4f::Cyan;
#endif

NOM_EXPORT Color4i
make_color_from_hex_string(const std::string& hex_encoding);

NOM_EXPORT Color4i
make_color_from_string(const std::string& color);

} // namespace nom

#include "Color4.inl"

#endif // include guard defined
