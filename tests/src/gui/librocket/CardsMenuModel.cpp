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
#include "nomlib/tests/gui/librocket/CardsMenuModel.hpp"

/// \brief Disable NOM_ASSERT macros so that they do not interfere with tests
/// that check for failure conditions; i.e.: libRocketDataGridTest
/// ::DataSourceModelSanity.
#if defined( NOM_ASSERT )
  #undef NOM_ASSERT
  #define NOM_ASSERT(expr)
#endif // defined NOM_ASSERT

namespace nom {

// Card

Card::Card()
{
  //
}

Card::~Card()
{
  //
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

const std::string& Card::num() const
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
  this->num_ = std::to_string(num);
}

// CardsMenuModel

CardsMenuModel::CardsMenuModel( const std::string& source,
                               const std::string& table_name ) :
  Rocket::Controls::DataSource( source.c_str() )
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );

  this->set_table_name( table_name );
}

CardsMenuModel::~CardsMenuModel()
{
  NOM_LOG_TRACE_PRIO( NOM_LOG_CATEGORY_TRACE, nom::NOM_LOG_PRIORITY_VERBOSE );
}

void CardsMenuModel::GetRow( Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns )
{
  if( table == this->table_name().c_str() )
  {
    for( auto i = 0; i != columns.size(); ++i )
    {
      // TODO: Implement column
      if( columns[i] == "id" )
      {
        continue;
      }
      else if( columns[i] == "name" )
      {
        row.push_back( this->db_[row_index].name().c_str() );
      }
      else if( columns[i] == "num" )
      {
        row.push_back( this->db_[row_index].num().c_str() );
      }
    } // end for cols loop
  }
} // end GetRow func

int CardsMenuModel::GetNumRows( const Rocket::Core::String& table )
{
  if( table == this->table_name().c_str() )
  {
    return this->db_.size();
  }

  // Err; ..invalid table?
  return -1;
}

const std::string& CardsMenuModel::table_name() const
{
  return this->table_name_;
}

void CardsMenuModel::set_table_name( const std::string& name )
{
  this->table_name_ = name;
}

int CardsMenuModel::num_rows()
{
  return this->GetNumRows( this->table_name().c_str() );
}

void CardsMenuModel::row( int row, const std::string& column_name, nom::StringList& rows )
{
  Rocket::Core::StringList results;
  Rocket::Core::StringList columns;

  columns.push_back( column_name.c_str() );

  this->GetRow( results, this->table_name().c_str(), row, columns );

  for( auto itr = results.begin(); itr != results.end(); ++itr )
  {
    rows.push_back( (*itr).CString() );
  }
}

int CardsMenuModel::insert_card( int pos, const Card& card )
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

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos;
}

int CardsMenuModel::insert_cards( int pos, const CardList& cards )
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

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos;
}

int CardsMenuModel::append_card( const Card& card )
{
  int rows = this->num_rows();

  this->db_.push_back( card );

  NotifyRowAdd( this->table_name().c_str(),
                // The index of the first row added
                rows,
                // The number of rows added -- including first row
                1 );

  // Resulting (modified) size
  return this->num_rows();
}

int CardsMenuModel::append_cards( const CardList& cards )
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

  // Resulting (modified) size
  return this->num_rows();
}

bool CardsMenuModel::empty() const
{
  return this->db_.empty();
}

void CardsMenuModel::erase_cards()
{
  this->db_.clear();

  NotifyRowChange( this->table_name().c_str() );
}

int CardsMenuModel::erase_card( int pos )
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

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos; // -1
}

int CardsMenuModel::erase_cards( int begin_pos, int end_pos )
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

    // Resulting (modified) size
    return this->num_rows();
  }

  // Err
  return nom::npos; // -1
}

std::string CardsMenuModel::dump()
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
  this->cards_.push_back(Card( 44,"Iron Giant",1));
  this->cards_.push_back(Card( 45,"Behemoth",1));
  this->cards_.push_back(Card( 46,"Chimera",1));
  this->cards_.push_back(Card( 47,"PuPu",1));
  this->cards_.push_back(Card( 48,"Elastoid",1));
  this->cards_.push_back(Card( 49,"GIM47N",1));
  this->cards_.push_back(Card( 50,"Malboro",1));
  this->cards_.push_back(Card( 51,"Ruby Dragon",1));
  this->cards_.push_back(Card( 52,"Elnoyle",1));
  this->cards_.push_back(Card( 53,"Tonberry King",1));
  this->cards_.push_back(Card( 54,"Wedge, Biggs",1));

  return true;
}

int CardCollection::lookup_id( const std::string& name ) const
{
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr )
  {
    if( (*itr).name() == name )
    {
      // Successful match
      return (*itr).id();
    }
  }

  // No match
  return nom::npos;
}

std::string CardCollection::lookup_name( int id ) const
{
  for( auto itr = this->cards_.begin(); itr != this->cards_.end(); ++itr )
  {
    if( (*itr).id() == id )
    {
      // Successful match
      return (*itr).name();
    }
  }

  // No match
  return "\0";
}

std::string CardCollection::dump()
{
  std::stringstream os;

  os << "cards " << "[" << this->cards_.size() << "]" << std::endl;

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
