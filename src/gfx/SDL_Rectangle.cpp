/******************************************************************************

    SDL Rectangle Primitive

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Rectangle.hpp"

nom::Rectangle::Rectangle ( void )
{
  this->coords = nom::Coords ( 0, 0, 0, 0 );
  this->color = nom::Color ( 0, 0, 0 );
}

nom::Rectangle::~Rectangle ( void )
{
  // ...
}

// Constructor variant for creating a rectangle from an existing type
// This is a "shallow" copy assignment
nom::Rectangle::Rectangle ( const nom::Rectangle& rect )
{
  this->coords = rect.coords;
  this->color = rect.color;
}

nom::Rectangle::Rectangle ( const nom::Coords& coords, const nom::Color& color )
{
  this->coords = coords;
  this->color = color;
}

void nom::Rectangle::Update ( void )
{
  // Stub
}

void nom::Rectangle::Draw ( void* video_buffer ) const
{
  SDL_Rect rectangle = getSDL_Rect ( this->coords );
  unsigned int rectangle_color = 0;
  SDL_Surface *buffer = static_cast<SDL_Surface*> ( video_buffer );

  rectangle_color = getColorAsInt ( buffer->format, this->color );

  if ( SDL_FillRect ( buffer, &rectangle, rectangle_color ) != 0 )
  {
NOMLIB_LOG_ERR ( SDL_GetError() );
    return;
  }

  // Destruct as soon as we are done to free memory
  // that could otherwise be held too long before
  // cleaned up, resulting in slow, but steady climb
  // in memory usage
#ifndef NOMLIB_SYSTEM_LINUX
  Rectangle::~Rectangle();
#endif
}
