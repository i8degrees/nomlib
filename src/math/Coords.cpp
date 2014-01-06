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

const Coords Coords::null ( -1, -1, -1, -1 );

Coords::Coords ( void ) : x ( 0 ), y ( 0 ), w ( 0 ), h ( 0 ) {}

Coords::~Coords ( void ) {}

Coords::Coords ( int32 x, int32 y, int32 w, int32 h ) : x ( x ),
                                                        y ( y ),
                                                        w ( w ),
                                                        h ( h )
{}

Coords::Coords ( int32 x, int32 y ) : x ( x ), y ( y ),
                                      w ( 0 ), h ( 0 )
{}

Coords::Coords ( const Coords& coords ) : x ( coords.x ), y ( coords.y ),
                                          w ( coords.w ),
                                          h ( coords.h )
{}

Coords Coords::getPosition ( void ) const
{
  return Coords ( this->x, this->y );
}

Coords Coords::getSize ( void ) const
{
  return Coords ( this->w, this->h );
}

void Coords::setPosition ( int32 x, int32 y )
{
  this->x = x;
  this->y = y;
}

void Coords::setSize ( int32 width, int32 height )
{
  this->w = width;
  this->h = height;
}

bool Coords::contains ( int32 x, int32 y ) const
{
  return  (
            ( x >= this->x && x <= this->x + this->w )    &&
            ( y >= this->y && y <= this->y + this->h )
          );
}

bool Coords::contains ( const Coords& pos ) const
{
  return this->contains ( pos.x, pos.y );
}

bool Coords::intersects ( Coords& rectangle ) const
{
  uint32 leftA, leftB = 0;
  uint32 rightA, rightB = 0;
  uint32 topA, topB = 0;
  uint32 bottomA, bottomB = 0;

  // Calculate sides of RectA
  leftA = this->x;
  rightA = this->x + this->w;
  topA = this->y;
  bottomA = this->y + this->h;

  // Calculate sides of RectB
  leftB = rectangle.x;
  rightB = rectangle.x + rectangle.w;
  topB = rectangle.y;
  bottomB = rectangle.y + rectangle.h;

  if ( bottomA <= topB ) return false;
  if ( topA >= bottomB ) return false;
  if ( rightA <= leftB ) return false;
  if ( leftA >= rightB ) return false;

  return true; // we've got a collision!
}

bool Coords::isNull ( void ) const
{
  if ( *this == Coords::null )
  {
    return true;
  }

  return false;
}

const Coords& Coords::get ( void ) const
{
  return *this;
}

std::ostream& operator << ( std::ostream& os, const Coords& coords )
{
  os << coords.x << coords_delimiter
  << coords.y << coords_delimiter
  << coords.w << coords_delimiter
  << coords.h;
  return os;
}

Coords& Coords::operator = ( const Coords& other )
{
  this->x = other.x;
  this->y = other.y;
  this->w = other.w;
  this->h = other.h;

  return *this;
}

bool operator == ( const Coords& left, const Coords& right )
{
  return (left.x == right.x ) &&
         (left.y == right.y ) &&
         (left.w == right.w ) &&
         (left.h == right.h );
}

bool operator != ( const Coords& left, const Coords& right )
{
  return ! ( left == right );
}

Coords operator + ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32> ( left.x + right.x ),
                    static_cast<int32> ( left.y + right.y ),
                    static_cast<int32> ( left.w + right.w ),
                    static_cast<int32> ( left.h + right.h )
                  );
}

Coords operator ++ ( Coords& left )
{
  return Coords  (  static_cast<int32> ( left.x ++ ),
                    static_cast<int32> ( left.y ++ ),
                    static_cast<int32> ( left.w ++ ),
                    static_cast<int32> ( left.h ++ )
                  );
}

Coords operator - ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32> ( left.x - right.x ),
                    static_cast<int32> ( left.y - right.y ),
                    static_cast<int32> ( left.w - right.w ),
                    static_cast<int32> ( left.h - right.h )
                  );
}

Coords operator -- ( Coords& left )
{
  return Coords  (  static_cast<int32> ( left.x -- ),
                    static_cast<int32> ( left.y -- ),
                    static_cast<int32> ( left.w -- ),
                    static_cast<int32> ( left.h -- )
                  );
}

Coords operator * ( const Coords& left, const Coords& right )
{
  return Coords  ( static_cast<int32> ( left.x * right.x ),
                        static_cast<int32> ( left.y * right.y ),
                        static_cast<int32> ( left.w * right.w ),
                        static_cast<int32> ( left.h * right.h )
                      );
}

Coords operator / ( const Coords& left, const Coords& right )
{
  return Coords  (  static_cast<int32> ( left.x / right.x ),
                    static_cast<int32> ( left.y / right.y ),
                    static_cast<int32> ( left.w / right.w ),
                    static_cast<int32> ( left.h / right.h )
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
