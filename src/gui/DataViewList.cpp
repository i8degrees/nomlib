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
#include "nomlib/gui/DataViewList.hpp"

// Private headers
#include "nomlib/graphics/Text.hpp"
#include "nomlib/gui/String.hpp"

// Forward declarations
#include "nomlib/gui/DataViewListStore.hpp"
#include "nomlib/gui/DataViewItem.hpp"
#include "nomlib/gui/DataViewColumn.hpp"
#include "nomlib/gui/UIStyle.hpp"
#include "nomlib/gui/UIEventDispatcher.hpp"
#include "nomlib/gui/UIEvent.hpp"
#include "nomlib/gui/UIWidgetEvent.hpp"

namespace nom {

DataViewList::DataViewList( void ) :
  updated_( false )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );
}

DataViewList::~DataViewList( void )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );
}

DataViewList::DataViewList  (
                              UIWidget* parent,
                              int64 id,
                              const Point2i& pos,
                              const Size2i& size
                            ) :
  UIWidget( parent, id, pos, size ),  // Base class
  updated_( false )
{
  NOM_LOG_TRACE( NOM_LOG_CATEGORY_TRACE_GUI );

  // Auto-generate a name tag for our widget.
  this->set_name( "data_view_list" );

  // Declare a default item store instance so that we do not crash if the
  // developer forgets to setup items.
  this->set_item_store( new DataViewListStore() );
}

bool DataViewList::valid( void ) const
{
  if( this->store() != nullptr && this->position() != Point2i::null &&
      this->size() != Size2i::null
    )
  {
    return true;
  }

  return false;
}

DataViewListStore* DataViewList::store( void ) const
{
  NOM_ASSERT( this->store_ != nullptr );

  return this->store_.get();
}

void DataViewList::set_item_store( DataViewListStore* store )
{
  this->store_.reset( store );

  this->update_columns();
  this->update_items();
}

void DataViewList::update_columns( void )
{
  uint col_count = 0;
  uint width = 0; // Label's text width
  uint height = 0; // Label's text height

  // Alignment offsets
  int x_offset = 0;
  int y_offset = 0;

  Text::unique_ptr header;
  Point2i pos = this->position();

  col_count = this->store()->columns_size();

  if( col_count < 1 ) return;

  for( auto cols = 0; cols != col_count; ++cols )
  {
    DataViewColumn column = this->store()->column( cols );

    // Use the column's style object if available. If not, use the default font
    // given to us by our parent UIWidget.
    if( column.style() != nullptr && column.style()->font()->valid() )
    {
      header = Text::unique_ptr( new Text( column.title(), column.style()->font() ) );
    }
    else
    {
      header = Text::unique_ptr( new Text( column.title(), this->font() ) );
    }

    // Text dimensions with respect to font
    width = header->width();
    height = header->height();

    // header->set_position( Point2i( pos.x + 4, pos.y ) );
    // if( column.id() == 1 )
    // {
      // Size2i widget_size( 300, 200 );
      // header->set_position( Point2i( ( this->position().x + 4 + ( ( this->size().w - width ) / col_count - this->store()->column( cols-1 ).width() ) * cols ), this->position().y ) );
    // }
    // else
    {
      header->set_position( Point2i( ( this->position().x + 4 + ( ( this->size().w - width ) / col_count ) * cols ), this->position().y ) );
    }
    // header->set_position( Point2i( ( this->position().x + column.width()/col_count ) * cols, this->position().y ) );

    // Set the size of each label to be the width of the overall container --
    // DataViewList -- minus the label's text width, divided by two.
    //
    // Fudged in as a "stub" for the time being.
    header->set_size( Size2i( width, height ) );

    // Convert our far more simplistic IDataViewColumn::Alignment enumeration
    // to nom::Text's native type.
    switch( column.alignment() )
    {
      case nom::IDataViewColumn::Left:
      default:
      {
        // header->set_alignment( Text::Alignment::TopLeft );
        break;
      }

      case nom::IDataViewColumn::Center:
      {
        // header->set_alignment( Text::Alignment::TopCenter );
        // x_offset = header->position().x + ( this->size().w / col_count - header->size().w ) / 2;
        x_offset = header->position().x + ( ( ( this->size().w - header->size().w ) / col_count ) * cols ) / 2;
        y_offset = header->position().y;
        header->set_position( Point2i( x_offset, y_offset ) );
        break;
      }

      case nom::IDataViewColumn::Right:
      {
        // header->set_alignment( Text::Alignment::TopRight );
        // x_offset = header->position().x + ( this->size().w / col_count - header->size().w );
        x_offset = header->position().x + ( this->size().w - header->size().w - 10 ) / col_count;
        y_offset = header->position().y;
        header->set_position( Point2i( x_offset, y_offset ) );
        break;
      }
    }

    // FIXME: re-enable me once we fix the cropping rendered text bug that
    // has been noted in our project files.
    //
    // Prevent rendering of text that is longer in length than its container's
    // set size parameters; see set_size method call above.
    // header->set_features( Text::ExtraRenderingFeatures::CropText );

    this->drawable_headers_.push_back( std::move( header ) );
  }

  this->updated_ = true;
}

void DataViewList::update_items( void )
{
  // Label's text width & height, as will be rendered; with respect to loaded
  // font.
  uint width = 0;
  uint height = 0;

  // Alignment offsets
  int x_offset = 0;
  int y_offset = 0;

  Point2i pos = this->position();

  // Non-owned pointers
  Text* text_item = nullptr;
  Transformable* sprite_item;

  Point2i col_pos;
  Size2i col_size;

  for( auto cols = 0; cols != this->store()->columns_size(); ++cols )
  {
    DataViewColumn column = this->store()->column( cols );

    col_pos = this->drawable_headers_.at( cols )->position();
    col_size = this->drawable_headers_.at( cols )->size();

    NOM_DUMP(col_pos);
    NOM_DUMP(col_size);

    for( auto rows = 0; rows != this->store()->items_size( cols ); ++rows )
    {
      DataViewItem* row = this->store()->item( cols, rows );

      // if( NOM_ISA( String*, row->data() ) == true )
      if( NOM_ISA( Text*, row->data() ) == true )
      {
        // String* text = dynamic_cast<String*>( row->data() );
        text_item = dynamic_cast<Text*>( row->data() );

        // Use the item's style object if available. If not, use the default
        // font given to us by our parent UIWidget.
        if( row->style() != nullptr && row->style()->font()->valid() )
        {
          // text_item = Text::unique_ptr( new Text( text->text(), row->style()->font() ) );
          text_item->set_font( row->style()->font() );
        }
        else
        {
          // text_item = Text::unique_ptr( new Text( text->text(), this->font() ) );
          text_item->set_font( this->font() );
        }

        width = text_item->width();
        height = text_item->height();

        // Origin coordinates of item text
        text_item->set_position( Point2i( col_pos.x + 4, pos.y + 10 ) );

        // Set the size of each label to be the width of the column container, and
        // the height of each label to the height of the text, with respect to the
        // rendered text.
        text_item->set_size( Size2i( width, height ) );

        // Convert our far more simplistic IDataViewColumn::Alignment enumeration
        // to nom::Text's native type.
        switch( column.alignment() )
        {
          case nom::IDataViewColumn::Left:
          default:
          {
            // text_item->set_alignment( Text::Alignment::MiddleLeft );
            x_offset = text_item->position().x;
            y_offset = text_item->position().y;
            text_item->set_position( Point2i( x_offset, y_offset ) );
            break;
          }

          case nom::IDataViewColumn::Center:
          {
            // text_item->set_alignment( Text::Alignment::MiddleCenter );
            x_offset = text_item->position().x + ( ( this->size().w - text_item->size().w ) / this->store()->items_size( cols ) ) / 2;
            y_offset = text_item->position().y;
            text_item->set_position( Point2i( x_offset, y_offset ) );
            break;
          }

          case nom::IDataViewColumn::Right:
          {
            // text_item->set_alignment( Text::Alignment::MiddleRight );
            x_offset = text_item->position().x + ( this->size().w / this->store()->items_size( cols ) - text_item->size().w );
            // x_offset = text_item->position().x + ( this->size().w - text_item->size().w );// / this->store()->items_size( cols );
            y_offset = text_item->position().y;
            text_item->set_position( Point2i( x_offset, y_offset ) );
            break;
          }
        }

        if( rows >= this->store()->items_size( cols ) - 1 )
        {
          pos.y = this->position().y;
        }
        else
        {
          // Use the height dimension of the first column to horizontally align
          // the rows.
          DataViewItem* col0_item_height = this->store()->item( 0, rows );
          if( col0_item_height != nullptr )
          {
            Transformable* icon_item = dynamic_cast<Transformable*>( col0_item_height->data() );
            if( icon_item != nullptr )
            {
              pos.y += icon_item->size().h;
            }
            else
            {
              pos.y += height;
            }
          }
        }

        // // FIXME: re-enable me once we fix the cropping rendered text bug that
        // // has been noted in our project files.
        // //
        // // Prevent rendering of text that is longer in length than its container's
        // // set size parameters; see set_size method call above.
        // text_item->set_features( Text::ExtraRenderingFeatures::CropText );
      }
      else if( NOM_ISA( Transformable*, row->data() ) == true )
      {
        sprite_item = dynamic_cast<Transformable*>( row->data() );

        if( sprite_item == nullptr )
        {
          // TODO: Err handling
          break;
        }

        width = sprite_item->size().w;
        height = sprite_item->size().h;

        // NOM_DUMP(width);
        // NOM_DUMP(height);

        // Origin coordinates of item
        // sprite_item->set_position( Point2i( pos.x + 4, pos.y + 10 ) );
        sprite_item->set_position( Point2i( col_pos.x, pos.y + 10 ) );

        if( rows >= this->store()->items_size( cols ) - 1 )
        {
          pos.y = this->position().y;
        }
        else
        {
          pos.y += height;
        }

        // Only necessary with engine classes that have not been converted to
        // auto-update themselves
        // sprite_item->update();
      }
    }
  }

  this->updated_ = true;
}

void DataViewList::draw( RenderTarget& target ) const
{
  uint col = 0;
  for( auto itr = this->drawable_headers_.begin(); itr != this->drawable_headers_.end(); ++itr )
  {
    (*itr)->draw( target );

    for( auto pos = 0; pos != this->store()->items_size( col ); ++pos )
    {
      DataViewItem* item = this->store()->item( col, pos );

      if( NOM_ISA( IDrawable*, item->data() ) == true )
      {
        IDrawable* obj = static_cast<IDrawable*>( item->data() );
        if( obj )
        {
          obj->draw( target );
        }
      }
    }

    ++col;
  }
}

// Protected scope

void DataViewList::update( void )
{
  if( this->updated_ ) return;

  this->update_columns();
  this->update_items();
}

void DataViewList::set_selection_item( uint col, uint row )
{
  DataViewItem* item = this->store()->item( col, row );
  Text* text_item = this->store()->item_text( col, row );

  if( text_item && item && item->selection() == true )
  {
    // text_item->set_color( Color4i::Red );
  }
  else if( text_item && item && item->selection() == false )
  {
    // text_item->set_color( Color4i::White );
  }
}

void DataViewList::on_mouse_up( const Event& evt )
{
  // Registered event for selection on_click
  if( evt.type == SDL_MOUSEBUTTONUP )
  {
    Point2i mouse_coords( evt.mouse.x, evt.mouse.y );

    // Counter for array index of each element
    uint index = 0;
    for( auto it = this->drawable_headers_.begin(); it != this->drawable_headers_.end(); ++it )
    {
      if( (*it) == nullptr )
      {
        // TODO: Err handling
        break;
      }

      if( this->contains_label( it->get(), mouse_coords ) )
      {
        UIWidgetTreeEvent ev;

        // Signal type
        ev.set_type( UIEvent::MOUSE_UP );

        // Item's column
        ev.set_column( index );
        ev.set_row( 0 );
        ev.set_data( it->get() );

         // Underlying event
        ev.set_event( evt );

        // Widget's unique identifier
        ev.set_id( this->id() );

        this->dispatcher()->emit( ev );

        break;
      }

      for( auto row = 0; row != this->store()->items_size( index ); ++row )
      {
        DataViewItem* item = this->store()->item( index, row );

        if( NOM_ISA( Transformable*, item->data() ) == true )
        {
          Transformable* obj = dynamic_cast<Transformable*>( item->data() );

          if( obj == nullptr )
          {
            // TODO: Err handling
            NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not send UIWidgetEvent for Transformable item: NULL." );
            break;
          }

          IntRect object( obj->position().x, obj->position().y, obj->size().w, obj->size().h );

          if( object.contains( mouse_coords ) )
          {
            UIWidgetTreeEvent ev;

            // Signal type
            ev.set_type( UIEvent::MOUSE_UP );

            ev.set_column( index );
            ev.set_row( row );

            // Underlying event
            ev.set_event( evt );

            // Widget's unique identifier
            ev.set_id( this->id() );

            if( item->selection() == true )
            {
              ev.set_selection( false );
              item->set_selection( false );
            }
            else
            {
              ev.set_selection( true );
              item->set_selection( true );
            }

            this->set_selection_item( index, row );

            // Widget's tree item data with emitted signal
            ev.set_data( obj );

            this->dispatcher()->emit( ev );

            break;
          }
        }
      }

      ++index;
    }
  }
}

void DataViewList::on_key_down( const Event& evt )
{
  // Registered event for selection on_click
  if( evt.type == SDL_KEYDOWN )
  {
    uint col = 1;
    for( auto row = 0; row != this->store()->items_size( col ); ++row )
    {
      DataViewItem* item = this->store()->item( col, row );

      if( NOM_ISA( Transformable*, item->data() ) == true )
      {
        Transformable* obj = dynamic_cast<Transformable*>( item->data() );

        if( obj == nullptr )
        {
          // TODO: Err handling
          NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not send UIWidgetEvent for Transformable item: NULL." );
          break;
        }

        if( item->selection() == true )
        {
          if( evt.key.sym == SDLK_UP && this->store()->items_size( col ) > row-1 )
          {
            // NOM_DUMP("U");
            item->set_selection( false );
            DataViewItem* prev_item = this->store()->item( col, --row );
            prev_item->set_selection( true );
          }
          else if( evt.key.sym == SDLK_DOWN && ( row < this->store()->items_size( col ) - 1 ) )
          {
            // NOM_DUMP("D");
            item->set_selection( false );
            DataViewItem* next_item = this->store()->item( col, ++row );
            next_item->set_selection( true );
          }
        }
      }
    }
  }
}

} // namespace nom
