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

Gradient::Gradient ( void )
{
  Color4u gradient_color[2];
  gradient_color[0] = Color4u::LightGray;
  gradient_color[1] = Color4u::Gray;

  this->initialize  (
                      gradient_color,
                      Coords ( 0, 0, 0, 0 ), 0, 0, Gradient::FillDirection::Left
                    );
}

Gradient::Gradient  (
                      Color4u gradient_color[2],
                      const Coords& bounds, int32 x_margin, int32 y_margin,
                      Gradient::FillDirection direction
                    )
  : gradient {}, coords { bounds },
  x_margin ( x_margin ), y_margin ( y_margin ), fill_direction ( direction ),
  enable_dithering ( true )
{
  this->setStartColor ( gradient_color[0] );
  this->setEndColor ( gradient_color[1] );
}
/* FIXME
{
  this->initialize  (
                      gradient_color,
                      bounds, x_margin, y_margin, direction
                    );
}
FIXME */

Gradient::~Gradient ( void ) {}

void Gradient::initialize (
                            Color4u gradient_color[2],
                            const Coords& bounds,
                            int32 x_margin, int32 y_margin,
                            Gradient::FillDirection direction
                          )
{
  this->setStartColor ( gradient_color[0] );
  this->setEndColor ( gradient_color[1] );
  this->setSize ( bounds.width, bounds.height );
  this->setPosition ( bounds.x, bounds.y );
  this->setMargins ( x_margin, y_margin );
  this->setFillDirection ( direction );

  // Internal option
  this->enableDithering ( true );
}

const Coords Gradient::getPosition ( void ) const
{
  return this->coords;
}

const Coords Gradient::getSize ( void ) const
{
  return this->coords;
}

bool Gradient::dithering ( void ) const
{
  return this->enable_dithering;
}

void Gradient::setPosition ( int32 x, int32 y )
{
  this->coords.x = x;
  this->coords.y = y;
}

void Gradient::setSize ( int32 width, int32 height )
{
  this->coords.width = width;
  this->coords.height = height;
}

Color4u Gradient::getStartColor ( void ) const
{
  return this->gradient[0];
}

Color4u Gradient::getEndColor ( void ) const
{
  return this->gradient[1];
}

void Gradient::setStartColor ( const Color4u& starting_color )
{
  this->gradient[0] = starting_color;
}

void Gradient::setEndColor ( const Color4u& ending_color )
{
  this->gradient[1] = ending_color;
}

void Gradient::reverseColors ( void )
{
  std::swap ( this->gradient[0], this->gradient[1] );
}

Gradient::FillDirection Gradient::getFillDirection ( void ) const
{
  return this->fill_direction;
}

void Gradient::setFillDirection ( Gradient::FillDirection direction )
{
  this->fill_direction = direction;
}

void Gradient::setMargins ( int32 x, int32 y )
{
  this->x_margin = x;
  this->y_margin = y;
}

void Gradient::enableDithering ( bool toggle )
{
  this->enable_dithering = toggle;
}

void Gradient::strategyTopDown ( void )
{
  uint32 y_offset = ( this->coords.y + this->coords.height ) - this->y_margin;

  float currentR = (float) gradient[0].red;
  float currentG = (float) gradient[0].green;
  float currentB = (float) gradient[0].blue;

  float destR = (float) ( gradient[1].red - gradient[0].red )      / ( float ) ( this->coords.height - this->y_margin );
  float destG = (float) ( gradient[1].green - gradient[0].green )  / ( float ) ( this->coords.height - this->y_margin );
  float destB = (float) ( gradient[1].blue - gradient[0].blue )    / ( float ) ( this->coords.height - this->y_margin );

  for ( uint32 rows = this->coords.y + this->y_margin; rows < y_offset; rows++ )
  {
    this->rectangles.push_back ( IDrawable::UniquePtr ( new Rectangle ( Coords ( this->coords.x + this->x_margin, rows, this->coords.width - this->x_margin, 1 ), Color4u ( currentR, currentG, currentB ) ) ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop
}

void Gradient::strategyLeftToRight ( void )
{
  uint32 x_offset = ( this->coords.x + this->coords.width ) - this->x_margin;

  float currentR = (float) gradient[0].red;
  float currentG = (float) gradient[0].green;
  float currentB = (float) gradient[0].blue;

  float destR = (float) ( gradient[1].red - gradient[0].red )      / ( float ) ( this->coords.width - this->x_margin );
  float destG = (float) ( gradient[1].green - gradient[0].green )  / ( float ) ( this->coords.width - this->x_margin );
  float destB = (float) ( gradient[1].blue - gradient[0].blue )    / ( float ) ( this->coords.width - this->x_margin );

  for ( uint32 rows = this->coords.x + this->x_margin; rows < x_offset; rows++ )
  {
    this->rectangles.push_back ( IDrawable::UniquePtr ( new Rectangle ( Coords ( rows, this->coords.y + this->y_margin, 1, this->coords.height - this->y_margin ), Color4u ( currentR, currentG, currentB ) ) ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop
}

void Gradient::update ( void )
{
  this->rectangles.clear();

  if ( this->getFillDirection() == FillDirection::Top )
  {
    this->strategyTopDown();
  }
  else if ( this->getFillDirection() == FillDirection::Bottom )
  {
    this->reverseColors();
    this->strategyTopDown();
  }
  else if ( this->getFillDirection() == FillDirection::Left )
  {
    this->strategyLeftToRight();
  }
  else if ( this->getFillDirection() == FillDirection::Right )
  {
    this->reverseColors();
    this->strategyLeftToRight();
  }
}

void Gradient::draw ( RenderTarget target ) const
{
  for ( auto idx = 0; idx != this->rectangles.size(); ++idx )
  {
    this->rectangles[idx]->draw ( target );
  }
}


} // namespace nom
