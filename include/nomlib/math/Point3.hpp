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

#include "export.hpp"
#include "nomlib/config.hpp"

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string POINT3_DELIMITER = ", ";

/// \brief 3D Point class
template <typename T>
struct Point3
{
  /// Default constructor; initialize values to Point3<T>::null
  Point3() :
    x(-1),
    y(-1),
    z(-1)
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor
  ~Point3()
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Constructor variant for initializing x, y, z at construction
  Point3(T x, T y, T z)  :
    x(x),
    y(y),
    z(z)
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Copy constructor
  ///
  /// \remarks The explicit keyword here will result in compile-time errors
  /// in any instance that it finds incompatible casting occurring, such as if
  /// you try to down-cast a Point3<int> to a Point3<float>.
  template <typename U>
  explicit Point3<T>(const Point3<U>& copy)  :
    x( NOM_SCAST(T, copy.x) ),
    y( NOM_SCAST(T, copy.y) ),
    z( NOM_SCAST(T, copy.z) )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Obtain a reference of the object.
  inline const Point3<T>& get() const
  {
    return *this;
  }

  /// \brief Null value
  ///
  /// \remarks  Null value implementation depends on signed (negative) numbers.
  static const Point3 null;

  /// \brief Zero value constant.
  static const Point3 zero;

  T x;
  T y;
  T z;
};

/// Point3 object defined using signed integers
typedef Point3<int> Point3i;

/// Point3 object defined using floating-point numbers
typedef Point3<real32> Point3f;

/// Point3 object defined using double precision floating-point numbers
typedef Point3<real64> Point3d;

#if 0
/// Null value for a nom::Point3 using signed integers
template <> NOM_EXPORT const Point3i Point3i::null;

/// Null value for a nom::Point3 using floating point numbers
template <> NOM_EXPORT const Point3f Point3f::null;

/// Null value for a nom::Point3 using double precision floating point numbers
template <> NOM_EXPORT const Point3d Point3d::null;

/// \brief Zero value for a nom::Point3 using signed integers
template <> NOM_EXPORT const Point3i Point3i::zero;

/// \brief Zero value for a nom::Point3 using 32-bit floating-point numbers.
template <> NOM_EXPORT const Point3f Point3f::zero;

/// \brief Zero value for a nom::Point3 using double precision (64-bit)
/// floating-point numbers.
template <> NOM_EXPORT const Point3d Point3d::zero;
#endif

} // namespace nom

#include "Point3.inl"

#endif // include guard defined
