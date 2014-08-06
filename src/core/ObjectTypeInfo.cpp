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
#include "nomlib/core/ObjectTypeInfo.hpp"

namespace nom {

ObjectTypeInfo::ObjectTypeInfo( void ) :
  type_{ typeid( this ) }
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo::~ObjectTypeInfo( void )
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo::ObjectTypeInfo( const ObjectTypeInfo::self_type& rhs ) :
  type_{ rhs.type() }
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo::self_type& ObjectTypeInfo::operator =( const ObjectTypeInfo::self_type& rhs )
{
  this->set_type( rhs.type() );

  return *this;
}

ObjectTypeInfo::ObjectTypeInfo( const ObjectTypeInfo::value_type& object ) :
  type_{ object }
{
  // NOM_LOG_TRACE( NOM );
}

const std::string ObjectTypeInfo::name( void ) const
{
  return this->type().name();
}

size_t ObjectTypeInfo::hash_code( void ) const
{
  return this->type().hash_code();
}

void ObjectTypeInfo::set_type( const ObjectTypeInfo::value_type& object )
{
  this->type_ = object;
}

bool ObjectTypeInfo::operator ==( const self_type& rhs ) const
{
  return( this->type() == rhs.type() );
}

bool ObjectTypeInfo::operator !=( const self_type& rhs ) const
{
  return( this->type() != rhs.type() );
}

// Private scope

const ObjectTypeInfo::value_type& ObjectTypeInfo::type( void ) const
{
  return this->type_;
}

} // namespace nom
