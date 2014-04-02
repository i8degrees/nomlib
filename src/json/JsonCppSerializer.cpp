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
#include "nomlib/json/JsonCppSerializer.hpp"

namespace nom {

JsonCppSerializer::JsonCppSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

JsonCppSerializer::~JsonCppSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

bool JsonCppSerializer::serialize( const Value& source, const std::string& output ) const
{
  std::ofstream fp;
  Json::StyledStreamWriter writer( JSONCPP_INDENTION_LEVEL );
  Json::Value value;  // JsonCPP library container

  if( this->write( source, value ) == false )
  {
    NOM_LOG_ERR ( NOM, "Failed to serialize data." );
    return false;
  }

  // Finally, output our serialized nom::Value object!
  fp.open( output );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Unable to write JSON output in file: " + output );
    return false;
  }

  writer.write( fp, value );

  fp.close();

  return true;
}

bool JsonCppSerializer::unserialize( const std::string& input, Value& dest ) const
{
  std::ifstream fp;
  Json::Reader parser;
  Json::Value value;            // JsonCpp interface container for our JSON
                                // parsing.

  fp.open( input );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "File access failure on file: " + input );
    return false;
  }

  // Attempt to read input file into memory.
  if ( parser.parse( fp, value ) == false )
  {
    NOM_LOG_ERR ( NOM, "Failure to parse JSON input file: " + input );
    return false;
  }

  fp.close();

  if( this->read( value, dest ) == false )
  {
    // TODO: Err handling
    NOM_STUBBED( NOM );
    return false;
  }

  return true;
}

const std::string JsonCppSerializer::stringify( const Value& input ) const
{
  std::stringstream os;
  Json::Value output;
  Value buffer( input );

  if( this->read( output, buffer ) == false )
  {
    NOM_LOG_ERR ( NOM, "Failed to read data stream in for stringifying." );
    return "\0";
  }

  if( this->write( input, output ) == false )
  {
    NOM_LOG_ERR ( NOM, "Failed to stringify data stream." );
    return "\0";
  }

  os << output;

  return os.str();
}

bool JsonCppSerializer::write( const Value& source, Json::Value& dest ) const
{
  uint idx = 0;
  std::string root_key, key;

  // Begin iterating through the entire container; values, arrays, objects and
  // even objects within objects.
  for( Value::ConstIterator itr = source.begin(); itr != source.end(); ++itr )
  {
    ValueConstIterator member( itr );

    // Top-level array or object member key.
    root_key = member.key();

    // FIXME: (This handles array JSON serialization)
    // [ { ... } ]
    if( root_key == "\0" )
    {
      if( itr->object_type() )
      {
        #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
          NOM_DUMP( itr->type_name() );
          NOM_DUMP( member.key() );
        #endif

        nom::Object objects = itr->object();

        // Iterate through each object and store every member's key / value pair
        // we find
        for( Value::ConstIterator itr = objects.begin(); itr != objects.end(); ++itr )
        {
          nom::ValueConstIterator member( itr );

          key = member.key();

          #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
            NOM_DUMP( itr->type_name() );
            NOM_DUMP( member.key() );
            NOM_DUMP( itr->stringify() );
          #endif

          if( dest[idx][key].isArray() )
          {
            if( this->serialize_array( itr->ref(), dest[idx][key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
              return false;
            }
          }

          if( dest[idx][key].isObject() )
          {
            if( this->serialize_object( itr->ref(), dest[idx][key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not serialize values; invalid object?" );
              return false;
            }
          }
        } // end for objects loop
      } // end if object type

      // EOF JSON Object
      ++idx;

    } // end if root_key == "\0" (JSON array top-level)

    // FIXME: (this now handles JSON serialization as mapped objects)
    // { "root": { ... } }
    else if( itr->object_type() )
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( itr->type_name() );
        NOM_DUMP( member.key() );
      #endif

      nom::Object objects = itr->object();

      // Iterate through each object and store every member's key / value pair
      // we find
      for( Value::ConstIterator itr = objects.begin(); itr != objects.end(); ++itr )
      {
        nom::ValueConstIterator member( itr );

        key = member.key();

        #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
          NOM_DUMP( itr->type_name() );
          NOM_DUMP( member.key() );
          NOM_DUMP( itr->stringify() );
        #endif

        // Array within an object.
        if( dest[root_key][key].isArray() )
        {
          if( this->serialize_array( itr->ref(), dest[root_key][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
            return false;
          }
        }

        // Object within an object.
        if( dest[root_key][key].isObject() )
        {
          if( this->serialize_object( itr->ref(), dest[root_key][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not serialize values; invalid object?" );
            return false;
          }
        }
      } // end for objects loop

      // EOF JSON Object
      ++idx;

    } // end if JSON object

    // { "key": [ ... ] }
    else if( itr->array_type() )
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( itr->type_name() );
        NOM_DUMP( member.key() );
      #endif

      if( this->serialize_array( itr->ref(), dest[root_key] ) == false )
      {
        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;
      }

      // EOF JSON Array
      ++idx;
    }
    else // Non-array, non-object nom::Value type
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( itr->type_name() );
        NOM_DUMP( member.key() );
        NOM_DUMP( itr->stringify() );
      #endif

      NOM_LOG_ERR( NOM, "Could not serialize values; not an array or an object?" );
      return false;
    }
  }

  return true;
}

bool JsonCppSerializer::read( const Json::Value& source, Value& dest ) const
{
  Json::Value::Members members; // Member keys of each object iteration

  // Temporary buffering objects we use to collect input in form of JSON objects;
  // we append individual objects to the overall container as we iterate through
  // the entire underlying container.
  Object object, objects;
  Array array;

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

          if( this->unserialize_array( source[idx][key], object[key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
            return false;
          }
          else if( this->unserialize_object( source[idx][key], object[key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
            return false;
          }

        } // end for object member pairs loop

        array.push_back( object );
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

        this->unserialize_array( source[root_key], val );

        objects[root_key] = val;
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
            if( this->unserialize_array( source[root_key][key], object[key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
              return false;
            }
          }
          // else if( source[root_key][key].isObject() )
          // {
            if( this->unserialize_object( source[root_key][key], object[key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
              return false;
            }
          // }
        } // end for object member pairs loop

        objects[root_key] = object;

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
    dest = array;
  }
  else
  {
    dest = objects;
  }

  return true;
}

const std::string JsonCppSerializer::dump( const Json::Value& object, int depth ) const
{
  uint index = 0;
  std::string key;
  std::stringstream os; // Output buffer

  depth += 1; // Recursive method.

  os << this->dump_key( object );

  // nom::Value object is a tree of key & value pairs.
  if( object.size() > 0 )
  {
    os << std::endl;

    for( auto itr = object.begin(); itr != object.end(); ++itr )
    {
      // Array index & unmapped objects
      index = itr.index();

      // The key portion of the pair.
      key = itr.memberName();

      // Show the current depth.
      for ( uint tab = 0 ; tab < depth; ++tab )
      {
        os << "-";
      }

      os << " subvalue (";

      // Dump the member key portion of the data object; when we have a non-null
      // member key name, we need to format it a tad bit differently in order
      // to not have redundant information output.
      if( key != "" )
      {
        os << key;

        if( ! object[key].isArray() && ! object[key].isObject() )
        {
          os << ": ";
        }
        os << this->dump_value( object[key] );
        os << ")";
      }
      else
      {
        os << this->dump_value( object[index] );
        os << ")";
      }

      // Iterate onwards to the next level of the tree via recursion.
      os << this->dump( *itr, depth );
    }

    return os.str();
  }
  else
  {
    // Move onto the next element of the tree!
    os << std::endl;
  }

  return os.str();
}

const std::string JsonCppSerializer::dump_key( const Json::Value& key ) const
{
  std::stringstream os; // Output buffer

  if( key.isNull() )
  {
    os << this->print_key( "null", 0 );
  }
  else if( key.isInt() )
  {
    os << this->print_key( "SignedInteger", sizeof( key.asInt() ) );
  }
  else if( key.isUInt() )
  {
    os << this->print_key( "UnsignedInteger", sizeof( key.asUInt() ) );
  }
  else if( key.isDouble() )
  {
    os << this->print_key( "RealNumber", sizeof( key.asDouble() ) );
  }
  else if( key.isString() )
  {
    os << this->print_key( "String", sizeof( key.asString() ) );
  }
  else if( key.isBool() )
  {
    os << this->print_key( "Boolean", sizeof( key.asBool() ) );
  }
  else if( key.isArray() )
  {
    os << this->print_key( "ArrayValues", key.size() );
  }
  else if( key.isObject() )
  {
    os << this->print_key( "ObjectValues", key.size() );
  }
  else // Unknown type
  {
    os << this->print_key( "Unknown", 0 );
  }

  return os.str();
}

const std::string JsonCppSerializer::dump_value( const Json::Value& val ) const
{
  std::stringstream os; // Output buffer

  if( val.isNull() ) // Type 0
  {
    os << this->print_value( "null" );
  }
  else if( val.isInt() ) // Type 1
  {
    os << this->print_value( std::to_string( val.asInt() ) );
  }
  else if( val.isUInt() ) // Type 2
  {
    os << this->print_value( std::to_string( val.asUInt() ) );
  }
  else if( val.isDouble() ) // Type 3
  {
    os << this->print_value( std::to_string( val.asDouble() ) );
  }
  else if( val.isString() ) // Type 4
  {
    os << this->print_value( val.asString() );
  }
  else if( val.isBool() ) // Type 5
  {
    // Represent boolean values as a string.
    if( val.asBool() )
    {
      os << this->print_value( "true" );
    }
    else
    {
      os << this->print_value( "false" );
    }
  }
  else if( val.isArray() ) // Type 6
  {
    // Special case -- must be handled by JsonCppSerializer::dump.
  }
  else if( val.isObject() ) // Type 7
  {
    // Special case -- must be handled by JsonCppSerializer::dump.
  }
  else // Unknown type
  {
    os << "Unknown (" << val.type() << ")";
  }

  return os.str();
}

const std::string JsonCppSerializer::print_key( const std::string& type, uint size ) const
{
  std::stringstream os;

  os << " [type=" << type << ", " << "size=" << size << "]";

  return os.str();
}

const std::string JsonCppSerializer::print_value( const std::string& value ) const
{
  std::stringstream os;

  os << value;

  return os.str();
}

bool JsonCppSerializer::write_value( const Value& object, Json::Value& dest ) const
{
  switch( object.type() )
  {
    // Unknown type
    default:
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
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
    {
      dest = Json::Value();
      break;
    }

    case Value::ValueType::SignedInteger:
    {
      dest = object.get_int();
      break;
    }

    case Value::ValueType::UnsignedInteger:
    {
      dest = object.get_uint();
      break;
    }

    case Value::ValueType::RealNumber:
    {
      dest = object.get_double();
      break;
    }

    case Value::ValueType::String:
    {
      dest = object.get_cstring();
      break;
    }

    case Value::ValueType::Boolean:
    {
      dest = object.get_bool();
      break;
    }

    case Value::ValueType::ArrayValues:
    {
      // Special case -- must be handled by JsonCppSerializer::serialize_array.
      break;
    }

    case Value::ValueType::ObjectValues:
    {
      // Special case -- must be handled by JsonCppSerializer::serialize_object.
      break;
    }
  }

  return true;
}

bool JsonCppSerializer::write_array_value( const Value& object, Json::Value& dest ) const
{
  switch( object.type() )
  {
    // Unknown type
    default:
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        // NOM_DUMP( member.key() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;
    }

    case Value::ValueType::Null:
    {
      dest.append( Json::Value() );
      break;
    }

    case Value::ValueType::SignedInteger:
    {
      dest.append( object.get_int() );
      break;
    }
    case Value::ValueType::UnsignedInteger:
    {
      dest.append( object.get_uint() );
      break;
    }

    case Value::ValueType::RealNumber:
    {
      dest.append ( object.get_double() );
      break;
    }

    case Value::ValueType::String:
    {
      dest.append( object.get_cstring() );
      break;
    }

    case Value::ValueType::Boolean:
    {
      dest.append( object.get_bool() );
      break;
    }
  }

  return true;
}

bool JsonCppSerializer::serialize_array( const Value& object, Json::Value& dest ) const
{
  uint index = 0;
  Json::Value arr;

  Array array = object.array();

  for( auto itr = array.begin(); itr != array.end(); ++itr )
  {
    switch( itr->type() )
    {
      default: // Unknown type
      {
        #if defined( NOM_DEBUG_JSONCPP_UNSERIALIZER_VALUES )
          NOM_DUMP( itr->type() );
          NOM_DUMP( itr->ref() );
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
        if( write_array_value( itr->ref(), dest ) == false )
        {
          // TODO: Err handling
          NOM_STUBBED( NOM );
          return false;
        }

        break;
      }

      // Invalid JSON
      case Value::ValueType::ArrayValues:
      {
        NOM_LOG_ERR( NOM, "Could not serialize values; an array cannot exist within another array." );
        return false;

        break;
      }

      // [ { "key": [ { ... } ] } ]
      case Value::ValueType::ObjectValues:
      {
        if( this->serialize_object( itr->ref(), dest[index] ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
          return false;
        }

        break;
      }
    }

    // EOF array index
    ++index;
  }

  return true;
}

bool JsonCppSerializer::serialize_object( const Value& object, Json::Value& dest ) const
{
  switch( object.type() )
  {
    default: // Unknown type
    {
      #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
        NOM_DUMP( object.type() );
        NOM_DUMP( object );
      #endif

      // TODO: Err handling
      NOM_STUBBED( NOM );
      return false;

      break;
    }

    // Object within object containing values (top-level).
    case Value::ValueType::Null:
    case Value::ValueType::SignedInteger:
    case Value::ValueType::UnsignedInteger:
    case Value::ValueType::RealNumber:
    case Value::ValueType::String:
    case Value::ValueType::Boolean:
    {
      if( this->write_value( object, dest ) == false )
      {
        // TODO: Err handling
        NOM_STUBBED( NOM );
        return false;
      }

      break;
    }

    // Handle arrays within an object (top-level).
    case Value::ValueType::ArrayValues:
    {
      if( this->serialize_array( object, dest ) == false )
      {
        NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
        return false;
      }

      break;
    }

    case Value::ValueType::ObjectValues:
    {
      Json::Value obj;

      for( auto itr = object.begin(); itr != object.end(); ++itr )
      {
        ValueConstIterator member( itr );
        std::string key = member.key();

        Value value = itr->ref();

        switch( value.type() )
        {
          default: // Unknown type
          {
            #if defined( NOM_DEBUG_JSONCPP_SERIALIZER_VALUES )
              NOM_DUMP( value.type() );
              // NOM_DUMP( key() );
              NOM_DUMP( value );
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
            if( write_value( value, obj[key] ) == false )
            {
              // TODO: Err handling
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

          // Handle arrays within an object within an object.
          case Value::ValueType::ArrayValues:
          {
            if( this->serialize_array( value, obj[key] ) == false )
            {
              // TODO: Error message
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }

          // Handle objects within an object.
          case Value::ValueType::ObjectValues:
          {
            if( this->serialize_object( value, obj[key] ) == false )
            {
              // TODO: Error message
              NOM_STUBBED( NOM );
              return false;
            }

            break;
          }
        } // end switch objects[key] type

        dest = obj;

      } // end for object loop
    } // end Value::ValueType::ObjectValues type

  } // end switch object type

  return true;
}

bool JsonCppSerializer::read_value( const Json::Value& object, Value& dest ) const
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
    // Special case -- must be handled by JsonCppSerializer::unserialize_array.
  }
  else if( object.isObject() )
  {
    // Special case -- must be handled by JsonCppSerializer::unserialize_object.
  }
  else // Unknown type -- not handled
  {
    #if defined( NOM_DEBUG_JSONCPP_UNSERIALIZER_VALUES )
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

bool JsonCppSerializer::unserialize_array( const Json::Value& object, Value& dest ) const
{
  Array arr;

  // NOM_DUMP( object.size() );
  for( auto i = 0; i != object.size(); ++i )
  {
    switch( object[i].type() )
    {
      default: // Unknown type???
      {
        #if defined( NOM_DEBUG_JSONCPP_UNSERIALIZER_VALUES )
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

      case Value::ValueType::ArrayValues: // Invalid JSON
      {
        NOM_LOG_ERR( NOM, "Could not unserialize values; an array cannot exist within another array." );
        return false;
      }

      case Value::ValueType::ObjectValues: // [ { "key": [ { ... } ] } ]
      {
        Value val;
        if( this->unserialize_object( object[i], val ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not unserialize values; invalid object???" );
          return false;
        }

        arr.push_back( val );

        break;
      }
    }
  }

  dest = arr;

  return true;
}

bool JsonCppSerializer::unserialize_object( const Json::Value& object, Value& dest ) const
{
  switch( object.type() )
  {
    default: // Unknown type???
    {
      #if defined( NOM_DEBUG_JSONCPP_UNSERIALIZER_VALUES )
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
      if( this->unserialize_array( object, dest ) == false )
      {
        NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
        return false;
      }

      break;
    }

    case Value::ValueType::ObjectValues:
    {
      Object obj;

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
              #if defined( NOM_DEBUG_JSONCPP_UNSERIALIZER_VALUES )
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

              if( this->unserialize_array( object[key], val ) == false )
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

              if( this->unserialize_object( object[key], val ) == false )
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

} // namespace nom
