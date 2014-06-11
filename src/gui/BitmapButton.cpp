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

BitmapButton::BitmapButton  (
                              UIWidget* parent,
                              int64 id,
                              const Point2i& pos,
                              const Size2i& size,
                              const Texture& image
                            ) :
  Button( parent, id, pos, size ) // Base class
{
  // NOM_LOG_TRACE( NOM );

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "bitmap_button" );

  this->set_default_bitmap( image );
  this->set_pressed_bitmap( image );
  this->set_focused_bitmap( image );
  this->set_disabled_bitmap( image );

  // Default state
  this->set_button_state( Button::State::Default );

  // Initialize the default event listeners for the widget.
  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

  this->update();
}

// BitmapButton::BitmapButton( UIWidget* parent, const Texture& image ) :
//   Button( parent, parent->id(), parent->position(), parent->size() ) // Base class
// {
//   // NOM_LOG_TRACE( NOM );

//   // Use explicitly set coordinates for our minimum widget size
//   this->set_minimum_size( parent->size() );

//   // Auto-generate a name tag for our widget.
//   this->set_name( "bitmap_button" );

//   this->set_default_bitmap( image );
//   this->set_pressed_bitmap( image );
//   this->set_focused_bitmap( image );
//   this->set_disabled_bitmap( image );

//   // Default state
//   this->set_button_state( Button::State::Default );

//   // Initialize the default event listeners for the widget.
//   NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

//   this->update();
// }

BitmapButton::~BitmapButton( void )
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo BitmapButton::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

const Size2i BitmapButton::size_hint( void ) const
{
  if( this->default_bitmap_.valid() )
  {
    return( Size2i( this->default_bitmap_.bounds().w, this->default_bitmap_.bounds().h ) );
  }

  // Explicitly set by the end-user (developer):
  return this->minimum_size();
}

void BitmapButton::draw( RenderTarget& target ) const
{
  switch( this->button_state() )
  {
    default:
    case Button::State::Default:
    {
      if( this->default_bitmap_.valid() == false )
      {
        break;
      }

      this->default_bitmap_.draw( target );
      break;
    }

    case Button::State::Pressed:
    {
      if( this->pressed_bitmap_.valid() == false )
      {
        break;
      }

      this->pressed_bitmap_.draw( target );
      break;
    }

    case Button::State::Focused:
    {
      if( this->focused_bitmap_.valid() == false )
      {
        break;
      }

      this->focused_bitmap_.draw( target );
      break;
    }

    case Button::State::Disabled:
    {
      if( this->disabled_bitmap_.valid() == false )
      {
        break;
      }

      this->disabled_bitmap_.draw( target );
      break;
    }
  }

  // Draw button's label text on top of the bitmap overlay.
  Button::draw( target );
}

const Texture& BitmapButton::default_bitmap( void ) const
{
  return this->default_bitmap_;
}

const Texture& BitmapButton::pressed_bitmap( void ) const
{
  return this->pressed_bitmap_;
}

const Texture& BitmapButton::focused_bitmap( void ) const
{
  return this->focused_bitmap_;
}

const Texture& BitmapButton::disabled_bitmap( void ) const
{
  return this->disabled_bitmap_;
}

void BitmapButton::set_default_bitmap( const Texture& image )
{
  this->default_bitmap_ = image;

  this->update_bounds();
}

void BitmapButton::set_pressed_bitmap( const Texture& image )
{
  this->pressed_bitmap_ = image;

  this->update_bounds();
}

void BitmapButton::set_focused_bitmap( const Texture& image )
{
  this->focused_bitmap_ = image;

  this->update_bounds();
}

void BitmapButton::set_disabled_bitmap( const Texture& image )
{
  this->disabled_bitmap_ = image;

  this->update_bounds();
}

// Protected scope

void BitmapButton::update_bounds( void )
{
  if( this->default_bitmap_.valid() == false )
  {
    NOM_LOG_ERR( NOM, "Could not update button: the bitmap is invalid." );
    return;
  }

  // Set the rendering position of the default bitmap to the widget's
  // coordinates.
  this->default_bitmap_.set_position( this->position() );

  // Set the rendering position of the pressed bitmap to the widget's
  // coordinates.
  this->pressed_bitmap_.set_position( this->position() );

  // Set the rendering position of the pressed bitmap to the widget's
  // coordinates.
  this->focused_bitmap_.set_position( this->position() );

  // Set the rendering position of the disabled bitmap to the widget's
  // coordinates.
  this->disabled_bitmap_.set_position( this->position() );

  // Set the size of the widget to match the default bitmap button.
  this->set_size( this->default_bitmap_.bounds().w, this->default_bitmap_.bounds().h );

  Button::update_bounds();
}

void BitmapButton::on_size_changed( const UIWidgetEvent& ev )
{
  Event evt = ev.event();

  if( evt.type != SDL_WINDOWEVENT_SIZE_CHANGED )
  {
    return;
  }

  if( this->decorator() )
  {
    // Update the attached decorator (border & possibly a background)
    this->decorator()->set_bounds( ev.resized_bounds_ );
  }

  // Update ourselves with the new rendering coordinates
  this->set_bounds( ev.resized_bounds_ );

  // Ensure that the label coordinates & dimensions are updated.
  Button::on_size_changed( ev );

  // FIXME? This corrects the alignment of the Button's text label when used
  // within this class.
  Button::update_bounds();

  this->update_bounds();

  this->update();
}

// void BitmapButton::on_mouse_down( const UIWidgetEvent& ev )
// {
//   // this->set_button_state( Button::State::Pressed );
// }

// void BitmapButton::on_mouse_up( const UIWidgetEvent& ev )
// {
//   // this->set_button_state( Button::State::Default );
// }

// void BitmapButton::on_mouse_enter( const UIWidgetEvent& ev )
// {
//   // this->set_button_state( Button::State::Pressed );
// }

// void BitmapButton::on_mouse_leave( const UIWidgetEvent& ev )
// {
//   // this->set_button_state( Button::State::Default );
// }

// Private scope

void BitmapButton::update( void )
{
  // Nothing to do
}

} // namespace nom
