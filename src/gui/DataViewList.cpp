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
// #include "nomlib/graphics/sprite/SpriteBatch.hpp"
#include "nomlib/gui/String.hpp"

// Forward declarations
#include "nomlib/gui/DataViewListStore.hpp"
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
    header->set_position( Point2i( ( this->position().x + 4 + ( ( this->size().w - width ) / col_count ) * cols ), this->position().y ) );
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
  Text::unique_ptr text_item;
  std::shared_ptr<Transformable> sprite_item;

  Point2i col_pos;
  Size2i col_size;

  if( this->store()->items_size() < 1 ) return;

  this->drawable_items_.clear();

  for( auto cols = 0; cols != this->store()->columns_size(); ++cols )
  {
    DataViewColumn column = this->store()->column( cols );

    col_pos = this->drawable_headers_.at( cols )->position();
    col_size = this->drawable_headers_.at( cols )->size();

    NOM_DUMP(col_pos);
    NOM_DUMP(col_size);

    for( auto rows = 0; rows != this->store()->items_size( cols ); ++rows )
    {
      IDataViewItem* row = this->store()->item( cols, rows );

      if( NOM_ISA( String*, row->data() ) == true )
      {
        String* text = dynamic_cast<String*>( row->data() );

        // Use the item's style object if available. If not, use the default
        // font given to us by our parent UIWidget.
        if( row->style() != nullptr && row->style()->font()->valid() )
        {
          text_item = Text::unique_ptr( new Text( text->str(), row->style()->font() ) );
        }
        else
        {
          text_item = Text::unique_ptr( new Text( text->str(), this->font() ) );
        }

        width = text_item->width();
        height = text_item->height();

        // Origin coordinates of item text
        // header_item->set_position( Point2i( pos.x + 4, pos.y + 10 ) );
        text_item->set_position( Point2i( col_pos.x + 4, pos.y + 10 ) );

        // Set the size of each label to be the width of the column container, and
        // the height of each label to the height of the text, with respect to the
        // rendered text.
        // text_item->set_size( Size2i( column.width(), height ) );
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
          IDataViewItem* col0_height = this->store()->item( 0, rows );
          if( col0_height != nullptr )
          {
            Transformable* icon_item = dynamic_cast<Transformable*>( col0_height->data() );
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

        this->drawable_items_.push_back( std::move( text_item ) );
      }
      else if( NOM_ISA( Transformable*, row->data() ) == true )
      {
        sprite_item.reset( dynamic_cast<Transformable*>( row->data() ) );

        if( sprite_item == nullptr )
        {
          // TODO: Err handling
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

        sprite_item->update();

        this->drawable_items_.push_back( sprite_item );
      }
    }
  }

  this->updated_ = true;
}

void DataViewList::draw( RenderTarget& target ) const
{
  // Column headers
  for( auto itr = this->drawable_headers_.begin(); itr != this->drawable_headers_.end(); ++itr )
  {
    (*itr)->draw( target );
  }

  // Items
  for( auto itr = this->drawable_items_.begin(); itr != this->drawable_items_.end(); ++itr )
  {
    IDrawable::shared_ptr obj = *itr;
    obj->draw( target );
  }
}

// Protected scope

void DataViewList::update( void )
{
  if( this->updated_ ) return;

  this->update_columns();
  this->update_items();
}

void DataViewList::on_mouse_down( const Event& evt )
{
  // Registered event for selection on_click
  if( evt.type == SDL_MOUSEBUTTONDOWN )
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

      IntRect label_bounds( (*it)->position().x, (*it)->position().y, (*it)->width(), (*it)->height() );

      if( label_bounds.contains( mouse_coords ) )
      {
        UIWidgetTreeEvent item;

        // Send the array index in our event; this signifies which choice was
        // selected.
        item.set_index( index );

        // Send the text of the selection.
        item.set_text( (*it)->text() );

        // Set the associated nom::Event object for this UI event.
        item.set_event( evt );

        // Associate the widget's unique identifier for this widget's event
        // object.
        item.set_id( this->id() );

        // Send the UI event object to the registered callback; public event
        // slot.
        this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

        // Processed events.
        // return true;
        return;
      }

      for( auto it = this->drawable_items_.begin(); it != this->drawable_items_.end(); ++it )
      {
        // if( NOM_ISA( Text*, it->get() ) == true )
        // {
        //   Text* obj = dynamic_cast<Text*>( it->get() );

        //   if( obj == nullptr )
        //   {
        //     // TODO: Err handling
        //     NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not send UIWidgetEvent for nom::Text drawable_item: NULL." );
        //   }

        //   IntRect label_bounds( obj->position().x, obj->position().y, obj->width(), obj->height() );

        //   if( label_bounds.contains( mouse_coords ) )
        //   {
        //     UIWidgetEvent item;

        //     // Send the array index in our event; this signifies which choice was
        //     // selected.
        //     item.set_index( index );

        //     // Send the text of the selection.
        //     item.set_text( obj->text() );

        //     // Set the associated nom::Event object for this UI event.
        //     item.set_event( evt );

        //     // Associate the widget's unique identifier for this widget's event
        //     // object.
        //     item.set_id( this->id() );

        //     // Send the UI event object to the registered callback; public event
        //     // slot.
        //     this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

        //     // Processed events.
        //     // return true;
        //     return;
        //   }
        // }
        if( NOM_ISA( Transformable*, it->get() ) == true )
        {
          Transformable* obj = dynamic_cast<Transformable*>( it->get() );

          if( obj == nullptr )
          {
            // TODO: Err handling
            NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not send UIWidgetEvent for Transformable item: NULL." );
          }

          IntRect object( obj->position().x, obj->position().y, obj->size().w, obj->size().h );

          if( object.contains( mouse_coords ) )
          {
            UIWidgetTreeEvent item;

            // Send the current frame identifier used for rendering of the
            // sprite sheet.
            item.set_index( index );

            // Send the text of the selection; the sprite sheet's filename
            // attribute.
            item.set_text( ObjectTypeInfo( obj->type() ).name() );

            // Set the associated nom::Event object for this UI event.
            item.set_event( evt );

            // Associate the widget's unique identifier for this widget's event
            // object.
            item.set_id( this->id() );

            Text* textual_item = NOM_DYN_PTR_CAST( Text*, it->get() );
            if( textual_item )
            {
              item.set_data( new String( textual_item->text() ) );
            }

            // Send the UI event object to the registered callback; public event
            // slot.
            this->dispatcher()->emit( UIEvent::MOUSE_DOWN, item );

            // Processed events.
            // return true;
            return;
          }
        }
      }

      ++index;
    }
  }

  // No processed events
  // return false;
}

} // namespace nom
