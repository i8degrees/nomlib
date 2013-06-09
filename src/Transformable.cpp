/******************************************************************************
    Transformable.cpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Transformable.hpp"

nom::Transformable::Transformable ( void )  : coords ( 0, 0, 0, 0 ),
                                              color ( 0, 0, 0, -1 )
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

const int32_t nom::Transformable::getX ( void ) const
{
  return this->coords.getX();
}

const int32_t nom::Transformable::getY ( void ) const
{
  return this->coords.getY();
}

const nom::Coords& nom::Transformable::getXY ( void ) const
{
  return this->coords;
}

void nom::Transformable::setX ( int32_t x_ )
{
  this->coords.setX ( x_ );
}

void nom::Transformable::setY ( int32_t y_ )
{
  this->coords.setY ( y_ );
}

void nom::Transformable::setXY ( int32_t x_, int32_t y_ )
{
  this->coords.setXY ( x_, y_ );
}

const nom::Color& nom::Transformable::getColor ( void ) const
{
  return this->color;
}

void nom::Transformable::setColor ( const nom::Color& color_ )
{
  this->color = color_;
}
