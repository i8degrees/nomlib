/******************************************************************************
    GCoords.cpp

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "GCoords.h"

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

// Compatibility with SDL_BlitSurface
SDL_Rect nom::Coords::getSDL_Rect ( void ) const
{
  SDL_Rect coords;

  coords.x = this->x;
  coords.y = this->y;
  coords.w = this->width;
  coords.h = this->height;

  return coords;
}

nom::Coords nom::Coords::get ( void ) const
{
  return *this;
}

signed int nom::Coords::getX ( void )
{
  return this->x;
}

signed int nom::Coords::getY ( void )
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

signed int nom::Coords::getWidth ( void )
{
  return this->width;
}

signed int nom::Coords::getHeight ( void )
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
