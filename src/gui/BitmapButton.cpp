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
#include "nomlib/gui/BitmapButton.hpp"

namespace nom {

BitmapButton::~BitmapButton( void )
{
  // NOM_LOG_TRACE( NOM );
}

BitmapButton::BitmapButton  (
                              UIWidget* parent,
                              int64 id,
                              const Point2i& pos,
                              const Size2i& size,
                              Texture* bitmap
                            ) :
  Button( parent, id, pos, size ),   // Base class
  bitmap_( bitmap )
{
  // NOM_LOG_TRACE( NOM );

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "bitmap_button" );

  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

  this->update();
}

ObjectTypeInfo BitmapButton::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

BitmapButton::SelfType& BitmapButton::operator =( const SelfType& rhs )
{
  this->bitmap_ = rhs.bitmap_;

  NOM_ASSERT( this->bitmap_ ); // nullptr check

  // this->update();

  return *this;
}

const Size2i BitmapButton::size_hint( void ) const
{
  if( this->bitmap_ != nullptr )
  {
    return( Size2i( this->bitmap_->bounds().w, this->bitmap_->bounds().h ) );
  }

  // If all else fails ... use the preset size of the widget as the preferred
  // size.
  //
  // Should we be using minimum_size here instead?
  return Button::size_hint();
  // return this->minimum_size();
}

void BitmapButton::update( void )
{
  if( this->bitmap_ == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not update button: bitmap is NULL." );
    return;
  }

  this->bitmap_->set_position( this->position() );

  this->set_bounds( IntRect( this->position().x, this->position().y, this->bitmap_->bounds().w, this->bitmap_->bounds().h ) );

  Button::update();
}

void BitmapButton::draw( RenderTarget& target ) const
{
  if( this->bitmap_ == nullptr ) return;

  this->bitmap_->draw( target );

  // Draw button's label text on top of the bitmap overlay.
  Button::draw( target );
}

bool BitmapButton::process_event( const nom::Event& ev )
{
  Point2i mouse_coords( ev.mouse.x, ev.mouse.y );

  // Button::process_event( ev );

  // FIXME (?):
  //
  // if( Button::process_event( ev ) ) return true;

  // Registered action for selection click event
  if( ev.type == SDL_MOUSEBUTTONUP )
  {
    IntRect bitmap_bounds( this->bitmap_->position().x, this->bitmap_->position().y, this->bitmap_->width(), this->bitmap_->height() );

    if( bitmap_bounds.contains( mouse_coords ) )
    {
      // Send the index and string text of the label that was selected during
      // the time of this event.
      UIWidgetEvent item_ev( 0, this->label_text(), ev );
      item_ev.set_id( this->id() );

      // Send the UI event object to the registered callback; public event slot.
      this->dispatcher()->emit( UIEvent::MOUSE_UP, item_ev );

      return true;
    } // end if mouse coordinates are within bitmap dimensions
  } // end if ev.type == SDL_MOUSEBUTTONUP

/* TODO
  // Registered event action for on mouse hover (enter)
  else if( itr->first == 2 && ev.type == SDL_MOUSEMOTION )
  {
    IntRect bitmap_bounds( this->bitmap_->position().x, this->bitmap_->position().y, this->bitmap_->width(), this->bitmap_->height() );

    if( bitmap_bounds.contains( mouse_coords ) )
    {
      UIWidgetEvent ev;

      // Send the array index in our event; this signifies which choice was
      // selected.
      ev.set_index( 0 );

      // Send the text of the selection.
      ev.set_text( this->label_text() );

      // Send the event to the registered callback for this event.
      itr->second( ev );

      return true;
    }
  }

  // Registered event action for on mouse hover (leave)
  else if( itr->first == 3 && ev.type == SDL_MOUSEMOTION )
  {
    IntRect bitmap_bounds( this->bitmap_->position().x, this->bitmap_->position().y, this->bitmap_->width(), this->bitmap_->height() );

    if( ! bitmap_bounds.contains( mouse_coords ) )
    {
      UIWidgetEvent ev;

      // Send the array index in our event; this signifies which choice was
      // selected.
      ev.set_index( 0 );

      // Send the text of the selection.
      ev.set_text( this->label_text() );

      // Send the event to the registered callback for this event.
      itr->second( ev );

      return true;
    }
  }
TODO */

  return false;
}

void BitmapButton::on_size_changed( const UIWidgetEvent& ev )
{
  Event evt = ev.event();

  if( evt.type != SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    return;
  }

  // this->set_updated( false );

  if( this->decorator() )
  {
    // Update the attached decorator (border & possibly a background)
    this->decorator()->set_bounds( ev.resized_bounds_ );
  }

  // Update ourselves with the new rendering coordinates
  this->set_bounds( ev.resized_bounds_ );

  // Ensure that the label coordinates & dimensions are updated.
  Button::on_size_changed( ev );

  this->update();
}

} // namespace nom
