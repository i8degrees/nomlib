/******************************************************************************
    Color.cpp

  Color container class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "Color.h"

nom::Color::Color ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "GColor::GColor (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = 0;
  this->green = 0;
  this->blue = 0;
  this->alpha = -1;
}

nom::Color::Color ( unsigned int r, unsigned int g, unsigned int b, signed int a )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "GColor::GColor (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = -1;
}

nom::Color::~Color ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "GColor::~GColor (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Stub
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

nom::Color nom::Color::getColor ( void ) const
{
  return nom::Color ( this->red, this->green, this->blue, this->alpha );
}

unsigned int nom::Color::getRed ( void ) const
{
  return this->red;
}

unsigned int nom::Color::getGreen ( void ) const
{
  return this->green;
}

unsigned int nom::Color::getBlue ( void ) const
{
  return this->blue;
}

unsigned int nom::Color::getAlpha ( void ) const
{
  return this->alpha;
}

void nom::Color::setColor ( unsigned int r, unsigned int g, unsigned int b, signed int a )
{
  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

void nom::Color::setRed ( unsigned int r )
{
  this->red = r;
}

void nom::Color::setGreen ( unsigned int g )
{
  this->green = g;
}

void nom::Color::setBlue ( unsigned int b )
{
  this->blue = b;
}

void nom::Color::setAlpha ( signed int a )
{
  this->alpha = a;
}

unsigned int nom::Color::getColorAsInt ( SDL_PixelFormat *pixel_buffer ) const
{
  if ( this->alpha != -1 )
  {
    return SDL_MapRGBA ( pixel_buffer, this->red, this->green, this->blue, this->alpha );
  }
  else
  {
    return SDL_MapRGB ( pixel_buffer, this->red, this->green, this->blue );
  }
}
