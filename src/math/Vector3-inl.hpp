/******************************************************************************

    Vector3 template class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_VECTOR3_HEADERS
#define NOMLIB_VECTOR3_HEADERS

#include "config.hpp"

namespace nom {

template <typename T>
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
typedef Vector3<nom::int32> Vector3i;
typedef Vector3<nom::uint32> Vector3u;
typedef Vector3<float> Vector3f;

} // namespace nom

#endif // NOMLIB_VECTOR3_HEADERS defined
