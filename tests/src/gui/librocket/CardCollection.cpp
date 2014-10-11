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
#include "nomlib/tests/gui/librocket/CardCollection.hpp"

namespace nom {

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

  // pg 5 (test partial page calculations)
  this->cards_.push_back(Card( 55,"Fujin, Raijin",0));
  this->cards_.push_back(Card( 56,"Elvoret",0));

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
