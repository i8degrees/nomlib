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
#ifndef NOMLIB_MATH_POINT3_HEADERS
#define NOMLIB_MATH_POINT3_HEADERS

#include "nomlib/config.hpp"

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string POINT3_DELIMITER = ", ";

/// \brief 3D Point class
template <typename T>
struct Point3
{
  /// Default constructor; initialize values to Point3<T>::null
  Point3 ( void ) :
    x ( -1 ),
    y ( -1 ),
    z ( -1 )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor
  ~Point3 ( void )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Constructor variant for initializing x, y, z at construction
  Point3 ( T x, T y, T z )  :
    x ( x ),
    y ( y ),
    z ( z )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Copy constructor
  template <typename U>
  explicit Point3<T> ( const Point3<U>& copy )  :
    x { static_cast<T> ( copy.x ) },
    y { static_cast<T> ( copy.y ) },
    z { static_cast<T> ( copy.z ) }
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Null value
  ///
  /// \remarks  Null value implementation depends on signed (negative) numbers.
  static const Point3 null;

  T x;
  T y;
  T z;
};

/// Pretty print a Point3 object using the following formatting:
///
///     <Point3.x>, <Point3.y>, <Point3.z>
///
/// An example print:
///
///     128, 144, 0
template <typename T>
inline std::ostream& operator << ( std::ostream& os, const Point3<T>& pos )
{
  os
  << pos.x
  << POINT3_DELIMITER
  << pos.y
  << POINT3_DELIMITER
  << pos.z;

  return os;
}

template <typename T>
inline bool operator == ( const Point3<T>& lhs, const Point3<T>& rhs )
{
  return  ( lhs.x == rhs.x )  &&  ( lhs.y == rhs.y )  && ( lhs.z == rhs.z );
}

template <typename T>
inline bool operator != ( const Point3<T>& lhs, const Point3<T>& rhs )
{
  return ! ( lhs == rhs );
}

/// Point3 object defined using signed integers
typedef Point3<int> Point3i;

/// Point3 object defined using floating-point numbers
typedef Point3<float> Point3f;

/// Point3 object defined using double precision floating-point numbers
typedef Point3<double> Point3d;

} // namespace nom

#endif // include guard defined
