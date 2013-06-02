/******************************************************************************
    GCoords.cpp

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "GCoords.h"

GCoords::GCoords ( void )
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

GCoords::GCoords ( signed int x_, signed int y_, signed int width_, signed height_ )
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

GCoords::~GCoords ( void )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "GCoords::~GCoords (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Cleanup
}

void GCoords::setCoords ( signed int x_, signed int y_,
                          signed int width_, signed int height_ )
{
  this->x = x_;
  this->y = y_;
  this->width = width_;
  this->height = height_;
}

// Compatibility with SDL_BlitSurface
SDL_Rect GCoords::getSDL_Rect ( void ) const
{
  SDL_Rect coords;

  coords.x = this->x;
  coords.y = this->y;
  coords.w = this->width;
  coords.h = this->height;

  return coords;
}

// GCoords return?
std::pair<signed int, signed int> GCoords::getXY ( void )
{
  return std::make_pair ( this->getX(), this->getY() );
}

signed int GCoords::getX ( void )
{
  return this->x;
}

signed int GCoords::getY ( void )
{
  return this->y;
}

void GCoords::setX ( signed int x_ )
{
  this->x = x_;
}

void GCoords::setY ( signed int y_ )
{
  this->y = y_;
}

void GCoords::setXY ( signed int x_, signed int y_ )
{
  this->x = x_;
  this->y = y_;
}

signed int GCoords::getWidth ( void )
{
  return this->width;
}

signed int GCoords::getHeight ( void )
{
  return this->height;
}

void GCoords::setWidth ( signed int width_ )
{
  this->width = width_;
}

void GCoords::setHeight ( signed int height_ )
{
  this->height = height_;
}

void GCoords::setDimensions ( signed int width_, signed int height_ )
{
  this->width = width_;
  this->height = height_;
}

void GCoords::updateXY ( signed int x_, signed int y_ )
{
  this->x += x_;
  this->y += y_;
}

void GCoords::updateCoords ( signed int x_, signed int y_, signed int width_, signed int height_ )
{
  this->x += x_;
  this->y += y_;
  this->width += width_;
  this->height += height_;
}
