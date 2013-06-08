/******************************************************************************
    Color.cpp

  Color container class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Color.h"

using namespace nom;

Color::Color ( void ) : red ( -1 ), green ( -1 ), blue ( -1 ), alpha ( -1 )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::Color (): " << "Hello, world!" << std::endl << std::endl;
  #endif
}

Color::Color ( int32_t r, int32_t g, int32_t b, int32_t a )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::Color (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

Color::~Color ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "Color::~Color (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Stub
}

// SDL color struct compatibility wrapper
SDL_Color Color::getSDL_Color ( void ) const
{
  SDL_Color color;

  color.r = this->getRed();
  color.g = this->getGreen();
  color.b = this->getBlue();

  return color;
}

Color Color::getColor ( void ) const
{
  return Color ( this->red, this->green, this->blue, this->alpha );
}

const int32_t Color::getRed ( void ) const
{
  return this->red;
}

const int32_t Color::getGreen ( void ) const
{
  return this->green;
}

const int32_t Color::getBlue ( void ) const
{
  return this->blue;
}

const int32_t Color::getAlpha ( void ) const
{
  return this->alpha;
}

void Color::setColor ( int32_t r, int32_t g, int32_t b, int32_t a )
{
  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

void Color::setRed ( int32_t r )
{
  this->red = r;
}

void Color::setGreen ( int32_t g )
{
  this->green = g;
}

void Color::setBlue ( int32_t b )
{
  this->blue = b;
}

void Color::setAlpha ( int32_t a )
{
  this->alpha = a;
}

int32_t Color::getColorAsInt ( void* pixel_format ) const
{
  if ( this->alpha != -1 )
    return SDL_MapRGBA ( ( SDL_PixelFormat* ) pixel_format, this->red, this->green, this->blue, this->alpha );
  else
    return SDL_MapRGB ( ( SDL_PixelFormat* ) pixel_format, this->red, this->green, this->blue );
}
