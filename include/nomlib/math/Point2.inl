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
#include "nomlib/math/Point2.hpp"

namespace nom {
#if 0
template <> const Point2i Point2i::null = {-1, -1};
template <> const Point2f Point2f::null = {-1.0f, -1.0f};
template <> const Point2d Point2d::null = {-1.0f, -1.0f};

template <> const Point2i Point2i::zero = {0, 0};
template <> const Point2f Point2f::zero = {0.0f, 0.0f};
template <> const Point2d Point2d::zero = {0.0f, 0.0f};
#endif
#if 1
/// Null value for a nom::Point2 using signed integers
template <> const Point2i Point2i::null ( -1, -1 );

/// Null value for a nom::Point2 using floating point numbers
template <> const Point2f Point2f::null ( -1, -1 );

/// Null value for a nom::Point2 using double precision floating point numbers
template <> const Point2d Point2d::null ( -1, -1 );

/// \brief Zero value for a nom::Point2 using signed integers
template <> const Point2i Point2i::zero( 0, 0 );

/// \brief Zero value for a nom::Point2 using floating point numbers.
template <> const Point2f Point2f::zero( 0, 0 );

/// \brief Zero value for a nom::Point2 using double precision floating point
/// numbers.
template <> const Point2d Point2d::zero( 0, 0 );
#endif

/// Pretty print a Point2 object using the following formatting:
///
///     <Point2.x>, <Point2.y>
///
/// An example print:
///
///     128, 144
template <typename T>
NOM_EXPORT
inline std::ostream& operator << ( std::ostream& os, const Point2<T>& pos )
{
  os
  << pos.x
  << POINT_DELIMITER
  << pos.y;

  return os;
}

/// \brief Method overload of binary operator == (equality)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Compare the equality between two objects.
///
/// \returns True if left operand is equal to the right operand.
template <typename T>
NOM_EXPORT
inline bool operator == ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( lhs.x == rhs.x )  &&  ( lhs.y == rhs.y );
}

/// \brief Lesser than comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT
inline bool operator <( const Point2<T> lhs, const Point2<T>& rhs )
{
  return  ( lhs.x < rhs.x ) && ( lhs.y < rhs.y );
}

/// \brief Greater than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT
inline bool operator >( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( rhs.x < lhs.x ) && ( rhs.y < lhs.y );
}

/// \brief Lesser than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT
inline bool operator <=( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( lhs.x <= rhs.x ) && ( lhs.y <= rhs.y );
}

/// \brief Greater than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
NOM_EXPORT
inline bool operator >=( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( rhs.x <= lhs.x ) && ( rhs.y <= lhs.y );
}

/// \brief Method overload of binary operator != (not equal)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Compare the inequality between two objects.
///
/// \returns True if left operand is not equal to the right operand.
template <typename T>
NOM_EXPORT
inline bool operator != ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return ! ( lhs == rhs );
}

/// \brief Method overload of binary operator + (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Addition of both objects.
template <typename T>
NOM_EXPORT
inline Point2<T> operator + ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x + rhs.x,
                      lhs.y + rhs.y
                    );
}

/// \brief Method overload of binary operator + (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Addition of both objects.
template <typename T>
NOM_EXPORT
inline Point2<T> operator +(int lhs, const Point2<T>& rhs)
{
  return Point2<T>(lhs + rhs.x, lhs + rhs.y);
}

/// \brief Method overload of binary operator + (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Addition of both objects.
template <typename T>
NOM_EXPORT
inline Point2<T> operator +(const Point2<T>& lhs, int rhs)
{
  return Point2<T>(lhs.x + rhs, lhs.y + rhs);
}

/// \brief Method overload of binary operator ++ (Addition by 1)
///
/// \param rhs Right operand.
///
/// \returns Addition of the right operand.
template <typename T>
NOM_EXPORT
inline Point2<T> operator ++ ( Point2<T>& rhs )
{
  return Point2<T>  ( ++rhs.x,
                      ++rhs.y
                    );
}

/// \brief Method overload of binary operator - (subtraction)
///
/// \param rhs Right operand.
///
/// \returns Opposite of the object.
template <typename T>
NOM_EXPORT
inline Point2<T> operator - ( const Point2<T>& rhs )
{
  return Point2<T>  ( -rhs.x,
                      -rhs.y
                    );
}

/// \brief Method overload of binary operator - (subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Subtraction of both objects.
template <typename T>
NOM_EXPORT
inline Point2<T> operator - ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x - rhs.x,
                      lhs.y - rhs.y
                    );
}

/// \brief Method overload of binary operator - (subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Subtraction of both objects.
template <typename T>
NOM_EXPORT
inline Point2<T> operator -(int lhs, const Point2<T>& rhs)
{
  return Point2<T>(lhs - rhs.x, lhs - rhs.y);
}

/// \brief Method overload of binary operator - (subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \returns Subtraction of both objects.
template <typename T>
NOM_EXPORT
inline Point2<T> operator -(const Point2<T>& lhs, int rhs)
{
  return Point2<T>(lhs.x - rhs, lhs.y - rhs);
}

/// \brief Method overload of binary operator -- (subtraction by 1)
///
/// \param rhs Right operand.
///
/// \returns Subtraction of the right operand.
template <typename T>
NOM_EXPORT
inline Point2<T> operator -- ( Point2<T>& rhs )
{
  return Point2<T>  ( --rhs.x,
                      --rhs.y
                    );
}

/// \brief Method overload of binary operator * (Multiplication)
///
/// \param rhs Left operand.
/// \param rhs Right operand.
///
/// \returns Multiplication of the right operand.
template <typename T>
NOM_EXPORT
inline Point2<T> operator * ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x * rhs.x,
                      lhs.y * rhs.y
                    );
}

/// \brief Method overload of binary operator * (Multiplication)
///
/// \param rhs Left operand.
/// \param rhs Right operand.
///
/// \returns Multiplication of the right operand.
template <typename T>
NOM_EXPORT
inline Point2<T> operator *(int lhs, const Point2<T>& rhs)
{
  return Point2<T>(lhs * rhs.x, lhs * rhs.y);
}

/// \brief Method overload of binary operator * (Multiplication)
///
/// \param rhs Left operand.
/// \param rhs Right operand.
///
/// \returns Multiplication of the right operand.
template <typename T>
NOM_EXPORT
inline Point2<T> operator *(const Point2<T>& lhs, int rhs)
{
  return Point2<T>(lhs.x * rhs, lhs.y * rhs);
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
NOM_EXPORT
inline Point2<T> operator / ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x / rhs.x,
                      lhs.y / rhs.y
                    );
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
NOM_EXPORT
inline Point2<T> operator /(int lhs, const Point2<T>& rhs)
{
  return Point2<T>(lhs / rhs.x, lhs / rhs.y);
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
NOM_EXPORT
inline Point2<T> operator /(const Point2<T>& lhs, int rhs)
{
  return Point2<T>(lhs.x / rhs, lhs.y / rhs);
}

/// \brief Method overload of binary operator += (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Addition of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand,
template <typename T>
NOM_EXPORT
inline Point2<T>& operator += ( Point2<T>& lhs, const Point2<T>& rhs )
{
  lhs.x += rhs.x;
  lhs.y += rhs.y;

  return lhs;
}

/// \brief Method overload of binary operator += (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Addition of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand,
template <typename T>
NOM_EXPORT
inline Point2<T>& operator +=(int lhs, const Point2<T>& rhs)
{
  lhs += rhs.x;
  lhs += rhs.y;

  return lhs;
}

/// \brief Method overload of binary operator += (Addition)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Addition of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand,
template <typename T>
NOM_EXPORT
inline Point2<T>& operator +=(Point2<T>& lhs, int rhs)
{
  lhs.x += rhs;
  lhs.y += rhs;

  return lhs;
}

/// \brief Method overload of binary operator -= (Subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Subtraction of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Point2<T>& operator -= ( Point2<T>& lhs, const Point2<T>& rhs )
{
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;

  return lhs;
}

/// \brief Method overload of binary operator -= (Subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Subtraction of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Point2<T>& operator -=(int lhs, const Point2<T>& rhs)
{
  lhs -= rhs.x;
  lhs -= rhs.y;

  return lhs;
}

/// \brief Method overload of binary operator -= (Subtraction)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Subtraction of both objects; result is assigned to the left
/// operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Point2<T>& operator -=(Point2<T>& lhs, int rhs)
{
  lhs.x -= rhs;
  lhs.y -= rhs;

  return lhs;
}

/// \brief Method overload of binary operator *= (Multiplication)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Multiplication of both objects; result is assigned to the
/// left operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Point2<T>& operator *= ( Point2<T>& lhs, const Point2<T>& rhs )
{
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;

  return lhs;
}

/// \brief Method overload of binary operator *= (Multiplication)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Multiplication of both objects; result is assigned to the
/// left operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Point2<T>& operator *=(int lhs, const Point2<T>& rhs)
{
  lhs *= rhs.x;
  lhs *= rhs.y;

  return lhs;
}

/// \brief Method overload of binary operator *= (Multiplication)
///
/// \param lhs Left operand.
/// \param rhs Right operand.
///
/// \remarks Multiplication of both objects; result is assigned to the
/// left operand.
///
/// \returns Reference to left operand.
template <typename T>
NOM_EXPORT
inline Point2<T>& operator *=(Point2<T>& lhs, int rhs)
{
  lhs.x *= rhs;
  lhs.y *= rhs;

  return lhs;
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
inline Point2<T>& operator /= ( Point2<T>& lhs, Point2<T>& rhs )
{
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;

  return lhs;
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
inline Point2<T>& operator /=(int lhs, Point2<T>& rhs)
{
  lhs /= rhs.x;
  lhs /= rhs.y;

  return lhs;
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
inline Point2<T>& operator /=(Point2<T>& lhs, int rhs)
{
  lhs.x /= rhs;
  lhs.y /= rhs;

  return lhs;
}

} // namespace nom
