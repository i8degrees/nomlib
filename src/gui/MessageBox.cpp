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
  updated_ ( false )
{
  //NOM_LOG_TRACE(NOM);
}

MessageBox::~MessageBox ( void )
{
  //NOM_LOG_TRACE(NOM);
}

/*
MessageBox::MessageBox ( const MessageBox& copy ) :
  Transformable { copy.position(), copy.size() },
  drawable ( copy.drawable ),
  labels ( copy.labels ),
  enabled_ ( copy.enabled_ ),
  updated_ ( copy.updated_ )
{
  //NOM_LOG_TRACE(NOM);
}
*/

/*
MessageBox& MessageBox::operator = ( const MessageBox& other )
{
  this->drawable = other.drawable;
  this->labels = other.labels;
  this->position_ = other.position();
  this->size_ = other.size();
  this->enabled_ = other.enabled_;
  this->updated_ = other.updated_;

  return *this;
}
*/

MessageBox::MessageBox  ( const Point2i& pos, const Size2i& size,
                          enum MessageBox::Style style,
                          const Gradient& background
                        ) :
  enabled_ ( true ),
  updated_ ( false )
{
  //NOM_LOG_TRACE(NOM);
  int padding = 1;

  Color4i gradient_color[2];
  gradient_color[0] = background.start_color();
  gradient_color[1] = background.end_color();

  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, pos, size, Point2i(0, 0), background.fill_direction() ) ) );

  if ( style == MessageBox::Style::Gray )
  {
    this->drawable.push_back ( GrayWindow::SharedPtr ( new GrayWindow ( pos, size, padding) ) );
  }

  // init geometry coords w/ arguments list
  this->set_position ( pos );
  this->set_size ( size );

  this->update();
}

MessageBox::MessageBox  ( const Point2i& pos, const Size2i& size,
                          GrayWindow::SharedPtr style,
                          Gradient::SharedPtr background
                        ) :
  enabled_ ( true ),
  updated_ ( false )
{
  //NOM_LOG_TRACE(NOM);
  int padding = 1;

  Color4i gradient_color[2];
  gradient_color[0] = background->start_color();
  gradient_color[1] = background->end_color();
  this->drawable.push_back ( Gradient::SharedPtr ( new Gradient ( gradient_color, pos, size, Point2i(0, 0), background->fill_direction() ) ) );

  if ( style != nullptr )
  {
    style->set_position ( pos );
    style->set_size ( size );
    style->set_padding ( padding );
    this->drawable.push_back ( style );
  }
  else // default frame style
  {
    this->drawable.push_back ( GrayWindow::SharedPtr ( new GrayWindow ( pos, size, padding ) ) );
  }

  // init geometry coords w/ arguments list
  this->set_position ( pos );
  this->set_size ( size );

  this->update();
}

/*
const Size2i& MessageBox::size ( void ) const
{
  return this->size();
}
*/

/*
const Point2i& MessageBox::position ( void ) const
{
  return this->position();
}
*/

bool MessageBox::enabled ( void ) const
{
  if ( this->enabled_ == false ) return false;

  return true;
}

const std::string& MessageBox::title_string ( void ) const
{
  // nom::Text should be handling all of the necessary validity checks for us
  return this->labels[0].text();
}

const std::string& MessageBox::text_string ( void ) const
{
  // nom::Text should be handling all of the necessary validity checks for us
  return this->labels[1].text();
}

const IntRect MessageBox::title_bounds ( void ) const
{
  return IntRect ( this->labels[0].position().x, this->labels[0].position().y, this->labels[0].width(), this->labels[0].height() );
}

const IntRect MessageBox::text_bounds ( void ) const
{
  return IntRect ( this->labels[1].position().x, this->labels[1].position().y, this->labels[1].width(), this->labels[1].height() );
}

void MessageBox::disable ( void )
{
  this->enabled_ = false;
}

void MessageBox::enable ( void )
{
  this->enabled_ = true;
}

void MessageBox::set_title ( const Text& title )
{
  this->updated_ = false;

  if ( title.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Text for nom::MessageBox is invalid" );
    return;
  }

  this->labels[0] = title;
  // This positions the title text of the message box on top of the second "top"
  // bordering color of nom::GrayWindow, commented as "top1".
  //
  // The original coords.x value was + 8, but I think + 4 looks best.
  this->labels[0].set_position ( Point2i ( this->position().x + 4, this->position().y ) );
  this->labels[0].set_size ( this->size() );

  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->labels[0].set_alignment ( title.alignment() );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::set_text ( const Text& text )
{
  this->updated_ = false;

  if ( text.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Text for nom::MessageBox is invalid" );
    return;
  }

  this->labels[1] = text;
  this->labels[1].set_position ( this->position() );
  this->labels[1].set_size ( this->size() );

  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->labels[1].set_alignment ( text.alignment() );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::update ( void )
{
  if ( this->updated_ == true ) return;

  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->update();
  }

  this->updated_ = true;
}

void MessageBox::draw ( RenderTarget target ) const
{
  if ( this->enabled() == false ) return;

  for ( auto it = this->drawable.begin(); it != this->drawable.end(); ++it )
  {
    IDrawable::SharedPtr obj = *it;
    obj->draw ( target );
  }

  // nom::Text should be handling all of the necessary validity checks for us;
  // if a Text object is not valid, the routine should return immediately,
  // discarding any data prepared within that frame.
  this->labels[0].draw ( target );
  this->labels[1].draw ( target );
}

} // namespace nom
