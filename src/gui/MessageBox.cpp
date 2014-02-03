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

MessageBox::MessageBox( void ) :
  enabled_ ( false ),
  updated_ ( false )
{
  // NOM_LOG_TRACE( NOM );

  this->update();
}

MessageBox::~MessageBox( void )
{
  // NOM_LOG_TRACE( NOM );
}

MessageBox::self_type& MessageBox::operator =( const self_type& rhs )
{
  this->title_ = rhs.title();
  this->message_ = rhs.message();
  this->enabled_ = rhs.enabled_;
  this->updated_ = rhs.updated_;

  return *this;
}

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

  // Auto-generate a name tag for our widget.
  this->set_name( "message_box" );

  this->update();
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

void MessageBox::disable( void )
{
  this->enabled_ = false;
}

void MessageBox::enable( void )
{
  this->enabled_ = true;
}

// Text::Alignment MessageBox::title_alignment( void ) const
// {
//   return this->labels[0].alignment();
// }

// Text::Alignment MessageBox::text_alignment( void ) const
// {
//   return this->labels[1].alignment();
// }

void MessageBox::set_title_label( const Text& title )
{
  this->updated_ = false;

  // Sanity check
  if ( title.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Title Text object in nom::MessageBox is invalid" );
    return;
  }

  this->title_ = title;

  // We must handle certain text alignments in a particular way, otherwise we
  // potentially end up with text that overlaps.
  switch( this->title_.alignment() )
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

  // FIXME:
  //
  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->title_.set_alignment ( title.alignment() );

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->title_.set_features( Text::ExtraRenderingFeatures::CropText );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::set_title_text( const std::string& text )
{
  this->updated_ = false;

  // Sanity check
  if ( this->title_.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Title Text object in nom::MessageBox is invalid" );
    return;
  }

  this->title_.set_text( text );
  this->title_.set_position( this->position() );
  this->title_.set_size( this->size() );

  // FIXME (?):
  //
  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->title_.set_alignment( this->title_.alignment() );

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->title_.set_features( Text::ExtraRenderingFeatures::CropText );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

// void MessageBox::set_title_label( const std::string& text )
// {
//   this->updated_ = false;

//   this->labels[0].set_text( text );

//   this->update();
// }

// void MessageBox::set_title_font( const Font& font )
// {
//   this->updated_ = false;

//   this->labels[0].set_font( font );

//   this->update();
// }

// void MessageBox::set_title_font_size( uint point_size )
// {
//   this->updated_ = false;

//   this->labels[0].set_text_size( point_size );

//   this->update();
// }

// void MessageBox::set_title_alignment( Text::Alignment align )
// {
//   this->updated_ = false;

//   this->labels[0].set_alignment( align );

//   this->update();
// }

// void MessageBox::set_title( const std::string& text, const Font& font, uint point_size, Text::Alignment align )
// {
//   this->updated_ = false;

//   this->set_title_label( text );
//   this->set_title_font( font );
//   this->set_title_font_size( point_size );
//   this->set_title_alignment( align );

//   this->update();
// }

void MessageBox::set_message_label( const Text& text )
{
  this->updated_ = false;

  // Sanity check
  if ( text.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Message Text object in nom::MessageBox is invalid" );
    return;
  }

  this->message_ = text;

  // We must handle certain text alignments in a particular way, otherwise we
  // potentially end up with text that overlaps.
  switch( this->message_.alignment() )
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
      this->message_.set_position( Point2i( this->position().x, this->position().y + text.height() ) );
      break;
    }
  }

  this->message_.set_size( this->size() );

  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->message_.set_alignment( text.alignment() );

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->message_.set_features( Text::ExtraRenderingFeatures::CropText );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

// void MessageBox::set_text( const std::string& text, const Font& font, uint point_size, Text::Alignment align )
// {
//   this->updated_ = false;

//   this->set_text_label( text );
//   this->set_text_font( font );
//   this->set_text_font_size( point_size );
//   this->set_text_alignment( align );

//   this->update();
// }

// void MessageBox::set_text_label( const std::string& text )
// {
//   this->updated_ = false;

//   this->labels[1].set_text( text );

//   this->update();
// }

// void MessageBox::set_text_font( const Font& font )
// {
//   this->updated_ = false;

//   this->labels[1].set_font( font );

//   this->update();
// }

// void MessageBox::set_text_font_size( uint point_size )
// {
//   this->updated_ = false;

//   this->labels[1].set_text_size( point_size );

//   this->update();
// }

// void MessageBox::set_text_alignment( Text::Alignment align )
// {
//   this->updated_ = false;

//   this->labels[1].set_alignment( align );

//   this->update();
// }

void MessageBox::set_message_text( const std::string& text )
{
  this->updated_ = false;

  // Sanity check
  if ( this->message_.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Message Text object in nom::MessageBox is invalid" );
    return;
  }

  this->message_.set_text( text );
  this->message_.set_position( this->position() );
  this->message_.set_size( this->size() );

  // FIXME (?):
  //
  // In order to preserve the text alignment of the original object, we must
  // copy the state of the original alignment *after* we set the positioning
  // on the new object we create for this class
  this->message_.set_alignment ( this->message_.alignment() );

  // Prevent rendering of text that is longer in length than its container's
  // set size parameters (see above).
  this->message_.set_features( Text::ExtraRenderingFeatures::CropText );

  // Update our message box -- we have new objects in the rendering pipeline!
  this->update();
}

void MessageBox::draw( RenderTarget& target ) const
{
  if ( this->enabled() == false ) return;

  UIWidget::draw( target );

  // NOTE: Validity is checked before attempting to render.
  this->title_.draw ( target );
  this->message_.draw ( target );
}

// Protected scope

void MessageBox::update( void )
{
  if ( this->updated_ == true ) return;

  UIWidget::update();

  this->updated_ = true;
}

bool MessageBox::process_event( const nom::Event& ev )
{
  Point2i mouse_coords( ev.mouse.x, ev.mouse.y );

  // Base class
  UIWidget::process_event( ev );

  // FIXME (?):
  //
  // if( UIWidget::process_event( ev ) ) return true;

  // Execute the registered action for a single mouse click event
  if( ev.type == SDL_MOUSEBUTTONDOWN )
  {
    IntRect window( this->global_bounds() );

    // Execute the registered action for a single mouse click on the bounds of a
    // caption or message event.
    if( this->title_bounds().contains( mouse_coords ) )
    {
      UIWidgetEvent item;

      // Send the array index in our event; this signifies which choice was
      // selected -- the caption text label.
      item.set_index( 0 );

      // Send the text of the selection.
      item.set_text( this->title_.text() );

      // Set the associated nom::Event object for this UI event.
      item.set_event( ev );

      // Associate the widget's unique identifier for this widget's event
      // object.
      item.set_id( this->id() );

      // Send the UI event object to the registered callback; public event slot.
      this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

      // Processed events
      return true;
    }

    else if( this->message_bounds().contains( mouse_coords ) )
    {
      UIWidgetEvent item;

      // Send the array index in our event; this signifies which choice was
      // selected -- the message text label.
      item.set_index( 1 );

      // Send the text of the selection.
      item.set_text( this->message_.text() );

      // Set the associated nom::Event object for this UI event.
      item.set_event( ev );

      // Associate the widget's unique identifier for this widget's event
      // object.
      item.set_id( this->id() );

      // Send the UI event object to the registered callback; public event slot.
      this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

      // Processed events
      return true;
    }

    // FIXME: This method is duplicating what UIWidget::process_event covers..?
    else if( window.contains( mouse_coords ) )
    {
      UIWidgetEvent item;

      // Send an invalid index state to signify that no text labels, groups,
      // or similar are selected.
      item.set_index( -1 );

      // Send the window object instance's name identifier.
      item.set_text( this->name() );

      // Set the associated nom::Event object for this UI event.
      item.set_event( ev );

      // Associate the widget's unique identifier for this widget's event
      // object.
      item.set_id( this->id() );

      // Send the UI event object to the registered callback; public event slot.
      this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

      // Processed events.
      return true;
    }

  } // end if ev.type == SDL_MOUSEBUTTONDOWN

  // No processed events
  return false;
}

// Private scope

const Text& MessageBox::title( void ) const
{
  return this->title_;
}

const Text& MessageBox::message( void ) const
{
  return this->message_;
}

} // namespace nom
