/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

******************************************************************************/
#include "nomlib/graphics/Line.hpp"

namespace nom {

Line::Line ( void )
{
  this->updated = true;
  this->Update();
}

Line::Line ( const Coords& coords, const Color& color )
{
  this->coords = coords;
  this->color = color;

  this->updated = false;
  this->Update();
}

Line::Line ( int32 x, int32 y, int32 width, int32 height, const Color& color )
{
  this->coords = Coords ( x, y, width, height );
  this->color = color;

  this->updated = false;
  this->Update();
}

Line::~Line ( void )
{
  // ...
}

// Recompute line offsets
void Line::Update ( void )
{
  if ( this->updated == true )
    return;

  // temporary calculation offsets based on user's initial given coordinates
  int32 x1 = this->coords.x;
  int32 y1 = this->coords.y;
  int32 x2 = this->coords.width;
  int32 y2 = this->coords.height;

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
  int32 ystep = ( y1 < y2 ) ? 1 : -1;
  int32 y = ( int32 ) y1;

  int32 maxX = ( int32 ) x2;

  for ( int32 x = ( int32 ) x1; x < maxX; ++x )
  {
    if ( steep )
      this->pixels.push_back ( nom::make_unique<Pixel> ( Pixel ( y, x, this->color ) ) );
    else
      this->pixels.push_back ( nom::make_unique<Pixel> ( Pixel ( x, y, this->color ) ) );

    error -= dy;

    if( error < 0 )
    {
      y += ystep;
      error += dx;
    }
  }

  this->updated = true;
}

void Line::Draw ( SDL_Surface* video_buffer ) const
{
  for ( ulong idx = 0; idx < this->pixels.size(); ++idx )
    this->pixels[idx]->Draw ( video_buffer );
}


} // namespace nom
