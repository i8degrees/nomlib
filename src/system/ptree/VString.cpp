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
  index_ ( 0 )
{
  //NOM_LOG_TRACE(NOM);
}

VString::~VString( void )
{
  //NOM_LOG_TRACE(NOM);
}

VString::VString( ArrayIndex val ) :
  index_ ( val )
{
  //NOM_LOG_TRACE(NOM);
}

VString::VString( const char* val ) :
  value_ { std::string( val ) },
  index_ ( 0 )
{
  //NOM_LOG_TRACE(NOM);
}

VString::VString( const std::string& val )  :
  value_ ( val )
{
  //NOM_LOG_TRACE(NOM);
}

VString::VString( const SelfType& copy )  :
  value_ ( copy.value_ ),
  index_ ( copy.index_ )
{
  // NOM_LOG_TRACE(NOM);
}

VString::SelfType& VString::operator =( const SelfType& other )
{
  this->value_ = other.value_;
  this->index_ = other.index_;

  return *this;
}

bool VString::operator <( const VString& other ) const
{
  if( ! this->value_.empty() ) // Member key
  {
    return this->value_ < other.value_;
  }
  else // Array or Object index
  {
    return this->index_ < other.index_;
  }
  // if ( this->valid() )
  // {
    // return strcmp( this->value_, other.value_ ) < 0;
  // }
}

bool VString::operator ==( const VString& other ) const
{
  if( ! this->value_.empty() ) // Member key
  {
    return this->value_ < other.value_;
  }
  else // Array or Object index
  {
    return this->index_ < other.index_;
  }
  // if ( this->valid() )
  // {
    // return strcmp( this->value_, other.value_ ) == 0;
  // }
}

/*
ArrayIndex VString::operator[]( ArrayIndex val )
{
  // this->index_ = val;

  // return this->index_;
  NOM_DUMP(val);
  return val;
}

const std::string& VString::operator[]( const char* value )
{
  // this->value_ = val;

  // return this->value_;
  NOM_DUMP(value);
  return value;
}
*/

const std::string& VString::c_str( void ) const
{
  return this->value_;
/*
  if( ! this->value_.empty() ) // Member key
  {
    return this->value_;
  }
  else // Array/Object index
  {
    return std::to_string( this->index_ );
  }
*/
}

ArrayIndex VString::index( void ) const
{
  return this->index_;
}

} // namespace nom
