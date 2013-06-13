/******************************************************************************
    Coords.cpp

  Coordinate values (x, y, z, width, height ) wrapper

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Coords.hpp"

// Default constructor; sets all values to their respective defaults
nom::Coords::Coords ( void )  : x ( 0 ), y ( 0 ), z ( 0 ),
                                width ( 0 ), height ( 0 )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "Coords::Coords (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  // Nothing else to do
}

// Constructor variant for setting coords by x, y, width, height values
nom::Coords::Coords ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "Coords::Coords (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->x = x;
  this->y = y;
  this->width = width;
  this->height = height;
}

// Constructor variant for setting coords with an exiting Coords object
// This is a "shallow" copy assignment
nom::Coords::Coords ( const nom::Coords& coords )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "Coords::Coords (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->x = coords.x;
  this->y = coords.y;
  this->width = coords.width;
  this->height = coords.height;
}

nom::Coords::~Coords ( void )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "Coords::~Coords (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Nothing to clean up!
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
