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
    this->x = static_cast<T> ( copy.x );
    this->y = static_cast<T> ( copy.y );
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

template <typename T>
inline bool operator == ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return  ( lhs.x == rhs.x )  &&  ( lhs.y == rhs.y );
}

template <typename T>
inline bool operator != ( const Point2<T>& lhs, const Point2<T>& rhs )
{
  return ! ( lhs == rhs );
}

/// Point2 object defined using signed integers
typedef Point2<int> Point2i;

/// Point2 object defined using floating-point numbers
typedef Point2<float> Point2f;

/// Point2 object defined using double precision floating-point numbers
typedef Point2<double> Point2d;

} // namespace nom

#endif // include guard defined
