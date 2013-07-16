/******************************************************************************

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/math/Color.hpp"

namespace nom {

const Color Color::Black ( 0, 0, 0 );
const Color Color::White ( 255, 255, 255 );
const Color Color::Red ( 255, 0, 0 );
const Color Color::Green ( 0, 255, 0 );
const Color Color::Blue ( 0, 0, 255 );
const Color Color::Yellow ( 255, 255, 255 );
const Color Color::Magenta ( 255, 0, 255 );
const Color Color::Cyan ( 0, 255, 255 );
const Color Color::Transparent ( 0, 0, 0 );

const Color Color::Gray ( 99, 99, 99, 255 );

Color::Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( -1 ) // SDL_ALPHA_OPAQUE (255)
{
  // Nothing to initialize
}

Color::Color ( uint8_t red, uint8_t green, uint8_t blue, int32_t alpha )
{
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->alpha = alpha;
}

Color::Color ( const Color& color )
{
  this->red = color.red;
  this->green = color.green;
  this->blue = color.blue;
  this->alpha = color.alpha;
}

Color::~Color ( void )
{
  // Nothing to clean up
}

const Color Color::getColor ( void ) const
{
  return Color ( this->red, this->green, this->blue, this->alpha );
}

Color& Color::operator = ( const Color& other )
{
  this->red = other.red;
  this->green = other.green;
  this->blue = other.blue;
  this->alpha = other.alpha;

  return *this;
}


} // namespace nom
