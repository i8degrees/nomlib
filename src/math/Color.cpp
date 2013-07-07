/******************************************************************************

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Color.hpp"

// Predefined color constants
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

// Default constructor; sets the color to their respective defaults
nom::Color::Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( -1 ) // SDL_ALPHA_OPAQUE (255)
{
  // Nothing to initialize
}

// Constructor variant for setting a color using RGBA values
nom::Color::Color ( uint8_t red, uint8_t green, uint8_t blue, int32_t alpha )
{
  this->red = red;
  this->green = green;
  this->blue = blue;
  this->alpha = alpha;
}

// Constructor variant for setting a color with an exiting nom::Color object
// This is a "shallow" copy assignment
nom::Color::Color ( const nom::Color& color )
{
  this->red = color.red;
  this->green = color.green;
  this->blue = color.blue;
  this->alpha = color.alpha;
}

// Default constructor; exists solely as a stub
nom::Color::~Color ( void )
{
  // Nothing to clean up
}

// Convenience getter helper for obtaining a color by object
const nom::Color nom::Color::getColor ( void ) const
{
  return nom::Color ( this->red, this->green, this->blue, this->alpha );
}

{

}
