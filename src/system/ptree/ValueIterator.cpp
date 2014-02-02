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
#include "nomlib/system/ptree/ValueIterator.hpp"

namespace nom {

ValueIterator::ValueIterator( void )
{
  //NOM_LOG_TRACE(NOM);
}

ValueIterator::~ValueIterator( void )
{
  //NOM_LOG_TRACE(NOM);
}

ValueIterator::ValueIterator( const ValueIterator& copy )
{
  //NOM_LOG_TRACE(NOM);
  this->copy( copy );
}

ValueIterator::ValueIterator( const ArrayIterator& itr ) :
  ValueIteratorBase ( itr )
{
  //NOM_LOG_TRACE(NOM);
}

ValueIterator::ValueIterator( const ObjectIterator& itr ) :
  ValueIteratorBase ( itr )
{
  //NOM_LOG_TRACE(NOM);
}

ValueIterator::SelfType& ValueIterator::operator =( const SelfType& other )
{
  this->copy( other );

  return *this;
}

ValueIterator::ValueTypeReference ValueIterator::operator *( void ) const
{
  return this->dereference();
}

ValueIterator::ValueTypePointer ValueIterator::operator ->( void ) const
{
  return this->pointer();
}

ValueIterator::SelfType& ValueIterator::operator ++( void )
{
   this->increment();

   return *this;
}

ValueIterator::SelfType ValueIterator::operator ++( sint )
{
   SelfType itr( *this );
   ++*this;

   return itr;
}

ValueIterator::SelfType& ValueIterator::operator --( void )
{
   this->decrement();

   return *this;
}

ValueIterator::SelfType ValueIterator::operator --( sint )
{
   SelfType itr( *this );

   return itr;
}

} // namespace nom
