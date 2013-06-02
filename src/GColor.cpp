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

// SDL compatibility wrapper

SDL_Color GColor::getSDL_Color ( void ) const
{
  SDL_Color color;

  color.r = this->getRed();
  color.g = this->getGreen();
  color.b = this->getBlue();

  return color;
}

GColor GColor::getColor ( void ) const
{
  return GColor ( this->red, this->green, this->blue, this->alpha );
}

unsigned int GColor::getRed ( void ) const
{
  return this->red;
}

unsigned int GColor::getGreen ( void ) const
{
  return this->green;
}

unsigned int GColor::getBlue ( void ) const
{
  return this->blue;
}

unsigned int GColor::getAlpha ( void ) const
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

void GColor::setRed ( unsigned int r )
{
  this->red = r;
}

void GColor::setGreen ( unsigned int g )
{
  this->green = g;
}

void GColor::setBlue ( unsigned int b )
{
  this->blue = b;
}

void GColor::setAlpha ( unsigned int a )
{
  this->alpha = a;
}

unsigned int GColor::mapRGB ( SDL_PixelFormat *video_buffer, unsigned int r,
                              unsigned int g, unsigned int b )
{
  unsigned int color = 0;

  color = SDL_MapRGB ( video_buffer, r, g, b );

  return color;
}

unsigned int GColor::mapRGB ( SDL_PixelFormat *video_buffer, GColor c )
{
  unsigned int color = 0;

  color = SDL_MapRGB ( video_buffer, c.getRed(), c.getGreen(), c.getBlue() );

  return color;
}

unsigned int GColor::mapRGBA (  SDL_PixelFormat *video_buffer, unsigned int r,
                                unsigned int g, unsigned int b, unsigned int a )
{
  unsigned int color = 0;

  color = SDL_MapRGBA ( video_buffer, r, g, b, a );

  return color;
}

unsigned int GColor::mapRGBA (  SDL_PixelFormat *video_buffer, GColor c )
{
  unsigned int color = 0;

  color = SDL_MapRGBA ( video_buffer, c.getRed(), c.getGreen(), c.getBlue(), c.getAlpha() );

  return color;
}
