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
#include "nomlib/json/Value.hpp"

namespace nom {
namespace JSON {

Value::Value ( void ) :
object_ { Json::Value ( Json::nullValue ) }, pos_ ( 0 ) {}

Value::~Value ( void ) {}

Value::Value ( const Value& copy )
{
  this->object_ = copy.object_;
  this->pos_ = copy.pos_;
}

Value::Value ( const ValueType& copy )
{
  this->object_ = copy;
  this->pos_ = 0;
}

Value& Value::operator = ( const Value& other )
{
  this->object_ = other.object_;
  this->pos_ = other.pos_;

  return *this;
}

Value& Value::operator = ( const ValueType& other )
{
  this->object_ = other;
  this->pos_ = 0;

  return *this;
}

ValueType& Value::get ( void ) const
{
  return this->object_;
}

uint32 Value::size ( void ) const
{
  return this->object_.size();
}

JSONValueType Value::type ( void ) const
{
  return this->object_.type();
}

JSONValueType Value::type ( int index ) const
{
  return this->object_[index].type();
}

JSONValueType Value::type ( const std::string& key ) const
{
  return this->object_[this->pos_][key].type();
}

JSONMemberType Value::members ( int index ) const
{
  return this->object_[index].getMemberNames();
}

std::vector<int> Value::get_ints ( const std::string& key )
{
  std::vector<int> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asInt() );
  }
  return members;
}

int Value::get_int ( const std::string& key )
{
  return this->object_[this->pos_][key].asInt();
}

std::vector<uint> Value::get_uints ( const std::string& key )
{
  std::vector<uint> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asUInt() );
  }
  return members;
}

uint Value::get_uint ( const std::string& key )
{
  return this->object_[this->pos_][key].asUInt();
}

std::vector<std::string> Value::get_strings ( const std::string& key )
{
  std::vector<std::string> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asString() );
  }
  return members;
}

std::string Value::get_string ( const std::string& key )
{
  return this->object_[this->pos_][key].asString();
}

std::vector<bool> Value::get_bools ( const std::string& key )
{
  std::vector<bool> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asBool() );
  }
  return members;
}

bool Value::get_bool ( const std::string& key )
{
  return this->object_[this->pos_][key].asBool();
}

void Value::insert ( const std::string& key )
{
  this->object_[this->pos_][key] = Json::Value ( Json::nullValue );
}

void Value::insert ( const std::string& key, int value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, std::vector<int> values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void Value::insert ( const std::string& key, uint value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, const std::vector<uint>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void Value::insert ( const std::string& key, double value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, float value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, const std::string& value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key,
                     const std::vector<std::string>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void Value::insert ( const std::string& key, bool value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void Value::insert ( const Value& values )
{
  this->object_.append ( values.object_[values.pos_] );
}

void Value::endl ( void )
{
  ++this->pos_;
}

} // namespace JSON
} // namespace nom
