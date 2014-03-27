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
#include "nomlib/json/JsonSerializer.hpp"

namespace nom {

JsonSerializer::JsonSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

JsonSerializer::~JsonSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

bool JsonSerializer::serialize( const Value& source, const std::string& output ) const
{
  std::ofstream fp;
  Json::StyledStreamWriter writer( JSONCPP_INDENTION_LEVEL );
  Json::Value value;  // JsonCPP library container

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
    if( root_key == "\0" )
    {
      if( itr->object_type() )
      {
        #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
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

          #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
            NOM_DUMP( itr->type_name() );
            NOM_DUMP( member.key() );
            NOM_DUMP( itr->stringify() );
          #endif

          // if( this->serialize_array( itr->ref(), value[idx][key] ) == false )
          // {
          //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
          //   return false;
          // }
          // else if
          if( this->serialize_object( itr->ref(), value[idx][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not serialize values; invalid object?" );
            return false;
          }

          // nom::Array within an nom::Object:
          // { "root": [ 8, 2, 4, 10 ] }
          if( itr->array_type() )
          {
            #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
              NOM_DUMP( itr->type_name() );
              NOM_DUMP( member.key() );
            #endif

            nom::Array elements = itr->array();
            for( Value::ConstIterator itr = elements.begin(); itr != elements.end(); ++itr )
            {
              #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
                NOM_DUMP( itr->type_name() );
                NOM_DUMP( member.key() );
                NOM_DUMP( itr->stringify() );
              #endif

              if( this->serialize_array( itr->ref(), value[idx][key] ) == false )
              {
                NOM_LOG_ERR( NOM, "Could not serialize values; invalid array?" );
                return false;
              }
              // else if( this->serialize_object( itr->ref(), value[idx][key] ) == false )
              // {
              //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
              //   return false;
              // }
            } // end for array elements loop
          } // end if JSON array
        }
      }

      // EOF JSON Object
      ++idx;
    }

    // FIXME: (this now handles JSON serialization as mapped objects)
    // { "root": null }
    else if( itr->object_type() )
    {
      #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
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

        #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
          NOM_DUMP( itr->type_name() );
          NOM_DUMP( member.key() );
          NOM_DUMP( itr->stringify() );
        #endif

        if( this->serialize_object( itr->ref(), value[root_key][key] ) == false )
        {
          NOM_LOG_ERR( NOM, "Could not serialize values; invalid object?" );
          return false;
        }

        // nom::Array within an nom::Object:
        // { "root": [ 8, 2, 4, 10 ] }
        if( itr->array_type() )
        {
          #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
            NOM_DUMP( itr->type_name() );
            NOM_DUMP( member.key() );
          #endif

          nom::Array elements = itr->array();
          for( Value::ConstIterator itr = elements.begin(); itr != elements.end(); ++itr )
          {
            #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
              NOM_DUMP( itr->type_name() );
              NOM_DUMP( member.key() );
              NOM_DUMP( itr->stringify() );
            #endif

            if( this->serialize_array( itr->ref(), value[root_key][key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
              return false;
            }
            // else if( this->serialize_object( itr->ref(), value[root_key][key] ) == false )
            // {
            //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
            //   return false;
            // }
          } // end for array elements loop
        } // end if JSON array
      } // end for objects loop

      // EOF JSON Object
      ++idx;

    } // end if JSON object
    else // Non-array, non-object nom::Value type
    {
      #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
        NOM_DUMP( itr->type_name() );
        NOM_DUMP( member.key() );
        NOM_DUMP( itr->stringify() );
      #endif

      NOM_LOG_ERR( NOM, "Could not serialize values; not an array or an object?" );
      return false;
    }
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

bool JsonSerializer::unserialize( const std::string& input, Value& dest ) const
{
  std::ifstream fp;
  Json::Reader parser;
  Json::Value value;            // JsonCpp interface container for our JSON
                                // parsing.
  Json::Value::Members members; // Member keys of each object iteration


  // Temporary buffering objects we use to collect input in form of JSON objects;
  // we append individual objects to the overall container as we iterate through
  // the entire underlying container.
  Object object, objects;
  Array array;

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

  // Begin iterating through the container's objects to begin transferring to
  // our nom::Value object.
  for( auto idx = 0; idx != value.size(); ++idx )
  {
    if( value.isArray() )
    {
      object.clear();

      if( value[idx].isObject() )
      {
        // NOM_DUMP( object_members.size() );

        std::vector<std::string> object_members = value[idx].getMemberNames();

        for( auto pos = 0; pos != object_members.size(); ++pos )
        {
          std::string key = object_members[pos];

          // { "map": [ { ... } ] }
          // NOM_DUMP( key );
          // NOM_DUMP( value[idx][key] );

          if( this->unserialize_array( value[idx][key], object[key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
            return false;
          }
          else if( this->unserialize_object( value[idx][key], object[key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
            return false;
          }

        } // end for object member pairs loop

        array.push_back( object );
      }
    }
    else if( value.isObject() )
    {
      object.clear();

      std::vector<std::string> root_members = value.getMemberNames();

      // Top-level array or object member key
      std::string root_key = root_members[idx];

      if( value[root_key].isObject() )
      {
        // NOM_DUMP( root_key );
        // NOM_DUMP( object_members.size() );

        std::vector<std::string> object_members = value[root_key].getMemberNames();

        for( auto pos = 0; pos != object_members.size(); ++pos )
        {
          std::string key = object_members[pos];

          // NOM_DUMP( key );

          if( this->unserialize_array( value[root_key][key], object[key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
            return false;
          }
          else if( this->unserialize_object( value[root_key][key], object[key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
            return false;
          }
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

const std::string JsonSerializer::stringify( const Value& obj ) const
{
  std::stringstream os;

  os << obj;

  return os.str();
}

const std::string JsonSerializer::dump( const Json::Value& object, int depth ) const
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

const std::string JsonSerializer::dump_key( const Json::Value& key ) const
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

const std::string JsonSerializer::dump_value( const Json::Value& val ) const
{
  std::stringstream os; // Output buffer

  if( val.isNull() ) // Type 0
  {
    // Type is void of representation.
  }
  else if( val.isInt() ) // Type 1
  {
    os << this->print_value( val.asInt() );
  }
  else if( val.isUInt() ) // Type 2
  {
    os << this->print_value( val.asUInt() );
  }
  else if( val.isDouble() ) // Type 3
  {
    os << this->print_value( val.asDouble() );
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
    // Type is handled in nom::JsonSerializer::dump via recursion.
  }
  else if( val.isObject() ) // Type 7
  {
    // Type is handled in nom::JsonSerializer::dump via recursion.
  }
  else // Unknown type
  {
    os << "Unknown (" << val.type() << ")";
  }

  return os.str();
}

const std::string JsonSerializer::print_key( const std::string& type, uint size ) const
{
  std::stringstream os;

  os << " [type=" << type << ", " << "size=" << size << "]";

  return os.str();
}

/*
const std::string JsonSerializer::print_value( const std::string& val ) const
{
  std::stringstream os;

  os << val;

  return os.str();
}
*/

bool JsonSerializer::serialize_array( const Value& object, Json::Value& dest ) const
{
  if( object.null_type() )
  {
    // [ { "array": [ null ] } ]
    dest.append (Json::Value() );
  }
  else if( object.int_type() )
  {
    // [ { "array": [ -1 ] } ]
    dest.append( object.get_int() );
  }
  else if( object.uint_type() )
  {
    // [ { "array": [ 1 ] } ]
    dest.append( object.get_uint() );
  }
  else if( object.double_type() )
  {
    // [ { "array": [ 1.2 ] } ]
    dest.append( object.get_double() );
  }
  else if( object.string_type() )
  {
    // [ { "array": [ "string" ] } ]
    dest.append( object.get_cstring() );
  }
  else if( object.bool_type() )
  {
    // [ { "array": [ true ] } ]
    dest.append( object.get_bool() );
  }
  // else if( object.array_type() )
  // {
    // if( this->serialize_array( object, dest ) == false )
    // {
    //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
    //   return false;
    // }
  // }
  // else if( object.object_type() )
  // {
  //   if( this->serialize_object( object, dest ) == false )
  //   {
  //     NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
  //     return false;
  //   }
  // }
  else
  {
    #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
      NOM_DUMP( object.type_name() );
      // NOM_DUMP( member.key() );
      NOM_DUMP( object.stringify() );
    #endif

    // This non-fatal info err is not necessarily anything of concern; we do
    // yet handle recursing deeper into the node hierarchy -- note that we do
    // not even bother checking for array or object node types here.
    NOM_LOG_ERR( NOM, "Potential non-handled case in serialization of array node." );
  }

  return true;
}

bool JsonSerializer::serialize_object( const Value& object, Json::Value& dest ) const
{
  if( object.null_type() )
  {
    // [ { "array": [ null ] } ]
    dest = Json::Value();
  }
  else if( object.int_type() )
  {
    // [ { "array": [ -1 ] } ]
    dest = object.get_int();
  }
  else if( object.uint_type() )
  {
    // [ { "array": [ 1 ] } ]
    dest = object.get_uint();
  }
  else if( object.double_type() )
  {
    // [ { "array": [ 1.2 ] } ]
    dest = object.get_double();
  }
  else if( object.string_type() )
  {
    // [ { "array": [ "string" ] } ]
    dest = object.get_cstring();
  }
  else if( object.bool_type() )
  {
    // [ { "array": [ true ] } ]
    dest = object.get_bool();
  }
  // else if( object.array_type() )
  // {
    // if( this->serialize_array( object, dest ) == false )
    // {
    //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
    //   return false;
    // }
  // }
  // else if( object.object_type() )
  // {
  //   if( this->serialize_object( object, dest ) == false )
  //   {
  //     NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
  //     return false;
  //   }
  // }
  else
  {
    #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
      NOM_DUMP( object.type_name() );
      // NOM_DUMP( member.key() );
      NOM_DUMP( object.stringify() );
    #endif

    // This non-fatal info err is not necessarily anything of concern; we do
    // yet handle recursing deeper into the node hierarchy -- note that we do
    // not even bother checking for array or object node types here.
    NOM_LOG_ERR( NOM, "Potential non-handled case in serialization of object node." );
  }

  return true;
}

bool JsonSerializer::unserialize_array( const Json::Value& object, Value& dest ) const
{
  Array arr;

  // NOM_DUMP( object.size() );
  for( auto i = 0; i != object.size(); ++i )
  {
    if( object[i].isNull() )
    {
      // [ { "array": [ null ] } ]
      arr.push_back( Value() );
    }
    else if( object[i].isInt() )
    {
      // [ { "array": [ -1 ] } ]
      arr.push_back( object[i].asInt() );
    }
    else if( object[i].isUInt() )
    {
      // [ { "array": [ 1 ] } ]
      arr.push_back( object[i].asUInt() );
    }
    else if( object[i].isDouble() )
    {
      // [ { "array": [ 1.2 ] } ]
      arr.push_back( object[i].asDouble() );
    }
    else if( object[i].isString() )
    {
      // [ { "array": [ "string" ] } ]
      arr.push_back( object[i].asCString() );
    }
    else if( object[i].isBool() )
    {
      // [ { "array": [ true ] } ]
      arr.push_back( object[i].asBool() );
    }
    // else if( object[i].isArray() )
    // {
      // if( this->serialize_array( arr, object[i] ) == false )
      // {
      //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
      //   return false;
      // }
    // }
    // else if( object[i].isObject() )
    // {
      // if( this->serialize_object( arr, object[i] ) == false )
      // {
      //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid object???" );
      //   return false;
      // }
    // }
    else
    {
      #if defined( NOM_DEBUG_JSON_UNSERIALIZER_VALUES )
        NOM_DUMP( object[i].type() );
        // NOM_DUMP( member.key() );
        NOM_DUMP( object[i] );
      #endif

      // This non-fatal info err is not necessarily anything of concern; we do
      // yet handle recursing deeper into the node hierarchy -- note that we do
      // not even bother checking for array or object node types here.
      NOM_LOG_ERR( NOM, "Potential non-handled case in un-serialization of array node." );
    }
  }

  dest = arr;

  return true;
}

bool JsonSerializer::unserialize_object( const Json::Value& object, Value& dest ) const
{
  if( object.isNull() )
  {
    // NOM_DUMP( "null" );
    dest = Value();
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
  // else if( object.isArray() )
  // {
    // if( this->unserialize_array( object, dest ) == false )
    // {
    //   NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid array???" );
    //   return false;
    // }
  // }
  // else if( object.isObject() )
  // {
    // if( this->unserialize_object( object, dest ) == false )
    // {
    //   NOM_LOG_ERR( NOM, "Could not un-serialize values; invalid object???" );
    //   return false;
    // }
  // }
  else
  {
    #if defined( NOM_DEBUG_JSON_UNSERIALIZER_VALUES )
      NOM_DUMP( object.type() );
      // NOM_DUMP( member.key() );
      NOM_DUMP( object );
    #endif

    // This non-fatal info err is not necessarily anything of concern; we do
    // yet handle recursing deeper into the node hierarchy -- note that we do
    // not even bother checking for array or object node types here.
    NOM_LOG_ERR( NOM, "Potential non-handled case in un-serialization of object node." );
  }

  return true;
}

} // namespace nom
