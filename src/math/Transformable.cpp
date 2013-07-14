/******************************************************************************

    Inheritance class for adding Coords & Color methods

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Transformable.hpp"

namespace nom {

Transformable::Transformable ( void )  :  coords ( 0, 0, 0, 0 ),
                                          color ( 0, 0, 0, 255 )
{
  // Nothing to initialize
}

Transformable::Transformable ( const Coords& coords, const Color& color )
{
  this->coords = coords;
  this->color = color;
}

Transformable::~Transformable ( void )
{
  // Nothing to clean up!
}

const Coords Transformable::getPosition ( void ) const
{
  return Coords ( this->coords.x, this->coords.y );
}

const Coords Transformable::getSize ( void ) const
{
  return Coords ( this->coords.width, this->coords.height );
}

// Variant #1 setter for position coordinates
void Transformable::setPosition ( int32_t x, int32_t y )
{
  this->coords.setPosition ( x, y );
}

// Variant #2 setter for position coordinates
void Transformable::setPosition ( int32_t x, int32_t y, int32_t width, int32_t height )
{
  this->coords = Coords ( x, y, width, height );
}

// Variant #3 setter for position coordinates; do we have enough options yet???
void Transformable::setPosition ( const Coords& coords )
{
  this->coords = coords;
}

void Transformable::setSize ( int32_t width, int32_t height )
{
  this->coords.setSize ( width, height );
}

void Transformable::setSize ( const Coords& size )
{
  this->coords.setSize ( size.width, size.height );
}

void Transformable::move ( uint32_t x, uint32_t y )
{
  this->coords.x += x;
  this->coords.y += y;
}

void Transformable::move ( const Coords& offsets )
{
  this->coords.x += offsets.x;
  this->coords.y += offsets.y;
}

const Color& Transformable::getColor ( void ) const
{
  return this->color;
}

void Transformable::setColor ( const Color& color_ )
{
  this->color = color_;
}

void Transformable::setColor ( uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha )
{
  this->color = Color ( red, green, blue, alpha );
}


} // namespace nom
