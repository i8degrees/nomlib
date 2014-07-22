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
#include "nomlib/gui/DataViewItem.hpp"

// Forward declarations
#include "nomlib/gui/UIStyle.hpp"
// #include "nomlib/gui/String.hpp"
#include "nomlib/graphics/IDrawable.hpp"

namespace nom {

DataViewItem::DataViewItem( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewItem::~DataViewItem( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewItem::DataViewItem( IObject* data )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );

  this->data_ = data;
}

ObjectTypeInfo DataViewItem::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

IObject* DataViewItem::data( void ) const
{
  return this->data_;
}

bool DataViewItem::selection( void ) const
{
  return this->selection_;
}

std::shared_ptr<UIStyle> DataViewItem::style( void ) const
{
  return this->style_;
}

void DataViewItem::set_style( std::shared_ptr<UIStyle> style )
{
  this->style_ = style;
}

void DataViewItem::set_selection( bool state )
{
  this->selection_ = state;
}

// DataViewTextItem

DataViewTextItem::DataViewTextItem( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewTextItem::~DataViewTextItem( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewTextItem::DataViewTextItem( const std::string& data ) :
  DataViewItem{ new Text( data ) }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewTextItem::DataViewTextItem( const char* data ) :
  DataViewItem{ new Text( data ) }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

ObjectTypeInfo DataViewTextItem::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

// DataViewDrawableItem

DataViewDrawableItem::DataViewDrawableItem( void ) :
  data_{ nullptr }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewDrawableItem::~DataViewDrawableItem( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewDrawableItem::DataViewDrawableItem( IDrawable* data ) :
  // data_{ data->clone() }
  data_{ data }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

ObjectTypeInfo DataViewDrawableItem::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

IObject* DataViewDrawableItem::data( void ) const
{
  return this->data_;
}

} // namespace nom
