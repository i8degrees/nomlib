/******************************************************************************

    Coordinate values (x, y, z, width, height ) wrapper

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Coords.hpp"

// Default constructor; sets all values to their respective defaults
nom::Coords::Coords ( void )  : x ( 0 ), y ( 0 ), z ( 0 ),
                                width ( 0 ), height ( 0 )
{
  // Nothing to initialize
}

// Constructor variant for setting coords by x, y, width, height values
nom::Coords::Coords ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

// Constructor variant for setting coords with an exiting Coords object
// This is a "shallow" copy assignment
nom::Coords::Coords ( const nom::Coords& coords )
{
  this->x = coords.x;
  this->y = coords.y;
  this->width = coords.width;
  this->height = coords.height;
}

nom::Coords::~Coords ( void )
{
  // Nothing to clean up
}

nom::Coords nom::Coords::getPosition ( void ) const
{
  return nom::Coords ( this->x, this->y );
}

nom::Coords nom::Coords::getSize ( void ) const
{
  return nom::Coords ( this->width, this->height );
}

void nom::Coords::setPosition ( int32_t x, int32_t y )
{
  this->x = x;
  this->y = y;
}

void nom::Coords::setSize ( int32_t width, int32_t height )
{
  this->width = width;
  this->height = height;
}

// TODO: test me
bool nom::Coords::contains ( int32_t x, int32_t y ) const
{
  int32_t X = this->x + this->width;
  int32_t Y = this->y + this->height;

  return ( x >= X ) && ( y >= Y );
}

// TODO: test me
bool nom::Coords::contains ( const nom::Coords& pos ) const
{
  return this->contains ( pos.x, pos.y );
}

// TODO: test me
// Checks to see if our rectangle overlaps with another
bool nom::Coords::intersects ( nom::Coords& rectangle ) const
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

/// SDL compatibility wrapper primarily for SDL_BlitSurface
SDL_Rect nom::Coords::getSDL_Rect ( void ) const
{
  SDL_Rect coords;

  coords.x = this->x;
  coords.y = this->y;
  coords.w = this->width;
  coords.h = this->height;

  return coords;
}
