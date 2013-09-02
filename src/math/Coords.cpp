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
#include "nomlib/math/Coords.hpp"

namespace nom {

Coords::Coords ( void ) : x ( 0 ), y ( 0 ), z ( 0 ), width ( 0 ), height ( 0 )
{}

Coords::Coords ( int32 x, int32 y, int32 w, int32 h ) : x ( x ),
                                                        y ( y ),
                                                        z ( 0 ),
                                                        width ( w ),
                                                        height ( h )
{}

Coords::Coords ( int32 x, int32 y ) : x ( x ),
                                      y ( y ),
                                      z ( 0 ),
                                      width ( 0 ),
                                      height ( 0 )
{}

Coords::Coords ( const Coords& coords ) : x ( coords.x ), y ( coords.y ),
                                          z ( coords.z ),
                                          width ( coords.width ),
                                          height ( coords.height )
{}

Coords::~Coords ( void ) {}

Coords Coords::getPosition ( void ) const
{
  return Coords ( this->x, this->y );
}

Coords Coords::getSize ( void ) const
{
  return Coords ( this->width, this->height );
}

void Coords::setPosition ( int32_t x, int32_t y )
{
  this->x = x;
  this->y = y;
}

void Coords::setSize ( int32_t width, int32_t height )
{
  this->width = width;
  this->height = height;
}

bool Coords::contains ( int32_t x, int32_t y ) const
{
  int32_t X = this->x + this->width;
  int32_t Y = this->y + this->height;

  return ( x >= X ) && ( y >= Y );
}

bool Coords::contains ( const Coords& pos ) const
{
  return this->contains ( pos.x, pos.y );
}

bool Coords::intersects ( Coords& rectangle ) const
{
  unsigned int leftA, leftB = 0;
  unsigned int rightA, rightB = 0;
  unsigned int topA, topB = 0;
  unsigned int bottomA, bottomB = 0;

  // Calculate sides of RectA
  leftA = this->x;
  rightA = this->x + this->width;
  topA = this->y;
  bottomA = this->y + this->height;

  // Calculate sides of RectB
  leftB = rectangle.x;
  rightB = rectangle.x + rectangle.width;
  topB = rectangle.y;
  bottomB = rectangle.y + rectangle.height;

  if ( bottomA <= topB )
    return false;
  if ( topA >= bottomB )
    return false;
  if ( rightA <= leftB )
    return false;
  if ( leftA >= rightB )
    return false;

  return true; // we've got a collision!
}

std::ostream& operator << ( std::ostream& os, const Coords& coords )
{
  os << coords.x << coords_delimiter
  << coords.y << coords_delimiter
  << coords.width << coords_delimiter
  << coords.height;
  return os;
}

Coords& Coords::operator = ( const Coords& other )
{
  this->x = other.x;
  this->y = other.y;
  this->z = other.z;
  this->width = other.width;
  this->height = other.height;

  return *this;
}

bool operator == ( const Coords& left, const Coords& right )
{
  return (left.x == right.x ) &&
         (left.y == right.y ) &&
         (left.width == right.width ) &&
         (left.height == right.height );
}

bool operator != ( const Coords& left, const Coords& right )
{
  return ! ( left == right );
}

Coords operator + ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32_t> ( left.x + right.x ),
                    static_cast<int32_t> ( left.y + right.y ),
                    static_cast<int32_t> ( left.width + right.width ),
                    static_cast<int32_t> ( left.height + right.height )
                  );
}

Coords operator ++ ( Coords& left )
{
  return Coords  (  static_cast<int32_t> ( left.x ++ ),
                    static_cast<int32_t> ( left.y ++ ),
                    static_cast<int32_t> ( left.width ++ ),
                    static_cast<int32_t> ( left.height ++ )
                  );
}

Coords operator - ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32_t> ( left.x - right.x ),
                    static_cast<int32_t> ( left.y - right.y ),
                    static_cast<int32_t> ( left.width - right.width ),
                    static_cast<int32_t> ( left.height - right.height )
                  );
}

Coords operator -- ( Coords& left )
{
  return Coords  (  static_cast<int32_t> ( left.x -- ),
                    static_cast<int32_t> ( left.y -- ),
                    static_cast<int32_t> ( left.width -- ),
                    static_cast<int32_t> ( left.height -- )
                  );
}

Coords operator * ( const Coords& left, const Coords& right )
{
  return Coords  ( static_cast<int32_t> ( left.x * right.x ),
                        static_cast<int32_t> ( left.y * right.y ),
                        static_cast<int32_t> ( left.width * right.width ),
                        static_cast<int32_t> ( left.height * right.height )
                      );
}

Coords operator / ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32_t> ( left.x / right.x ),
                    static_cast<int32_t> ( left.y / right.y ),
                    static_cast<int32_t> ( left.width / right.width ),
                    static_cast<int32_t> ( left.height / right.height )
                  );
}

Coords& operator += ( Coords& left, const Coords& right )
{
  return left = left + right;
}

Coords& operator -= ( Coords& left, const Coords& right )
{
  return left = left - right;
}

Coords& operator *= ( Coords& left, const Coords& right )
{
  return left = left * right;
}


} // namespace nom
