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

// Forward declarations
#include "nomlib/gui/UIEvent.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"
#include "nomlib/gui/UIEventDispatcher.hpp"

namespace nom {

MessageBox::MessageBox  (
                          UIWidget* parent,
                          int64 id,
                          const Point2i& pos,
                          const Size2i& size
                        ) :
  UIWidget( parent, id, pos, size ),
  enabled_ ( true ),
  updated_ ( false )
{
  // NOM_LOG_TRACE( NOM );

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "message_box" );

  // Initialize the default event listeners for the widget.
  // NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

  // this->update();
}

MessageBox::~MessageBox( void )
{
  // NOM_LOG_TRACE( NOM );
}

bool MessageBox::enabled( void ) const
{
  if ( this->enabled_ == false ) return false;

  return true;
}

const std::string& MessageBox::title_text( void ) const
{
  return this->title_.text();
}

const std::string& MessageBox::message_text( void ) const
{
  return this->message_.text();
}

const IntRect MessageBox::title_bounds( void ) const
{
  return IntRect( this->title_.global_bounds() );
}

const IntRect MessageBox::message_bounds( void ) const
{
  return IntRect( this->message_.global_bounds() );
}

Text::Alignment MessageBox::title_alignment( void ) const
{
  return this->title_.alignment();
}

Text::Alignment MessageBox::message_alignment( void ) const
{
  return this->message_.alignment();
}

void MessageBox::disable( void )
{
  this->enabled_ = false;
}

void MessageBox::enable( void )
{
  this->enabled_ = true;
}

void MessageBox::set_title( const std::string& text, const Font& font, uint point_size )
{
  this->updated_ = false;

  this->set_title_text( text );
  this->set_title_font( font );
  this->set_title_font_size( point_size );
  this->set_title_alignment( Text::Alignment::TopLeft );

  this->update();
}

void MessageBox::set_title_text( const std::string& text )
{
  this->updated_ = false;

  this->title_.set_text( text );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::set_title_font( const Font& font )
{
  this->updated_ = false;

  this->title_.set_font( font );

  this->update();
}

void MessageBox::set_title_font_size( uint point_size )
{
  this->updated_ = false;

  this->title_.set_text_size( point_size );

  this->update();
}

void MessageBox::set_title_alignment( Text::Alignment align )
{
  this->updated_ = false;

  this->title_.set_alignment( align );

  this->update();
}

void MessageBox::set_message( const std::string& text, const Font& font, uint point_size )
{
  this->updated_ = false;

  this->set_message_text( text );
  this->set_message_font( font );
  this->set_message_font_size( point_size );
  this->set_message_alignment( Text::Alignment::MiddleCenter );

  this->update();
}

void MessageBox::set_message_text( const std::string& text )
{
  this->updated_ = false;

  this->message_.set_text( text );

  this->update();
}

void MessageBox::set_message_font( const Font& font )
{
  this->updated_ = false;

  this->message_.set_font( font );

  this->update();
}

void MessageBox::set_message_font_size( uint point_size )
{
  this->updated_ = false;

  this->message_.set_text_size( point_size );

  this->update();
}

void MessageBox::set_message_alignment( Text::Alignment align )
{
  this->updated_ = false;

  this->message_.set_alignment( align );

  this->update();
}

void MessageBox::draw( RenderTarget& target ) const
{
  // Enabling this fixes the rendering of any set decorators, when running in
  // circumstances where we (essentially) wish to use this object as an
  // intermediate GUI object (rather than the retained mode that we officially
  // support), such as in the case of examples/app.cpp.
  //
  // TODO: Carefully consider the possible performance ramifications of calling
  // the base widget by default. (As an absolute *last resort*, perhaps an
  // alternative could be an internal flagging of state? I think, probably not...)
  //
  // NOTE: This is a temporary patch for fixing the lack of rendered decorators
  // for the message boxes used in examples/app.cpp.
  if( this->parent() == nullptr )
  {
    UIWidget::draw( target );
  }

  if ( this->enabled() == false ) return;

  if( this->title_.valid() )
  {
    this->title_.draw ( target );
  }

  if( this->message_.valid() )
  {
    this->message_.draw ( target );
  }
}

// Protected scope

void MessageBox::update( void )
{
  // if ( this->updated_ == true ) return;

  // Enabling this fixes the updating of any set decorators, when running in
  // circumstances where we (essentially) wish to use this object as an
  // intermediate GUI object (rather than the retained mode that we officially
  // support), such as in the case of examples/app.cpp.
  //
  // TODO: Carefully consider the possible performance ramifications of calling
  // the base widget by default. (As an absolute *last resort*, perhaps an
  // alternative could be an internal flagging of state? I think, probably not...)
  //
  // NOTE: This is a temporary patch for fixing the lack of rendered decorators
  // for the message boxes used in examples/app.cpp.
  if( this->parent() == nullptr )
  {
    UIWidget::update();
  }

  if( this->title_.font().valid() == false )
  {
    this->title_.set_font( this->font() );
  }

  if( this->message_.font().valid() == false )
  {
    this->message_.set_font( this->font() );
  }

  // We must handle certain text alignments in a particular way, otherwise we
  // potentially end up with text that overlaps.
  switch( this->title_alignment() )
  {
    default:
    {
      this->title_.set_position( this->position() );
      break;
    }

    case Text::Alignment::TopLeft:
    {
      // This positions the title text of the message box on top of the second "top"
      // bordering color of nom::GrayFrame, commented as "top1".
      //
      // The original coords.x value was + 8, but I think + 4 looks best.
      this->title_.set_position( Point2i ( this->position().x + 4, this->position().y ) );
      break;
    }
  }

  this->title_.set_size( this->size() );

  // if( this->title_.alignment() != this->title_alignment() )
  // {
    // this->title_.set_alignment( this->title_alignment() );
  // }

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->title_.set_features( Text::ExtraRenderingFeatures::CropText );

  // We must handle certain text alignments in a particular way, otherwise we
  // potentially end up with text that overlaps.
  switch( this->message_alignment() )
  {
    default:
    {
      this->message_.set_position( this->position() );
      break;
    }

    // Ensure that there is enough spacing between the title (caption) label
    // text and the message label text.
    case Text::Alignment::TopLeft:
    case Text::Alignment::TopCenter:
    case Text::Alignment::TopRight:
    {
      // this->message_.set_position( Point2i( this->position().x, this->position().y + text.height() / 2 ) );
      this->message_.set_position( Point2i( this->position().x, this->position().y + this->message_bounds().h ) );
      break;
    }
  }

  this->message_.set_size( this->size() );

  // if( this->message_.alignment() != this->message_alignment() )
  // {
    // this->message_.set_alignment( this->message_alignment() );
  // }

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->message_.set_features( Text::ExtraRenderingFeatures::CropText );

  this->updated_ = true;
}

void MessageBox::on_mouse_down( const Event& evt )
{
  if( evt.type == SDL_MOUSEBUTTONDOWN )
  {
    UIWidgetEvent wev;
    Point2i mouse( evt.mouse.x, evt.mouse.y );

    IntRect title_bounds = this->title_bounds();
    IntRect message_bounds = this->message_bounds();

    if( title_bounds.contains( mouse ) )
    {
      wev = UIWidgetEvent( 0, this->title_text(), evt, this->id() );

      // Send the UI event object to the registered private event callback.
      this->dispatcher()->emit( UIEvent::ON_MOUSE_DOWN, wev );

      // Send the UI event object to the registered public event callback.
      this->dispatcher()->emit( UIEvent::MOUSE_DOWN, wev );
    }
    else if( message_bounds.contains( mouse ) )
    {
      wev = UIWidgetEvent( 1, this->message_text(), evt, this->id() );

      // Send the UI event object to the registered private event callback.
      this->dispatcher()->emit( UIEvent::ON_MOUSE_DOWN, wev );

      // Send the UI event object to the registered public event callback.
      this->dispatcher()->emit( UIEvent::MOUSE_DOWN, wev );
    }
  } // end if evt.type is SDL_MOUSEBUTTONDOWN
}

} // namespace nom
