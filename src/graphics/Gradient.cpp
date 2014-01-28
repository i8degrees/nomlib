/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, 2014 Jeffrey Carpenter <i8degrees@gmail.com>
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
  Color4i gradient_color[2];
  gradient_color[0] = Color4i::LightGray;
  gradient_color[1] = Color4i::Gray;

  this->initialize  (
                      gradient_color,
                      Point2i( 0, 0), Size2i(0, 0 ), Point2i(0, 0), Gradient::FillDirection::Left
                    );
}

Gradient::Gradient  (
                      Color4i gradient_color[2],
                      const Point2i& pos, const Size2i& size,
                      const Point2i& margin,
                      Gradient::FillDirection direction
                    ) :
  Transformable ( pos, size ),
  margins_ ( margin ),
  fill_direction_ ( direction ),
  enable_dithering_ ( true )
{
  this->set_start_color ( gradient_color[0] );
  this->set_end_color ( gradient_color[1] );
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
                            Color4i gradient_color[2],
                            const Point2i& pos, const Size2i& size,
                            const Point2i& margin,
                            Gradient::FillDirection direction
                          )
{
  this->set_start_color ( gradient_color[0] );
  this->set_end_color ( gradient_color[1] );
  this->set_size ( size );
  this->set_position ( pos );
  this->set_margins ( margin );
  this->set_fill_direction ( direction );

  // Internal option
  this->enable_dithering ( true );
}

/*
const Size2i& Gradient::size ( void ) const
{
  return this->size_;
}
*/

bool Gradient::dithering ( void ) const
{
  return this->enable_dithering_;
}

const Point2i& Gradient::margins ( void ) const
{
  return this->margins_;
}

/*
void Gradient::set_size ( const Size2i& size )
{
  this->size_ = size;
}
*/

Color4i Gradient::start_color ( void ) const
{
  return this->gradient_[0];
}

Color4i Gradient::end_color ( void ) const
{
  return this->gradient_[1];
}

void Gradient::set_start_color ( const Color4i& starting_color )
{
  this->gradient_[0] = starting_color;
}

void Gradient::set_end_color ( const Color4i& ending_color )
{
  this->gradient_[1] = ending_color;
}

void Gradient::reverse_colors ( void )
{
  std::swap ( this->gradient_[0], this->gradient_[1] );
}

Gradient::FillDirection Gradient::fill_direction ( void ) const
{
  return this->fill_direction_;
}

void Gradient::set_fill_direction ( Gradient::FillDirection direction )
{
  this->fill_direction_ = direction;
}

void Gradient::set_margins ( const Point2i& margin )
{
  this->margins_ = margin;
}

void Gradient::enable_dithering ( bool toggle )
{
  this->enable_dithering_ = toggle;
}

void Gradient::strategy_top_down ( void )
{
  uint32 y_offset = ( this->position().y + this->size().h ) - this->margins().y;

  float currentR = (float) gradient_[0].r;
  float currentG = (float) gradient_[0].g;
  float currentB = (float) gradient_[0].b;

  float destR = (float) ( gradient_[1].r - gradient_[0].r )      / ( float ) ( this->size().h - this->margins().y );
  float destG = (float) ( gradient_[1].g - gradient_[0].g )  / ( float ) ( this->size().h - this->margins().y );
  float destB = (float) ( gradient_[1].b - gradient_[0].b )    / ( float ) ( this->size().h - this->margins().y );

  for ( uint32 rows = this->position().y + this->margins().y; rows < y_offset; rows++ )
  {
    // Calculate rendering offsets
    IntRect render_coords = IntRect( this->position().x + this->margins().x, rows, this->size().w - this->margins().x, 1 );
    Color4i render_color = Color4i( currentR, currentG, currentB );

    // Queue up to render
    this->rectangles_.push_back ( IDrawable::UniquePtr ( new Rectangle( render_coords, render_color ) ) );

    if ( this->dithering() )
    {
      currentR += destR;
      currentG += destG;
      currentB += destB;
    }
  } // end blit loop
}

void Gradient::strategy_left_right ( void )
{
  uint32 x_offset = ( this->position().x + this->size().w ) - this->margins().x;

  float currentR = (float) gradient_[0].r;
  float currentG = (float) gradient_[0].g;
  float currentB = (float) gradient_[0].b;

  float destR = (float) ( gradient_[1].r - gradient_[0].r )      / ( float ) ( this->size().w - this->margins().x );
  float destG = (float) ( gradient_[1].g - gradient_[0].g )  / ( float ) ( this->size().w - this->margins().x );
  float destB = (float) ( gradient_[1].b - gradient_[0].b )    / ( float ) ( this->size().w - this->margins().x );

  for ( uint32 rows = this->position().x + this->margins().x; rows < x_offset; rows++ )
  {
    // Calculate rendering offsets
    IntRect render_coords = IntRect( rows, this->position().y + this->margins().y, 1, this->size().h - this->margins().y );
    Color4i render_color = Color4i( currentR, currentG, currentB );

    // Queue up to render
    this->rectangles_.push_back ( IDrawable::UniquePtr ( new Rectangle ( render_coords, render_color ) ) );

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
  this->rectangles_.clear();

  if ( this->fill_direction() == FillDirection::Top )
  {
    this->strategy_top_down();
  }
  else if ( this->fill_direction() == FillDirection::Bottom )
  {
    this->reverse_colors();
    this->strategy_top_down();
  }
  else if ( this->fill_direction() == FillDirection::Left )
  {
    this->strategy_left_right();
  }
  else if ( this->fill_direction() == FillDirection::Right )
  {
    this->reverse_colors();
    this->strategy_left_right();
  }
}

void Gradient::draw ( RenderTarget target ) const
{
  for ( auto idx = 0; idx != this->rectangles_.size(); ++idx )
  {
    this->rectangles_[idx]->draw ( target );
  }
}

} // namespace nom
