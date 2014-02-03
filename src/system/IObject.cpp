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
#include "nomlib/system/IObject.hpp"

namespace nom {

uint64 IObject::total_alloc_bytes = 0;
uint64 IObject::total_dealloc_bytes = 0;

IObject::IObject( void )
{
  // NOM_LOG_TRACE( NOM );
}

IObject::~IObject( void )
{
  // NOM_LOG_TRACE( NOM );
}

ObjectTypeInfo IObject::type( void ) const
{
  return NOM_OBJECT_TYPE_INFO( self_type );
}

bool IObject::is_type( const ObjectTypeInfo& rhs ) const
{
  // Object is of the same type -- name and hash code meta-data should always
  // match.
  if( this->type() == rhs )
  {
    return true;
  }

  return false;
}

void* IObject::operator new( size_t mem )
{
  // std::stringstream os;

  // os << "Memory allocation of: " << mem << " bytes.";
  // NOM_LOG_INFO( NOM, os.str() );

  IObject::total_alloc_bytes = IObject::total_alloc_bytes + mem;

  return std::malloc( mem );
}

void IObject::operator delete( void* ptr )
{
  // std::stringstream os;

  // os << "Memory deallocation of: " << sizeof( ptr ) << " bytes.";
  // NOM_LOG_INFO( NOM, os.str() );

  IObject::total_dealloc_bytes = IObject::total_dealloc_bytes + sizeof( ptr );

  std::free( ptr );
}

} // namespace nom
