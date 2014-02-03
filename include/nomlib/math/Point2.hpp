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
#ifndef NOMLIB_MATH_POINT2_HEADERS
#define NOMLIB_MATH_POINT2_HEADERS

#include <iostream>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string POINT_DELIMITER = ", ";

/// \brief 2D point container
template <typename T>
struct Point2
{
  /// Default constructor; initialize values to Point2<T>::null
  Point2 ( void ) :
    x ( -1 ),
    y ( -1 )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor
  ~Point2 ( void )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Constructor variant for initializing x, y at construction
  Point2 ( T x, T y ) :
    x ( x ),
    y ( y )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Copy constructor
  ///
  /// \remarks The explicit keyword here will result in compile-time errors
  /// in any instance that it finds incompatible casting occurring, such as if
  /// you try to down-cast a Point2<int> to a Point2<float>.
  template <typename U>
  explicit Point2 ( const Point2<U>& copy ) :
    x { static_cast<T> ( copy.x ) },
    y { static_cast<T> ( copy.y ) }
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Obtain a reference of the object.
  inline const Point2<T>& get ( void ) const
  {
    return *this;
  }

  /// \brief Null value
  ///
  /// \remarks  Null value implementation depends on signed (negative) numbers.
  static const Point2 null;

  /// Represents the X-axis coordinate point
  T x;

  /// Represents the Y-axis coordinate point
  T y;
};

/// Pretty print a Point2 object using the following formatting:
///
///     <Point2.x>, <Point2.y>
///
/// An example print:
///
///     128, 144
template <typename T>
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
inline bool operator == ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( lhs.x == rhs.x )  &&  ( lhs.y == rhs.y );
}

/// \brief Lesser than comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
inline bool operator <( const Point2<T> lhs, const Point2<T>& rhs )
{
  return  ( lhs.x < rhs.x ) && ( lhs.y < rhs.y );
}

/// \brief Greater than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
inline bool operator >( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( rhs.x < lhs.x ) && ( rhs.y < lhs.y );
}

/// \brief Lesser than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
inline bool operator <=( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( lhs.x <= rhs.x ) && ( lhs.y <= rhs.y );
}

/// \brief Greater than or equal to comparison operator.
///
/// \param lhs Left operand.
/// \param rhs Right operand.
template <typename T>
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
inline Point2<T> operator + ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x + rhs.x,
                      lhs.y + rhs.y
                    );
}

/// \brief Method overload of binary operator ++ (Addition by 1)
///
/// \param rhs Right operand.
///
/// \returns Addition of the right operand.
template <typename T>
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
inline Point2<T> operator - ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x - rhs.x,
                      lhs.y - rhs.y
                    );
}

/// \brief Method overload of binary operator -- (subtraction by 1)
///
/// \param rhs Right operand.
///
/// \returns Subtraction of the right operand.
template <typename T>
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
inline Point2<T> operator * ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x * rhs.x,
                      lhs.y * rhs.y
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
inline Point2<T> operator / ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return Point2<T>  ( lhs.x / rhs.x,
                      lhs.y / rhs.y
                    );
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
inline Point2<T>& operator += ( Point2<T>& lhs, const Point2<T>& rhs )
{
  lhs.x += rhs.x;
  lhs.y += rhs.y;

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
inline Point2<T>& operator -= ( Point2<T>& lhs, const Point2<T>& rhs )
{
  lhs.x -= rhs.x;
  lhs.y -= rhs.y;

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
///
/// \todo See tests/math.cpp at Point2 Unit Test 2, Result[1]
template <typename T>
inline Point2<T>& operator *= ( Point2<T>& lhs, const Point2<T>& rhs )
{
  lhs.x *= rhs.x;
  lhs.y *= rhs.y;

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
inline Point2<T>& operator /= ( Point2<T>& lhs, Point2<T>& rhs )
{
  lhs.x /= rhs.x;
  lhs.y /= rhs.y;

  return lhs;
}

/// Point2 object defined using signed integers
typedef Point2<int> Point2i;

/// Point2 object defined using floating-point numbers
typedef Point2<float> Point2f;

/// Point2 object defined using double precision floating-point numbers
typedef Point2<double> Point2d;

} // namespace nom

#endif // include guard defined
