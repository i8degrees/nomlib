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

void nom::Line::Update ( void )
{
  this->pixel.setColor ( this->color );
}

// Bresenham's line algorithm
void nom::Line::Draw ( void* video_buffer )
{
  // temporary calculation offsets based on user's initial given coordinates
  int32_t x1 = this->coords.getX();
  int32_t y1 = this->coords.getY();
  int32_t x2 = this->coords.getWidth();
  int32_t y2 = this->coords.getHeight();

  bool steep = ( fabs ( y2 - y1 ) > fabs ( x2 - x1 ) );

  if ( steep )
  {
      std::swap ( x1, y1 );
      std::swap ( x2, y2 );
  }

  if ( x1 > x2 )
  {
      std::swap ( x1, x2 );
      std::swap ( y1, y2 );
  }

  float dx = x2 - x1;
  float dy = fabs ( y2 - y1 );

  float error = dx / 2.0f;
  int32_t ystep = ( y1 < y2 ) ? 1 : -1;
  int32_t y = ( int32_t ) y1;

  int32_t maxX = ( int32_t ) x2;

  for ( int32_t x = ( int32_t ) x1; x < maxX; x++ )
  {
    if ( steep )
      pixel.setPosition ( y, x );
    else
      pixel.setPosition ( x, y );

    pixel.Draw ( video_buffer );

    error -= dy;

    if( error < 0 )
    {
      y += ystep;
      error += dx;
    }
  }
}
