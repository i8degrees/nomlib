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
NOM_LOG_TRACE ( NOM );

  this->enabled = true;
  this->label_alignment = TextAlignment::MiddleLeft;
}

MessageBox::MessageBox ( void )
{
  this->initialize();
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          enum FrameStyle style, const Gradient& background
                        )
{
  int32 padding = 1;
  this->initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  this->background = Gradient ( background.getStartColor(), background.getEndColor(), this->coords.x, this->coords.y, this->coords.width, this->coords.height, background.getFillDirection(), 0, 0 );

  if ( style == FrameStyle::Gray )
  {
    this->frame = std::make_shared<GrayFrame> ( GrayFrame ( x, y, width, height, padding ) );
  }
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          std::shared_ptr<IFrame> style,
                          const Gradient& background
                        )
{
  int32 padding = 1;

  this->initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

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
NOM_LOG_TRACE ( NOM );
}

bool MessageBox::isEnabled ( void ) const
{
  if ( ! this->enabled ) return false;

  return true;
}

const std::string MessageBox::getWindowTitle ( void )
{
  if ( this->label ) return this->window_title->getText();

  return "\0";
}

const std::string MessageBox::getLabel ( void )
{
  if ( this->label ) return this->label->getText();

  return "\0";
}

void MessageBox::disable ( void )
{
  this->enabled = false;
}

void MessageBox::enable ( void )
{
  this->enabled = true;
}

void MessageBox::setWindowTitleFont ( const IFont* font )
{
  this->window_title = IFont::SharedPtr ( font->clone() );

  // This positions the title text of the message box on top of the second "top"
  // bordering color of GrayFrame, commented as "top1".
  //
  // The original coords.x value was + 8, but I think + 4 looks best.
  this->window_title->setPosition ( Coords ( this->coords.x + 4, this->coords.y, this->coords.width, this->coords.height ) );

  this->window_title->setText ( "INFO." );
  this->window_title->setTextJustification ( TextAlignment::MiddleLeft );
}

void MessageBox::setLabelFont ( const IFont* font )
{
  this->label = IFont::SharedPtr ( font->clone() );
  this->label->setPosition ( this->coords );
}

void MessageBox::setLabelPosition ( const Coords& pos )
{
  if ( ! this->label ) return;

  this->label->setPosition ( pos );
}

void MessageBox::setLabelTextAlignment ( enum TextAlignment alignment )
{
  this->label_alignment = alignment;
}

void MessageBox::setWindowTitle ( const std::string& text )
{
  if ( ! this->window_title ) return;

  this->window_title->setText ( text );
}

void MessageBox::setLabel ( const std::string& text )
{
  if ( ! this->label ) return;

  this->label->setText ( text );
}

void MessageBox::Update ( void )
{
  if ( this->frame != nullptr )
  {
    this->frame->Update();
  }

  if ( ! this->label ) return;

  switch ( this->label_alignment )
  {
    default:
    case TextAlignment::MiddleLeft: // TODO
    {
      this->label->setTextJustification ( TextAlignment::MiddleLeft );
    }
    break;

    case TextAlignment::MiddleCenter:
    {
      this->label->setTextJustification ( TextAlignment::MiddleCenter );
    }
    break;

    case TextAlignment::MiddleRight: // TODO
    {
      this->label->setTextJustification ( TextAlignment::MiddleRight );
    }
    break;
  } // end switch

  this->label->Update();
}

void MessageBox::Draw ( void* video_buffer ) const
{
  if ( this->isEnabled() == false ) return;

  this->background.Draw ( video_buffer );

  if ( this->frame != nullptr )
  {
    this->frame->Draw( video_buffer );
  }

  if ( this->window_title )
  {
    this->window_title->Draw ( video_buffer );
  }

  if ( ! this->label ) return;

  this->label->Draw ( video_buffer );
}


  } // namespace ui
} // namespace nom
