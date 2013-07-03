/******************************************************************************

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

nom::Transformable::Transformable ( const nom::Coords& coords, const nom::Color& color )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::Transformable (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->coords = coords;
  this->color = color;
}

nom::Transformable::~Transformable ( void )
{
  #ifdef DEBUG_TRANSFORMABLE_OBJ
    std::cout << "Transformable::~Transformable (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Nothing to clean up!
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

void nom::Transformable::setSize ( const nom::Coords& size )
{
  this->coords.setSize ( size.width, size.height );
}

void nom::Transformable::move ( uint32_t x, uint32_t y )
{
  this->coords.x += x;
  this->coords.y += y;
}

void nom::Transformable::move ( const nom::Coords& offsets )
{
  this->coords.x += offsets.x;
  this->coords.y += offsets.y;
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

