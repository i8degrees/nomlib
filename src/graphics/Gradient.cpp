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
  gradient_color[0] = NOM_COLOR4U_LIGHT_GRAY;
  gradient_color[1] = NOM_COLOR4U_GRAY;

  this->initialize  (
                      gradient_color,
                      Coords ( 0, 0, 0, 0 ), 0, 0, Gradient::FillDirection::Left
                    );
}

Gradient::Gradient  (
                      Color4u gradient_color[2],
                      const Coords& bounds, int32 x_margin, int32 y_margin,
                      Gradient::FillDirection direction
                    ) :
  coords_ { bounds },
  x_margin_ ( x_margin ),
  y_margin_ ( y_margin ),
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
                            Color4u gradient_color[2],
                            const Coords& bounds,
                            int32 x_margin, int32 y_margin,
                            Gradient::FillDirection direction
                          )
{
  this->set_start_color ( gradient_color[0] );
  this->set_end_color ( gradient_color[1] );
  this->set_size ( bounds.width, bounds.height );
  this->set_position ( bounds.x, bounds.y );
  this->set_margins ( x_margin, y_margin );
  this->set_fill_direction ( direction );

  // Internal option
  this->enable_dithering ( true );
}

const Coords Gradient::position ( void ) const
{
  return this->coords_;
}

const Coords Gradient::size ( void ) const
{
  return this->coords_;
}

bool Gradient::dithering ( void ) const
{
  return this->enable_dithering_;
}

void Gradient::set_position ( int32 x, int32 y )
{
  this->coords_.x = x;
  this->coords_.y = y;
}

void Gradient::set_size ( int32 width, int32 height )
{
  this->coords_.width = width;
  this->coords_.height = height;
}

Color4u Gradient::start_color ( void ) const
{
  return this->gradient_[0];
}

Color4u Gradient::end_color ( void ) const
{
  return this->gradient_[1];
}

void Gradient::set_start_color ( const Color4u& starting_color )
{
  this->gradient_[0] = starting_color;
}

void Gradient::set_end_color ( const Color4u& ending_color )
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

void Gradient::set_margins ( int32 x, int32 y )
{
  this->x_margin_ = x;
  this->y_margin_ = y;
}

void Gradient::enable_dithering ( bool toggle )
{
  this->enable_dithering_ = toggle;
}

void Gradient::strategy_top_down ( void )
{
  uint32 y_offset = ( this->coords_.y + this->coords_.height ) - this->y_margin_;

  float currentR = (float) gradient_[0].red;
  float currentG = (float) gradient_[0].green;
  float currentB = (float) gradient_[0].blue;

  float destR = (float) ( gradient_[1].red - gradient_[0].red )      / ( float ) ( this->coords_.height - this->y_margin_ );
  float destG = (float) ( gradient_[1].green - gradient_[0].green )  / ( float ) ( this->coords_.height - this->y_margin_ );
  float destB = (float) ( gradient_[1].blue - gradient_[0].blue )    / ( float ) ( this->coords_.height - this->y_margin_ );

  for ( uint32 rows = this->coords_.y + this->y_margin_; rows < y_offset; rows++ )
  {
    this->rectangles_.push_back ( IDrawable::UniquePtr ( new Rectangle ( Coords ( this->coords_.x + this->x_margin_, rows, this->coords_.width - this->x_margin_, 1 ), Color4u ( currentR, currentG, currentB ) ) ) );

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
  uint32 x_offset = ( this->coords_.x + this->coords_.width ) - this->x_margin_;

  float currentR = (float) gradient_[0].red;
  float currentG = (float) gradient_[0].green;
  float currentB = (float) gradient_[0].blue;

  float destR = (float) ( gradient_[1].red - gradient_[0].red )      / ( float ) ( this->coords_.width - this->x_margin_ );
  float destG = (float) ( gradient_[1].green - gradient_[0].green )  / ( float ) ( this->coords_.width - this->x_margin_ );
  float destB = (float) ( gradient_[1].blue - gradient_[0].blue )    / ( float ) ( this->coords_.width - this->x_margin_ );

  for ( uint32 rows = this->coords_.x + this->x_margin_; rows < x_offset; rows++ )
  {
    this->rectangles_.push_back ( IDrawable::UniquePtr ( new Rectangle ( Coords ( rows, this->coords_.y + this->y_margin_, 1, this->coords_.height - this->y_margin_ ), Color4u ( currentR, currentG, currentB ) ) ) );

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
