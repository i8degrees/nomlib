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
#include "nomlib/gui/DataViewListStore.hpp"

// Private headers
#include "nomlib/system/make_unique.hpp"

// Forward declarations
#include "nomlib/gui/UIStyle.hpp"
#include "nomlib/gui/DataViewColumn.hpp"
#include "nomlib/gui/DataViewItem.hpp"

#include "nomlib/graphics/Text.hpp"
#include "nomlib/graphics/sprite/SpriteBatch.hpp"

namespace nom {

DataViewListStore::DataViewListStore( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewListStore::~DataViewListStore( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

const DataViewColumn& DataViewListStore::column( uint col ) const
{
  auto res = this->items_.find( col );

  NOM_ASSERT( res != this->items_.end() );

  return res->first;
}

DataViewItem* DataViewListStore::item( uint col, uint pos ) const
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    if( pos < this->items_.at( col ).size() )
    {
      return res->second.at( pos ).get();
    }
  }

  return nullptr;
}

Text* DataViewListStore::item_text( uint col, uint pos ) const
{
  return dynamic_cast<Text*>( this->item( col, pos )->data() );
}

SpriteBatch* DataViewListStore::item_sprite( uint col, uint pos ) const
{
  return dynamic_cast<SpriteBatch*>( this->item( col, pos )->data() );
}

uint DataViewListStore::columns_size( void ) const
{
  return this->items_.size();
}

uint DataViewListStore::items_size( uint cols_id ) const
{
  auto res = this->items_.find( cols_id );

  if( res != this->items_.end() )
  {
    return res->second.size();
  }

  return 0;
}

const DataViewListStore::ColumnNames DataViewListStore::column_names( void ) const
{
  std::vector<std::string> col_names;

  for( auto itr = this->items_.begin(); itr != this->items_.end(); ++itr )
  {
    col_names.push_back( (itr)->first.title() );
  }

  return col_names;
}

void DataViewListStore::clear_columns( void )
{
  this->items_.clear();
}

void DataViewListStore::clear_items( uint col )
{
  this->items_.at( col ).clear();
}

bool DataViewListStore::erase_item( uint col, uint pos )
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    NOM_ASSERT( pos <= res->second.size() );

    this->items_.at( col ).erase( res->second.begin() + pos );

    return true;
  }

  return false;
}

void DataViewListStore::insert_column( uint cols, const DataViewColumn& col )
{
  ItemPair p( col, {} );

  this->items_.insert( p );
  // auto res = this->items_.find( cols );

  // if( res != this->items_.end() )
  // {
    // this->items_.emplace_hint( this->items_.cbegin(), p );
  // }
}

void DataViewListStore::append_column( const DataViewColumn& col )
{
  return this->insert_column( this->columns_size(), col );
}

bool DataViewListStore::insert_text_item( uint col, uint pos, const DataViewTextItem& item )
{
  return this->insert_item( col, pos, new DataViewTextItem( item ) );
}

bool DataViewListStore::append_text_item( uint col, const DataViewTextItem& item )
{
  return this->insert_item( col, this->items_size( col ), new DataViewTextItem( item ) );
}

bool DataViewListStore::insert_bitmap_item( uint col, uint pos, const DataViewDrawableItem& item )
{
  return this->insert_item( col, pos, new DataViewDrawableItem( item ) );
}

bool DataViewListStore::append_bitmap_item( uint col, const DataViewDrawableItem& item )
{
  return this->insert_item( col, this->items_size( col ), new DataViewDrawableItem( item ) );
}

bool DataViewListStore::insert_item( uint col, uint pos, DataViewItem* value )
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    NOM_ASSERT( pos <= res->second.size() );

    res->second.insert( res->second.begin() + pos, std::shared_ptr<DataViewItem>( value ) );

    return true;
  }

  return false;
}

bool DataViewListStore::append_item( uint col, DataViewItem* value )
{
  return this->insert_item( col, this->items_size( col ), value );
}

void DataViewListStore::insert_items( uint col, const ValueTypeContainer& value )
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    this->items_.at( col ) = value;
  }
}

bool DataViewListStore::set_item_style( uint col, uint pos, std::shared_ptr<UIStyle> style )
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    res->second.at( pos )->set_style( style );

    return true;
  }

  return false;
}

bool DataViewListStore::set_items_style( uint col, std::shared_ptr<UIStyle> style )
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    for( auto itr = res->second.begin(); itr != res->second.end(); ++itr )
    {
      (*itr)->set_style( style );
    }

    return true;
  }

  return false;
}

// void DataViewListStore::set_items_selection( uint col, int value )
// {
//   auto res = this->items_.find( col );

//   if( res != this->items_.end() )
//   {
//     for( auto itr = res->second.begin(); itr != res->second.end(); ++itr )
//     {
//       (*itr)->set_selection( value );
//     }
//   }
// }

} // namespace nom
