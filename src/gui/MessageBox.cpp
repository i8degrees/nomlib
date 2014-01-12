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
#include "nomlib/gui/MessageBox.hpp"

namespace nom {

MessageBox::MessageBox ( void ) :
  enabled_ ( true ),
  updated ( false )
{
  NOM_LOG_TRACE(NOM);
}

MessageBox::~MessageBox ( void )
{
  NOM_LOG_TRACE ( NOM );
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          enum MessageBox::Style style,
                          const Gradient& background
                        ) :
  enabled_ ( true ),
  updated ( false )
{
  int32 padding = 1;

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  Color4i gradient_color[2];
  gradient_color[0] = background.start_color();
  gradient_color[1] = background.end_color();

  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, Coords ( this->coords.x, this->coords.y, this->coords.w, this->coords.h ), 0, 0, background.fill_direction() ) ) );

  if ( style == MessageBox::Style::Gray )
  {
    this->drawable.push_back ( GrayFrame::SharedPtr ( new GrayFrame ( x, y, width, height, padding ) ) );
  }

  this->update();
}

MessageBox::MessageBox  (
                          int32 x, int32 y, int32 width, int32 height,
                          GrayFrame::SharedPtr style,
                          Gradient::SharedPtr background
                        ) :
  enabled_ ( true ),
  updated ( false )
{
  int32 padding = 1;

  // init geometry coords w/ arguments list
  this->coords = Coords ( x, y, width, height );

  Color4i gradient_color[2];
  gradient_color[0] = background->start_color();
  gradient_color[1] = background->end_color();
  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, Coords ( this->coords.x, this->coords.y, this->coords.w, this->coords.h ), 0, 0, background->fill_direction() ) ) );

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

  this->update();
}

MessageBox& MessageBox::operator= ( const MessageBox& other )
{
  this->drawable = other.drawable;
  this->labels = other.labels;
  this->coords = other.coords;
  this->enabled_ = other.enabled_;
  this->updated = other.updated;

  return *this;
}

bool MessageBox::enabled ( void ) const
{
  if ( this->enabled_ == false ) return false;

  return true;
}

const std::string MessageBox::title ( void )
{
  // nom::Label should be handling all of the necessary validity checks for us
  return this->labels[0].text();
}

const std::string MessageBox::text ( void )
{
  // nom::Label should be handling all of the necessary validity checks for us
  return this->labels[1].text();
}

const Point2i MessageBox::size ( void ) const
{
  return Point2i ( this->coords.w, this->coords.h );
}

const Point2i MessageBox::position ( void ) const
{
  return Point2i ( this->coords.x, this->coords.y );
}

void MessageBox::disable ( void )
{
  this->enabled_ = false;
}

void MessageBox::enable ( void )
{
  this->enabled_ = true;
}

void MessageBox::set_title ( const Label& title )
{
  this->updated = false;

  if ( title.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Label text for nom::MessageBox is invalid" );
    return;
  }

  this->labels[0] = title;
  // This positions the title text of the message box on top of the second "top"
  // bordering color of GrayFrame, commented as "top1".
  //
  // The original coords.x value was + 8, but I think + 4 looks best.
  this->labels[0].set_position ( IntRect ( this->coords.x + 4, this->coords.y, this->coords.w, this->coords.h ) );

  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->labels[0].set_alignment ( title.alignment() );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::set_text ( const Label& text )
{
  this->updated = false;

  if ( text.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Label text for nom::MessageBox is invalid" );
    return;
  }

  this->labels[1] = text;
  this->labels[1].set_position ( IntRect ( this->coords.x, this->coords.y, this->coords.w, this->coords.h ) );

  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->labels[1].set_alignment ( text.alignment() );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::update ( void )
{
  if ( this->updated == true ) return;

  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->update();
  }

  this->updated = true;
}

void MessageBox::draw ( RenderTarget target ) const
{
  if ( this->enabled() == false ) return;

  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->draw ( target );
  }

  // nom::Label should be handling all of the necessary validity checks for us;
  // if a Label object is not valid, the routine should return immediately,
  // discarding any data prepared within that frame.
  this->labels[0].draw ( target );
  this->labels[1].draw ( target );
}

} // namespace nom
