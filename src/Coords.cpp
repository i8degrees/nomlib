/******************************************************************************
    Coords.cpp

  Coordinate values (x, y, z, width, height ) wrapper

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Coords.h"

nom::Coords::Coords ( void )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "GCoords::GCoords (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->x = 0;
  this->y = 0;
  this->z = 0;
  this->width = 0;
  this->height = 0;
}

nom::Coords::Coords ( signed int x_, signed int y_, signed int width_, signed height_ )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "GCoords::GCoords (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->x = x_;
  this->y = y_;
  this->z = 0;
  this->width = width_;
  this->height = height_;
}

nom::Coords::~Coords ( void )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "GCoords::~GCoords (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Cleanup
}

void nom::Coords::setCoords ( signed int x_, signed int y_,
                          signed int width_, signed int height_ )
{
  this->x = x_;
  this->y = y_;
  this->width = width_;
  this->height = height_;
}

signed int nom::Coords::getX ( void ) const
{
  return this->x;
}

signed int nom::Coords::getY ( void ) const
{
  return this->y;
}

void nom::Coords::setX ( signed int x_ )
{
  this->x = x_;
}

void nom::Coords::setY ( signed int y_ )
{
  this->y = y_;
}

void nom::Coords::setXY ( signed int x_, signed int y_ )
{
  this->x = x_;
  this->y = y_;
}

signed int nom::Coords::getWidth ( void ) const
{
  return this->width;
}

signed int nom::Coords::getHeight ( void ) const
{
  return this->height;
}

void nom::Coords::setWidth ( signed int width_ )
{
  this->width = width_;
}

void nom::Coords::setHeight ( signed int height_ )
{
  this->height = height_;
}

void nom::Coords::setDimensions ( signed int width_, signed int height_ )
{
  this->width = width_;
  this->height = height_;
}

void nom::Coords::updateXY ( signed int x_, signed int y_ )
{
  this->x += x_;
  this->y += y_;
}

void nom::Coords::updateCoords ( signed int x_, signed int y_, signed int width_, signed int height_ )
{
  this->x += x_;
  this->y += y_;
  this->width += width_;
  this->height += height_;
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
