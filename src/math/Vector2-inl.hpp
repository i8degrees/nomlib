/******************************************************************************

    Vector2 template class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#ifndef NOMLIB_VECTOR2_HEADERS
#define NOMLIB_VECTOR2_HEADERS

#include "config.hpp"

namespace nom {

template <typename T>
class Vector2
{
  public:
    /// Default constructor; sets all values to their respective defaults
    Vector2 ( void )
    {
      this->x = 0;
      this->y = 0;
    }

    /// Constructor variant for initializing x, y at construction
    Vector2 ( T x_, T y_ )  : x ( x_ ), y ( y_ ) {}

    /// Copy constructor
    template <typename U>
    Vector2<T> ( const Vector2<U>& copy )
    {
      this->x = static_cast<T> ( copy.x );
      this->y = static_cast<T> ( copy.y );
    }

    /// Copy assignment constructor
    Vector2& operator = ( const T& other )
    {
      this->x = other.x;
      this->y = other.y;
    }

    /// Do nothing destructor
    ~Vector2 ( void ) {}

    T x;
    T y;
};

// Define the most common types
typedef Vector2<nom::int32> Vector2i;
typedef Vector2<nom::uint32> Vector2u;
typedef Vector2<float> Vector2f;

} // namespace nom

#endif // NOMLIB_VECTOR2_HEADERS defined
