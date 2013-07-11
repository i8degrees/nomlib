/******************************************************************************

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Color.hpp"

const nom::Color nom::Color::Black ( 0, 0, 0 );
const nom::Color nom::Color::White ( 255, 255, 255 );
const nom::Color nom::Color::Red ( 255, 0, 0 );
const nom::Color nom::Color::Green ( 0, 255, 0 );
const nom::Color nom::Color::Blue ( 0, 0, 255 );
const nom::Color nom::Color::Yellow ( 255, 255, 255 );
const nom::Color nom::Color::Magenta ( 255, 0, 255 );
const nom::Color nom::Color::Cyan ( 0, 255, 255 );
const nom::Color nom::Color::Transparent ( 0, 0, 0 );

const nom::Color nom::Color::Gray ( 99, 99, 99, 255 );

nom::Color::Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( -1 ) // SDL_ALPHA_OPAQUE (255)
{
  // Nothing to initialize
}

nom::Color::Color ( uint8_t red, uint8_t green, uint8_t blue, int32_t alpha )
{
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->alpha = alpha;
}

nom::Color::Color ( const nom::Color& color )
{
  this->red = color.red;
  this->green = color.green;
  this->blue = color.blue;
  this->alpha = color.alpha;
}

nom::Color::~Color ( void )
{
  // Nothing to clean up
}

const nom::Color nom::Color::getColor ( void ) const
{
  return nom::Color ( this->red, this->green, this->blue, this->alpha );
}

nom::Color& nom::Color::operator = ( const nom::Color& other )
{
  this->red = other.red;
  this->green = other.green;
  this->blue = other.blue;
  this->alpha = other.alpha;

  return *this;
}
