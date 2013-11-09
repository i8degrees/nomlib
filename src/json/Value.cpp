/******************************************************************************

  nomlib - C++11 cross-platform game engine

Copyright (c) 2013, Jeffrey Carpenter <jeffrey.carp@gmail.com>
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
    object_ { Json::Value ( Json::nullValue ) }, key_ { "\0" }
{}

Value::~Value ( void ) {}

Value::Value ( const Value& copy )
{
  this->object_ = copy.object_;
  this->key_ = copy.key_;
}

Value& Value::operator = ( const ValueType& other )
{
  this->object_ = other;
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

JSONValueType Value::type ( const std::string& key, int index ) const
{
  return this->object_[index][key].type();
}

int Value::get_int ( int index )
{
  return this->object_[index].asInt();
}

std::vector<int> Value::get_ints ( const std::string& key, int index )
{
  std::vector<int> members;
  for ( uint i = 0; i < this->object_[index][key].size(); ++i )
  {
    members.push_back ( this->object_[index][key][i].asInt() );
  }
  return members;
}

int Value::get_int ( const std::string& key, int index )
{
  return this->object_[index][key].asInt();
}

uint Value::get_uint ( int index )
{
  return this->object_[index].asUInt();
}

std::vector<uint> Value::get_uints ( int index )
{
  std::vector<uint> members;
  for ( uint i = 0; i < this->object_[i].size(); ++i )
  {
    members.push_back ( this->object_[index][i].asUInt() );
  }
  return members;
}

uint Value::get_uint ( const std::string& key )
{
  return this->object_[key].asUInt();
}

const char* Value::c_str ( int index )
{
  return this->object_[index].asCString();
}

std::string Value::get_string ( int index )
{
  return this->object_[index].asString();
}

std::vector<std::string> Value::get_strings ( int index )
{
  std::vector<std::string> members;
  for ( uint i = 0; i < this->object_[i].size(); ++i )
  {
    members.push_back ( this->object_[index][i].asString() );
  }
  return members;
}

const char* Value::c_str ( const std::string& key )
{
  return this->object_[key].asCString();
}

std::string Value::get_string ( const std::string& key, int index )
{
  return this->object_[index][key].asString();
}

bool Value::get_bool ( int index )
{
  return this->object_[index].asBool();
}

std::vector<bool> Value::get_bools ( int index )
{
  std::vector<bool> members;
  for ( uint i = 0; i < this->object_[i].size(); ++i )
  {
    members.push_back ( this->object_[index][i].asBool() );
  }
  return members;
}

bool Value::get_bool ( const std::string& key )
{
  return this->object_[key].asBool();
}

ValueType Value::value ( int index )
{
  return this->object_[index];
}

std::vector<ValueType> Value::values ( int index )
{
  std::vector<ValueType> members;
  for ( uint i = 0; i < this->object_[i].size(); ++i )
  {
    members.push_back ( this->object_[index][i] );
  }
  return members;
}

ValueType Value::value ( const std::string& key )
{
  return this->object_[key];
}

void Value::insert ( const std::string& key, int index )
{
  this->object_[index][key] = Json::Value ( Json::nullValue );
}

void Value::insert ( const std::string& key, int value, int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, std::vector<int> values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[key].append ( * member );
  }
}

void Value::insert ( const std::string& key, uint value, int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, const std::vector<uint>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[key].append ( * member );
  }
}

void Value::insert ( const std::string& key, double value, int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, float value, int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, const char* value, int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key, const std::string& value,
                     int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key,
                     const std::vector<std::string>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[key].append ( * member );
  }
}

void Value::insert ( const std::string& key, bool value, int index )
{
  this->object_[index][key] = Json::Value ( value );
}

void Value::insert ( const std::string& key,
                     const std::vector<ValueType>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[key].append ( *member );
  }
}

} // namespace JSON
} // namespace nom
