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
#ifndef NOMLIB_MATH_SIZE2_HEADERS
#define NOMLIB_MATH_SIZE2_HEADERS

#include <iostream>

#include "nomlib/config.hpp"

namespace nom {

/// \brief Delimiter character to use with << operator
const std::string SIZE2_DELIMITER = ", ";

/// \brief Size coordinates (width & height) container
template <typename T>
struct Size2
{
  /// Default constructor; initialize values to Size2<T>::null
  Size2 ( void ) :
    w ( -1 ),
    h ( -1 )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Destructor
  ~Size2 ( void )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// Constructor variant for initializing width & height at construction
  Size2 ( T w, T h ) :
    w ( w ),
    h ( h )
  {
    //NOM_LOG_TRACE(NOM);
  }

  /// \brief Copy constructor
  ///
  /// \remarks The explicit keyword here will result in compile-time errors
  /// in any instance that it finds incompatible casting occurring, such as if
  /// you try to down-cast a Size2<int> to a Size2<float>.
  template <typename U>
  explicit Size2 ( const Size2<U>& copy ) :
    w { static_cast<T> ( copy.w ) },
    h { static_cast<T> ( copy.h ) }
  {
    this->w = static_cast<T> ( copy.w );
    this->h = static_cast<T> ( copy.h );
  }

  /// \brief Null value
  ///
  /// \remarks  Null value implementation depends on signed (negative) numbers.
  static const Size2 null;

  /// Represents the width coordinate point
  T w;

  /// Represents the height coordinate point
  T h;
};

/// Pretty print a Size2 object using the following formatting:
///
///     <Size2.x>, <Size2.y>
///
/// An example print:
///
///     128, 144
template <typename T>
inline std::ostream& operator << ( std::ostream& os, const Size2<T>& pos )
{
  os
  << pos.w
  << SIZE2_DELIMITER
  << pos.h;

  return os;
}

template <typename T>
inline bool operator == ( const Size2<T>& lhs, const Size2<T>& rhs )
{
  return  ( lhs.w == rhs.w )  &&  ( lhs.h == rhs.h );
}

template <typename T>
inline bool operator != ( const Size2<T>& lhs, const Size2<T>& rhs )
{
  return ! ( lhs == rhs );
}

/// Size2 object defined using signed integers
typedef Size2<int> Size2i;

/// Size2 object defined using floating point numbers
typedef Size2<float> Size2f;

/// Size2 object defined using double-precision floating point numbers
typedef Size2<double> Size2d;

} // namespace nom

#endif // include guard defined
