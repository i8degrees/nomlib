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
#include "nomlib/serializers/JsonCppSerializer.hpp"

namespace nom {

JsonCppSerializer::JsonCppSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

JsonCppSerializer::~JsonCppSerializer( void )
{
  //NOM_LOG_TRACE(NOM);
}

std::string JsonCppSerializer::serialize  ( const Value& source )
{
  std::stringstream os;
  Json::Value buffer;

  if( this->write( source, buffer ) == false )
  {
    NOM_LOG_ERR ( NOM, "Could not serialize the source object." );
    return "\0";
  }

  // if( options == Features::HumanReadable )
  // {
    // Json::StyledStreamWriter writer( JSONCPP_INDENTION_LEVEL );
    // writer.write( os, buffer );
  // }
  // else // Features::Compact
  // {
    Json::FastWriter writer;
    os << writer.write( buffer );
  // }

  return os.str();
}

bool JsonCppSerializer::save  ( const Value& source, const std::string& filename )
{
  std::ofstream fp;

  fp.open( filename );

  if ( ! fp.is_open() || ! fp.good() )
  {
    NOM_LOG_ERR ( NOM, "Could not save output to file: " + filename );
    return false;
  }

  // nom::Value to file
  fp << this->serialize( source );

  fp.close();

  return true;
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

        Object objects = itr->object();

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
            if( this->write_array( itr->ref(), dest[idx][key] ) == false )
            {
              NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
              return false;
            }
          }

          if( dest[idx][key].isObject() )
          {
            if( this->write_object( itr->ref(), dest[idx][key] ) == false )
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
        // FIXME: should be if( source[root_key][key].object_type() )
        if( itr->ref().array_type() )
        {
          if( this->write_array( itr->ref(), dest[root_key][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
            return false;
          }
        }

        // Object within an object.
        // Does *NOT* work here: if( itr->ref().object_type() )
        // FIXME: if( source[root_key][key].object_type() )
        // {
          if( this->write_object( itr->ref(), dest[root_key][key] ) == false )
          {
            NOM_LOG_ERR( NOM, "Could not serialize values; invalid object?" );
            return false;
          }
        // }
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

      if( this->write_array( itr->ref(), dest[root_key] ) == false )
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
      // return false;
    }
  }

  return true;
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
      // Special case -- must be handled by JsonCppSerializer::write_array.
      break;
    }

    case Value::ValueType::ObjectValues:
    {
      // Special case -- must be handled by JsonCppSerializer::write_object.
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

bool JsonCppSerializer::write_array( const Value& object, Json::Value& dest ) const
{
  uint index = 0;

  Value array = object.array();

  for( auto itr = array.begin(); itr != array.end(); ++itr )
  {
    switch( itr->type() )
    {
      default: // Unknown type
      {
        #if defined( NOM_DEBUG_JSONCPP_DESERIALIZER_VALUES )
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

      // TODO: verify that this works; (Resources/examples/json/inventory.json)
      case Value::ValueType::ArrayValues:
      {
        NOM_LOG_ERR( NOM, "Could not serialize values; an array cannot exist within another array." );
        return false;

        // if( this->write_array( itr->ref(), dest ) == false )
        // {
        //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
        //   return false;
        // }

        break;
      }

      // [ { "key": [ { ... } ] } ]
      case Value::ValueType::ObjectValues:
      {
        if( this->write_object( itr->ref(), dest[index] ) == false )
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

bool JsonCppSerializer::write_object( const Value& object, Json::Value& dest ) const
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
      // TODO: verify to see if this works; (Resources/examples/json/inventory.json)
      //
      // if( this->write_array( object, dest ) == false )
      // {
      //   NOM_LOG_ERR( NOM, "Could not serialize values; invalid array???" );
      //   return false;
      // }

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
            if( this->write_array( value, obj[key] ) == false )
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
            if( this->write_object( value, obj[key] ) == false )
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

} // namespace nom
