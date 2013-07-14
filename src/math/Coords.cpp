/******************************************************************************

    Coordinate values (x, y, z, width, height ) wrapper

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Coords.hpp"

namespace nom {

Coords::Coords ( void )  : x ( 0 ), y ( 0 ), z ( 0 ),
                                width ( 0 ), height ( 0 )
{
  // Nothing to initialize
}

Coords::Coords ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

Coords::Coords ( const Coords& coords )
{
  this->x = coords.x;
  this->y = coords.y;
  this->width = coords.width;
  this->height = coords.height;
}

Coords::~Coords ( void )
{
  // Nothing to clean up
}

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

Coords& Coords::operator = ( const Coords& other )
{
  this->x = other.x;
  this->y = other.y;
  this->z = other.z;
  this->width = other.width;
  this->height = other.height;

  return *this;
}


} // namespace nom
