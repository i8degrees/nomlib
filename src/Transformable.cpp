/******************************************************************************
    Transformable.cpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Transformable.hpp"

nom::Transformable::Transformable ( void )  : coords ( 0, 0, 0, 0 ),
                                              color ( 0, 0, 0, SDL_ALPHA_OPAQUE )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::Transformable (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

nom::Transformable::~Transformable ( void )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::~Transformable (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Cleanup vars
}

int32_t nom::Transformable::getX ( void ) const
{
  return this->coords.getX();
}

int32_t nom::Transformable::getY ( void ) const
{
  return this->coords.getY();
}

const nom::Coords& nom::Transformable::getXY ( void ) const
{
  return this->coords;
}

void nom::Transformable::setX ( int32_t x )
{
  this->coords.setX ( x );
}

void nom::Transformable::setY ( int32_t y )
{
  this->coords.setY ( y );
}

// Phase out? new method get/setPosition
void nom::Transformable::setXY ( int32_t x, int32_t y )
{
  this->coords.setXY ( x, y );
}

const nom::Coords& nom::Transformable::getPosition ( void ) const
{
  return this->coords;
}

const nom::Coords nom::Transformable::getSize ( void ) const
{
  return nom::Coords ( this->coords.getWidth(), this->coords.getHeight() );
}

// Variant #1 setter for position coordinates
void nom::Transformable::setPosition ( int32_t x, int32_t y )
{
  this->coords.setXY ( x, y );
}

// Variant #2 setter for position coordinates
void nom::Transformable::setPosition ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  this->coords.setCoords ( x, y, width, height );
}

// Variant #3 setter for position coordinates; do we have enough options yet???
void nom::Transformable::setPosition ( const nom::Coords& coords )
{
  this->coords = coords;
}

void nom::Transformable::setSize ( int32_t width, int32_t height )
{
  this->coords.setDimensions ( width, height );
}

int32_t nom::Transformable::getWidth ( void ) const
{
  return this->coords.getWidth();
}

void nom::Transformable::setWidth ( int32_t width, int32_t height )
{
  this->coords.setWidth ( width );
}

int32_t nom::Transformable::getHeight ( void ) const
{
  return this->coords.getHeight();
}

void nom::Transformable::setHeight ( int32_t height )
{
  this->coords.setHeight ( height );
}

void nom::Transformable::updateXY ( int32_t x, int32_t y )
{
  int32_t x_offset = this->coords.getX();
  int32_t y_offset = this->coords.getY();

  x_offset += x;
  y_offset += y;
  this->coords.setXY ( x_offset, y_offset );
}

const nom::Color& nom::Transformable::getColor ( void ) const
{
  return this->color;
}

void nom::Transformable::setColor ( const nom::Color& color_ )
{
  this->color = color_;
}

