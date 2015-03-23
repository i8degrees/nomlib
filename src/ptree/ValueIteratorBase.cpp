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
#include "nomlib/ptree/ValueIteratorBase.hpp"

namespace nom {

ValueIteratorBase::ValueIteratorBase() :
  type_(IteratorType::Null)
{
  //NOM_LOG_TRACE(NOM);
}

ValueIteratorBase::~ValueIteratorBase()
{
  //NOM_LOG_TRACE(NOM);
}

ValueIteratorBase::ValueIteratorBase(const ObjectIterator& rhs) :
  object_(rhs),
  type_(IteratorType::ObjectValues)
{
  //NOM_LOG_TRACE(NOM);
}

bool ValueIteratorBase::valid() const
{
  if( this->type() != IteratorType::Null ) return true;

  return false;
}

const char* ValueIteratorBase::key() const
{
  if( this->type() == IteratorType::ObjectValues )
  {
    if( this->object_->first.c_str() != nullptr )
    {
      return this->object_->first.c_str();
    }
    else
    {
      return "\0";

      // Note that this also effects the print out of nom::Value::dump
      // std::string index = std::to_string( this->index() );

      // return index.c_str();
    }
  }
  else // IteratorType::NullValue ???
  {
    // Member key not found
    // return "\0";
      // std::string index = std::to_string( this->index() );

      // return index.c_str();
    return "\0";
  }
}

bool ValueIteratorBase::key(const std::string& member) const
{
  if( this->type() == IteratorType::ObjectValues )
  {
    auto itr = this->object_->first.c_str();

    if( itr != member ) return false;

    // Member key found
    return true;
  }
  else // IteratorType::Null ???
  {
    return false; // Member key not found
  }
}

ArrayIndex ValueIteratorBase::index() const
{
  const VString& key = this->object_->first;

  return key.index();
}

void ValueIteratorBase::copy(const SelfType& rhs)
{
  this->object_ = rhs.object_;
  this->type_ = rhs.type();
}

ValueIteratorBase::ValueTypeReference ValueIteratorBase::dereference() const
{
  return this->object_->second;
}

ValueIteratorBase::ValueTypePointer ValueIteratorBase::pointer() const
{
  if( this->type() == IteratorType::ObjectValues )
  {
    return this->object_->second.get();
  }
  else // IteratorType::NulLValue
  {
    return nullptr;
  }
}

bool ValueIteratorBase::operator ==(const SelfType& rhs) const
{
  if( this->type() == IteratorType::ObjectValues )
  {
    return this->object_ == rhs.object_;
  }
  else // IteratorType::NullValue
  {
    return ( rhs.type() == IteratorType::Null );
  }
}

bool ValueIteratorBase::operator !=(const SelfType& rhs) const
{
  if( this->type() == IteratorType::ObjectValues )
  {
    return ! ( this->object_ == rhs.object_ );
  }
  else // IteratorType::NullValue
  {
    return ! ( rhs.type() == IteratorType::Null );
  }
}

ValueIteratorBase::DifferenceType
ValueIteratorBase::operator -(const SelfType& rhs) const
{
  return distance(rhs);
}

void ValueIteratorBase::increment()
{
  if( this->type() == IteratorType::ObjectValues )
  {
    ++this->object_;
  }
  else // IteratorType::NullValue
  {
    return; // Do nothing -- we aren't initialized!
  }
}

void ValueIteratorBase::decrement()
{
  if( this->type() == IteratorType::ObjectValues )
  {
    --this->object_;
  }
  else // IteratorType::NullValue
  {
    return; // Do nothing -- we are not initialized!
  }
}

ValueIteratorBase::DifferenceType
ValueIteratorBase::distance(const SelfType& rhs) const
{
  if( this->type() == IteratorType::ObjectValues )
  {
    return std::distance( this->object_, rhs.object_ );
  }
  else // IteratorType::NullValue
  {
    return 0;
  }
}

enum ValueIteratorBase::IteratorType ValueIteratorBase::type() const
{
  return this->type_;
}

} // namespace nom
