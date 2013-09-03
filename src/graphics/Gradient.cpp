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
#include "nomlib/graphics/Gradient.hpp"

namespace nom {

// Default constructor; initializes with respective defaults
Gradient::Gradient ( void )  :  gradient{ Color::Gray,
                                          Color::Gray
                                        },
                                x_margin ( 0 ), y_margin ( 0 ),
                                direction ( 0 )
{
NOM_LOG_TRACE ( NOM );
}

// Constructor variant
Gradient::Gradient  ( const Color& starting_color,
                      const Color& ending_color,
                      int32 x, int32 y, int32 width, int32 height,
                      uint32 direction, uint32 x_margin, uint32 y_margin
                    )
                    : gradient{ starting_color, ending_color },
                      coords{ x, y, width, height },
                      x_margin ( x_margin ), y_margin ( y_margin ),
                      direction ( direction )
{
  this->Update();
}

Gradient::~Gradient ( void )
{
NOM_LOG_TRACE ( NOM );
}

const Coords Gradient::getPosition ( void ) const
{
  return this->coords;
}

const Coords Gradient::getSize ( void ) const
{
  return this->coords;
}

void Gradient::setPosition ( const Coords& pos )
{
  this->coords = pos;
  this->Update();
}

void Gradient::setSize ( const Coords& pos )
{
  this->coords = pos;
  this->Update();
}

Color Gradient::getStartColor ( void ) const
{
  return this->gradient[0];
}

Color Gradient::getEndColor ( void ) const
{
  return this->gradient[1];
}

void Gradient::setStartColor ( const Color& starting_color )
{
  this->gradient[0] = starting_color;
  this->Update();
}

void Gradient::setEndColor ( const Color& ending_color )
{
  this->gradient[1] = ending_color;
  this->Update();
}

uint32 Gradient::getFillDirection ( void ) const
{
  return this->direction;
}

void Gradient::setFillDirection ( const uint32 direction )
{
  this->direction = direction;
  this->Update();
}

void Gradient::Update ( void )
{
  this->lines.clear();

  uint32 x_offset = this->coords.x + this->coords.width;

  float currentR = (float) gradient[0].red;
  float currentG = (float) gradient[0].green;
  float currentB = (float) gradient[0].blue;

  float destR = (float) ( gradient[1].red - gradient[0].red )      / ( float ) ( this->coords.width - this->x_margin );
  float destG = (float) ( gradient[1].green - gradient[0].green )  / ( float ) ( this->coords.width - this->x_margin );
  float destB = (float) ( gradient[1].blue - gradient[0].blue )    / ( float ) ( this->coords.width - this->x_margin );

  for ( uint32 rows = this->coords.x; rows < x_offset - this->x_margin; rows++ )
  {
    this->lines.push_back ( std::shared_ptr<Rectangle> ( new Rectangle ( Coords ( rows, this->coords.y, 1, this->coords.height - this->y_margin ), Color ( currentR, currentG, currentB ) ) ) );

    if ( this->direction == 0 )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
    else if ( this->direction == 1 ) // reversed
    {
      currentR -= destR;
      currentG -= destG;
      currentB -= destB;
    }
  } // end for coords blit loop
}

void Gradient::Draw ( void* video_buffer ) const
{
  for ( auto it = this->lines.begin(); it != this->lines.end(); ++it )
  {
    std::shared_ptr<IDrawable> obj = *it;
    obj->Draw ( video_buffer );
  }
}


} // namespace nom
