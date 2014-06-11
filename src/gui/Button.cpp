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

  // Use explicitly set coordinates for our minimum widget size
  this->set_minimum_size( size );

  // Auto-generate a name tag for our widget.
  this->set_name( "button" );

  // Default state
  this->set_button_state( Button::State::Default );

  // Initialize the default event listeners for the widget.
  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WINDOW_SIZE_CHANGED, this->on_size_changed );

  NOM_CONNECT_UIEVENT( this, UIEvent::ON_WIDGET_UPDATE, this->on_update );

  // Widget's focus type.
  this->set_focus_policy( FocusPolicy::StrongFocus );
}

// Button::Button( UIWidget* parent ) :
//   UIWidget( parent, parent->id(), parent->position(), parent->size() ) // Base class
// {
//   // NOM_LOG_TRACE( NOM );

//   // Use explicitly set coordinates for our minimum widget size
//   this->set_minimum_size( parent->size() );

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

const Size2i Button::size_hint( void ) const
{
  // Total text height requirements for stored items
  sint total_text_height = 0;

  // Default point size of font
  uint point_size = nom::DEFAULT_FONT_SIZE;

  // Calculate the total text height requirements for the widget.
  FontMetrics face = this->label_.font()->metrics();

  // NOM_DUMP( this->name() );
  // NOM_DUMP( face.name );
  // NOM_DUMP( this->label_->text() );

  // Use the point size of the widget's style, if one has been set:
  if( this->style() != nullptr )
  {
    point_size = this->style()->font_size();
  }

  // Maximum pixel height of the font's glyph
  total_text_height += this->label_.font()->newline( point_size );

  // NOM_DUMP( this->name() );
  // NOM_DUMP( total_text_height );

  // If we have calculated a total text height requirement, we can stop here,
  // using the total text height for our preferred height field.
  if( total_text_height > 0 )
  {
    // Text label's width & height (with respect to rendered font):
    return Size2i( this->label_.width(), total_text_height );
  }

  // We do not have any text labels stored, so assume a widget with a height
  // large enough for one item.
  if( this->font().valid() == true )
  {
    // NOM_DUMP( this->name() );
    // NOM_DUMP( this->font()->newline( point_size ) );

    // Text label's width & height (with respect to rendered font):
    return Size2i( this->label_.width(), this->font()->newline( point_size ) );
  }

  // Explicitly set by the end-user (developer):
  return Size2i( this->minimum_size() );
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

  NOM_ASSERT( this->style() != nullptr );

  this->label_.set_font( this->font() );
  this->label_.set_text_size( this->style()->font_size() );
  this->label_.set_text( this->text_ );
  this->label_.set_position( this->position() );
  this->label_.set_size( this->size() );
  this->label_.set_alignment( this->style()->text_alignment() );
  this->label_.set_style( this->style()->font_style() );
  this->label_.set_color( this->style()->font_color() );

  this->update_bounds();
  this->update();
}

void Button::set_button_state( Button::State state )
{
  this->button_state_ = state;
}

// Protected scope

void Button::update_bounds( void )
{
  this->label_.set_position( this->position() );
  this->label_.set_size( this->size() );
}

void Button::on_update( const UIWidgetEvent& ev )
{
  // NOM_LOG_TRACE( NOM );

  Event evt = ev.event();

  if( evt.type != UIEvent::ON_WIDGET_UPDATE )
  {
    return;
  }

  this->label_.set_font( this->font() );

  this->update_bounds();
  this->update();
}

void Button::on_size_changed( const UIWidgetEvent& ev )
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

  // Updating the text label's coordinates and dimensions also ensures that its
  // alignment is recalculated.
  this->label_.set_position( this->position() );
  this->label_.set_size( this->size() );

  this->update_bounds();

  this->update();
}

void Button::on_mouse_down( const UIWidgetEvent& ev )
{
  this->set_button_state( Button::State::Pressed );

  UIWidgetEvent evt( this->button_state(), this->name(), ev.event(), this->id() );

  // Send the UI event object to the registered private event callback.
  this->dispatcher()->emit( UIEvent::ON_MOUSE_DOWN, evt );

  // Send the UI event object to the registered public event callback.
  this->dispatcher()->emit( UIEvent::MOUSE_DOWN, evt );
}

void Button::on_mouse_up( const UIWidgetEvent& ev )
{
  this->set_button_state( Button::State::Default );

  UIWidgetEvent evt( this->button_state(), this->name(), ev.event(), this->id() );

  // Send the UI event object to the registered private event callback.
  this->dispatcher()->emit( UIEvent::ON_MOUSE_DOWN, evt );

  // Send the UI event object to the registered public event callback.
  this->dispatcher()->emit( UIEvent::MOUSE_DOWN, evt );
}

void Button::on_mouse_enter( const UIWidgetEvent& ev )
{
  // this->set_button_state( Button::State::Pressed );

  // Send the button state and text string of the set label at the time of
  // the event.
  UIWidgetEvent evt( this->button_state(), this->label_text(), ev.event(), this->id() );

  // Send the UI event object to the registered private event callback.
  this->dispatcher()->emit( UIEvent::ON_MOUSE_MOTION_ENTER, evt );

  // Send the UI event object to the registered public event callback.
  this->dispatcher()->emit( UIEvent::MOUSE_MOTION_ENTER, evt );
}

void Button::on_mouse_leave( const UIWidgetEvent& ev )
{
  // this->set_button_state( Button::State::Default );

  // Send the button state and text string of the set label at the time of
  // the event.
  UIWidgetEvent evt( this->button_state(), this->label_text(), ev.event(), this->id() );

  // Send the UI event object to the registered private event callback.
  this->dispatcher()->emit( UIEvent::ON_MOUSE_MOTION_LEAVE, evt );

  // Send the UI event object to the registered public event callback.
  this->dispatcher()->emit( UIEvent::MOUSE_MOTION_LEAVE, evt );
}

// Private scope

void Button::update( void )
{
  // Nothing to do
}

} // namespace nom
