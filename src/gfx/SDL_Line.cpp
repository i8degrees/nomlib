/******************************************************************************

    SDL Line Primitive; line computation logic derives from Bresenham's line
  algorithm

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "nomlib/gfx/SDL_Line.hpp"

namespace nom {

Line::Line ( void )
{
  this->pixels.clear();

  this->setPosition ( 0, 0 );
  this->setSize ( 0, 0 );
  this->setColor ( 0, 0, 0, 255 );
}

Line::~Line ( void )
{

  this->pixels.clear();
}

Line::Line ( const Coords& coords, const Color& color )
{
  this->coords = coords;
  this->color = color;

  this->Update();
}

Line::Line ( int32_t x, int32_t y, int32_t width, int32_t height, const Color& color )
{
  this->setPosition ( Coords ( x, y, width, height ) );
  this->setColor ( color );

  this->Update();
}

// Recompute line offsets
void Line::Update ( void )
{
  // temporary calculation offsets based on user's initial given coordinates
  int32_t x1 = this->coords.x;
  int32_t y1 = this->coords.y;
  int32_t x2 = this->coords.width;
  int32_t y2 = this->coords.height;

  // A surprisingly fast growing memory leak occurs here if we do not clear our
  // pixels array every update cycle
  this->pixels.clear();

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

  for ( int32_t x = ( int32_t ) x1; x < maxX; ++x )
  {
    if ( steep )
      this->pixels.push_back ( std::unique_ptr<Pixel> ( new Pixel ( y, x, this->color ) ) );
    else
      this->pixels.push_back ( std::unique_ptr<Pixel> ( new Pixel ( x, y, this->color ) ) );

    error -= dy;

    if( error < 0 )
    {
      y += ystep;
      error += dx;
    }
  }
}

void Line::Draw ( void* video_buffer ) const
{
  for ( ulong idx = 0; idx < this->pixels.size(); ++idx )
    this->pixels[idx]->Draw ( video_buffer );
}


} // namespace nom
