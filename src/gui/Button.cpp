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
#include "nomlib/gui/Button.hpp"

// Private headers
#include "nomlib/graphics/fonts/FontMetrics.hpp"

// Forward declarations
#include "nomlib/graphics/Texture.hpp"
#include "nomlib/graphics/Text.hpp"

#include "nomlib/gui/IDecorator.hpp"
#include "nomlib/gui/UIStyle.hpp"
#include "nomlib/gui/UIEventDispatcher.hpp"
#include "nomlib/gui/UIEvent.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"

namespace nom {

Button::Button  (
                  UIWidget* parent,
                  int64 id,
                  const Point2i& pos,
                  const Size2i& size
                ) :
  UIWidget( parent, id, pos, size )   // Base class
{
  // NOM_LOG_TRACE( NOM );

  // Set the size policy of the widget to use explicitly set size dimensions.
  //
  // Note that the size policy is only used when the widget is used inside a
  // layout.
  if( size != Size2i::null )
  {
    // Use explicitly set coordinates as the widget's minimum size requirements.
    //
    // Note that if size is invalid (NULL), the minimum size returned will be
    // Size2i(0,0)
    //
    // Note that the size policy is only used when the widget is used inside a
    // layout.
    this->set_minimum_size( size );

    // this->set_size_policy( UILayoutPolicy::Policy::Minimum, UILayoutPolicy::Policy::Fixed );
    this->set_size_policy( UILayoutPolicy::Policy::Fixed, UILayoutPolicy::Policy::Fixed );
  }

  // Set the size policy of the widget to use dimensions that are calculated
  // with respect to the text label string, font, point size, etc.
  //
  // Note that the size policy is only used when the widget is used inside a
  // layout.
  else
  {
    this->set_size_policy( UILayoutPolicy::Policy::Preferred, UILayoutPolicy::Policy::Fixed );
  }

  // Widget's focus type.
  this->set_focus_policy( FocusPolicy::StrongFocus );

  // Auto-generate a name tag for our widget.
  this->set_name( "button" );

  // Default state
  this->set_button_state( Button::State::Default );

  // Use UIWidget's default event listener for UIEvent::ON_WINDOW_SIZE_CHANGED;
  // calls UIWidget::on_size_changed. This is necessary for use within a layout.

  // Use UIWidget's default event listener for UIEvent::ON_WIDGET_UPDATE; calls
  // UIWidget::on_update. This is necessary for use within a layout.
}

// Button::Button( UIWidget* parent ) :
//   UIWidget( parent, parent->id(), parent->position(), parent->size() ) // Base class
// {
//   // NOM_LOG_TRACE( NOM );

//   // Use explicitly set coordinates for our minimum widget size
//   this->set_minimum_size( parent->size() );

//   this->set_size_policy( UILayoutPolicy::Policy::Preferred, UILayoutPolicy::Policy::Fixed );

//   // Auto-generate a name tag for our widget.
//   this->set_name( "button" );

//   // Default state
//   this->set_button_state( Button::State::Default );

//   // Initialize the default event listeners for the widget.
//   NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

//   NOM_CONNECT_UIEVENT( this, UIEvent::ON_WIDGET_UPDATE, this->on_update );

//   // Widget's focus type.
//   this->set_focus_policy( FocusPolicy::StrongFocus );
// }

Button::~Button( void )
{
  // NOM_LOG_TRACE( NOM );
}

const Size2i Button::minimum_size( void ) const
{
  Size2i msize = UIWidget::minimum_size();
  Size2i min;

  if( msize > Size2i( 0, 0 ) )
  {
     min = msize;
  }
  else
  {
    min = this->size_hint();
  }

  // NOM_DUMP( this->name() );
  // NOM_DUMP( min );

  return min;
}

const Size2i Button::size_hint( void ) const
{
  // Maximum text width requirements for the label (with respect to font).
  //
  // We dedicate two times the width required in order to: a) help account for
  // dynamic length text labels that are set after initialization of the
  // layout -- the layout manager *should* catch this early enough, but just
  // in case; b) Simple, minimal approach to getting the look of the button
  // to *feel* right in terms of size dimensions (in comparison to standard GUI
  // elements).
  int total_text_width = this->label_.width() * 2;

  // Total text height requirements for the label (with respect to font).
  //
  // Note that this variable will be scaled up by a factor of two as well; see
  // the above note regarding total_text_width for my reasoning logic on
  // requesting this.
  sint total_text_height = 0;

  uint point_size = nom::DEFAULT_FONT_SIZE;

  FontMetrics face = this->label_.font()->metrics();

  // NOM_DUMP( this->name() );
  // NOM_DUMP( face.name );
  // NOM_DUMP( this->label_->text() );

  // Use the point size of the widget's style, if one has been set:
  if( this->style() != nullptr )
  {
    point_size = this->style()->font_size();
  }

  total_text_height = this->label_.font()->newline( point_size ) * 2;

  return Size2i( total_text_width, total_text_height );
}

ObjectTypeInfo Button::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

void Button::draw( RenderTarget& target ) const
{
  // UIWidget::draw( target );

  if( this->label_.valid() == true )
  {
    this->label_.draw( target );
  }
}

Button::State Button::button_state( void ) const
{
  return this->button_state_;
}

const std::string& Button::label_text( void ) const
{
  return this->label_.text();
}

void Button::set_label( const std::string& text )
{
  this->text_ = text;

  this->label_.set_font( this->font() );

  // Custom style
  if( this->style() != nullptr )
  {
    this->label_.set_text_size( this->style()->font_size() );
    this->label_.set_alignment( this->style()->text_alignment() );
    this->label_.set_style( this->style()->font_style() );
    this->label_.set_color( this->style()->font_color() );
  }
  else  // Default style
  {
    this->label_.set_alignment( Anchor::MiddleCenter );
  }

  this->label_.set_text( this->text_ );
  this->label_.set_position( this->position() );
  this->label_.set_size( this->size() );

  this->update();
}

void Button::set_button_state( Button::State state )
{
  this->button_state_ = state;
}

// Protected scope

void Button::on_mouse_down( const Event& evt )
{
  this->set_button_state( Button::State::Pressed );

  UIWidgetEvent ev;
  ev.set_type( UIEvent::MOUSE_DOWN );
  ev.set_event( evt );
  ev.set_id( this->id() );
  ev.set_index( this->button_state() );
  ev.set_text( this->label_text() );

  this->dispatcher()->emit( ev );
}

void Button::on_mouse_up( const Event& evt )
{
  this->set_button_state( Button::State::Default );

  UIWidgetEvent ev;
  ev.set_type( UIEvent::MOUSE_UP );
  ev.set_event( evt );
  ev.set_id( this->id() );
  ev.set_index( this->button_state() );
  ev.set_text( this->label_text() );

  this->dispatcher()->emit( ev );
}

void Button::on_mouse_enter( const Event& evt )
{
  // this->set_button_state( Button::State::Pressed );

  // Send the button state and text string of the set label at the time of
  // the event.
  UIWidgetEvent ev;
  ev.set_type( UIEvent::MOUSE_MOTION_ENTER );
  ev.set_event( evt );
  ev.set_id( this->id() );
  ev.set_index( this->button_state() );
  ev.set_text( this->label_text() );

  this->dispatcher()->emit( ev );
}

void Button::on_mouse_leave( const Event& evt )
{
  // this->set_button_state( Button::State::Default );

  // Send the button state and text string of the set label at the time of
  // the event.
  UIWidgetEvent ev;
  ev.set_type( UIEvent::MOUSE_MOTION_LEAVE );
  ev.set_event( evt );
  ev.set_id( this->id() );
  ev.set_index( this->button_state() );
  ev.set_text( this->label_text() );

  this->dispatcher()->emit( ev );
}

void Button::update( void )
{
  this->label_.set_position( this->position() );
  this->label_.set_size( this->size() );

  // Sync the text label's font; this is necessary when the font is changed
  // after the text has been set (using Button::set_label).
  if( this->label_.font() != this->font() )
  {
    this->label_.set_font( this->font() );
  }
}

} // namespace nom
