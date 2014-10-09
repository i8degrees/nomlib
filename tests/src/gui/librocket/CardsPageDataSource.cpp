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
#include "nomlib/tests/gui/librocket/CardsPageDataSource.hpp"

/// \brief Disable NOM_ASSERT macros so that they do not interfere with tests
/// that check for failure conditions; i.e.: libRocketDataGridTest
/// ::DataSourceModelSanity.
#if defined( NOM_ASSERT )
  #undef NOM_ASSERT
  #define NOM_ASSERT(expr)
#endif // defined NOM_ASSERT

using namespace Rocket::Core;

namespace nom {

CardsPageDataSource::CardsPageDataSource( const std::string& source,
                                          const std::string& table_name ) :
  Rocket::Controls::DataSource( source.c_str() )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  // Defaults
  this->set_per_page(11);
  this->set_total_pages(0);
  this->set_page(0);
  this->set_table_name(table_name);
}

CardsPageDataSource::~CardsPageDataSource()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void CardsPageDataSource::GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns )
{
  // The starting ID position to begin reading from the storage container
  int row_offset = row_index + ( this->per_page() * this->page() );

  // The last ID position; should match return value of ::GetNumRows
  int row_end = this->per_page();

  // Out of bounds
  if( row_offset >= this->num_rows() ) return;

  if( table == this->table_name().c_str() )
  {
    for( auto i = 0; i != columns.size(); ++i )
    {
      if( columns[i] == "status" && row_index < row_end )
      {
        if( row_index < row_end && this->db_[row_offset].num() < 1 )
        {
          // Unavailable
          row.push_back("0");
        }
        else
        {
          // Available
          row.push_back("1");
        }
      }
      else if( row_index < row_end && columns[i] == "available" )
      {
        String card_class =
          "unavailable-card";

        if( this->db_[row_offset].num() > 0 ) {
          card_class = "available-card";
        }

        row.push_back(card_class);
      }
      else if( row_index < row_end && columns[i] == "id" )
      {
        String card_id =
          this->db_[row_offset].name().c_str();

        row.push_back(card_id);
      }
      else if( row_index < row_end && columns[i] == "name" )
      {
        String card_name =
          this->db_[row_offset].name().c_str();

        row.push_back(card_name);
      }
      else if( row_index < row_end && columns[i] == "num" )
      {
        String card_num =
          std::to_string( this->db_[row_offset].num() ).c_str();

        row.push_back(card_num);
      }
    } // end for cols loop
  }
} // end GetRow func

int CardsPageDataSource::GetNumRows( const Rocket::Core::String& table )
{
  if( table == this->table_name().c_str() )
  {
    // Influences the maximum number of row items returned from ::GetRow
    return this->per_page();
  }

  NOM_ASSERT( table == this->table_name().c_str() );

  // Err; ..invalid table?
  return -1;
}

const std::string& CardsPageDataSource::table_name() const
{
  return this->table_name_;
}

void CardsPageDataSource::set_table_name( const std::string& name )
{
  this->table_name_ = name;
}

nom::size_type CardsPageDataSource::num_rows()
{
  return this->db_.size();
}

void CardsPageDataSource::row(  nom::StringList& row,
                            // const std::string& table,
                            int row_index,
                            const StringList& columns )
{
  Rocket::Core::StringList ret_rows;
  Rocket::Core::StringList ret_columns;

  for( auto itr = columns.begin(); itr != columns.end(); ++itr )
  {
    ret_columns.push_back( (*itr).c_str() );
  }

  // this->GetRow( ret_rows, table.c_str(), row_index, ret_columns );
  this->GetRow( ret_rows, this->table_name().c_str(), row_index, ret_columns );

  for( auto itr = ret_rows.begin(); itr != ret_rows.end(); ++itr )
  {
    row.push_back( (*itr).CString() );
  }
}

int CardsPageDataSource::insert_card( int pos, const Card& card )
{
  int rows = this->num_rows();

  NOM_ASSERT( pos < rows );
  if( pos < rows )
  {
    this->db_.at(pos) = card;

    NotifyRowChange(  this->table_name().c_str(),
                      // First row index changed -- zero-based index
                      // pos,
                      // Translate internal position to page position
                      this->map_page_row( pos, this->page() ),
                      // The number of rows changed -- including the first
                      // row
                      1 );

    // Update page count
    this->update();

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos;
}

int CardsPageDataSource::insert_cards( int pos, const CardList& cards )
{
  int rows = this->num_rows();

  NOM_ASSERT( pos < rows );
  if( pos < rows )
  {
    int idx = 0;
    for( auto itr = cards.begin(); itr != cards.end(); ++itr )
    {
      this->db_.insert( this->db_.begin() + (pos + idx), (*itr) );
      ++idx;
    }
    // this->db_.insert( this->db_.begin() + pos, cards.size() );

    NotifyRowChange(  this->table_name().c_str(),
                      // First row index changed -- zero-based index
                      // pos,
                      // Translate internal position to page position
                      this->map_page_row( pos, this->page() ),
                      // The number of rows changed -- including the first
                      // row
                      cards.size() );

    // Update page count
    this->update();

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos;
}

int CardsPageDataSource::append_card( const Card& card )
{
  int rows = this->num_rows();

  this->db_.push_back( card );

  NotifyRowAdd( this->table_name().c_str(),
                // The index of the first row added
                // rows,
                // Translate internal position to page position
                this->map_page_row( rows, this->page() ),
                // The number of rows added -- including first row
                1 );

  // Update page count
  this->update();

  // Resulting (modified) size
  return this->num_rows();
}

int CardsPageDataSource::append_cards( const CardList& cards )
{
  int rows = this->num_rows();

  for( auto itr = cards.begin(); itr != cards.end(); ++itr )
  {
    this->db_.push_back( (*itr) );
  }

  NotifyRowAdd( this->table_name().c_str(),
                // The index of the first row added
                // rows,
                // Translate internal position to page position
                this->map_page_row( rows, this->page() ),
                // The number of rows added -- including first row
                cards.size() );

  // Update page count
  this->update();

  // Resulting (modified) size
  return this->num_rows();
}

bool CardsPageDataSource::empty() const
{
  return this->db_.empty();
}

void CardsPageDataSource::erase_cards()
{
  this->db_.clear();

  NotifyRowChange( this->table_name().c_str() );

  // Update page count
  this->update();
}

int CardsPageDataSource::erase_card( int pos )
{
  int rows = this->num_rows();

  NOM_ASSERT( pos < rows );
  if( pos < rows )
  {
    this->db_.erase( this->db_.begin() + pos );

    NotifyRowRemove(  this->table_name().c_str(),
                      // First row index removed -- zero-based index
                      // pos,
                      // Translate internal position to page position
                      this->map_page_row( pos, this->page() ),
                      // The number of rows changed -- including the first
                      // row
                      1 );

    // Update page count
    this->update();

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos; // -1
}

int CardsPageDataSource::erase_cards( int begin_pos, int end_pos )
{
  int rows = this->num_rows();

  NOM_ASSERT( (begin_pos < rows) && (end_pos < rows) );
  if( (begin_pos < rows) && (end_pos < rows) )
  {
    this->db_.erase(  (this->db_.begin() + begin_pos),
                      (this->db_.begin() + end_pos) );

    NotifyRowRemove(  this->table_name().c_str(),
                      // First row index removed -- zero-based index
                      // begin_pos,
                      // Translate internal position to page position
                      this->map_page_row( begin_pos, this->page() ),
                      // The number of rows changed -- including the first
                      // row
                      begin_pos + end_pos );

    // Update page count
    this->update();

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos; // -1
}

std::string CardsPageDataSource::dump()
{
  std::stringstream os;

  // Table && number of rows (size)
  os
  << "table: " << this->table_name()
  << "[" << this->num_rows() << "]" << std::endl;

  os << std::endl;
  for( auto itr = this->db_.begin(); itr != this->db_.end(); ++itr )
  {
    os
    // ID && card name
    << (*itr).id()
    << " "
    << (*itr).name()

    // Number of cards
    << "\t"
    << (*itr).num()
    << std::endl;
  }

  return os.str();
}

const Card& CardsPageDataSource::lookup_by_name( const std::string& name ) const
{
  for( auto itr = this->db_.begin(); itr != this->db_.end(); ++itr )
  {
    if( (*itr).name() == name )
    {
      // Successful match
      return *itr;
    }
  }

  // No match
  return Card::null;
}

const Card& CardsPageDataSource::lookup_by_id( int id ) const
{
  for( auto itr = this->db_.begin(); itr != this->db_.end(); ++itr )
  {
    if( (*itr).id() == id )
    {
      // Successful match
      return *itr;
    }
  }

  // No match
  return Card::null;
}

int CardsPageDataSource::per_page() const
{
  return this->per_page_;
}

int CardsPageDataSource::total_pages() const
{
  return this->total_pages_;
}

int CardsPageDataSource::page() const
{
  return this->page_;
}

void CardsPageDataSource::set_per_page( int pg )
{
  this->per_page_ = pg;
}

void CardsPageDataSource::set_page( int page )
{
  this->page_ = page;

  // NotifyRowChange( this->table_name().c_str() );

  // This is necessary for ::GetRow to immediately update its row indexes to
  // match the requested page
  NotifyRowChange(  this->table_name().c_str(),
                    // First row index changed -- zero-based index
                    0,
                    // The number of rows changed -- including the first
                    // row
                    11 );

  // Update total page count
  this->update();
}

// curr_page * per_page + selection
int CardsPageDataSource::map_page_row(int index, int pg) const
{
  int selection = -1;
  NOM_ASSERT( index < this->num_rows() );
  NOM_ASSERT( index < 0 && pg < 0 );
  NOM_ASSERT( pg < this->total_pages() );

  selection = index - ( this->per_page() * pg );
  NOM_ASSERT( selection != -1 );

  return selection;
}

// Private scope

void CardsPageDataSource::set_total_pages( int num_pages )
{
  this->total_pages_ = num_pages;
}

void CardsPageDataSource::update()
{
  // Calculate total number of pages we need...
  float pages = this->num_rows() / this->per_page();
  pages = floor(pages + 1.0f);

  this->set_total_pages( pages );
}

// CardStatusFormatter

CardStatusFormatter::CardStatusFormatter() :
  Rocket::Controls::DataFormatter("card_status")
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

CardStatusFormatter::CardStatusFormatter( const std::string& formatter ) :
  Rocket::Controls::DataFormatter( formatter.c_str() )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

CardStatusFormatter::~CardStatusFormatter()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void CardStatusFormatter::FormatData( Rocket::Core::String& formatted_data,
                                      const Rocket::Core::StringList& raw_data )
{
  std::string styling = "";

  if( raw_data[0] == "0" ) {
    formatted_data = String("<status class='unavailable' ") +
      styling.c_str() + String(" />");
  }
  else if( raw_data[0] == "1" ) {
    formatted_data = String("<status class='available' ") +
      styling.c_str() + String(" />");
  }
}

// CardNameFormatter

CardNameFormatter::CardNameFormatter() :
  Rocket::Controls::DataFormatter("card_name")
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

CardNameFormatter::CardNameFormatter( const std::string& formatter ) :
  Rocket::Controls::DataFormatter( formatter.c_str() )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

CardNameFormatter::~CardNameFormatter()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void CardNameFormatter::FormatData( Rocket::Core::String& formatted_data,
                                    const Rocket::Core::StringList& raw_data )
{
  if( raw_data.size() == 3 ) {

    formatted_data = "<card class='" + raw_data[0] + "'" +
      "id='" + raw_data[1] + "'>" + raw_data[2] + "</card>";
  }
}

} // namespace nom
