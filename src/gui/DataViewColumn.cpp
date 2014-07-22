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
#include "nomlib/gui/DataViewColumn.hpp"

// Forward declarations
#include "nomlib/gui/UIStyle.hpp"

namespace nom {

DataViewColumn::DataViewColumn( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewColumn::~DataViewColumn( void )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewColumn::DataViewColumn( uint id ) :
  id_{ id },
  min_width_{ 0 },
  width_{ 0 },
  alignment_{ DataViewColumn::Alignment::Left }
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
}

DataViewColumn::DataViewColumn( uint id, const std::string& name, int width, uint32 align )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );

  this->id_ = id;
  this->title_ = name;
  this->min_width_ = -1; // Not used; reserved
  this->width_ = width;
  this->alignment_ = align;
}

bool DataViewColumn::operator <( const self_type& rhs ) const
{
  return( this->id() < rhs.id() );
}

bool DataViewColumn::operator ==( const self_type& rhs ) const
{
  return( this->id() == rhs.id() );
}

uint DataViewColumn::id( void ) const
{
  return this->id_;
}

const std::string& DataViewColumn::title( void ) const
{
  return this->title_;
}

int DataViewColumn::width( void ) const
{
  return this->width_;
}

uint32 DataViewColumn::alignment( void ) const
{
  return this->alignment_;
}

bool DataViewColumn::sortable( void ) const
{
  return this->sortable_;
}

bool DataViewColumn::hidden( void ) const
{
  return this->visibility_;
}

std::shared_ptr<UIStyle> DataViewColumn::style( void ) const
{
  return this->style_;
}

void DataViewColumn::set_style( std::shared_ptr<UIStyle> style )
{
  this->style_ = style;
}

// DataViewPage::DataViewPage( void ) :
//   page_{ 0 }
// {
//   //
// }

// DataViewPage::~DataViewPage( void )
// {
//   //
// }

// DataViewPage::DataViewPage( int pg ) :
//   page_{ pg }
// {
//   // NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE_GUI, SDL_LOG_PRIORITY_VERBOSE );
// }

// bool DataViewPage::operator <( const self_type& rhs ) const
// {
//   return( this->page() < rhs.page() );
// }

// bool DataViewPage::operator ==( const self_type& rhs ) const
// {
//   return( this->page() == rhs.page() );
// }

// int DataViewPage::page( void ) const
// {
//   return this->page_;
// }

// // int DataViewPage::total_pages( void ) const
// // {
// //   return this->total_pages_;
// // }

// // int DataViewPage::selected( void ) const
// // {
// //   return this->selected_;
// // }

// const DataViewColumn& DataViewPage::column( void ) const
// {
//   return this->column_;
// }

// void DataViewPage::set_page_column( const DataViewColumn& col )
// {
//   this->column_ = col;
// }

} // namespace nom
