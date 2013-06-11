/******************************************************************************
    SDL_Pixel.cpp

  SDL Point (Pixel) Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Pixel.hpp"

nom::Pixel::Pixel ( void )
{
  this->coords.setCoords ( 0, 0, 0, 0 );
  this->color.setColor ( 0, 0, 0, SDL_ALPHA_OPAQUE );
}

nom::Pixel::~Pixel ( void )
{
  // ...
}

nom::Pixel::Pixel ( const nom::Coords& coords, const nom::Color& color )
{
  this->coords = coords;
  this->color = color;
}

void nom::Pixel::Draw ( void* video_buffer )
{
  // ...
}
