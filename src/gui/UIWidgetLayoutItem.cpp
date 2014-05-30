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
#include "nomlib/gui/UIWidgetLayoutItem.hpp"

// Private forward declarations
#include "nomlib/gui/UIWidget.hpp"

namespace nom {

UIWidgetLayoutItem::UIWidgetLayoutItem( void ) :
  directions_{ Orientations::Horizontal }
{
  // NOM_LOG_TRACE( NOM );
}

UIWidgetLayoutItem::~UIWidgetLayoutItem( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIWidgetLayoutItem::UIWidgetLayoutItem( UIWidget* widget ) :
  directions_{ Orientations::Horizontal }
{
  this->widget_.reset( widget );
}

bool UIWidgetLayoutItem::empty( void ) const
{
  UIWidget* widget = this->widget();

  // Empty -- pointer is NULL
  if( widget == nullptr )
  {
    return true;
  }

  // Empty -- widget is not visible
  if( widget->visible() == false )
  {
    return true;
  }

  // Empty -- widget is a top-level widget (window)
  if( this->widget()->is_window() )
  {
    return true;
  }

  // Not empty
  return false;
}

enum Orientations UIWidgetLayoutItem::directions( void ) const
{
  return this->directions_;
}

IntRect UIWidgetLayoutItem::bounds( void ) const
{
  // Sanity checks
  if( this->empty() == false )
  {
    return this->widget()->global_bounds();
  }

  return IntRect( 0, 0, 0, 0 );
}

Size2i UIWidgetLayoutItem::maximum_size( void ) const
{
  // Sanity checks
  if( this->empty() == false )
  {
    return this->widget()->maximum_size();
  }

  return Size2i( 0, 0 );
}

Size2i UIWidgetLayoutItem::minimum_size( void ) const
{
  // Sanity checks
  if( this->empty() == false )
  {
    return this->widget()->minimum_size();
  }

  return Size2i( 0, 0 );
}

void UIWidgetLayoutItem::set_bounds( const IntRect& rect )
{
  UIWidget* widget = nullptr;

  // Sanity checks
  if( this->empty() == true )
  {
    return;
  }

  // Size2i extra = this->bounds().size() - rect.size();
// NOM_DUMP(extra);

  // int x = rect.x;
  // int y = rect.y;

  uint32 align = this->alignment();
  Size2i bsize = rect.size();

  bsize.max( this->maximum_size() );

  // Horizontal or vertical alignments
  if( align & ( Alignment::X_MASK | Alignment::Y_MASK ) )
  {
    Size2i hint( this->size_hint() );

    // TODO: Size policy handling -- see QWidgetItem::setGeomtry.

    // Special horizontal alignment handling
    if( align & Alignment::X_MASK )
    {
      bsize.w = std::min( bsize.w, hint.w );
// NOM_DUMP(bsize.w);
    }

    // Special vertical alignment handling
    if( align & Alignment::Y_MASK )
    {
      bsize.h = std::min( bsize.h, hint.h );
// NOM_DUMP(bsize.h);
    }
  }

// NOM_DUMP( bsize );

  // TODO: Alignment directions -- see QWidgetItem::setGeomtry.

  // TODO: Bounds checking

  // Sanity check
  widget = this->widget();

  if( widget == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not set bounds for layout item: widget is NULL." );
    return;
  }

  // Notify the widget via private event listener that we have a boundary
  // change, so that it can take care of updating itself properly.
  UIWidgetEvent evt;
  evt.set_index( widget->id() );
  evt.set_text( widget->name() );
  evt.set_id( widget->id() );

  Event ev;
  ev.type = SDL_WINDOWEVENT_SIZE_CHANGED;
  ev.timestamp = ticks();
  ev.window.event = SDL_WINDOWEVENT_SIZE_CHANGED;
  ev.window.data1 = rect.size().w;
  ev.window.data2 = rect.size().h;
  ev.window.window_id = widget->id();
  evt.resized_bounds_ = rect;
  evt.set_event( ev );

  widget->emit( UIEvent::ON_WINDOW_SIZE_CHANGED, evt );

  // TODO: Clean up alignment logic
  //
  // Alignment probably really shouldn't be happening here at all.
  //
  // Also note how we are playing a dangerous logic game (read: confusing) by
  // sending a size change event right before updating the boundary of a layout
  // item! At the very least, it is a waste of resources -- calling the widget's
  // update twice in one method!

  // Alignment bounding for the layout item
  int x_offset = 0;
  int y_offset = 0;

  // Anchor::TopLeft, Anchor::Left, Anchor::BottomLeft
  if( align & Alignment::X_LEFT )
  {
    x_offset = rect.position().x;
  }

  // Anchor::Top, Anchor::Center, Anchor::Bottom
  if( align & Alignment::X_CENTER )
  {
    x_offset = rect.position().x + ( rect.size().w - bsize.w ) / 2;
  }

  // Anchor::TopRight, Anchor::Right, Anchor::BottomRight
  if( align & Alignment::X_RIGHT )
  {
    x_offset = rect.position().x + ( rect.size().w - bsize.w );
  }

  // Anchor::TopLeft, Anchor::Top, Anchor::TopRight
  if( align & Alignment::Y_TOP )
  {
    y_offset = rect.position().y;
  }

  // Anchor::Left, Anchor::Center, Anchor::Right
  if( align & Alignment::Y_CENTER )
  {
    y_offset = rect.position().y + ( rect.size().h - bsize.h ) / 2;
  }

  // Anchor::BottomLeft, Anchor::Bottom, Anchor::BottomRight
  if( align & Alignment::Y_BOTTOM )
  {
    y_offset = rect.position().y + ( rect.size().h - bsize.h );
  }

  widget->set_bounds( IntRect( Point2i( x_offset, y_offset ), rect.size() ) );

// NOM_DUMP( rect.position() );
// NOM_DUMP( rect.size() );
// NOM_DUMP( x_offset );
// NOM_DUMP( y_offset );

  // FIXME: Do we really need this here? I *think* it may be required for
  // alignments? It would cut down on updates considerably if we could remove
  // this.
  widget->set_updated( false );
  widget->update();
}

Size2i UIWidgetLayoutItem::size_hint( void ) const
{
  // Sanity checks
  if( this->empty() == false )
  {
    return this->widget()->size_hint();
  }

  return Size2i( 0, 0 );
}

UIWidget* UIWidgetLayoutItem::widget( void ) const
{
  return this->widget_.get();
}

} // namespace nom
