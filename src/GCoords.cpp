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

  this->coords = std::make_pair ( -1, -1 );
  this->dims = std::make_pair ( -1, -1 );
}

GCoords::GCoords ( signed int x, signed int y, signed int width, signed height )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "GCoords::GCoords (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->coords = std::make_pair ( x, y );
  this->dims = std::make_pair ( width, height );
}

GCoords::~GCoords ( void )
{
  #ifdef DEBUG_COORDS_OBJ
    std::cout << "GCoords::~GCoords (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif
}

void GCoords::setCoords ( signed int x, signed int y,
                          signed int width, signed int height )
{
  this->coords = std::make_pair ( x, y );
  this->dims = std::make_pair ( width, height );
}

signed int GCoords::getX ( void )
{
  return std::get<0>(coords);
}

signed int GCoords::getY ( void )
{
  return std::get<1>(coords);
}

std::pair<signed int, signed int> GCoords::getXY ( void )
{
  return this->coords;
}

void GCoords::setXY ( signed int x, signed int y )
{
  this->coords = std::make_pair ( x, y );
}

signed int GCoords::getWidth ( void )
{
  return std::get<0>(dims);
}

// Alias for getWidth()
signed int GCoords::getW ( void )
{
  return this->getWidth();
}

signed int GCoords::getHeight ( void )
{
  return std::get<1>(dims);
}

// Alias for getHeight()
signed int GCoords::getH ( void )
{
  return this->getHeight();
}

std::pair<signed int, signed int> GCoords::getDimensions ( void )
{
  return this->dims;
}

void GCoords::setDimensions ( signed int width, signed int height )
{
  this->dims = std::make_pair ( width, height );
}
