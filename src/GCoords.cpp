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
}

GCoords::GCoords ( unsigned int x, unsigned int y, unsigned int width, unsigned height )
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

void GCoords::setCoords ( unsigned int x, unsigned int y,
                          unsigned int width, unsigned int height )
{
  this->coords = std::make_pair ( x, y );
  this->dims = std::make_pair ( width, height );
}

unsigned int GCoords::getX ( void )
{
  return std::get<0>(coords);
}

unsigned int GCoords::getY ( void )
{
  return std::get<1>(coords);
}

std::pair<int, int> GCoords::getXY ( void )
{
  return this->coords;
}

void GCoords::setXY ( unsigned int x, unsigned int y )
{
  this->coords = std::make_pair ( x, y );
}

unsigned int GCoords::getWidth ( void )
{
  return std::get<0>(dims);
}

// Alias for getWidth()
unsigned int GCoords::getW ( void )
{
  return this->getWidth();
}

unsigned int GCoords::getHeight ( void )
{
  return std::get<1>(dims);
}

// Alias for getHeight()
unsigned int GCoords::getH ( void )
{
  return this->getHeight();
}

std::pair<int, int> GCoords::getDimensions ( void )
{
  return this->dims;
}

void GCoords::setDimensions ( unsigned int width, unsigned int height )
{
  this->dims = std::make_pair ( width, height );
}
