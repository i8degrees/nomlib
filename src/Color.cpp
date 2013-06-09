/******************************************************************************
    Color.cpp

    Utility class container for RGBA colors

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Color.h"

const nom::Color Black ( 0, 0, 0 );
const nom::Color White ( 255, 255, 255 );
const nom::Color Red ( 255, 0, 0 );
const nom::Color Green ( 0, 255, 0 );
const nom::Color Blue ( 0, 0, 255 );
const nom::Color Yellow ( 255, 255, 255 );
const nom::Color Magenta ( 255, 0, 255 );
const nom::Color Cyan ( 0, 255, 255 );
const nom::Color Transparent ( 0, 0, 0 );

nom::Color::Color ( void ) : red ( 0 ), green ( 0 ), blue ( 0 ), alpha ( -1 ) // SDL_ALPHA_OPAQUE (255)
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::Color (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

nom::Color::Color ( int32_t r, int32_t g, int32_t b, int32_t a )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::Color (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

nom::Color::~Color ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::~Color (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Stub
}

const nom::Color nom::Color::getColor ( void ) const
{
  return Color ( this->red, this->green, this->blue, this->alpha );
}

int32_t nom::Color::getRed ( void ) const
{
  return this->red;
}

int32_t nom::Color::getGreen ( void ) const
{
  return this->green;
}

int32_t nom::Color::getBlue ( void ) const
{
  return this->blue;
}

int32_t nom::Color::getAlpha ( void ) const
{
  return this->alpha;
}

void nom::Color::setColor ( int32_t r, int32_t g, int32_t b, int32_t a )
{
  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

void nom::Color::setRed ( int32_t r )
{
  this->red = r;
}

void nom::Color::setGreen ( int32_t g )
{
  this->green = g;
}

void nom::Color::setBlue ( int32_t b )
{
  this->blue = b;
}

void nom::Color::setAlpha ( int32_t a )
{
  this->alpha = a;
}

uint32_t nom::Color::getColorAsInt ( void* pixel_format ) const
{
  if ( this->alpha != -1 )
    return SDL_MapRGBA ( ( SDL_PixelFormat* ) pixel_format, this->red, this->green, this->blue, this->alpha );
  else
    return SDL_MapRGB ( ( SDL_PixelFormat* ) pixel_format, this->red, this->green, this->blue );
}

// SDL color struct compatibility wrapper
SDL_Color nom::Color::getSDL_Color ( void ) const
{
  SDL_Color color;

  color.r = this->getRed();
  color.g = this->getGreen();
  color.b = this->getBlue();

  return color;
}
