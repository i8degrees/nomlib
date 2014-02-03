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
#include "nomlib/gui/Drawables.hpp"

namespace nom {

Drawables::Drawables( void )
{
  // NOM_LOG_TRACE( NOM );

  this->set_updated( false );
}

Drawables::~Drawables( void )
{
  // NOM_LOG_TRACE( NOM );
}

Drawables::Drawables( const Drawables::container& object )
{
  // NOM_LOG_TRACE( NOM );

  this->assign( object );

  this->set_updated( false );
}

const Drawables::container& Drawables::drawables( void ) const
{
  return this->drawables_;
}

bool Drawables::updated( void ) const
{
  return this->updated_;
}

void Drawables::set_updated( bool state )
{
  this->updated_ = state;
}

void Drawables::assign( const container& objects )
{
  this->drawables_ = objects;

  this->set_updated( false );
}

void Drawables::insert( uint pos, const value_type::raw_ptr object )
{
  auto itr = this->drawables_.begin() + pos;

  this->drawables_.insert( itr, IDrawable::shared_ptr( object ) );

  this->set_updated( false );
}

void Drawables::push_back( const value_type::raw_ptr object )
{
  this->drawables_.push_back( IDrawable::shared_ptr( object ) );

  this->set_updated( false );
}

const Drawables::value_type::shared_ptr& Drawables::at( uint pos )
{
  return this->drawables_.at( pos );
}

bool Drawables::empty( void ) const
{
  return this->drawables_.empty();
}

uint Drawables::size( void ) const
{
  return this->drawables_.size();
}

void Drawables::clear( void )
{
  this->drawables_.clear();

  this->set_updated( false );
}

Drawables::iterator Drawables::erase( uint pos )
{
  auto itr = this->drawables_.begin() + pos;

  return( this->drawables_.erase( itr ) );

  this->set_updated( false );
}

/*
bool Drawables::erase( const value_type::raw_ptr object )
{
  Drawables::container objects = this->drawables();

  for( auto itr = objects.begin(); itr != objects.end(); ++itr )
  {
    // Erase the object based on object type equality
    if( object->type() == (*itr)->type() )
    {
      this->drawables().erase( itr );
      this->set_updated( false );

      return true;
    }
  }

  return false;
}
*/

Drawables::const_iterator Drawables::begin( void ) const
{
  return this->drawables_.begin();
}

Drawables::const_iterator Drawables::end( void ) const
{
  return this->drawables_.end();
}

Drawables::iterator Drawables::begin( void )
{
  return this->drawables_.begin();
}

Drawables::iterator Drawables::end( void )
{
  return this->drawables_.end();
}

} // namespace nom

