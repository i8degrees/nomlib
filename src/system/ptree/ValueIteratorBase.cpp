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
#include "nomlib/system/ptree/ValueIteratorBase.hpp"

namespace nom {

ValueIteratorBase::ValueIteratorBase ( void ) :
  type_ ( IteratorType::Null )
{
  //NOM_LOG_TRACE(NOM);
}

ValueIteratorBase::~ValueIteratorBase ( void )
{
  //NOM_LOG_TRACE(NOM);
}

ValueIteratorBase::ValueIteratorBase( const ArrayIterator& itr ) :
  array_ ( itr ),
  type_ ( IteratorType::ArrayValues )
{
  //NOM_LOG_TRACE(NOM);
  // this->iterator_.array_ = new ArrayIterator( itr );
}

ValueIteratorBase::ValueIteratorBase( const ObjectIterator& itr ) :
  object_ ( itr ),
  type_ ( IteratorType::ObjectValues )
{
  //NOM_LOG_TRACE(NOM);
  // this->iterator_.object_ = new ObjectIterator( itr );
}

bool ValueIteratorBase::valid( void ) const
{
  if( this->type() != IteratorType::Null ) return true;

  return false;
}

const char* ValueIteratorBase::key( void ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // return( this->object_->first.index() );
    return "\0"; // Not a valid nom::Object
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // Handle null-terminated member key -- member key value not found
    if( this->object_->first.c_str() != nullptr )
    {
      // Valid nom::Object member key
      return this->object_->first.c_str();
    }
    else
    {
      NOM_DUMP("index");
      std::string index = std::to_string( this->object_->first.index() );
      NOM_DUMP(index);
      return index.c_str();
    }
  }
  else // IteratorType::NullValue ???
  {
    // Member key not found
    return "\0";
  }
}

bool ValueIteratorBase::key( const std::string& member ) const
{
  // Not valid; we are not iterating through an nom::Object
  if( this->type() == IteratorType::ArrayValues )
  {
    return false;
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    auto itr = this->object_->first.c_str();
    if( itr != member ) return false;
    // if( this->object_->key != member ) return false;  // Member key not found
    return true;                                      // Member key found
  }
  else // IteratorType::Null ???
  {
    return false; // Member key not found
  }
}

// TODO: Handle index of array
int ValueIteratorBase::index( void ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    NOM_STUBBED(NOM);
    // return this->distance( this->array_ );
    return -1;
  }
  else // IteratorType::ObjectValue
  {
    return -1; // Not an array; no index to be returned!
  }
}

void ValueIteratorBase::copy( const SelfType& other )
{
  // if( this->type() == IteratorType::ArrayValue )
  // {
    // this->iterator_.array_ = other.iterator_.array_;
  // }
  // else if( this->type() == IteratorType::ObjectValue )
  // {
    // this->iterator_.object_ = other.iterator_.object_;
  // }

  this->array_ = other.array_;
  this->object_ = other.object_;
  this->type_ = other.type();
}

ValueIteratorBase::ValueTypeReference ValueIteratorBase::dereference( void ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // return this->iterator_.array_->ref();
    return this->array_->ref();
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // return this->iterator_.object_->value->ref();
    // return this->object_->value->ref();
    return this->object_->second;
  }
  else // IteratorType::NullValue
  {
    // return Value();
    // return this->object_->value->ref();
    return this->object_->second;
  }
}

ValueIteratorBase::ValueTypePointer ValueIteratorBase::pointer( void ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // return this->iterator_.array_->get();
    return this->array_->get();
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // return this->iterator_.object_->value->get();
    // return this->object_->value;
    // return this->object_->value->get();
    return this->object_->second.get();
  }
  else // IteratorType::NulLValue
  {
    return nullptr;
  }
}

bool ValueIteratorBase::operator ==( const SelfType& other ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // return this->iterator_.array_ == other.iterator_.array_;
    return this->array_ == other.array_;
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // return this->iterator_.object_ == other.iterator_.object_;
    return this->object_ == other.object_;
  }
  else // IteratorType::NullValue
  {
    return ( other.type() == IteratorType::Null );
  }
}

bool ValueIteratorBase::operator !=( const SelfType& other ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // return ! ( this->iterator_.array_ == other.iterator_.array_ );
    return ! ( this->array_ == other.array_ );
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // return ! ( this->iterator_.object_ == other.iterator_.object_ );
    return ! ( this->object_ == other.object_ );
  }
  else // IteratorType::NullValue
  {
    return ! ( other.type() == IteratorType::Null );
  }
}

ValueIteratorBase::DifferenceType ValueIteratorBase::operator -( const SelfType& other ) const
{
  return distance( other );
}

void ValueIteratorBase::increment( void )
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // ++this->iterator_.array_;
    ++this->array_;
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // ++this->iterator_.object_;
    ++this->object_;
  }
  else // IteratorType::NullValue
  {
    return; // Do nothing -- we aren't initialized!
  }
}

void ValueIteratorBase::decrement( void )
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // --this->iterator_.array_;
    --this->array_;
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // --this->iterator_.object_;
    --this->object_;
  }
  else // IteratorType::NullValue
  {
    return; // Do nothing -- we are not initialized!
  }
}

ValueIteratorBase::DifferenceType ValueIteratorBase::distance( const SelfType& other ) const
{
  if( this->type() == IteratorType::ArrayValues )
  {
    // return std::distance( this->iterator_.array_, other.iterator_.array_ );
    return std::distance( this->array_, other.array_ );
  }
  else if( this->type() == IteratorType::ObjectValues )
  {
    // return std::distance( this->iterator_.object_, other.iterator_.object_ );
    return std::distance( this->object_, other.object_ );
  }
  else // IteratorType::NullValue
  {
    return 0;
  }
}

enum ValueIteratorBase::IteratorType ValueIteratorBase::type( void ) const
{
  return this->type_;
}

} // namespace nom
