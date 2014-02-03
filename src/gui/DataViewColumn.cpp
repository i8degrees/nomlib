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

namespace nom {

DataViewColumn::DataViewColumn( void )
{
  // NOM_LOG_TRACE( NOM );
}

DataViewColumn::~DataViewColumn( void )
{
  // NOM_LOG_TRACE( NOM );
}

DataViewColumn::DataViewColumn( uint id, const std::string& name, int width, enum IDataViewColumn::Alignment align )
{
  // NOM_LOG_TRACE( NOM );

  this->id_ = id;
  this->title_ = name;
  this->min_width_ = -1; // Not used; reserved
  this->width_ = width;
  this->alignment_ = align;
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

IDataViewColumn::Alignment DataViewColumn::alignment( void ) const
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

const IFont::shared_ptr& DataViewColumn::font( void ) const
{
  return this->font_;
}

} // namespace nom
