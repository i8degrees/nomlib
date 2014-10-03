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
#include "nomlib/librocket/UIDataViewList.hpp"

// Forward declarations (third-party)
// #include <Rocket/Controls/DataSource.h>

// Private headers (third-party)
#include <Rocket/Core/ElementUtilities.h>

// Forward declarations
#include "nomlib/tests/gui/librocket/CardsDataSource.hpp"

using namespace Rocket::Core;

namespace nom {

UIDataViewList::UIDataViewList( const Point2i& pos, const Size2i& dims ) :
  UIWidget( pos, dims ),
  data_source_( nullptr ),
  database_( nullptr )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  this->_initialize();
}

UIDataViewList::~UIDataViewList()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

bool UIDataViewList::initialize()
{
  this->_initialize();

  NOM_ASSERT( this->valid() != false );

  if( this->valid() == true )
  {
    this->set_position( this->position() );

    this->set_document_size();

    this->show();

    return true;
  }

  return false;
}

std::string UIDataViewList::column_title( int col ) const
{
  ElementList tags;

  NOM_ASSERT( this->valid() != false );
  if( this->valid() == true )
  {
    Element* parent = this->document()->GetElementById("content");
    NOM_ASSERT( parent != nullptr );
    if( parent )
    {
      Rocket::Core::ElementUtilities::GetElementsByTagName( tags, parent, "datagridcolumn" );

      NOM_ASSERT( col < tags.size() );
      if( col < tags.size() )
      {
        return tags[col]->GetInnerRML().CString();
      }
    }
  }

  // ..No title found?
  return "\0";
}

bool UIDataViewList::set_column_title( int col, const std::string& name )
{
  ElementList tags;

  NOM_ASSERT( this->valid() != false );
  if( this->valid() == true )
  {
    Element* parent = this->document()->GetElementById("content");
    NOM_ASSERT( parent != nullptr );
    if( parent )
    {
      Rocket::Core::ElementUtilities::GetElementsByTagName( tags, parent, "datagridcolumn" );

      NOM_ASSERT( col < tags.size() );
      if( col < tags.size() )
      {
        tags[col]->SetInnerRML( name.c_str() );
        return true;
      }
    }
  }

  return false;
}

CardsDataSource* UIDataViewList::data_source() const
{
  return this->data_source_;
}

void UIDataViewList::set_data_source( CardsDataSource* model )
{
  this->data_source_ = model;
}

CardCollection* UIDataViewList::database() const
{
  return this->database_;
}

void UIDataViewList::set_database( CardCollection* db )
{
  this->database_ = db;
}

int UIDataViewList::current_page() const
{
  return this->current_page_;
}

int UIDataViewList::total_pages() const
{
  return this->total_pages_;
}

void UIDataViewList::set_total_pages( int num_pages )
{
  this->total_pages_ = num_pages;
}

void UIDataViewList::set_current_page( int pg )
{
  this->current_page_ = pg;
}

int UIDataViewList::per_page() const
{
  return this->per_page_;
}

// current_page_ + 1 * id(selection_) - 1 = 5*11-1
int UIDataViewList::selection() const
{
  return this->selection_;
}

void UIDataViewList::set_selection( int idx )
{
  this->selection_ = idx;
}

// cards.begin() = id = per_page() * pg
// cards.end() = id + per_page()
void UIDataViewList::set_page( int pg )
{
  CardsDataSource* model = this->data_source();
  CardCollection* db = this->database();

  NOM_ASSERT( model != nullptr && db != nullptr);
  if( model && db )
  {
    CardList cards = db->cards();

    int start_id = this->per_page() * pg;
    int end_id = start_id + this->per_page();

    // Sanity check
    if( start_id > db->num_rows() ) return;

    int i = 0;
    for( auto idx = start_id; idx != end_id; ++idx )
    {
      model->insert_card( i, cards[idx] );
      ++i;
    }
  }
}

// Private scope

void UIDataViewList::_initialize()
{
  this->per_page_ = 11;
  // this->total_pages = db.size() / per_page();
  // this->total_pages = 10;
  this->set_total_pages( 5 );
  this->set_current_page( 0 );
  this->set_selection( 0 );
}

} // namespace nom
