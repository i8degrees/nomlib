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
#include "nomlib/gui/UILayout.hpp"

// Forward declarations
#include "nomlib/gui/UIWidget.hpp"
#include "nomlib/gui/UISpacerItem.hpp"

namespace nom {

UILayout::UILayout( void ) :
  spacing_{ 1 },
  contraints_{ UILayout::SetDefaultConstraint },
  directions_{ Orientations::Horizontal }
{
  // NOM_LOG_TRACE( NOM );
}

UILayout::~UILayout( void )
{
  // NOM_LOG_TRACE( NOM );
}

UILayout::UILayout( UIWidget* parent ) :
  spacing_{ 1 },
  contraints_{ UILayout::SetDefaultConstraint },
  directions_{ Orientations::Horizontal }
{
  // NOM_LOG_TRACE( NOM );

  this->parent_ = parent;

  this->add_child_widget( parent );
}

Orientations UILayout::directions( void ) const
{
  return this->directions_;
}

IntRect UILayout::bounds( void ) const
{
  return this->bounds_;
}

void UILayout::invalidate( void )
{
  this->invalid_ = true;

  this->update();
}

bool UILayout::empty( void ) const
{
  if( this->count() == 0 )
  {
    return true;
  }

  return false;
}

UILayout::raw_ptr UILayout::layout( void )
{
  return this;
}

UIWidget* UILayout::parent( void ) const
{
  return this->parent_;
}

Size2i UILayout::maximum_size( void ) const
{
  return UILayoutItem::layout_max_;
}

Size2i UILayout::minimum_size( void ) const
{
  return Size2i( 0, 0 );
}

void UILayout::set_bounds( const IntRect& rect )
{
  this->bounds_ = rect;
}

bool UILayout::activate( void ) const
{
  NOM_STUBBED( NOM );

  return false;
}

void UILayout::add_widget( UIWidget* widget )
{
  this->add_item( new UIWidgetLayoutItem( widget ) );
}

int UILayout::find_widget( const UIWidget* widget ) const
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item->widget() == widget )
    {
      return pos;
    }

    ++pos;
  }

  // Not found
  return npos;
}

bool UILayout::enabled( void ) const
{
  return this->enabled_;
}

int UILayout::erase_item( UILayoutItem* layout )
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item == layout )
    {
      this->erase( pos );

      // Successful removal
      return pos;
    }

    ++pos;
  }

  // Nothing removed
  return npos;
}

int UILayout::erase_widget( UIWidget* widget )
{
  UILayoutItem* item = nullptr;

  int pos = 0;
  while( pos < this->count() )
  {
    item = this->at( pos );

    if( item->widget() == widget )
    {
      this->erase( pos );

      // Successful removal
      return pos;
    }

    ++pos;
  }

  // Nothing removed
  return npos;
}

void UILayout::set_alignment( uint32 align )
{
  IntRect parent_bounds;
  int layout_x = this->bounds().x;
  int layout_y = this->bounds().y;
  int layout_width = this->bounds().w;
  int layout_height = this->bounds().h;
  UIWidget* widget = nullptr;

  // Alignment offsets
  int x_offset = 0;
  int y_offset = 0;
  IntRect offset_bounds;

  if( this->parent() != nullptr )
  {
    widget = this->parent();
    parent_bounds = widget->parent_bounds();

    // NOM_DUMP( widget->name() );
    // NOM_DUMP( widget->parent_bounds() );
  }

  if( parent_bounds == IntRect::null )
  {
    NOM_LOG_ERR( NOM, "Could not set layout's alignment: parent dimensions are invalid." );
    return;
  }

  layout_x = nom::round( parent_bounds.x + layout_x / layout_width );
  layout_y = nom::round( parent_bounds.y + layout_y / layout_height );

  UILayoutItem::set_alignment( align );

  // Anchor::TopLeft, Anchor::Left, Anchor::BottomLeft
  if( align & Alignment::X_LEFT )
  {
    x_offset = layout_x;
  }

  // Anchor::TopCenter, Anchor::MiddleCenter, Anchor::BottomCenter
  if( align & Alignment::X_CENTER )
  {
    x_offset = layout_x + ( parent_bounds.w - layout_width ) / 2;
  }

  // Anchor::TopRight, Anchor::Right, Anchor::BottomRight
  if( align & Alignment::X_RIGHT )
  {
    x_offset = layout_x + ( parent_bounds.w - layout_width );
  }

  // Anchor::TopLeft, Anchor::TopCenter, Anchor::TopRight
  if( align & Alignment::Y_TOP )
  {
    y_offset = layout_y;
  }

  // Anchor::MiddleLeft, Anchor::MiddleCenter, Anchor::MiddleRight
  if( align & Alignment::Y_CENTER )
  {
    y_offset = layout_y + ( parent_bounds.h - layout_height ) / 2;
  }

  // Anchor::BottomLeft, Anchor::BottomCenter, Anchor::BottomRight
  if( align & Alignment::Y_BOTTOM )
  {
    y_offset = layout_y + ( parent_bounds.h - layout_height );
  }

// NOM_DUMP( x_offset );
// NOM_DUMP( y_offset );
// NOM_DUMP( layout_width );
// NOM_DUMP( layout_height );

  offset_bounds = IntRect( Point2i( x_offset, y_offset ), Size2i( layout_width, layout_height ) );

  this->set_bounds( offset_bounds );

  NOM_ASSERT( widget != nullptr );

  if( widget != nullptr )
  {
    UIWidgetEvent evt( widget->id(), widget->name(), widget->id() );

    // Notify the parent widget that we have a boundary change, so that it can
    // take care of updating itself properly (decorator & friends)
    Event ev;
    ev.type = SDL_WINDOWEVENT_SIZE_CHANGED;
    ev.timestamp = ticks();
    ev.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
    ev.window.data1 = layout_width;
    ev.window.data2 = layout_height;
    // ev.window.window_id = widget->id();

    evt.resized_bounds_ = offset_bounds;
    evt.set_event( ev );

    widget->emit( nom::UIEvent::ON_WINDOW_SIZE_CHANGED, evt );
  }
}

bool UILayout::set_alignment( const UIWidget* widget, uint32 align )
{
  UILayout* layout = nullptr;
  UILayoutItem* item = nullptr;
  // UISpacerItem* sp = nullptr;
  UIWidget* widget_item = nullptr;

  // int spacer = 0;                   // UISpacerItem
  int pos = 0;

  // Alignment offsets
  int x_offset = 0;
  int y_offset = 0;

  int widget_x = 0;
  int widget_y = 0;
  int widget_width = 0;
  int widget_height = 0;

  int layout_width = 0;
  int layout_height = 0;

  pos = find_widget( widget );

  if( pos == nom::npos )
  {
    // Did you forget to insert the widget into the layout?
    NOM_LOG_ERR( NOM, "Could not set alignment for widget: widget was not found in the layout." );
    return false;
  }

  item = this->at( pos );

  NOM_ASSERT( item != nullptr );

  // FIXME: Clean up logic; ideally, we'd like to handle all layout item
  // types?
  // sp = item->spacer_item();
  // if( sp != nullptr )
  // {
    // #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
      // NOM_DUMP("sp");
      // NOM_DUMP( item->minimum_size() );
      // NOM_DUMP( item->size_hint() );
      // NOM_DUMP( item->bounds() );
    // #endif

    // if( this->horiz() )
    // {
      // spacer = sp->size_hint().w;
    // }
    // else
    // {
      // spacer = sp->size_hint().h;
    // }
  // }

  // NOM_DUMP_VAR( "sp: ", spacer );

  if( item->widget() == widget )
  {
    item->set_alignment( align );
    // this->invalidate();

    layout = widget->parent()->layout();
    widget_item = item->widget();

    NOM_ASSERT( widget != nullptr );
    NOM_ASSERT( layout != nullptr );

    widget_x = widget->global_bounds().x;
    widget_y = widget->global_bounds().y;
    widget_width = widget->size().w;
    widget_height = widget->size().h;

    layout_width = layout->bounds().w;
    layout_height = layout->bounds().h;

    // NOM_DUMP( widget->name() );
    // NOM_DUMP( widget_x );
    // NOM_DUMP( widget_y );
    // NOM_DUMP( widget_width );
    // NOM_DUMP( widget_height );
    // // NOM_DUMP( layout_x );
    // // NOM_DUMP( layout_y );
    // NOM_DUMP( layout_width );
    // NOM_DUMP( layout_height );
    // NOM_DUMP( item->alignment() );  // Destination alignment

    // if( widget->name() == "button0" )
    {
      // if( spacer > 0 )
      {
        // widget_x -= spacer + this->spacing();
        // widget_y += spacer + this->spacing();
        // widget_x -= spacer;
      }
    }

    // Anchor::TopLeft, Anchor::Left, Anchor::BottomLeft
    if( align & Alignment::X_LEFT )
    {
      x_offset = widget_x;
    }

    // Anchor::TopCenter, Anchor::MiddleCenter, Anchor::BottomCenter
    if( align & Alignment::X_CENTER )
    {
      if( this->horiz() )
      {
        x_offset = widget_x + ( layout_width / 4 - widget_width ) / 2;
      }
      else
      {
        x_offset = widget_x + ( layout_width - widget_width ) / 2;
      }
    }

    // Anchor::TopRight, Anchor::Right, Anchor::BottomRight
    if( align & Alignment::X_RIGHT )
    {
      if( this->horiz() )
      {
        x_offset = widget_x + ( layout_width / 4 - widget_width );
      }
      else
      {
        x_offset = widget_x + ( layout_width - widget_width );
      }
    }

    // Anchor::TopLeft, Anchor::TopCenter, Anchor::TopRight
    if( align & Alignment::Y_TOP )
    {
      y_offset = widget_y;
    }

    // Anchor::MiddleLeft, Anchor::MiddleCenter, Anchor::MiddleRight
    if( align & Alignment::Y_CENTER )
    {
      if( this->horiz() )
      {
        y_offset = widget_y + ( layout_height - widget_height ) / 2;
      }
      else
      {
        y_offset = widget_y + ( layout_height / 4 - widget_height ) / 2;
      }
    }

    // Anchor::BottomLeft, Anchor::BottomCenter, Anchor::BottomRight
    if( align & Alignment::Y_BOTTOM )
    {
      if( this->horiz() )
      {
        y_offset = widget_y + ( layout_height - widget_height );
      }
      else
      {
        y_offset = widget_y + ( layout_height / pos - widget_height );
      }
    }

    item->set_bounds( IntRect( Point2i( x_offset, y_offset ), Size2i( widget_width, widget_height ) ) );

    return true;
  } // end if item is widget

  // Err: Item found in layout was not the same as the one passed?
  return false;
}

bool UILayout::set_alignment( const UILayout::raw_ptr layout, uint32 align )
{
  UILayoutItem* item = nullptr;

  for( auto pos = 0; pos != this->count(); ++pos )
  {
    item = this->at( pos );

    if( item->layout() == layout )
    {
      item->set_alignment( align );
      this->invalidate();

      return true;
    }
  }

  return false;
}

void UILayout::set_margins( const IntRect& margins )
{
  this->margins_ = margins;
}

void UILayout::set_enabled( bool state )
{
  this->enabled_ = state;
}

void UILayout::set_size_constraint( enum SizeConstraint size )
{
  this->contraints_ = size;
}

void UILayout::set_spacing( int space )
{
  this->spacing_ = space;
}

enum UILayout::SizeConstraint UILayout::size_constraint( void ) const
{
  return this->contraints_;
}

int UILayout::spacing( void ) const
{
  return this->spacing_;
}

void UILayout::update( void )
{
  // NOM_STUBBED( NOM );
}

// Protected scope

void UILayout::add_child_widget( UIWidget* widget )
{
  widget->set_layout( this );
}

bool UILayout::horiz( void ) const
{
  // TODO: Should we treat this field as a bit-mask??? (Should we support
  // layouts that can go in both directions?)
  if( this->directions() == Orientations::Horizontal )
  {
    return true;
  }

  // Vertical layout
  return false;
}

} // namespace nom
