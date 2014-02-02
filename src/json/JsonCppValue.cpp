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
#include "nomlib/json/JsonCppValue.hpp"

namespace nom {

JsonCppValue::JsonCppValue( void ) :
  object_ { Json::Value ( Json::nullValue ) },
  pos_ ( 0 )
{
  //NOM_LOG_TRACE(NOM);
}

JsonCppValue::~JsonCppValue( void )
{
  //NOM_LOG_TRACE(NOM);
}

JsonCppValue::JsonCppValue( const JsonCppValue::SelfType& copy )
{
  this->object_ = copy.object_;
  this->pos_ = copy.pos_;
}

JsonCppValue::JsonCppValue( const Json::Value& copy )
{
  this->object_ = copy;
  this->pos_ = 0;
}

JsonCppValue::SelfType& JsonCppValue::operator =( const JsonCppValue::SelfType& other )
{
  this->object_ = other.object_;
  this->pos_ = other.pos_;

  return *this;
}

JsonCppValue::SelfType& JsonCppValue::operator =( const Json::Value& other )
{
  this->object_ = other;
  this->pos_ = 0;

  return *this;
}

Json::Value& JsonCppValue::get( void ) const
{
  return this->object_;
}

uint32 JsonCppValue::size( void ) const
{
  return this->object_.size();
}

JsonCppValue::JsonValueType JsonCppValue::type( void ) const
{
  return this->object_.type();
}

JsonCppValue::JsonValueType JsonCppValue::type( int index ) const
{
  return this->object_[index].type();
}

JsonCppValue::JsonValueType JsonCppValue::type( const std::string& key ) const
{
  return this->object_[this->pos_][key].type();
}

JsonCppValue::JsonMemberType JsonCppValue::members( int index ) const
{
  return this->object_[index].getMemberNames();
}

const std::string JsonCppValue::member( void ) const
{
  JsonMemberType member = this->object_[this->pos_].getMemberNames();

  return member[0];
}

bool JsonCppValue::null_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::nullValue );
}

bool JsonCppValue::null_type( int index ) const
{
  return ( this->type( index ) == Json::nullValue );
}

bool JsonCppValue::int_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::intValue );
}

bool JsonCppValue::int_type( int index ) const
{
  return ( this->type( index ) == Json::intValue );
}

bool JsonCppValue::uint_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::uintValue );
}

bool JsonCppValue::uint_type( int index ) const
{
  return ( this->type( index ) == Json::uintValue );
}

bool JsonCppValue::double_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::realValue );
}

bool JsonCppValue::double_type( int index ) const
{
  return ( this->type( index ) == Json::realValue );
}

bool JsonCppValue::string_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::stringValue );
}

bool JsonCppValue::string_type( int index ) const
{
  return ( this->type( index ) == Json::stringValue );
}

bool JsonCppValue::bool_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::booleanValue );
}

bool JsonCppValue::bool_type( int index ) const
{
  return ( this->type( index ) == Json::booleanValue );
}

bool JsonCppValue::array_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::arrayValue );
}

bool JsonCppValue::array_type( int index ) const
{
  return ( this->type( index ) == Json::arrayValue );
}

bool JsonCppValue::object_type( const std::string& key ) const
{
  return ( this->type( key ) == Json::objectValue );
}

bool JsonCppValue::object_type( int index ) const
{
  return ( this->type( index ) == Json::objectValue );
}

int JsonCppValue::get_int( const std::string& key ) const
{
  return this->object_[this->pos_][key].asInt();
}

const std::vector<int> JsonCppValue::get_ints( const std::string& key ) const
{
  std::vector<int> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asInt() );
  }
  return members;
}

uint JsonCppValue::get_uint( const std::string& key ) const
{
  return this->object_[this->pos_][key].asUInt();
}

const std::vector<uint> JsonCppValue::get_uints( const std::string& key ) const
{
  std::vector<uint> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asUInt() );
  }
  return members;
}

double JsonCppValue::get_double( const std::string& key ) const
{
  return this->object_[this->pos_][key].asDouble();
}

const std::vector<double> JsonCppValue::get_doubles( const std::string& key ) const
{
  std::vector<double> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asDouble() );
  }

  return members;
}

float JsonCppValue::get_float( const std::string& key ) const
{
  return this->object_[this->pos_][key].asFloat();
}

const std::vector<float> JsonCppValue::get_floats( const std::string& key ) const
{
  std::vector<float> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asFloat() );
  }

  return members;
}

const std::string JsonCppValue::get_string( const std::string& key ) const
{
  return this->object_[this->pos_][key].asString();
}

const std::vector<std::string> JsonCppValue::get_strings( const std::string& key ) const
{
  std::vector<std::string> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asString() );
  }
  return members;
}

bool JsonCppValue::get_bool( const std::string& key )
{
  return this->object_[this->pos_][key].asBool();
}

const std::vector<bool> JsonCppValue::get_bools( const std::string& key ) const
{
  std::vector<bool> members;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    members.push_back ( this->object_[this->pos_][key][i].asBool() );
  }
  return members;
}

const Array JsonCppValue::get_values( const std::string& key ) const
{
  Array values;
  for ( uint i = 0; i < this->object_[this->pos_][key].size(); ++i )
  {
    Json::Value val = this->object_[this->pos_][key][i];

    if( val.isNull() ) // Type 0
    {
      values.push_back( Value() );
    }
    else if( val.isInt()  ) // ValueType::SignedInteger (type 1)
    {
      values.push_back( val.asInt() );
    }
    else if( val.isUInt() ) // ValueType::UnsignedInteger (type 2)
    {
      values.push_back( val.asUInt() );
    }
    else if( val.isDouble() ) // ValueType::RealValue (type 3)
    {
      values.push_back( val.asDouble() );
    }
    else if( val.isString() ) // ValueType::StringValue (type 4)
    {
      values.push_back( val.asCString() );
    }
    else if( val.isBool() ) // ValueType::BooleanValue (type 5)
    {
      values.push_back( val.asBool() );
    }
    else if( val.isArray() )
    {
      // TODO
    }
    else if( val.isObject() )
    {
      // TODO
    }
    else
    {
      values.push_back( Value() ); // Null
      return values;
    }
  }
  return values;
}

void JsonCppValue::insert( const std::string& key )
{
  this->object_[this->pos_][key] = Json::Value( Json::nullValue );
}

void JsonCppValue::insert( const std::string& key, int value )
{
  this->object_[this->pos_][key] = Json::Value( value );
}

void JsonCppValue::insert( const std::string& key, const std::vector<int> values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void JsonCppValue::insert( const std::string& key, uint value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void JsonCppValue::insert( const std::string& key, const std::vector<uint>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void JsonCppValue::insert( const std::string& key, double value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void JsonCppValue::insert( const std::string& key, float value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void JsonCppValue::insert( int index, const char* val )
{
  this->object_[index] = Json::Value ( val );

  // this->endl();
}

void JsonCppValue::insert( const std::string& key, const std::string& value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void JsonCppValue::insert(  const std::string& key,
                            const std::vector<std::string>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void JsonCppValue::insert( const std::string& key, const char* val )
{
  this->object_[this->pos_][key] = Json::Value ( val );
}

void JsonCppValue::insert(  const std::string& key,
                            const std::vector<char*>& values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    this->object_[this->pos_][key].append ( *member );
  }
}

void JsonCppValue::insert( const std::string& key, bool value )
{
  this->object_[this->pos_][key] = Json::Value ( value );
}

void JsonCppValue::insert( const std::string& key, const SelfType& values )
{
  NOM_STUBBED(NOM);
}

void JsonCppValue::insert( const SelfType& values )
{
  this->object_.append( values.object_[values.pos_] );
  // this->endl();
}

void JsonCppValue::insert( const std::string& key, const Array values )
{
  for ( auto member = values.begin(); member != values.end(); ++member )
  {
    if( member->null_type() ) // ValueType::NullValue (type 0)
    {
      this->object_[this->pos_][key].append ( Json::Value() );
    }
    if( member->int_type() ) // ValueType::SignedInteger (type 1)
    {
      this->object_[this->pos_][key].append ( member->get_int() );
    }
    else if( member->uint_type() ) // ValueType::UnsignedInteger (type 2)
    {
      this->object_[this->pos_][key].append ( member->get_uint() );
    }
    else if( member->double_type() ) // ValueType::RealValue (type 3)
    {
      this->object_[this->pos_][key].append ( member->get_double() );
    }
    else if( member->string_type() ) // ValueType::StringValue (type 4)
    {
      this->object_[this->pos_][key].append ( member->get_cstring() );
    }
    else if( member->bool_type() ) // ValueType::BooleanValue (type 5)
    {
      this->object_[this->pos_][key].append ( member->get_bool() );
    }
    else
    {
      // TODO:
      // this->object_[this->pos_][key].append ( Json::Value() );
    }
  }
}

void JsonCppValue::endl( void )
{
  ++this->pos_;
}

void JsonCppValue::dump_key( const JsonCppValue& val ) const
{
  Json::Value value = val.get();

  switch ( value.type() )
  {
    // Unkonwn type; let jsoncpp try figuring it out
    default: printf ( " [type=%d, size=%d]", value.type(), value.size() ); break;

    case 0: // NULL
    {
      printf ( " [type=nullValue, size=%d]", value.size() );
      break;
    }

    case 1: // JSON signed integer
    {
      printf ( " [type=intValue, size=%lu]", sizeof ( value.asInt() ) );
      break;
    }

    case 2: // JSON unsigned integer
    {
      printf ( " [type=uintValue, size=%lu]", sizeof ( value.asUInt() ) );
      break;
    }

    case 3: // JSON double
    {
      printf ( " [type=realValue, size=%lu]", sizeof ( value.asDouble() ) );
      break;
    }

    case 4: // JSON string
    {
      printf ( " [type=stringValue, size=%lu]", value.asString().length() );
      break;
    }

    case 5: // JSON boolean
    {
      printf ( " [type=booleanValue, size=%lu]", sizeof ( value.asBool() ) );
      break;
    }

    case 6: // JSON Array
    {
      printf ( " [type=arrayValue, size=%d]", value.size() );
      break;
    }

    case 7: // JSON object
    {
      printf ( " [type=objectValue, size=%d]", value.size() );
      break;
    }
  } // switch key.type()
}

void JsonCppValue::dump_value( const JsonCppValue& val ) const
{
  Json::Value value = val.get();

  if ( value.isString() )
  {
    printf ( "string (%s)", value.asCString() );
  }
  else if ( value.isBool() )
  {
    printf ( "bool (%d)", value.asBool() );
  }
  else if ( value.isInt() )
  {
    printf ( "int (%d)", value.asInt() );
  }
  else if ( value.isUInt() )
  {
    printf ( "uint (%u)", value.asUInt() );
  }
  else if ( value.isDouble() )
  {
    printf ( "double (%f)", value.asDouble() );
  }
  else // Unkonwn type; let jsoncpp try figuring it out
  {
    printf ( "unknown type=%d", value.type() );
  }
}

bool JsonCppValue::dump( const JsonCppValue& obj, int depth ) const
{
  Json::Value object = obj.get();

  depth += 1;

  this->dump_key ( object );

  if ( object.size() > 0 )
  {
    std::cout << std::endl;
    for ( auto itr = object.begin() ; itr != object.end() ; ++itr )
    {
      // Pretty print depth.
      for ( auto tab = 0 ; tab < depth; tab++ )
      {
        std::cout << "-";
      }

      std::cout << " subvalue (";
      this->dump_value ( itr.key() );
      std::cout << ") -";
      this->dump ( *itr, depth );
    }

    return true;
  }
  else
  {
    std::cout << " ";
    this->dump_value ( object );
    std::cout << std::endl;
  }

  return true;
}

const std::string JsonCppValue::stringify( void ) const
{
  std::stringstream os;

  os << object_;

  return os.str();
}

std::ostream& operator <<( std::ostream& os, const JsonCppValue& val )
{
  os << val.get();

  return os;
}

} // namespace nom
