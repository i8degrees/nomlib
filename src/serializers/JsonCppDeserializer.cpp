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
#include "nomlib/serializers/JsonCppDeserializer.hpp"

#include "nomlib/core/helpers.hpp"

// Private headers (third-party)
#include "jsoncpp/json.h"

namespace nom {

JsonCppDeserializer::JsonCppDeserializer( void ) :
  options_{ DeserializerOptions::StrictMode }
{
  // NOM_LOG_TRACE( NOM );
}

JsonCppDeserializer::~JsonCppDeserializer( void )
{
  // NOM_LOG_TRACE( NOM );
}

JsonCppDeserializer::JsonCppDeserializer( uint32 options ) :
  options_{ options }
{
  // NOM_LOG_TRACE( NOM );
}

Value JsonCppDeserializer::deserialize  ( const std::string& source )
{
  bool collect_comments = false;
  Json::Reader parser;
  Json::Value buffer;
  Value output;

  if( this->options_ & DeserializerOptions::StrictMode )
  {
    // Comments are forbidden and top-level node must be an array or object
    parser = Json::Reader( Json::Features::strictMode() );
  }

  else if( this->options_ & DeserializerOptions::ParseComments )
  {
    // Parsing of comments for the writer to serialize (broken / not implemented).
    collect_comments = true;
  }
  else // Non-strict JSON compliance
  {
    Json::Features f;
    f.strictRoot_ = false;
    parser = Json::Reader( f );
  }

  // Attempt to read input file into memory; string to Json::Value
  if ( parser.parse( source, buffer, collect_comments ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not parse the input source object as JSON." );
    return Value::null;
  }

  // Transform Json::Value to nom::Value
  if( this->read( buffer, output ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not deserialize the input source object." );
    return Value::null;
  }

  return output;
}

bool JsonCppDeserializer::load  ( const std::string& filename, Value& output )
{
  std::ifstream fp;
  std::stringstream buffer;

  fp.open( filename );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Could not access file: " + filename );
    return false;
  }

  buffer << fp.rdbuf();

  // File buffer to nom::Value
  output = this->deserialize( buffer.str() );

  fp.close();

  return true;
}

bool JsonCppDeserializer::read( const Json::Value& source, Value& dest ) const
{
  Json::Value::Members members; // Member keys of each object iteration

  // Temporary buffering objects we use to collect input in form of JSON objects;
  // we append individual objects to the overall container as we iterate through
  // the entire underlying container.
  Value object, objects;
  Value array;

  // Begin iterating through the container's objects to begin transferring to
  // our nom::Value object.
  for( auto idx = 0; idx != source.size(); ++idx )
  {
    if( source.isArray() )
    {
      object.clear();

      // [ { ... } ]
      if( source[idx].isObject() )
      {
        // NOM_DUMP( object_members.size() );

        std::vector<std::string> object_members = source[idx].getMemberNames();

        for( auto pos = 0; pos != object_members.size(); ++pos )
        {
          std::string key = object_members[pos];

          // NOM_DUMP( key );
          // NOM_DUMP( value[idx][key] );

          if( this->read_array( source[idx][key], objects[idx][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
            return false;
          }
          else if( this->read_object( source[idx][key], objects[idx][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
            return false;
          }
        } // end for object member pairs loop
      }
    }

    // { "key": { ... } }
    else if( source.isObject() )
    {
      object.clear();

      std::vector<std::string> root_members = source.getMemberNames();

      // Top-level array or object member key
      std::string root_key = root_members[idx];

      // { "key": [ ... ] }
      if( source[root_key].isArray() )
      {
        Value val;

        // NOM_DUMP(root_key);
        // NOM_DUMP(value[root_key]);
        // NOM_DUMP( value[root_key].size() );

        this->read_array( source[root_key], val );

        objects[root_key] = val;
        // objects[root_key].push_back( val );
      }

      // { "key": { ... } }
      if( source[root_key].isObject() )
      {
        // NOM_DUMP( root_key );
        // NOM_DUMP( object_members.size() );

        std::vector<std::string> object_members = source[root_key].getMemberNames();

        for( auto pos = 0; pos != object_members.size(); ++pos )
        {
          std::string key = object_members[pos];

          // NOM_DUMP( key );

          if( source[root_key][key].isArray() )
          {
            if( this->read_array( source[root_key][key], objects[root_key][key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
              return false;
            }
          }
          // else if( source[root_key][key].isObject() )
          // {
            if( this->read_object( source[root_key][key], objects[root_key][key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
              return false;
            }
          // }
        } // end for object member pairs loop

        // objects[root_key] = object;

      } // end if container node is an object
    }
  }

  // Copy our built nom::Value object tree to the specified destination object;
  // we must always derive our top-level (parent) nom::Value object from either
  // an array or object node type.
  //
  // Note that we use this top-level parent object to decide the type of hier-
  // archy we need to start out with, either: a) JSON array(s) containing
  // objects; b) JSON object(s) containing objects.
  if( array.size() > 0 )
  {
    dest = objects;
  }
  else
  {
    dest = objects;
  }

  return true;
}

bool JsonCppDeserializer::read_value( const Json::Value& object, Value& dest ) const
{
  if( object.isNull() )
  {
    // NOM_DUMP( "null" );
    dest = Value::null;
  }
  else if( object.isInt() )
  {
    // NOM_DUMP( object.asInt() );
    dest = object.asInt();
  }
  else if( object.isUInt() )
  {
    // NOM_DUMP( object.asUInt() );
    dest = object.asUInt();
  }
  else if( object.isDouble() )
  {
    // NOM_DUMP( object.asDouble() );
    dest = object.asDouble();
  }
  else if( object.isString() )
  {
    // NOM_DUMP( object.asString() );
    dest = object.asCString();
  }
  else if( object.isBool() )
  {
    // NOM_DUMP( object.asBool() );
    dest = object.asBool();
  }
  else if( object.isArray() )
  {
    // Special case -- must be handled by JsonCppDeserializer::read_array.
  }
  else if( object.isObject() )
  {
    // Special case -- must be handled by JsonCppDeserializer::read_object.
  }
  else // Unknown type -- not handled
  {
    #if defined( NOM_DEBUG_JSONCPP_DESERIALIZER_VALUES )
      NOM_DUMP( object.type() );
      // NOM_DUMP( member.key() );
      NOM_DUMP( object );
    #endif

    // TODO: Err handling
    NOM_STUBBED( NOM );
    return false;
  }

  return true;
}

bool JsonCppDeserializer::read_array( const Json::Value& object, Value& dest ) const
{
  // Array arr;
  Value arr;

  // NOM_DUMP( object.size() );
  for( auto i = 0; i != object.size(); ++i )
  {
    switch( object[i].type() )
    {
      default: // Unknown type???
      {
        #if defined( NOM_DEBUG_JSONCPP_DESERIALIZER_VALUES )
          NOM_DUMP( object[i].type() );
          // NOM_DUMP( member.key() );
          NOM_DUMP( object[i] );
        #endif

        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;

        break;
      }

      case Value::ValueType::Null:
      case Value::ValueType::SignedInteger:
      case Value::ValueType::UnsignedInteger:
      case Value::ValueType::RealNumber:
      case Value::ValueType::String:
      case Value::ValueType::Boolean:
      {
        Value val;

        if( this->read_value( object[i], val ) == false )
        {
          // TODO: Err handling
          NOM_STUBBED( NOM );
          return false;
        }

        arr.push_back( val );
        break;
      }

      // TODO: verify that this works; (Resources/examples/json/inventory.json)
      case Value::ValueType::ArrayValues:
      {
        // NOM_LOG_ERR( NOM, "Could not deserialize values; an array cannot exist within another array." );
        // return false;

        Value val;

        if( this->read_array( object[i], val ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not deserialize values; invalid array???" );
          return false;
        }

        arr.push_back( val );

        break;
      }

      case Value::ValueType::ObjectValues: // [ { "key": [ { ... } ] } ]
      {
        Value val;
        if( this->read_object( object[i], val ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not deserialize values; invalid object???" );
          return false;
        }

        arr.push_back( val );

        break;
      }
    }
  }

  dest = arr;
  // dest.push_back( arr );

  return true;
}

bool JsonCppDeserializer::read_object( const Json::Value& object, Value& dest ) const
{
  switch( object.type() )
  {
    default: // Unknown type???
    {
      #if defined( NOM_DEBUG_JSONCPP_DESERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        // NOM_DUMP( member.key() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    case Value::ValueType::Null:
    case Value::ValueType::SignedInteger:
    case Value::ValueType::UnsignedInteger:
    case Value::ValueType::RealNumber:
    case Value::ValueType::String:
    case Value::ValueType::Boolean:
    {
      if( this->read_value( object, dest ) == false )
      {
        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;
      }

      break;
    }

    case Value::ValueType::ArrayValues:
    {
      if( this->read_array( object, dest ) == false )
      {
        NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
        return false;
      }

      break;
    }

    case Value::ValueType::ObjectValues:
    {
      Value obj;

      for( auto itr = object.begin(); itr != object.end(); ++itr )
      {
        std::vector<std::string> members = object.getMemberNames();

        for( auto it = members.begin(); it != members.end(); ++it )
        {
          std::string key = *it;

          // NOM_DUMP( key );

          switch( object[key].type() )
          {
            default:
            {
              #if defined( NOM_DEBUG_JSONCPP_DESERIALIZER_VALUES )
                NOM_DUMP( object[key].type() );
                // NOM_DUMP( member.key() );
                NOM_DUMP( object );
              #endif

              break;
            }

            case Json::nullValue:
            case Json::intValue:
            case Json::uintValue:
            case Json::realValue:
            case Json::stringValue:
            case Json::booleanValue:
            {
              if( this->read_value( object[key], obj[key] ) == false )
              {
                // TODO: Err handling
                NOM_STUBBED( NOM );
                return false;
              }

              break;
            }

            case Json::arrayValue:
            {
              Value val;

              if( this->read_array( object[key], val ) == false )
              {
                // TODO: Error message
                NOM_STUBBED( NOM );
                return false;
              }

              obj[key] = val;
              break;
            }

            case Json::objectValue:
            {
              Value val;

              if( this->read_object( object[key], val ) == false )
              {
                // TODO: Error message
                NOM_STUBBED( NOM );
                return false;
              }

              obj[key] = val;
              break;
            }
          } // end switch object[key] type
        } // end for object members loop

        dest = obj;
      } // end for objects loop
    } // end Json::objectValue type
  } // end switch object type

  return true;
}

std::unique_ptr<IValueDeserializer> make_unique_json_deserializer()
{
  std::unique_ptr<IValueDeserializer> deserializer =
    nom::make_unique<JsonCppDeserializer>();

  return std::move(deserializer);
}

} // namespace nom
