/******************************************************************************

    SDL Line Primitive; line computation logic derives from Bresenham's line
  algorithm

  Copyright (c) 2013 Jeffrey Carpenter
  All rights reserved.

******************************************************************************/
#include "SDL_Line.hpp"

nom::Line::Line ( void )
{
  this->pixels.clear();

  this->updated = false;

  this->setPosition ( 0, 0 );
  this->setSize ( 0, 0 );
  this->setColor ( 0, 0, 0, 255 );
}

nom::Line::~Line ( void )
{
  this->updated = false;

  // Goodbye cruel pixels!
  //for ( std::vector<nom::Pixel*>::const_iterator it = this->pixels.begin(); it != this->pixels.end(); ++it )
  //{
    //delete *it;
  //}

  this->pixels.clear();
}

nom::Line::Line ( const nom::Coords& coords, const nom::Color& color )
{
  this->pixels.clear();

  this->updated = false;

  this->coords = coords;
  this->color = color;
}

nom::Line::Line ( int32_t x, int32_t y, int32_t width, int32_t height, const nom::Color& color )
{
  this->pixels.clear();

  this->updated = false;

  this->setPosition ( nom::Coords ( x, y, width, height ) );
  this->setColor ( color );
}

// Recompute line offsets
void nom::Line::Update ( void )
{
  // temporary calculation offsets based on user's initial given coordinates
  int32_t x1 = this->coords.x;
  int32_t y1 = this->coords.y;
  int32_t x2 = this->coords.width;
  int32_t y2 = this->coords.height;

  if ( this->updated == true )
    return;

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
      this->pixels.push_back ( std::unique_ptr<nom::Pixel> ( new nom::Pixel ( y, x, this->color ) ) );
      //this->pixels.push_back ( new nom::Pixel ( y, x, this->color ) );
    else
      this->pixels.push_back ( std::unique_ptr<nom::Pixel> ( new nom::Pixel ( x, y, this->color ) ) );
      //this->pixels.push_back ( new nom::Pixel ( x, y, this->color ) );

    error -= dy;

    if( error < 0 )
    {
      y += ystep;
      error += dx;
    }
  }

  this->updated = true;
}

void nom::Line::Draw ( void* video_buffer ) const
{
  for ( nom::ulong idx = 0; idx < this->pixels.size(); ++idx )
    this->pixels[idx]->Draw ( video_buffer );
}
