/******************************************************************************
    Color.cpp

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

// Default constructor for setting a color to their respective defaults
nom::Color::Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( -1 ) // SDL_ALPHA_OPAQUE (255)
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::Color (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

// Constructor variant for setting a color using RGBA values
nom::Color::Color ( uint8_t red, uint8_t green, uint8_t blue, int32_t alpha )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::Color (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = red;
  this->green = green;
  this->blue = blue;
  this->alpha = alpha;
}

// Constructor variant for setting a color with an exiting nom::Color object
nom::Color::Color ( const nom::Color& color )
{
  this->red = color.red;
  this->green = color.green;
  this->blue = color.blue;
  this->alpha = color.alpha;
}

// Constructor variant for setting a color from a SDL_Color structure;
// backwards-compatibility wrapper
nom::Color::Color ( const SDL_Color& color )
{
  this->red = color.r;
  this->green = color.g;
  this->blue = color.b;
  //this->alpha = color.a;
}

// Default constructor; exists solely as a stub
nom::Color::~Color ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::~Color (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Nothing to do!
}

// Convenience getter helper for obtaining a color by object
const nom::Color nom::Color::getColor ( void ) const
{
  return nom::Color ( this->red, this->green, this->blue, this->alpha );
}

// Convenience getter helper for obtaining a color as an integer, respective to
// the video surface pixel format; color bits vary depending on bit-rate
uint32_t nom::Color::getColorAsInt ( void* pixel_format ) const
{
  return SDL_MapRGB ( ( SDL_PixelFormat* ) pixel_format, this->red, this->green, this->blue );
}

// Convenience getter helper for obtaining a color as an integer, respective to
// the video surface pixel format; color bits vary depending on bit-rate and
// alpha channel
uint32_t nom::Color::getAlphaColorAsInt ( void* pixel_format ) const
{
  return SDL_MapRGBA ( ( SDL_PixelFormat* ) pixel_format, this->red, this->green, this->blue, this->alpha );
}

// Returns a SDL color structure of our color object; backwards-compatibility
// wrapper
SDL_Color nom::Color::getSDL_Color ( void ) const
{
  SDL_Color color;

  color.r = this->red;
  color.g = this->green;
  color.b = this->blue;
  //color.a = this->alpha;

  return color;
}
