/******************************************************************************
    Transformable.cpp

  Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Transformable.hpp"

nom::Transformable::Transformable ( void )  : coords ( 0, 0, 0, 0 ),
                                              color ( 0, 0, 0, 255 )
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

// SDL_Cursor
int32_t nom::Transformable::getX ( void ) const
{
  return this->coords.x;
}

// SDL_Cursor
int32_t nom::Transformable::getY ( void ) const
{
  return this->coords.y;
}

const nom::Coords nom::Transformable::getPosition ( void ) const
{
  return nom::Coords ( this->coords.x, this->coords.y );
}

const nom::Coords nom::Transformable::getSize ( void ) const
{
  return nom::Coords ( this->coords.width, this->coords.height );
}

// Variant #1 setter for position coordinates
void nom::Transformable::setPosition ( int32_t x, int32_t y )
{
  this->coords.setPosition ( x, y );
}

// Variant #2 setter for position coordinates
void nom::Transformable::setPosition ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  this->coords = nom::Coords ( x, y, width, height );
}

// Variant #3 setter for position coordinates; do we have enough options yet???
void nom::Transformable::setPosition ( const nom::Coords& coords )
{
  this->coords = coords;
}

void nom::Transformable::setSize ( int32_t width, int32_t height )
{
  this->coords.setSize ( width, height );
}

/*
int32_t nom::Transformable::getWidth ( void ) const
{
  return this->coords.width;
}

void nom::Transformable::setWidth ( int32_t width )
{
  this->coords.width = width;
}

int32_t nom::Transformable::getHeight ( void ) const
{
  return this->coords.height;
}

void nom::Transformable::setHeight ( int32_t height )
{
  this->coords.height = height;
}
*/
void nom::Transformable::move ( int32_t x, int32_t y )
{
  this->coords.x += x;
  this->coords.y += y;
}

const nom::Color& nom::Transformable::getColor ( void ) const
{
  return this->color;
}

void nom::Transformable::setColor ( const nom::Color& color_ )
{
  this->color = color_;
}

void nom::Transformable::setColor ( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha )
{
  this->color = nom::Color ( red, green, blue, alpha );
}

