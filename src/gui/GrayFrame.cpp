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
#include "nomlib/gui/GrayFrame.hpp"

namespace nom {
  namespace ui {

GrayFrame::GrayFrame ( void )
{
  // ...
}

GrayFrame::GrayFrame ( int32 x, int32 y, int32 width, int32 height, int32 padding )
{
  this->setPosition ( x, y );
  this->setSize ( width, height, padding );

  this->Update();
}

GrayFrame::~GrayFrame ( void )
{
  this->frame.clear();
}

void GrayFrame::setPosition ( int32 x, int32 y )
{
  this->coords.setPosition ( x, y );

  this->Update();
}

void GrayFrame::setSize ( int32 width, int32 height, int32 padding )
{
  this->coords.setSize ( width, height );
  this->padding = padding;

  this->Update();
}

void GrayFrame::Update ( void )
{
  int32 x = this->coords.x;
  int32 y = this->coords.y;
  int32 width = this->coords.width;
  int32 height = this->coords.height;

  int32 x_offset = x + width;
  int32 y_offset = y + height;

  this->frame.clear();

  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x, y, x_offset - padding, y, Color( 41, 41, 41 ) ) ) ); // top0
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x, y + 1, x_offset - padding, y + 1, Color( 133, 133, 133 ) ) ) ); // top1
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x, y + 1, x, y_offset - padding, Color( 41, 41, 41 ) ) ) ); // left0
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x + 1, y + 2, x + 1, y_offset - padding, Color( 133, 133, 133 ) ) ) ); // left1
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x, y_offset - padding, x_offset - padding, y_offset - padding, Color( 57, 57, 57 ) ) ) ); //bottom0
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x, y_offset, x_offset + padding, y_offset, Color ( 41, 41, 41 ) ) ) ); // bottom1
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x_offset - padding, y, x_offset - padding, y_offset + padding, Color( 57, 57, 57 ) ) ) ); // right0
  this->frame.push_back ( std::shared_ptr<Line> ( new Line ( x_offset, y, x_offset, y_offset + padding, Color( 41, 41, 41 ) ) ) ); // right1
}

void GrayFrame::Draw ( void* video_buffer ) const
{
  // nom::Line needs pixel write access, so we must first lock the surface
  this->context.lock ( video_buffer );

  for ( auto it = this->frame.begin(); it != this->frame.end(); ++it )
  {
    std::shared_ptr<IDrawable> obj = *it;
    obj->Draw ( video_buffer );
  }

  // All done!
  this->context.unlock ( video_buffer );
}


  } // namespace ui
} // namespace nom
