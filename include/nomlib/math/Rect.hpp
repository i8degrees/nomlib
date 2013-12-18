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
#ifndef NOMLIB_MATH_RECT_HEADERS
#define NOMLIB_MATH_RECT_HEADERS

#include "nomlib/config.hpp"
#include "nomlib/math/Point2.hpp"

namespace nom {

/// Delimiter character to use with << operator
const std::string RECT_DELIMITER = ", ";

/// \brief Rectangle class container
template<typename T>
class Rect
{
  public:
    /// Default constructor.
    Rect ( void ) :
      left ( 0 ), top ( 0 ), width ( 0 ), height ( 0 ) {}

    /// Destructor.
    ~Rect ( void ) {}

    /// Construct a Rectangle from coordinates
    Rect ( T left, T top, T w, T h ) :
      left ( left ), top ( top ), width ( w ), height ( h ) {}

    /// Construct an object from two Point2 containers (position and size)
    Rect ( const Point2<T>& pos, const Point2<T>& size ) :
      left ( pos.x ), top ( pos.y ),
      width ( pos.x + size.w ),
      height ( pos.y + size.h ) {}

    /// Copy constructor
    template <typename U>
    Rect ( const Rect<U>& copy )
    {
      this->left = static_cast<T> ( copy.left );
      this->top = static_cast<T> ( copy.top );
      this->width = static_cast<T> ( copy.width );
      this->height = static_cast<T> ( copy.height );
    }

    /// Copy assignment operator
    inline Rect<T>& operator = ( const Rect<T>& other )
    {
      this->left = other.left;
      this->top = other.top;
      this->width = other.width;
      this->height = other.height;

      return *this;
    }

    /// Check to see if input coordinates X and Y are within the bounds
    /// of this object (think: colliding)
    bool contains ( int x, int y ) const
    {
      return  (
                ( x >= this->left && x <= this->left + this->width ) &&
                ( y >= this->top && y <= this->top + this->height )
              );
    }

    /// Check to see if an input Point2 container are within the bounds
    /// of this object (think: colliding)
    bool contains ( const Point2<T>& pos ) const
    {
      return this->contains ( pos.x, pos.y );
    }

    /// Checks to see if our rectangle overlaps with another
    bool intersects ( const Rect<T>& rectangle ) const
    {
      uint leftA, leftB = 0;
      uint rightA, rightB = 0;
      uint topA, topB = 0;
      uint bottomA, bottomB = 0;

      // Calculate sides of RectA
      leftA = this->left;
      rightA = leftA + this->width;
      topA = this->top;
      bottomA = topA + this->height;

      // Calculate sides of RectB
      leftB = rectangle.left;
      rightB = leftB + rectangle.width;
      topB = rectangle.top;
      bottomB = topB + rectangle.height;

      if ( bottomA <= topB ) return false; // No collision
      if ( topA >= bottomB ) return false; // No collision
      if ( rightA <= leftB ) return false; // No collision
      if ( leftA >= rightB ) return false; // No collision

      return true; // Collision!
    }

  public:
    /// Left coordinate of the rectangle (X coordinate of Rectangle)
    T left;
    /// Top coordinate of the rectangle (Y coordinate of Rectangle)
    T top;
    /// Width of the rectangle (right side of Rectangle)
    T width;
    /// Height of the rectangle (bottom side of Rectangle)
    T height;

    /// Reference alias for left
    T& x = left;
    /// Reference alias for top
    T& y = top;
    /// Reference alias for width
    T& w = width;
    /// Reference alias for height
    T& h = height;
};

/// Pretty print the Rectangle using the following format string:
///
/// <Rect.left>, <Rect.top>, <Rect.width>, <Rect.height>
///
/// \note This is a convenience for us, so that we are able to quickly debug
/// an object:
///
/// \code
///
/// NOM_DUMP_VAR ( my_rect );
///
/// # ...or...
///
/// std::cout << my_rect << std::endl;
/// \endcode
template <typename T>
inline std::ostream& operator << ( std::ostream& os, const Rect<T>& rect )
{
  os
  << rect.left
  << RECT_DELIMITER
  << rect.top
  << RECT_DELIMITER
  << rect.width
  << RECT_DELIMITER
  << rect.height;
  return os;
}

template <typename T>
inline bool operator == ( const Rect<T>& left, const Rect<T>& right )
{
    return ( left.left == right.left ) && ( left.width == right.width ) &&
           ( left.top == right.top ) && ( left.height == right.height );
}


////////////////////////////////////////////////////////////
template <typename T>
inline bool operator != ( const Rect<T>& left, const Rect<T>& right )
{
    return ! ( left == right );
}

/// Construct a signed integer Rectangle
typedef Rect<int> IntRect;

/// Construct a floating-point number Rectangle
typedef Rect<float> FloatRect;

} // namespace nom

#endif // include guard defined
