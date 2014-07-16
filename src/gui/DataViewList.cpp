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
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

// Forward declarations
#include "nomlib/gui/DataViewListStore.hpp"
#include "nomlib/gui/UIStyle.hpp"

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
  uint width = 0; // Label's text width
  Text::unique_ptr header;
  Point2i pos = this->position();

  if( this->store()->columns_size() < 1 ) return;

  for( auto cols = 0; cols != this->store()->columns_size(); ++cols )
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

    width = header->width();

    header->set_position( Point2i( pos.x + 4, pos.y ) );

    // Set the size of each label to be the width of the overall container --
    // DataViewList -- minus the label's text width, divided by two.
    //
    // Fudged in as a "stub" for the time being.
    header->set_size( Size2i( this->size().w - ( width / 2 ), this->size().h ) );

    // Convert our far more simplistic IDataViewColumn::Alignment enumeration
    // to nom::Text's native type.
    switch( column.alignment() )
    {
      case nom::IDataViewColumn::Left:
      default:
      {
        header->set_alignment( Text::Alignment::TopLeft );
        break;
      }

      case nom::IDataViewColumn::Center:
      {
        header->set_alignment( Text::Alignment::TopCenter );
        break;
      }

      case nom::IDataViewColumn::Right:
      {
        header->set_alignment( Text::Alignment::TopRight );
        break;
      }
    }

    // FIXME: re-enable me once we fix the cropping rendered text bug that
    // has been noted in our project files.
    //
    // Prevent rendering of text that is longer in length than its container's
    // set size parameters; see set_size method call above.
    header->set_features( Text::ExtraRenderingFeatures::CropText );

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

  Point2i pos = this->position();
  Text::unique_ptr header_item;
  std::shared_ptr<Transformable> item;

  if( this->store()->items_size() < 1 ) return;

  this->drawable_items_.clear();

  for( auto cols = 0; cols != this->store()->columns_size(); ++cols )
  {
    DataViewColumn column = this->store()->column( cols );

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
          header_item = Text::unique_ptr( new Text( text->str(), row->style()->font() ) );
        }
        else
        {
          header_item = Text::unique_ptr( new Text( text->str(), this->font() ) );
        }

        width = header_item->width();
        height = header_item->height();

        // Origin coordinates of item text
        header_item->set_position( Point2i( pos.x + 4, pos.y + 10 ) );

        // Set the size of each label to be the width of the column container, and
        // the height of each label to the height of the text, with respect to the
        // rendered text.
        header_item->set_size( Size2i( column.width(), height ) );

        // Convert our far more simplistic IDataViewColumn::Alignment enumeration
        // to nom::Text's native type.
        switch( column.alignment() )
        {
          case nom::IDataViewColumn::Left:
          default:
          {
            header_item->set_alignment( Text::Alignment::TopLeft );
            break;
          }

          case nom::IDataViewColumn::Center:
          {
            header_item->set_alignment( Text::Alignment::TopCenter );
            break;
          }

          case nom::IDataViewColumn::Right:
          {
            header_item->set_alignment( Text::Alignment::TopRight );
            break;
          }
        }

        if( rows >= this->store()->items_size( cols ) - 1 )
        {
          // FIXME: this calculation is fudged in.
          pos.x += ( width * 2 );
          pos.y = position().y;
        }
        else
        {
          // FIXME: this calculation is fudged in.
          pos.y += height * 2;
        }

        // // FIXME: re-enable me once we fix the cropping rendered text bug that
        // // has been noted in our project files.
        // //
        // // Prevent rendering of text that is longer in length than its container's
        // // set size parameters; see set_size method call above.
        header_item->set_features( Text::ExtraRenderingFeatures::CropText );

        this->drawable_items_.push_back( std::move( header_item ) );
      }
      else if( NOM_ISA( Transformable*, row->data() ) == true )
      {
        item.reset( dynamic_cast<Transformable*>( row->data() ) );

        if( item == nullptr )
        {
          // TODO: Err handling
        }

        width = item->size().w;
        height = item->size().h;

        // Origin coordinates of item
        item->set_position( Point2i( pos.x + 4, pos.y + 10 ) );

        // Set the size of each label to be the width of the column container, and
        // the height of each label to the height of the text, with respect to the
        // rendered text.
        // item->set_size( Size2i( column.width(), height ) );

        if( rows >= this->store()->items_size( cols ) - 1 )
        {
          // FIXME: this calculation is fudged in.
          pos.x += ( width * 2 );
          pos.y = position().y;
        }
        else
        {
          // FIXME: this calculation is fudged in.
          pos.y += height * 2;
        }

        item->update();

        this->drawable_items_.push_back( item );
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

void DataViewList::on_mouse_down( const UIWidgetEvent& ev )
{
  Event evt = ev.event();

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
        UIWidgetEvent item;

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
        if( NOM_ISA( Text*, it->get() ) == true )
        {
          Text* obj = dynamic_cast<Text*>( it->get() );

          if( obj == nullptr )
          {
            // TODO: Err handling
            NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not send UIWidgetEvent for nom::Text drawable_item: NULL." );
          }

          IntRect label_bounds( obj->position().x, obj->position().y, obj->width(), obj->height() );

          if( label_bounds.contains( mouse_coords ) )
          {
            UIWidgetEvent item;

            // Send the array index in our event; this signifies which choice was
            // selected.
            item.set_index( index );

            // Send the text of the selection.
            item.set_text( obj->text() );

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
        }
        else if( NOM_ISA( SpriteBatch*, it->get() ) == true )
        {
          SpriteBatch* obj = dynamic_cast<SpriteBatch*>( it->get() );

          if( obj == nullptr )
          {
            // TODO: Err handling
            NOM_LOG_ERR( NOM_LOG_CATEGORY_APPLICATION, "Could not send UIWidgetEvent for SpriteBatch drawable_item: NULL." );
          }

          IntRect sprite_bounds( obj->position().x, obj->position().y, obj->size().w, obj->size().h );

          if( sprite_bounds.contains( mouse_coords ) )
          {
            UIWidgetEvent item;

            // Send the current frame identifier used for rendering of the
            // sprite sheet.
            item.set_index( obj->frame() );

            // Send the text of the selection; the sprite sheet's filename
            // attribute.
            item.set_text( obj->sheet_filename() );

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
        }
      }

      ++index;
    }
  }

  // No processed events
  // return false;
}

} // namespace nom
