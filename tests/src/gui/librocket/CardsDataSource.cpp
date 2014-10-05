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
#include "nomlib/tests/gui/librocket/CardsDataSource.hpp"

/// \brief Disable NOM_ASSERT macros so that they do not interfere with tests
/// that check for failure conditions; i.e.: libRocketDataGridTest
/// ::DataSourceModelSanity.
#if defined( NOM_ASSERT )
  #undef NOM_ASSERT
  #define NOM_ASSERT(expr)
#endif // defined NOM_ASSERT

namespace nom {

// Card

const Card Card::null = Card();

Card::Card() :
  id_(-1),
  name_("\0"),
  num_(0)
{
}

Card::~Card()
{
}

Card::Card( int id, const std::string& name, int num_cards )
{
  this->set_id(id);
  this->set_name(name);
  this->set_num( num_cards );
}

int Card::id() const
{
  return this->id_;
}

const std::string& Card::name() const
{
  return this->name_;
}

int Card::num() const
{
  return this->num_;
}

void Card::set_id(int id)
{
  this->id_ = id;
}

void Card::set_name(const std::string& name)
{
  this->name_ = name;
}

void Card::set_num(int num)
{
  this->num_ = num;
}

// CardsDataSource

CardsDataSource::CardsDataSource( const std::string& source,
                                  const std::string& table_name ) :
  Rocket::Controls::DataSource( source.c_str() )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_per_page(11);
  this->set_total_pages(0);
  this->set_page(0);
  this->set_table_name(table_name);
}

CardsDataSource::~CardsDataSource()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void CardsDataSource::GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns )
{
  // The starting ID position to begin reading from the storage container
  int row_offset = row_index + ( this->per_page() * this->page() );

  // The last ID position; should match return value of ::GetNumRows
  int row_end = this->per_page();

  if( table == this->table_name().c_str() )
  {
    for( auto i = 0; i != columns.size(); ++i )
    {
      if( columns[i] == "status" && row_index < row_end )
      {
        if( row_index < row_end && this->db_[row_offset].num() < 1 )
        {
          // Unavailable
          row.push_back( "0" );
        }
        else
        {
          // Available
          row.push_back( "1" );
        }
      }
      else if( row_index < row_end && columns[i] == "name" )
      {
        row.push_back( this->db_[row_offset].name().c_str() );
      }
      else if( row_index < row_end && columns[i] == "num" )
      {
        row.push_back( std::to_string( this->db_[row_offset].num() ).c_str() );
      }
    } // end for cols loop
  }
} // end GetRow func

int CardsDataSource::GetNumRows( const Rocket::Core::String& table )
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

const std::string& CardsDataSource::table_name() const
{
  return this->table_name_;
}

void CardsDataSource::set_table_name( const std::string& name )
{
  this->table_name_ = name;
}

nom::size_type CardsDataSource::num_rows()
{
  return this->db_.size();
}

void CardsDataSource::row(  nom::StringList& row,
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

int CardsDataSource::insert_card( int pos, const Card& card )
{
  int rows = this->num_rows();

  NOM_ASSERT( pos < rows );
  if( pos < rows )
  {
    this->db_.at(pos) = card;

    NotifyRowChange(  this->table_name().c_str(),
                      // First row index changed -- zero-based index
                      pos,
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

int CardsDataSource::insert_cards( int pos, const CardList& cards )
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
                      pos,
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

int CardsDataSource::append_card( const Card& card )
{
  int rows = this->num_rows();

  this->db_.push_back( card );

  NotifyRowAdd( this->table_name().c_str(),
                // The index of the first row added
                rows,
                // The number of rows added -- including first row
                1 );

  // Update page count
  this->update();

  // Resulting (modified) size
  return this->num_rows();
}

int CardsDataSource::append_cards( const CardList& cards )
{
  int rows = this->num_rows();

  for( auto itr = cards.begin(); itr != cards.end(); ++itr )
  {
    this->db_.push_back( (*itr) );
  }

  NotifyRowAdd( this->table_name().c_str(),
                // The index of the first row added
                rows,
                // The number of rows added -- including first row
                cards.size() );

  // Update page count
  this->update();

  // Resulting (modified) size
  return this->num_rows();
}

bool CardsDataSource::empty() const
{
  return this->db_.empty();
}

void CardsDataSource::erase_cards()
{
  this->db_.clear();

  NotifyRowChange( this->table_name().c_str() );

  // Update page count
  this->update();
}

int CardsDataSource::erase_card( int pos )
{
  int rows = this->num_rows();

  NOM_ASSERT( pos < rows );
  if( pos < rows )
  {
    this->db_.erase( this->db_.begin() + pos );

    NotifyRowRemove(  this->table_name().c_str(),
                      // First row index removed -- zero-based index
                      pos,
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

int CardsDataSource::erase_cards( int begin_pos, int end_pos )
{
  int rows = this->num_rows();

  NOM_ASSERT( (begin_pos < rows) && (end_pos < rows) );
  if( (begin_pos < rows) && (end_pos < rows) )
  {
    this->db_.erase(  (this->db_.begin() + begin_pos),
                      (this->db_.begin() + end_pos) );

    NotifyRowRemove(  this->table_name().c_str(),
                      // First row index removed -- zero-based index
                      begin_pos,
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

std::string CardsDataSource::dump()
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

const Card& CardsDataSource::lookup_by_name( const std::string& name ) const
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

const Card& CardsDataSource::lookup_by_id( int id ) const
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

int CardsDataSource::per_page() const
{
  return this->per_page_;
}

int CardsDataSource::total_pages() const
{
  return this->total_pages_;
}

int CardsDataSource::page() const
{
  return this->page_;
}

void CardsDataSource::set_per_page( int pg )
{
  this->per_page_ = pg;

  // Update page count
  this->update();
}

void CardsDataSource::set_total_pages( int num_pages )
{
  this->total_pages_ = num_pages;
}

void CardsDataSource::set_page( int page )
{
  this->page_ = page;

  // This is necessary for ::GetRow to immediately update its row indexes
  NotifyRowChange( this->table_name().c_str() );

  this->update();
}

void CardsDataSource::update()
{
  this->set_total_pages( this->num_rows() / this->per_page() );
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

// CardFormatter

CardFormatter::CardFormatter() :
  Rocket::Controls::DataFormatter("card")
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

CardFormatter::CardFormatter( const std::string& formatter ) :
  Rocket::Controls::DataFormatter( formatter.c_str() )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

CardFormatter::~CardFormatter()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void CardFormatter::FormatData( Rocket::Core::String& formatted_data,
                                const Rocket::Core::StringList& raw_data )
{
  if( raw_data[0].Length() > 0 ) {
    formatted_data = "<card id='" + raw_data[0] + "'>" + raw_data[0] + "</card>";
  }
}

// CardCollection

CardCollection::CardCollection()
{
  // ...
}

CardCollection::~CardCollection()
{
  // ...
}

nom::size_type CardCollection::num_rows() const
{
  return this->cards_.size();
}

CardList CardCollection::cards() const
{
  return this->cards_;
}

bool CardCollection::load_db()
{
  // Pg 0 -- initial population
  this->cards_.push_back(Card( 0,"Geezard",1));
  this->cards_.push_back(Card( 1,"Fungar",1));
  this->cards_.push_back(Card( 2,"Bite Bug",0));
  this->cards_.push_back(Card( 3,"Red Bat",4));
  this->cards_.push_back(Card( 4,"Blobra",2));
  this->cards_.push_back(Card( 5,"Gayla",1));
  this->cards_.push_back(Card( 6,"Gesper",3));
  this->cards_.push_back(Card( 7,"Fastitocalon-F",0));
  this->cards_.push_back(Card( 8,"Blood Soul",1));
  this->cards_.push_back(Card( 9,"Caterchipillar",2));
  this->cards_.push_back(Card( 10,"Cockatrice",3));

  // pg 1
  this->cards_.push_back(Card( 11,"Grat",1));
  this->cards_.push_back(Card( 12,"Buel",1));
  this->cards_.push_back(Card( 13,"Mesmerize",0));
  this->cards_.push_back(Card( 14,"Glacial Eye",1));
  this->cards_.push_back(Card( 15,"Belhelmel",1));
  this->cards_.push_back(Card( 16,"Thrustaevis",1));
  this->cards_.push_back(Card( 17,"Anacondaur",1));
  this->cards_.push_back(Card( 18,"Creeps",0));
  this->cards_.push_back(Card( 19,"Grendel",1));
  this->cards_.push_back(Card( 20,"Jelleye",1));
  this->cards_.push_back(Card( 21,"Grand Mantis",3));

  // pg 2
  this->cards_.push_back(Card( 22,"Forbidden",1));
  this->cards_.push_back(Card( 23,"Armadodo",1));
  this->cards_.push_back(Card( 24,"TriFace",1));
  this->cards_.push_back(Card( 25,"Fastitocalon",0));
  this->cards_.push_back(Card( 26,"Snow Lion",1));
  this->cards_.push_back(Card( 27,"Ochu",1));
  this->cards_.push_back(Card( 28,"SAM08G",1));
  this->cards_.push_back(Card( 29,"Death Claw",1));
  this->cards_.push_back(Card( 30,"Cactaur",0));
  this->cards_.push_back(Card( 31,"Tonberry",1));
  this->cards_.push_back(Card( 32,"Abyss Worm",1));

  // pg 3
  this->cards_.push_back(Card( 33,"Turtapod",1));
  this->cards_.push_back(Card( 34,"Vysage",1));
  this->cards_.push_back(Card( 35,"T-Rexaur",1));
  this->cards_.push_back(Card( 36,"Bomb",1));
  this->cards_.push_back(Card( 37,"Blitz",1));
  this->cards_.push_back(Card( 38,"Wendigo",1));
  this->cards_.push_back(Card( 39,"Torama",1));
  this->cards_.push_back(Card( 40,"Imp",1));
  this->cards_.push_back(Card( 41,"Blue Dragon",1));
  this->cards_.push_back(Card( 42,"Adamantiose",1));
  this->cards_.push_back(Card( 43,"Hexadragon",1));

  // pg 4
  this->cards_.push_back(Card( 44,"Iron Giant",0));
  this->cards_.push_back(Card( 45,"Behemoth",0));
  this->cards_.push_back(Card( 46,"Chimera",0));
  this->cards_.push_back(Card( 47,"PuPu",1));
  this->cards_.push_back(Card( 48,"Elastoid",0));
  this->cards_.push_back(Card( 49,"GIM47N",0));
  this->cards_.push_back(Card( 50,"Malboro",0));
  this->cards_.push_back(Card( 51,"Ruby Dragon",0));
  this->cards_.push_back(Card( 52,"Elnoyle",0));
  this->cards_.push_back(Card( 53,"Tonberry King",0));
  this->cards_.push_back(Card( 54,"Wedge, Biggs",0));

  return true;
}

const Card& CardCollection::lookup_by_name( const std::string& name ) const
{
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr )
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

const Card& CardCollection::lookup_by_id( int id ) const
{
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr )
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

std::string CardCollection::dump()
{
  std::stringstream os;

  os << "cards " << "[" << this->num_rows() << "]" << std::endl;

  os << std::endl;
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr )
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

} // namespace nom
