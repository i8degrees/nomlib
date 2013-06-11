/******************************************************************************
    SDL_Line.cpp

  SDL Line Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Line.hpp"

nom::Line::Line ( void )
{
  this->coords.setCoords ( 0, 0, 0, 0 );
  this->color.setColor ( 0, 0, 0, SDL_ALPHA_OPAQUE );
}

nom::Line::~Line ( void )
{
  // ...
}

nom::Line::Line ( const nom::Coords& coords, const nom::Color& color )
{
  this->coords = coords;
  this->color = color;
}

void nom::Line::Draw ( void* video_buffer )
{
  // ...
}
