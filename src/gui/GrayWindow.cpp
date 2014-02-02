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
#include "nomlib/gui/GrayWindow.hpp"

namespace nom {

GrayWindow::GrayWindow ( void ) :
  padding_ ( 1 ),
  updated_ ( false )
{
  //NOM_LOG_TRACE ( NOM );
}

GrayWindow::~GrayWindow ( void )
{
  //NOM_LOG_TRACE ( NOM );
}

/*
GrayWindow& GrayWindow::operator = ( const GrayWindow& other )
{
  this->frame_position = other.frame_position;
  this->updated_ = other.updated_;
  this->padding = other.padding;

  return *this;
}
*/

GrayWindow::GrayWindow ( const IntRect& bounds, int pad ) :
  Transformable { Point2i( bounds.x, bounds.y ), Size2i( bounds.w, bounds.h ) },
  padding_ ( pad ),
  updated_ ( false )
{
  //NOM_LOG_TRACE ( NOM );
}

GrayWindow::GrayWindow ( const Point2i& pos, const Size2i& size, int pad ) :
  Transformable { pos, size },
  padding_ ( pad ),
  updated_ ( false )
{
  //NOM_LOG_TRACE ( NOM );
}

int GrayWindow::padding ( void ) const
{
  return this->padding_;
}

void GrayWindow::set_padding ( int pad )
{
  this->padding_ = pad;
}

void GrayWindow::update ( void )
{
  if ( this->updated_ == true )
    return;

  int x = this->position().x;
  int y = this->position().y;
  int width = this->size().w;
  int height = this->size().h;

  int x_offset = x + width;
  int y_offset = y + height;

  this->frame_.clear();

  Line top0 ( IntRect( x, y, x_offset - this->padding(), y), Color4i( 41, 41, 41 ) );
  Line top1 ( IntRect( x, y + 1, x_offset - this->padding(), y + 1), Color4i( 133, 133, 133 ) );

  Line left0 ( IntRect( x, y + 1, x, y_offset - this->padding()), Color4i( 41, 41, 41 ) );
  Line left1 ( IntRect( x + 1, y + 2, x + 1, y_offset - this->padding()), Color4i( 133, 133, 133 ) );

  Line right0 ( IntRect( x_offset, y, x_offset, y_offset - this->padding()), Color4i( 57, 57, 57 ) );
  Line right1 ( IntRect( x_offset, y, x_offset, y_offset - this->padding()), Color4i( 41, 41, 41 ) );

  Line bottom0 ( IntRect( x, y_offset - this->padding(), x_offset - this->padding(), y_offset - this->padding()), Color4i( 57, 57, 57 ) );
  Line bottom1 ( IntRect( x, y_offset - this->padding(), x_offset - this->padding(), y_offset - this->padding()), Color4i ( 41, 41, 41 ) );

  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( top0)) );
  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( top1)) );

  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( left0)) );
  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( left1)) );

  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( right0)) );
  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( right1)) );

  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( bottom0)) );
  this->frame_.push_back ( IDrawable::UniquePtr ( new Line( bottom1)) );

  this->updated_ = true;
}

void GrayWindow::draw ( RenderTarget target ) const
{
  for ( auto idx = 0; idx != this->frame_.size(); ++idx )
  {
    this->frame_[idx]->draw ( target );
  }
}

} // namespace nom
