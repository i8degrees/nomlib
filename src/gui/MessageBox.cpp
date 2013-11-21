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
  this->label_alignment = IFont::TextAlignment::MiddleLeft;
}

MessageBox::MessageBox ( void )
{
  this->initialize();
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          enum FrameStyle style,
                          const Gradient& background
                        )
{
  int32 padding = 1;
  this->initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  Color4u gradient_color[2];
  gradient_color[0] = background.getStartColor();
  gradient_color[1] = background.getEndColor();

  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, Coords ( this->coords.x, this->coords.y, this->coords.width, this->coords.height ), 0, 0, background.getFillDirection() ) ) );

  if ( style == FrameStyle::Gray )
  {
    this->drawable.push_back ( GrayFrame::SharedPtr ( new GrayFrame ( x, y, width, height, padding ) ) );
  }
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          GrayFrame::SharedPtr style,
                          Gradient::SharedPtr background
                        )
{
  int32 padding = 1;

  this->initialize();

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  Color4u gradient_color[2];
  gradient_color[0] = background->getStartColor();
  gradient_color[1] = background->getEndColor();
  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, Coords ( this->coords.x, this->coords.y, this->coords.width, this->coords.height ), 0, 0, background->getFillDirection() ) ) );

  if ( style != nullptr )
  {
    style->setPosition ( x, y );
    style->setSize ( width, height, padding );
    this->drawable.push_back ( style );
  }
  else // default frame style
  {
    this->drawable.push_back ( GrayFrame::SharedPtr ( new GrayFrame ( x, y, width, height, padding ) ) );
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

const Point2i MessageBox::size ( void ) const
{
  return Point2i ( this->coords.w, this->coords.h );
}

const Point2i MessageBox::position ( void ) const
{
  return Point2i ( this->coords.x, this->coords.y );
}

void MessageBox::setWindowTitleFont ( const IFont::RawPtr font )
{
  this->window_title = IFont::SharedPtr ( font->clone() );

  // This positions the title text of the message box on top of the second "top"
  // bordering color of GrayFrame, commented as "top1".
  //
  // The original coords.x value was + 8, but I think + 4 looks best.
  this->window_title->setPosition ( Coords ( this->coords.x + 4, this->coords.y, this->coords.width, this->coords.height ) );

  this->window_title->setText ( "INFO." );
  this->window_title->setTextJustification ( IFont::TextAlignment::MiddleLeft );

  this->drawable.push_back ( IDrawable::SharedPtr ( this->window_title ) );
}

void MessageBox::setLabelFont ( const IFont::RawPtr font )
{
  this->label = IFont::SharedPtr ( font->clone() );
  this->label->setPosition ( this->coords );

  this->drawable.push_back ( IDrawable::SharedPtr ( this->label ) );
}

void MessageBox::setLabelPosition ( const Coords& pos )
{
  if ( ! this->label ) return;

  this->label->setPosition ( pos );
}

void MessageBox::setLabelTextAlignment ( IFont::TextAlignment alignment )
{
  this->label_alignment = alignment;
}

void MessageBox::setWindowTitle ( const std::string& text )
{
  std::string text_buffer = text;

  if ( ! this->window_title ) return;

  // I LIKE TO YELL AT YOU IN ALL CAPS
  std::transform ( text_buffer.begin(), text_buffer.end(), text_buffer.begin(), ::toupper);

  this->window_title->setText ( text_buffer );
}

void MessageBox::setLabel ( const std::string& text )
{
  if ( ! this->label ) return;

  this->label->setText ( text );
}

void MessageBox::update ( void )
{
  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->update();
  }

  if ( ! this->label ) return;

  switch ( this->label_alignment )
  {
    default:
    case IFont::TextAlignment::MiddleLeft: // TODO
    {
      this->label->setTextJustification ( IFont::TextAlignment::MiddleLeft );
    }
    break;

    case IFont::TextAlignment::MiddleCenter:
    {
      this->label->setTextJustification ( IFont::TextAlignment::MiddleCenter );
    }
    break;

    case IFont::TextAlignment::MiddleRight: // TODO
    {
      this->label->setTextJustification ( IFont::TextAlignment::MiddleRight );
    }
    break;
  } // end switch
}

void MessageBox::draw ( RenderTarget target ) const
{
  if ( this->isEnabled() == false ) return;

  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->draw ( target );
  }
}


  } // namespace ui
} // namespace nom
