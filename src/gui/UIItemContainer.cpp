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
#include "nomlib/gui/UIItemContainer.hpp"

namespace nom {

UIItemContainer::UIItemContainer( void ) :
  selection_{ nom::npos }
{
  // NOM_LOG_TRACE( NOM );
}

UIItemContainer::~UIItemContainer( void )
{
  // NOM_LOG_TRACE( NOM );
}

UIItemContainer::UIItemContainer( const self_type& rhs ) :
  labels_{ rhs.item_labels() },
  selection_{ nom::npos }
{
  // NOM_LOG_TRACE( NOM );
}

UIItemContainer::self_type& UIItemContainer::operator =( const self_type& rhs )
{
  labels_ = rhs.item_labels();
  this->set_selection( rhs.selection() );

  return *this;
}

UIItemContainer::raw_ptr UIItemContainer::clone( void ) const
{
  return UIItemContainer::raw_ptr( new UIItemContainer( *this ) );
}

const UIItemContainer::self_type& UIItemContainer::get( void ) const
{
  return *this;
}

int UIItemContainer::selection( void ) const
{
  return this->selection_;
}

const std::string UIItemContainer::item_label( uint pos ) const
{
  if( pos <= this->size() )
  {
    return this->labels_.at( pos );
  }
  else
  {
    return "\0";
  }
}

const ItemStrings& UIItemContainer::item_labels( void ) const
{
  return this->labels_;
}

bool UIItemContainer::empty( void ) const
{
  return this->labels_.empty();
}

uint UIItemContainer::size( void ) const
{
  return this->labels_.size();
}

void UIItemContainer::clear( void )
{
  this->labels_.clear();
}

void UIItemContainer::erase( uint pos )
{
  this->labels_.erase( this->labels_.begin() + pos );
}

void UIItemContainer::set_selection( uint pos )
{
  // NOM_ASSERT( this->size() <= pos );
  if( this->size() <= pos )
  {
    // This indexed position does not exist; de-selection.
    this->selection_ = nom::npos;
  }

  this->selection_ = pos;
}

bool UIItemContainer::set_selection( const std::string& label )
{
  int pos = this->find( label );

  if( pos != nom::npos )
  {
    this->selection_ = pos;

    return true;
  }

  return false;
}

int UIItemContainer::find( const std::string& label )
{
  int pos = nom::npos;

  for( auto itr = this->labels_.begin(); itr != this->labels_.end(); ++itr )
  {
    if( *itr == label )
    {
      // Text label found
      return pos;
    }

    // Increment index position
    ++pos;
  }

  // Text label not found
  return pos;
}

int UIItemContainer::insert( uint pos, const std::string& label )
{
  this->labels_.insert( this->labels_.begin()+pos, label );

  return this->size();
}

int UIItemContainer::insert( uint pos, const std::initializer_list<std::string>& labels )
{
  this->labels_.insert( this->labels_.begin()+pos, labels );

  return this->size();
}

int UIItemContainer::append( const std::string& label )
{
  this->labels_.push_back( label );

  return this->size();
}

int UIItemContainer::append( const std::vector<std::string>& labels )
{
  for( auto itr = labels.begin(); itr != labels.end(); ++itr )
  {
    this->labels_.push_back( *itr  );
  }

  return this->size();
}

} // namespace nom
