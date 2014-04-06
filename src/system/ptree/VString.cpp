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
#include "nomlib/system/ptree/VString.hpp"

namespace nom {

VString::VString( void )  :
  value_( nullptr ),
  index_( 0 )
{
  //NOM_LOG_TRACE(NOM);
}

VString::~VString( void )
{
  //NOM_LOG_TRACE(NOM);

  // free( const_cast<char*> ( this->value_ ) );
  // this->value_ = nullptr;
}

VString::VString( ArrayIndex index ) :
  value_( nullptr ),
  index_( index )
{
  //NOM_LOG_TRACE(NOM);
}

VString::VString( const char* key ) :
  index_( 0 )
{
  //NOM_LOG_TRACE(NOM);

  uint size = strlen( key );
  this->value_ = priv::duplicate_string( key, size );
}

VString::VString( const std::string& key )  :
  index_( 0 )
{
  //NOM_LOG_TRACE(NOM);

  uint size = strlen( key.c_str() );
  this->value_ = priv::duplicate_string( key.c_str(), size );
}

VString::VString( const SelfType& copy )  :
  index_{ copy.index() }
{
  // NOM_LOG_TRACE(NOM);

  this->value_ = copy.value_;
}

VString::SelfType& VString::operator =( const SelfType& other )
{
  VString temp( other );
  this->swap( temp );

  return *this;
}

void VString::swap( VString& other )
{
  std::swap( this->value_, other.value_ );
  std::swap( this->index_, other.index_ );
}

bool VString::valid( void ) const
{
  if( this->c_str() != nullptr ) return true;

  return false;
}

bool VString::operator <( const VString& other ) const
{
  // Key member
  if( this->valid() && other.valid() )
  {
    return strcmp( this->c_str(), other.c_str() ) < 0;
  }

  // Array element index
  else
  {
    return( this->index() < other.index() );
  }
}

bool VString::operator ==( const VString& other ) const
{
  // Key member
  if( this->valid() && other.valid() )
  {
    return strcmp( this->c_str(), other.c_str() ) == 0;
  }

  // Array element index
  else
  {
    return( this->index() == other.index() );
  }
}

const char* VString::c_str( void ) const
{
  return this->value_;
}

const std::string VString::get_string( void ) const
{
  return std::string( this->c_str() );
}

ArrayIndex VString::index( void ) const
{
  return this->index_;
}

} // namespace nom
