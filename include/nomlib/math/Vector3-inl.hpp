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
#ifndef NOMLIB_VECTOR3_HEADERS
#define NOMLIB_VECTOR3_HEADERS

#include "nomlib/config.hpp"

namespace nom {

template <typename T>
/// \brief Vector3 template class
class Vector3
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Vector3 ( void )
    {
      this->x = 0;
      this->y = 0;
      this->z = 0;
    }

    /// Constructor variant for initializing x, y, z at construction
    Vector3 ( T x, T y, T z )
    {
      this->x = x;
      this->y = y;
      this->z = z;
    }

    /// Copy constructor
    template <typename U>
    Vector3<T> ( const Vector3<U>& copy )
    {
      this->x = static_cast<T> ( copy.x );
      this->y = static_cast<T> ( copy.y );
      this->z = static_cast<T> ( copy.z );
    }

    /// Copy assignment constructor
    Vector3& operator = ( const T& other )
    {
      this->x = other.x;
      this->y = other.y;
      this->z = other.z;
    }

    /// Do nothing destructor
    ~Vector3 ( void ) {}

    T x;
    T y;
    T z;
};

// Define the most common types
typedef Vector3<int32> Vector3i;
typedef Vector3<uint32> Vector3u;
typedef Vector3<float> Vector3f;


} // namespace nom

#endif // NOMLIB_VECTOR3_HEADERS defined
