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
#ifndef NOMLIB_VECTOR4_HEADERS
#define NOMLIB_VECTOR4_HEADERS

#include "nomlib/config.hpp"

namespace nom {

template <typename T>
/// \brief Vector4 template class
class Vector4
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Vector4 ( void )
    {
      this->x = x;
      this->y = y;
      this->z = z;
      this->t = t;
    }

    /// Constructor variant for initializing x, y, z, t at construction
    Vector4 ( T x, T y, T z, T t )
    {
      this->x = x;
      this->y = y;
      this->z = z;
      this->t = t;
    }

    /// Copy constructor
    template <typename U>
    Vector4<T> ( const Vector4<U>& copy )
    {
      this->x = static_cast<T> ( copy.x );
      this->y = static_cast<T> ( copy.y );
      this->z = static_cast<T> ( copy.z );
      this->t = static_cast<T> ( copy.t );
    }

    /// Copy assignment constructor
    Vector4& operator = ( const T& other )
    {
      this->x = other.x;
      this->y = other.y;
      this->z = other.z;
      this->t = other.t;
    }

    ~Vector4 ( void ) {}

    T x;
    T y;
    T z;
    T t;
};

// Define the most common types
typedef Vector4<int32> Vector4i;
typedef Vector4<uint32> Vector4u;
typedef Vector4<float> Vector4f;


} // namespace nom

#endif // NOMLIB_VECTOR4_HEADERS defined
