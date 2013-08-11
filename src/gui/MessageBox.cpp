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
#include "nomlib/gui/MessageBox.hpp"

namespace nom {
  namespace ui {

void MessageBox::initialize ( void )
{
NOM_LOG_CLASSINFO;

  this->enabled = true;
  this->coords = Coords( 0, 0, 0, 0 );
  this->frame.reset();
}

MessageBox::MessageBox ( void )
{
  initialize();
}

MessageBox::MessageBox  ( int32 x, int32 y, int32 width, int32 height,
                          enum FrameStyle, const Gradient& background
                        )
{
  int32 padding = 1;
  initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  this->background = background;

  this->background = Gradient ( background.getStartColor(), background.getEndColor(), this->coords.x, this->coords.y, this->coords.width, this->coords.height, background.getFillDirection(), 0, 0 );

  if ( FrameStyle::Gray )
  {
    this->frame = std::make_shared<GrayFrame> ( GrayFrame ( x, y, width, height, padding ) );
  }
  else // default is to draw no frame style
  {
    this->frame.reset();
  }
}

MessageBox::MessageBox  ( int32 x, int32 y, int32 width, int32 height,
                          std::shared_ptr<IFrame> style,
                          const Gradient& background
                        )
{
  int32 padding = 1;

  initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );
  this->background = background;

  this->background = Gradient ( background.getStartColor(), background.getEndColor(), this->coords.x, this->coords.y, this->coords.width, this->coords.height, background.getFillDirection(), 0, 0 );

  if ( style != nullptr )
  {
    this->frame = style;
    this->frame->setPosition ( x, y );
    this->frame->setSize ( width, height, padding );
  }
  else // default frame style
  {
    this->frame = std::make_shared<GrayFrame> ( GrayFrame ( x, y, width, height, padding ) );
  }

}

MessageBox::~MessageBox ( void )
{
NOM_LOG_CLASSINFO;

  this->disable();
}

bool MessageBox::isEnabled ( void )
{
  if ( this->enabled == true )
    return true;
  else
    return false;
}

void MessageBox::disable ( void )
{
  this->enabled = false;
}

void MessageBox::enable ( void )
{
  this->enabled = true;
}

void MessageBox::Update ( void )
{
  if ( this->frame != nullptr )
    this->frame->Update();
}

void MessageBox::Draw ( void* video_buffer ) const
{
  this->background.Draw ( video_buffer );

  if ( this->frame != nullptr )
    this->frame->Draw( video_buffer );
}


  } // namespace ui
} // namespace nom
