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
#include "nomlib/gui/GrayFrame.hpp"

namespace nom {

GrayFrame::GrayFrame ( void ) : updated ( false ) {}

GrayFrame::~GrayFrame ( void ) {}

GrayFrame& GrayFrame::operator = ( const GrayFrame& other )
{
  this->frame_position = other.frame_position;
  this->updated = other.updated;
  this->padding = other.padding;

  return *this;
}

GrayFrame::GrayFrame ( int32 x, int32 y, int32 width, int32 height, int32 padding )
{
  this->setPosition ( x, y );
  this->setSize ( width, height, padding );

  this->updated = false;
}

void GrayFrame::setPosition ( int32 x, int32 y )
{
  this->frame_position.setPosition ( x, y );
  this->updated = false;
}

void GrayFrame::setSize ( int32 width, int32 height, int32 padding )
{
  frame_position.setSize ( width, height );
  this->padding = padding;
  this->updated = false;
}

void GrayFrame::update ( void )
{
  if ( this->updated == true )
    return;

  int32 x = this->frame_position.x;
  int32 y = this->frame_position.y;
  int32 width = this->frame_position.w;
  int32 height = this->frame_position.h;

  int32 x_offset = x + width;
  int32 y_offset = y + height;

  this->frame.clear();

  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x, y, x_offset - padding, y, Color4i( 41, 41, 41 ) ) ) ); // top0
  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x, y + 1, x_offset - padding, y + 1, Color4i( 133, 133, 133 ) ) ) ); // top1

  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x, y + 1, x, y_offset - padding, Color4i( 41, 41, 41 ) ) ) ); // left0
  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x + 1, y + 2, x + 1, y_offset - padding, Color4i( 133, 133, 133 ) ) ) ); // left1

  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x_offset, y, x_offset, y_offset - padding, Color4i( 57, 57, 57 ) ) ) ); // right0
  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x_offset, y, x_offset, y_offset - padding, Color4i( 41, 41, 41 ) ) ) ); // right1

  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x, y_offset - padding, x_offset - padding, y_offset - padding, Color4i( 57, 57, 57 ) ) ) ); //bottom0
  this->frame.push_back ( IDrawable::UniquePtr ( new Line ( x, y_offset - padding, x_offset - padding, y_offset - padding, Color4i ( 41, 41, 41 ) ) ) ); // bottom1

  this->updated = true;
}

void GrayFrame::draw ( RenderTarget target ) const
{
  for ( auto idx = 0; idx != this->frame.size(); ++idx )
  {
    this->frame[idx]->draw ( target );
  }
}

} // namespace nom
