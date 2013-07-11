/******************************************************************************

    Vector4 template class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_VECTOR4_HEADERS
#define NOMLIB_VECTOR4_HEADERS

#include "config.hpp"

namespace nom {

template <typename T>
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
typedef Vector4<nom::int32> Vector4i;
typedef Vector4<nom::uint32> Vector4u;
typedef Vector4<float> Vector4f;

} // namespace nom

#endif // NOMLIB_VECTOR4_HEADERS defined
