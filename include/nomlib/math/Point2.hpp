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
#ifndef NOMLIB_POINT2D_HEADERS
#define NOMLIB_POINT2D_HEADERS

#include <iostream>

#include "nomlib/config.hpp"

namespace nom {

const std::string point_delimiter = ", ";

/// \brief 2D point container
template <typename T>
class Point2
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Point2 ( void ) : x ( 0 ), y ( 0 ) {}

    /// Destructor
    ~Point2 ( void ) {}

    /// Constructor variant for initializing x, y at construction
    Point2 ( T x, T y ) : x ( x ), y ( y ) {}

    /// Copy constructor
    template <typename U>
    Point2 ( const Point2<U>& copy )
    {
      this->x = static_cast<T> ( copy.x );
      this->y = static_cast<T> ( copy.y );
    }

    /// Copy assignment operator
    template <typename U>
    Point2<T>& operator = ( const Point2<U>& other )
    {
      this->x = static_cast<T> ( other.x );
      this->y = static_cast<T> ( other.y );

      return *this;
    }

  public:
    T x;
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
std::ostream& operator << ( std::ostream& os, const Point2<T>& coords )
{
  os  << coords.x << point_delimiter
      << coords.y;
  return os;
}

/// Point2D object defined with 32-bit signed integers
typedef Point2<int32> Point2i;

/// Point2D object defined with 32-bit unsigned integers
typedef Point2<uint32> Point2u;

/// Point2D object defined with floating-point integers.
typedef Point2<float> Point2f;

/// Point2D object defined with double precision floating-point integers.
typedef Point2<double> Point2d;

} // namespace nom

#endif // include guard defined
