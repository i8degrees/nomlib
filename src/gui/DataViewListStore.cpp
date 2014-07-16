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

// Forward declarations
#include "nomlib/gui/UIStyle.hpp"

namespace nom {

DataViewListStore::DataViewListStore( void )
{
  // NOM_LOG_TRACE(NOM);
}

DataViewListStore::~DataViewListStore( void )
{
  // NOM_LOG_TRACE(NOM);
}

const DataViewColumn& DataViewListStore::column( uint cols_id ) const
{
  return *this->cols_.at( cols_id );
}

DataViewListStore::ValueType* DataViewListStore::item( uint cols_id, uint row_id ) const
{
  auto res = this->items_.find( cols_id );

  if( res != this->items_.end() )
  {
    // TODO: validity / err checks

    return res->second.at( row_id );
  }

  return nullptr;
}

uint DataViewListStore::columns_size( void ) const
{
  return this->cols_.size();
}

uint DataViewListStore::items_size( uint cols_id ) const
{
  auto res = this->items_.find( cols_id );

  if( res != this->items_.end() )
  {
    // TODO: validity / err checks

    return res->second.size();
  }

  return 0;
}

uint DataViewListStore::items_size( void ) const
{
  return this->items_.size();
}

const DataViewListStore::ColumnNames DataViewListStore::column_names( void ) const
{
  std::vector<std::string> col_names;

  for( auto itr = this->cols_.begin(); itr != this->cols_.end(); ++itr )
  {
    col_names.push_back( (*itr)->title() );
  }

  return col_names;
}

bool DataViewListStore::insert_column( uint cols, const DataViewColumn& col )
{
  this->cols_.at( cols ) = std::unique_ptr<DataViewColumn>( new DataViewColumn( col ) );

  // TODO: validity / err checks

  return true;
}

bool DataViewListStore::append_column( const DataViewColumn& col )
{
  this->cols_.push_back( std::unique_ptr<DataViewColumn>( new DataViewColumn( col ) ) );

  // TODO: validity / err checks

  return true;
}

bool DataViewListStore::append_item( const ValueTypeContainer& value )
{
  ItemPair p( this->cols_.size(), value );
  this->items_.insert( p );

  // TODO: validity / err checks

  return true;
}

bool DataViewListStore::insert_item( uint cols_id, const ValueTypeContainer& value )
{
  ItemPair p( cols_id, value );
  this->items_.insert( p );

  // TODO: validity / err checks

  return true;
}

bool DataViewListStore::insert_item( uint cols_id, uint row_id, ValueType* value )
{
  auto res = this->items_.find( cols_id );

  if( res != this->items_.end() )
  {
    res->second.at( row_id ) = value;

    return true;
  }

  // TODO: validity / err checks

  return false;
}

bool DataViewListStore::append_item( uint col, ValueType* value )
{
  auto res = this->items_.find( col );

  if( res != this->items_.end() )
  {
    res->second.push_back( value );

    return true;
  }

  // TODO: validity / err checks

  return false;
}

bool DataViewListStore::set_item_style( uint col, std::shared_ptr<UIStyle> style )
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

// bool DataViewListStore::insert_item( uint cols_id, const DrawablesContainer& values )
// {
//   this->drawables_.push_back( values );

//   // TODO: validity / err checks

//   return true;
// }

// bool DataViewListStore::insert_item( uint cols_id, uint row_id, const IDrawable& value )
// {
//   this->drawables_.push_back( value );

//   // TODO: validity / err checks

//   return true;
// }

} // namespace nom
