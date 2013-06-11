/******************************************************************************
    SDL_Rectangle.cpp

  SDL Rectangle Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Rectangle.hpp"

nom::Rectangle::Rectangle ( void )
{
  this->coords.setCoords ( 0, 0, 0, 0 );
  this->color.setColor ( 0, 0, 0, SDL_ALPHA_OPAQUE );
}

nom::Rectangle::~Rectangle ( void )
{
  // ...
}

nom::Rectangle::Rectangle ( const nom::Coords& coords, const nom::Color& color )
{
  this->coords = coords;
  this->color = color;
}

void nom::Rectangle::Draw ( void* video_buffer )
{
  SDL_Rect rectangle = this->coords.getSDL_Rect();
  unsigned int rectangle_color = 0;
  SDL_Surface *buffer = (SDL_Surface*) video_buffer;

  rectangle_color = this->color.getColorAsInt ( buffer->format );

  if ( SDL_FillRect ( buffer, &rectangle, rectangle_color ) != 0 )
  {
    std::cout << "ERR in nom::Rectangle::Draw(): " << SDL_GetError() << std::endl;
  }
}
