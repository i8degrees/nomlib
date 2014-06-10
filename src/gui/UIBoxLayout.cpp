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
#include "nomlib/gui/UIBoxLayout.hpp"

// #define NOM_DEBUG_OUTPUT_LAYOUT_DATA

namespace nom {

UIBoxLayout::UIBoxLayout( void )
{
  // NOM_LOG_TRACE( NOM );

  // Default spacing between each layout item; this is applied equally to every
  // layout item, except for the first item.
  this->set_spacing( 1 );

  this->set_direction( LeftRight );
}

UIBoxLayout::~UIBoxLayout( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIBoxLayout::UIBoxLayout( enum Direction dir, UIWidget* parent ) :
  UILayout( parent )
{
  // NOM_LOG_TRACE( NOM );

  // Default spacing between each layout item; this is applied equally to every
  // layout item, except for the first item.
  this->set_spacing( 1 );

  this->set_direction( dir );
}

Orientations UIBoxLayout::directions( void ) const
{
  // Map the direction of our layout to the orientation of UILayoutItem.
  if( this->direction_ == Direction::LeftRight || this->direction_ == Direction::RightLeft )
  {
    return Orientations::Horizontal;
  }

  // Direction::TopBottom or Direction::BottomTop
  return Orientations::Vertical;
}

Size2i UIBoxLayout::size_hint( void ) const
{
  // Items container size
  int count = this->count();

  // Default preferred size; only used if nothing better comes along!
  Size2i hint( 0, 0 );

  for( auto idx = 0; idx < count; ++idx )
  {
    // Widget or layout pointer
    UILayoutItem* item = this->at( idx );

    if( item == nullptr )
    {
      NOM_LOG_ERR( NOM, "Could not calculate size_hint: item was NULL." );
      continue;
    }

    // TODO: Verify if we even need this
    /*
    if( item->spacer_item() != nullptr )
    {
      // hint += item->spacer_item()->size_hint();
      hint = item->spacer_item()->size_hint();
    }
    */
    // else
    {

      if( ! item->spacer_item() )
      {
        // Use the larger of the two objects compared.
        hint = hint.max( item->size_hint() );
      }
    }
  }

// NOM_DUMP( hint );

  return Size2i( hint.w , hint.h );
}

Size2i UIBoxLayout::minimum_size( void ) const
{
  // Items container size
  int count = this->count();

  // Frame bounds used for calculating a more accurate minimum size; only
  // filled if the item has an initialized decorator.
  IntRect frame_bounds;
  Size2i frame_size;
  Size2i offset;

  // Default minimum size; only used if nothing better comes along!
  Size2i msize( 0, 0 );

  for( auto idx = 0; idx < count; ++idx )
  {
    // Widget or layout pointer
    UILayoutItem* item = this->at( idx );

    if( item == nullptr )
    {
      NOM_LOG_ERR( NOM, "Could not calculate minimum_size: item was NULL." );
      continue;
    }

    // TODO: Verify if we even need this
/*
    if( item->spacer_item() != nullptr )
    {
      // msize += msize.max( item->spacer_item()->minimum_size() );
      msize = item->spacer_item()->minimum_size();
    }
*/
    // else
    {

      if( ! item->spacer_item() )
      {
        // Use the larger of the two objects compared.
        // msize = item->minimum_size();
        msize = msize.max( item->minimum_size() );
      }

      UIWidget* widget = item->widget();

      if( widget != nullptr )
      {
        IDecorator* decorator = item->widget()->decorator();

        if( decorator != nullptr )
        {
          frame_bounds = decorator->frame_bounds();
          frame_size = decorator->size();

          if( frame_bounds != IntRect::null && frame_size != Size2i::null )
          {
            offset.w = frame_size.w - frame_bounds.w;
            offset.h = frame_size.h - frame_bounds.h;

            // Append the extra dimensions onto our minimum_size calculation
            msize += offset;
          }
        }
      } // end if widget != nullptr
    }
  } // end for items loop

// NOM_DUMP( msize );

  return Size2i( msize.w , msize.h );
}

int UIBoxLayout::count( void ) const
{
  return this->items_.size();
}

UILayoutItem* UIBoxLayout::at( int pos ) const
{
  UILayoutItem* item = nullptr;

  // Bounds check
  if( pos < this->count() )
  {
    item = this->items_.at( pos );
  }

  return item;
}

enum UIBoxLayout::Direction UIBoxLayout::direction( void ) const
{
  return this->direction_;
}

void UIBoxLayout::set_bounds( const IntRect& rect )
{
  // Items container size
  int count = this->count();

  // Layout item size requirements; dependent upon the item's size policy
  Size2i item_size;

  // Relative coordinates (to parent widget)
  Point2i offset( 0, 0 );

  // Overall layout boundary calculation
  IntRect geom;

  // Total sum of item's boundaries; this *should* result in the overall
  // layout's boundaries.
  // IntRect total_bounds;

  int w_x = 0;          // Widget X offset
  int w_y = 0;          // Widget Y offset
  int widget_count = 0; // Number of widget items in layout
  int spacer = 0;       // UISpacerItem

  // FIXME (?): Set the overall layout bounds
  //
  // We are setting the layout dimensions to the individual item sizes, *not*
  // the overall, accumulative dimensions of the layout.
  UILayout::set_bounds( rect );

  // Conserve CPU cycles
  if( count < 1 )
  {
    return;
  }

  for( auto idx = 0; idx < count; ++idx )
  {
    // Widget or layout item pointer
    UILayoutItem* item = this->at( idx );

    // Log err and continue onwards to the next item
    if( item == nullptr )
    {
      NOM_LOG_ERR( NOM, "Could not set layout item's bounds: the item was NULL." );
      continue;
    }

    // FIXME: Clean up logic; ideally, we'd like to handle all layout item
    // types?
    UISpacerItem* sp = item->spacer_item();
    if( sp != nullptr )
    {
      #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
        NOM_DUMP("sp");
        NOM_DUMP( item->minimum_size() );
        NOM_DUMP( item->size_hint() );
        NOM_DUMP( item->bounds() );
      #endif

      if( this->horiz() )
      {
        spacer = sp->size_hint().w;
      }
      else
      {
        spacer = sp->size_hint().h;
      }
    }

    UIWidget* widget = item->widget();

    if( widget != nullptr )
    {
      ++widget_count;

      #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
        NOM_DUMP( widget->name() );
      #endif

      // Setup our layout positioning coordinates to be relative to the parent
      // widget (top-level window).
      if( widget->parent() != nullptr )
      {
        // The offset will eventually be added onto the layout item's bounds
        offset = widget->parent()->position();
      }

      UILayoutPolicy p = widget->size_policy();

      // FIXME: Include horizontal policy
      if( p.vertical_policy() == UILayoutPolicy::Policy::Minimum )
      {
        item_size.w = this->minimum_size().w;
        item_size.h = this->minimum_size().h;
      }
      else
      {
        item_size.w = this->size_hint().w;
        item_size.h = this->size_hint().h;
      }

      // Horizontal layout using preferred size height
      if( this->horiz() )
      {
        // The item's spacing as per UISpacerItem size (see above)
        w_x += spacer;

        // w_y = rect.y + offset.y;
        w_y += 0;

        // Only apply internal layout spacing if it is not the first widget.
        if( widget_count > 1 )
        {
          // The item's size is dependent upon the size policy type (see above)
          w_x += item_size.w;

          if( this->spacing() > 0 )
          {
            w_x += this->spacing();
          }
        } // end if widget_count > 1

      } // end if horizontal layout

      // Vertical layout using preferred size height
      else
      {
        // w_x = rect.x + offset.x;
        w_x += 0;
        // The item's spacing as per UISpacerItem size (see above)
        w_y += spacer;

        // Only apply internal layout spacing if it is not the first widget.
        if( widget_count > 1 )
        {
          // The item's size is dependent upon the size policy type (see above)
          w_y += item_size.h;

          if( this->spacing() > 0 )
          {
            w_y += this->spacing();
          }
        } // end if widget_count > 1

      } // end if vertical layout

      // Widget layout item stats
      #if defined( NOM_DEBUG_OUTPUT_LAYOUT_DATA )
        NOM_DUMP( item->minimum_size() );
        NOM_DUMP( item->size_hint() );
        NOM_DUMP( item->bounds() );
      #endif

      geom = IntRect( w_x + rect.x + offset.x, w_y + rect.y + offset.y, item_size.w, item_size.h );

      // TODO: Verify this comment??? (Do not think it is doing what I say it is!)
      //
      // Set each item's boundaries as per calculated layout requirements; this
      // method call takes care of internal item updates that will need to occur
      // shortly thereafter (think: resize / move).
      item->set_bounds( geom );

    } // end if widget != nullptr

    // total_bounds += item->bounds();

  } // end for layout items loop

// NOM_DUMP( total_bounds );
}

void UIBoxLayout::add_item( UILayoutItem* item )
{
  if( item == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not add layout item: item was NULL." );
    return;
  }

  this->insert_item( this->count(), item );
}

void UIBoxLayout::erase( int pos )
{
  if( pos < this->count() )
  {
    auto itr = this->items_.begin() + pos;

    this->items_.erase( itr );
  }

  this->invalidate();
}

void UIBoxLayout::insert_item( int pos, UILayoutItem* item )
{
  if( item == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not insert layout item: item was NULL." );
    return;
  }

  this->items_.insert( this->items_.begin() + pos, item );
}

void UIBoxLayout::append_item( UILayoutItem* item )
{
  if( item == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not append layout item: item was NULL." );
    return;
  }

  this->insert_item( this->count(), item );
}

void UIBoxLayout::insert_widget( int pos, UIWidget* widget )
{
  Point2i pos_offset;

  if( widget == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not insert widget: widget was NULL." );
    return;
  }

  this->insert_item( pos, new UIWidgetLayoutItem( widget ) );

// NOM_DUMP( widget->name() );

  if( widget->position() == Point2i::null )
  {
    if( widget->parent() != nullptr )
    {
// NOM_DUMP( widget->parent()->position() );

      pos_offset = widget->parent()->position();
    }
    else
    {
// NOM_DUMP( widget->position() );

      // TODO (?): Handle NULL widget positioning coordinates -- calculate 'em
      // off ... something else!
      pos_offset = widget->position();
    }
  }

  // Calculate the layout's requirements for the addition
  this->set_bounds( IntRect( pos_offset, widget->size() ) );
  // this->set_bounds( IntRect( pos_offset, widget->size_hint() ) );
}

void UIBoxLayout::insert_widget( int pos, UIWidget* widget, uint32 align )
{
  Point2i pos_offset;

  if( widget == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not insert widget: widget was NULL." );
    return;
  }

  this->insert_widget( pos, widget );

  NOM_STUBBED( NOM );

  // TODO: Alignment of widget item
}

void UIBoxLayout::append_widget( UIWidget* widget )
{
  if( widget == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not append widget: widget was NULL." );
    return;
  }

  this->insert_widget( this->count(), widget );
}

void UIBoxLayout::append_widget( UIWidget* widget, uint32 align )
{
  if( widget == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not append widget: widget was NULL." );
    return;
  }

  this->insert_widget( this->count(), widget, align );
}

void UIBoxLayout::insert_spacer( int pos, UISpacerItem* item )
{
  if( item == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not insert spacer item: item was NULL." );
    return;
  }

  // this->items_.insert( this->items_.begin() + pos, item );
  this->insert_item( pos, item );

  this->set_bounds( item->bounds() );
}

void UIBoxLayout::insert_spacer( int pos, int height )
{
  uint32 flags = nom::UILayoutPolicy::Policy::Fixed;

  // FIXME:
  Size2i spacer_height = Size2i( height, height );

  UISpacerItem* sp = new UISpacerItem( spacer_height, flags, flags );

  if( sp == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not insert spacer item: item was NULL." );
    return;
  }

  this->insert_spacer( pos, sp );

  this->set_bounds( sp->bounds() );
}

void UIBoxLayout::append_spacer( UISpacerItem* item )
{
  if( item == nullptr )
  {
    NOM_LOG_ERR( NOM, "Could not append spacer item: item was NULL." );
    return;
  }

  this->insert_spacer( this->count(), item );
}

void UIBoxLayout::append_spacer( int height )
{
  this->insert_spacer( this->count(), height );
}

void UIBoxLayout::set_position( const Point2i& pos )
{
  this->set_bounds( IntRect( pos, this->bounds().size() ) );
}

void UIBoxLayout::set_direction( enum Direction dir )
{
  this->direction_ = dir;
}

bool UIBoxLayout::horiz( void ) const
{
  if( this->direction_ == Direction::LeftRight || this->direction_ == Direction::RightLeft )
  {
    return true;
  }

  // Vertical
  return false;
}

} // namespace nom
