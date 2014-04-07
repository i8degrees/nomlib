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
#include "nomlib/ptree/ValueConstIterator.hpp"

namespace nom {

ValueConstIterator::ValueConstIterator( void )
{
  //NOM_LOG_TRACE(NOM);
}

ValueConstIterator::~ValueConstIterator( void )
{
  //NOM_LOG_TRACE(NOM);
}

ValueConstIterator::ValueConstIterator( const ValueConstIterator& copy )
{
  //NOM_LOG_TRACE(NOM);
  this->copy( copy );
}

ValueConstIterator::ValueConstIterator( const ObjectIterator& itr ) :
  ValueIteratorBase ( itr )
{
  //NOM_LOG_TRACE(NOM);
}

ValueConstIterator::SelfType& ValueConstIterator::operator =( const DerivedType& other )
{
  this->copy( other );

  return *this;
}

ValueConstIterator::ConstReference ValueConstIterator::operator *( void ) const
{
  return this->dereference(); // const nom::Value&
}

const ValueConstIterator::ValueTypePointer ValueConstIterator::operator ->( void ) const
{
  return this->pointer();
}

ValueConstIterator::SelfType& ValueConstIterator::operator ++( void )
{
   this->increment();

   return *this;
}

ValueConstIterator::SelfType ValueConstIterator::operator ++( sint )
{
   SelfType itr( *this );
   ++*this;

   return itr;
}

ValueConstIterator::SelfType& ValueConstIterator::operator --( void )
{
   this->decrement();

   return *this;
}

ValueConstIterator::SelfType ValueConstIterator::operator --( sint )
{
   SelfType itr( *this );

   return itr;
}

} // namespace nom
