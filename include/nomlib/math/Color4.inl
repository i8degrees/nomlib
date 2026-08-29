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
#include "nomlib/math/Color4.hpp"

// Private headers
#include "nomlib/core/strings.hpp"

namespace nom {

template <> const uint8 Color4u::ALPHA_TRANSPARENT = 0;
template <> const uint8 Color4u::ALPHA_OPAQUE = 255;

template <> const int16 Color4i::ALPHA_TRANSPARENT = 0;
template <> const int16 Color4i::ALPHA_OPAQUE = 255;

template <> const float Color4f::ALPHA_TRANSPARENT = 0.0f;
template <> const float Color4f::ALPHA_OPAQUE = 1.0f;

/// Null value for a nom::Color4 using signed integers
template <> const Color4i Color4i::null ( -1, -1, -1, Color4i::ALPHA_OPAQUE );

/// Null value for a nom::Color4 using floating point numbers
template <> const Color4f Color4f::null ( -1, -1, -1, Color4f::ALPHA_OPAQUE );

template <> const Color4i Color4i::Transparent(0, 0, 0, 0);
template <> const Color4i Color4i::Black (0, 0, 0);
template <> const Color4i Color4i::White (255, 255, 255);
template <> const Color4i Color4i::Red (255, 0, 0);
template <> const Color4i Color4i::Green (0, 255, 0);
template <> const Color4i Color4i::Blue (0, 0, 255);
template <> const Color4i Color4i::Yellow (255, 255, 0);
template <> const Color4i Color4i::Magenta (255, 0, 255);
template <> const Color4i Color4i::Cyan (0, 255, 255);
template <> const Color4i Color4i::Silver (192,192,192);
template <> const Color4i Color4i::Purple (128,0,128);
template <> const Color4i Color4i::Orange (255,165,0);
template <> const Color4i Color4i::LightGray (99, 99, 99);
template <> const Color4i Color4i::Gray (67, 67, 67);
template <> const Color4i Color4i::SkyBlue (110,144,190);

template <> const Color4f Color4f::Transparent(0.0f, 0.0f, 0.0f, 0.0f);
template <> const Color4f Color4f::Black (0.0f, 0.0f, 0.0f);
template <> const Color4f Color4f::White (1.0f, 1.0f, 1.0f);
template <> const Color4f Color4f::Red (1.0f, 0.0f, 0.0f);
template <> const Color4f Color4f::Green (0.0f, 1.0f, 0.0f);
template <> const Color4f Color4f::Blue (0.0f, 0.0f, 1.0f);
template <> const Color4f Color4f::Yellow (1.0f, 1.0f, 0.0f);
template <> const Color4f Color4f::Magenta (1.0f, 0.0f, 1.0f);
template <> const Color4f Color4f::Cyan (0.0f, 1.0f, 1.0f);

/// \brief Get the difference color blend of two colors.
///
/// \remarks Implements the color blending of the "Difference" layer mode.
template <typename T>
NOM_EXPORT inline
Color4<T> difference_color_blend( const Color4<T>& lhs, const Color4<T>& rhs )
{
  Color4<T> c;
  c.r = abs( lhs.r - rhs.r );
  c.g = abs( lhs.g - rhs.g );
  c.b = abs( lhs.b - rhs.b );

  return c;
}

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
NOM_EXPORT inline
std::ostream& operator << ( std::ostream& os, const Color4<T>& color )
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

/// Pretty prints nom::Color4 object
///
/// \remarks Uses the following formatting string:
/// \code
/// <color.r>, <color.g>, <color.b>, <color.a>
/// \endcode
NOM_EXPORT inline
std::ostream& operator <<( std::ostream& os, const Color4<float>& color )
{
  os
  << static_cast<float> ( color.r )
  << COLOR_DELIMITER
  << static_cast<float> ( color.g )
  << COLOR_DELIMITER
  << static_cast<float> ( color.b )
  << COLOR_DELIMITER
  << static_cast<float> ( color.a );

  return os;
}

/// \brief Equality comparison operator.
template <typename T>
NOM_EXPORT inline
bool operator == ( const Color4<T>& left, const Color4<T>& right )
{
  return ( left.r == right.r ) &&
         ( left.g == right.g ) &&
         ( left.b == right.b ) &&
         ( left.a == right.a );
}

/// \brief In-equality comparison operator.
template <typename T>
NOM_EXPORT inline
bool operator != ( const Color4<T>& left, const Color4<T>& right )
{
  return ! ( left == right );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
NOM_EXPORT inline
Color4<T> operator + ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T> (  static_cast<T> ( std::min ( left.r + right.r, 255 ) ),
                    static_cast<T> ( std::min ( left.g + right.g, 255 ) ),
                    static_cast<T> ( std::min ( left.b + right.b, 255 ) ),
                    static_cast<T> ( std::min ( left.a + right.a, 255 ) )
                  );
}

/// Values that exceed 255 are clamped to 255
NOM_EXPORT inline
Color4<float> operator +( const Color4<float>& lhs, const Color4<float>& rhs )
{
  return Color4<float>  (
                          std::min( ( lhs.r + rhs.r / 255 ), 1.0f ),
                          std::min( ( lhs.g + rhs.g / 255 ), 1.0f ),
                          std::min( ( lhs.b + rhs.b / 255 ), 1.0f ),
                          std::min( ( lhs.a + rhs.a / 255 ), 1.0f )
                        );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
NOM_EXPORT inline Color4<T> operator ++ ( Color4<T>& left )
{
  return Color4<T> (  static_cast<T> ( left.r-- ),
                    static_cast<T> ( left.g-- ),
                    static_cast<T> ( left.b-- ),
                    static_cast<T> ( left.a-- )
                );
}

template <typename T>
/// Values that exceed 255 are clamped to 255
NOM_EXPORT inline
Color4<T> operator - ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T>  (
                      static_cast<T> ( std::min( left.r - right.r, 255 ) ),
                      static_cast<T> ( std::min( left.g - right.g, 255 ) ),
                      static_cast<T> ( std::min( left.b - right.b, 255 ) ),
                      static_cast<T> ( std::min( left.a - right.a, 255 ) )
                    );
}

/// Values that exceed 255 are clamped to 255
NOM_EXPORT inline
Color4<float> operator -( const Color4<float>& lhs, const Color4<float>& rhs )
{
  return Color4<float>  (
                          std::min( ( lhs.r - rhs.r ) / 255, 1.0f ),
                          std::min( ( lhs.g - rhs.g ) / 255, 1.0f ),
                          std::min( ( lhs.b - rhs.b ) / 255, 1.0f ),
                          std::min( ( lhs.a - rhs.a ) / 255, 1.0f )
                        );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
NOM_EXPORT inline Color4<T> operator -- ( Color4<T>& left )
{
  return Color4<T> ( static_cast<T> ( left.r-- ),
                    static_cast<T> ( left.g-- ),
                    static_cast<T> ( left.b-- ),
                    static_cast<T> ( left.a-- )
                  );
}

/// Values that exceed 255 are clamped to 255
template <typename T>
NOM_EXPORT inline
Color4<T> operator * ( const Color4<T>& left, const Color4<T>& right )
{
  return Color4<T>  (
                      static_cast<T> ( left.r * right.r / 255 ),
                      static_cast<T> ( left.g * right.g / 255 ),
                      static_cast<T> ( left.b * right.b / 255 ),
                      static_cast<T> ( left.a * right.a / 255 )
                    );
}

template <typename T>
NOM_EXPORT inline
Color4<T>& operator += ( Color4<T>& left, const Color4<T>& right )
{
  return left = left + right;
}

template <typename T>
NOM_EXPORT inline
Color4<T>& operator -= ( Color4<T>& left, const Color4<T>& right )
{
  return left = left - right;
}

template <typename T>
NOM_EXPORT inline
Color4<T>& operator *= ( Color4<T>& left, const Color4<T>& right )
{
  return left = left * right;
}

/// \brief Lesser than comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT inline
bool operator <(const Color4<T> lhs, const Color4<T>& rhs)
{
  return  (lhs.r < rhs.r) && (lhs.r < rhs.r)  &&
          (lhs.g < rhs.g) && (lhs.g < rhs.g)  &&
          (lhs.b < rhs.b) && (lhs.b < rhs.b)  &&
          (lhs.a < rhs.a) && (lhs.a < rhs.a);
}

/// \brief Greater than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT inline
bool operator >(const Color4<T>& lhs, const Color4<T>& rhs)
{
  return  (rhs.r < lhs.r) && (rhs.r < lhs.r)  &&
          (rhs.g < lhs.g) && (rhs.g < lhs.g)  &&
          (rhs.b < lhs.b) && (rhs.b < lhs.b)  &&
          (rhs.a < lhs.a) && (rhs.a < lhs.a);
}

/// \brief Lesser than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT inline
bool operator <=(const Color4<T>& lhs, const Color4<T>& rhs)
{
  return  (lhs.r <= rhs.r) && (lhs.r <= rhs.r)  &&
          (lhs.g <= rhs.g) && (lhs.g <= rhs.g)  &&
          (lhs.b <= rhs.b) && (lhs.b <= rhs.b)  &&
          (lhs.a <= rhs.a) && (lhs.a <= rhs.a);
}

/// \brief Greater than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT inline
bool operator >=(const Color4<T>& lhs, const Color4<T>& rhs)
{
  return  (rhs.r <= lhs.r) && (rhs.r <= lhs.r)  &&
          (rhs.g <= lhs.g) && (rhs.g <= lhs.g)  &&
          (rhs.b <= lhs.b) && (rhs.b <= lhs.b)  &&
          (rhs.a <= lhs.a) && (rhs.a <= lhs.a);
}

/// \brief Method overload of binary operator / (Division)
///
/// \param rhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Division of both objects; result is assigned to the left operand.
///
/// \returns Reference to the left operand.
template <typename T>
NOM_EXPORT inline
Color4<T> operator /(const Color4<T>& lhs, const Color4<T>& rhs)
{
  return Color4<T>( lhs.r / rhs.r,
                    lhs.g / rhs.g,
                    lhs.b / rhs.b,
                    lhs.a / rhs.a
                  );
}

/// \brief Method overload of binary operator /= (Division)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Division of both objects; result is assigned to the
/// left operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Color4<T>& operator /=(Color4<T>& lhs, Color4<T>& rhs)
{
  lhs.r /= rhs.r;
  lhs.g /= rhs.g;
  lhs.b /= rhs.b;
  lhs.a /= rhs.a;

  return lhs;
}

Color4i
make_color_from_hex_string(const std::string& hex_encoding)
{
  std::string hex_str = hex_encoding;
  auto hex_str_len = hex_encoding.length();
  nom::size_type pos = 1;               // string position
  const nom::size_type NUM_CHARS = 2;   // increment by
  Color4i result = Color4i::Black;      // catch-all case

  if(hex_str.empty() == true) {
    return result;
  }

  if(hex_str[0] != '#') {
    pos = 0;
  }

  std::string red_str;
  std::string green_str;
  std::string blue_str;

  if(hex_str_len < 2) {
    red_str = "0";
  } else {
    red_str = hex_encoding.substr(pos, NUM_CHARS);
    pos += NUM_CHARS;
  }

  red_str = "0x" + red_str;

  if(hex_str_len < 3) {
    green_str = "0";
  } else {
    green_str = hex_encoding.substr(pos, NUM_CHARS);
    pos += NUM_CHARS;
  }

  green_str = "0x" + green_str;

  if(hex_str_len < 6) {
    blue_str = "0";
  } else {
    blue_str = hex_encoding.substr(pos, NUM_CHARS);
  }

  blue_str = "0x" + blue_str;

  int red_channel = nom::string_to_int(red_str.c_str());
  int green_channel = nom::string_to_int(green_str.c_str());
  int blue_channel = nom::string_to_int(blue_str.c_str());

  result.r = red_channel;
  result.g = green_channel;
  result.b = blue_channel;

  return result;
}

Color4i
make_color_from_string(const std::string& color)
{
  Color4i result(Color4i::Transparent);

  if( nom::compare_string_insensitive(color, "transparent") == 0 ) {
    result = Color4i::Transparent;
  } else if( nom::compare_string_insensitive(color, "white") == 0 ) {
    result = Color4i::White;
  } else if( nom::compare_string_insensitive(color, "red") == 0 ) {
    result = Color4i::Red;
  } else if( nom::compare_string_insensitive(color, "green") == 0 ) {
    result = Color4i::Green;
  } else if( nom::compare_string_insensitive(color, "blue") == 0 ) {
    result = Color4i::Blue;
  } else if( nom::compare_string_insensitive(color, "yellow") == 0 ) {
    result = Color4i::Yellow;
  } else if( nom::compare_string_insensitive(color, "magenta") == 0 ) {
    result = Color4i::Magenta;
  } else if( nom::compare_string_insensitive(color, "cyan") == 0 ) {
    result = Color4i::Cyan;
  } else if( nom::compare_string_insensitive(color, "silver") == 0 ) {
    result = Color4i::Silver;
  } else if( nom::compare_string_insensitive(color, "purple") == 0 ) {
    result = Color4i::Purple;
  } else if( nom::compare_string_insensitive(color, "orange") == 0 ) {
    result = Color4i::Orange;
  } else if(  nom::compare_string_insensitive(color, "lightgray") == 0 ||
              nom::compare_string_insensitive(color, "light gray") == 0 )
  {
    result = Color4i::LightGray;
  } else if( nom::compare_string_insensitive(color, "gray") == 0 ) {
    result = Color4i::Gray;
  } else if( nom::compare_string_insensitive(color, "skyblue") == 0 ) {
    result = Color4i::SkyBlue;
  }

  return result;
}

} // namespace nom
