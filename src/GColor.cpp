/******************************************************************************
    GColor.cpp

  Color container class

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "GColor.h"

GColor::GColor ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "GColor::GColor (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = 0;
  this->green = 0;
  this->blue = 0;
  this->alpha = -1;
}

GColor::GColor ( unsigned int r, unsigned int g, unsigned int b, unsigned int a )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "GColor::GColor (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = -1;
}

GColor::~GColor ( void )
{
  #ifdef DEBUG_COLOR_OBJ
    std::cout << "GColor::~GColor (): " << "Goodbye cruel world!" << std::endl << std::endl;
  #endif

  // Stub
}

GColor GColor::getColor ( void )
{
  return GColor ( this->red, this->green, this->blue, this->alpha );
}

unsigned int GColor::getRed ( void )
{
  return this->red;
}

unsigned int GColor::getGreen ( void )
{
  return this->green;
}

unsigned int GColor::getBlue ( void )
{
  return this->blue;
}

unsigned int GColor::getAlpha ( void )
{
  return this->alpha;
}

void GColor::setColor ( unsigned int r, unsigned int g, unsigned int b, unsigned int a )
{
  this->red = r;
  this->green = g;
  this->blue = b;
  this->alpha = a;
}

unsigned int GColor::mapRGB ( SDL_PixelFormat *video_buffer, GColor c )
{
  unsigned int color = 0;

  color = SDL_MapRGB ( video_buffer, c.getRed(), c.getGreen(), c.getBlue() );

  return color;
}

unsigned int GColor::mapRGBA ( SDL_PixelFormat *video_buffer, GColor c )
{
  unsigned int color = 0;

  color = SDL_MapRGBA ( video_buffer, c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha() );

  return color;
}
