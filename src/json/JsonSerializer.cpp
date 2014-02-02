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
  JsonCppSerializer writer; // JsonCpp library
  JsonCppValue value;       // API wrapper for interfacing with JsonCpp
  // Temporary buffering object we use to collect JSON input in form of array
  // elements found within JSON objects.
  Array values;

  // Temporary buffering object we use to collect input in form of JSON objects
  // Object objects;

  // Begin iterating through the entire container; values, arrays, objects and
  // even objects within objects.
  for( Value::ConstIterator itr = source.begin(); itr != source.end(); ++itr )
  {
    ValueConstIterator member( itr );

    // { "root": null }
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

        #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
          NOM_DUMP( itr->type_name() );
          NOM_DUMP( member.key() );
          NOM_DUMP( itr->stringify() );
        #endif

        if( itr->null_type() )
        {
          value.insert( member.key() );
        }
        else if( itr->int_type() )
        {
          value.insert( member.key(), itr->get_int() );
        }
        else if( itr->uint_type() )
        {
          value.insert( member.key(), itr->get_uint() );
        }
        else if( itr->double_type() )
        {
          value.insert( member.key(), itr->get_double() );
        }
        else if( itr->bool_type() )
        {
          value.insert( member.key(), itr->get_bool() );
        }
        else if( itr->string_type() )
        {
          value.insert( member.key(), itr->get_cstring() );
        }
        else
        {
          #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
            NOM_DUMP( itr->type_name() );
            NOM_DUMP( member.key() );
            NOM_DUMP( itr->stringify() );
          #endif

          if( itr->array_type() == false )
          {
            NOM_LOG_ERR( NOM, "Could not serialize values; invalid object?" );
            return false;
          }
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
            // New array elements
            values.clear();

            #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
              NOM_DUMP( itr->type_name() );
              NOM_DUMP( member.key() );
              NOM_DUMP( itr->stringify() );
            #endif

            if( itr->null_type() )
            {
              // [ { "array": [ null ] } ]
              values.push_back( Value() );
            }
            else if( itr->int_type() )
            {
              // [ { "array": [ -1 ] } ]
              values.push_back( itr->get_int() );
            }
            else if( itr->uint_type() )
            {
              // [ { "array": [ 1 ] } ]
              values.push_back( itr->get_uint() );
            }
            else if( itr->double_type() )
            {
              // [ { "array": [ 1.2 ] } ]
              values.push_back( itr->get_double() );
            }
            else if( itr->string_type() )
            {
              // [ { "array": [ "string" ] } ]
              values.push_back( itr->get_cstring() );
            }
            else if( itr->bool_type() )
            {
              // [ { "array": [ true ] } ]
              values.push_back( itr->get_bool() );
            }
            else
            {
              #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
                NOM_DUMP( itr->type_name() );
                NOM_DUMP( member.key() );
                NOM_DUMP( itr->stringify() );
              #endif

              NOM_LOG_ERR( NOM, "Could not serialize values; invalid array?" );
              return false;
            }

            // Store array elements
            value.insert( member.key(), values );
          } // end for array elements loop
        } // end if JSON array
      } // end for objects loop

      // EOF JSON Object
      value.endl();

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
  if( writer.serialize( value, output ) == false )
  {
    // TODO: additional err logging?
    return false;
  }

  return true;
}

bool JsonSerializer::unserialize( const std::string& input, Value& dest ) const
{
  JsonCppValue::JsonMemberType members; // Member keys of each object iteration
  JsonCppValue value; // API wrapper for JsonCpp
  JsonCppSerializer parser; // JsonCpp library

  // Temporary buffering object we use to collect JSON input in form of array
  // elements found within JSON objects.
  // nom::Array values;

  // Temporary buffering objects we use to collect input in form of JSON objects;
  // we append individual objects to the overall container as we iterate through
  // the entire underlying container.
  Object object;
  Object objects;

  if( parser.unserialize( input, value ) == false )
  {
    // TODO: additional err logging?
    return false;
  }

  uint index = 0;
  for( auto idx = 0; idx != value.size(); ++idx )
  {
    members = value.members( index );
    #if defined( NOM_DEBUG_JSON_UNSERIALIZER_VALUES )
      NOM_DUMP( value.type( index ) );
      NOM_DUMP(index);
    #endif

    if( value.object_type( idx ) )
    {
      // We have a new JSON object
      object.clear();

      for ( auto pos = 0; pos != members.size(); ++pos )
      {
        std::string key = members[pos];

        #if defined( NOM_DEBUG_JSON_UNSERIALIZER_VALUES )
          NOM_DUMP( key );
        #endif

        if ( value.null_type( key ) )
        {
          // object.push_back( Pair( key, Value() ) );
          object[key] = Value();
        }
        else if ( value.int_type( key ) )
        {
          // object.push_back( Pair( key, value.get_int( key ) ) );
          object[key] = value.get_int( key );
        }
        else if ( value.uint_type( key ) )
        {
          // object.push_back( Pair( key, value.get_uint( key ) ) );
          object[key] = value.get_uint( key );
        }
        else if( value.double_type( key ) )
        {
          // object.push_back( Pair( key, value.get_double( key ) ) );
          object[key] = value.get_double( key );
        }
        else if( value.bool_type( key ) )
        {
          // object.push_back( Pair( key, value.get_bool( key ) ) );
          object[key] = value.get_bool( key );
        }
        else if ( value.string_type( key ) )
        {
          // object.push_back( Pair( key, value.get_string( key ) ) );
          object[key] = value.get_string( key );
        }
        else if( value.array_type( key ) )
        {
          Array values = value.get_values( key );
          // object.push_back( Pair( key, values ) );
          object[key] = values;
        }
        else if( value.object_type( key ) )
        {
          // ++index;
        }
        else
        {
          // ++index;
        }
      } // end for member keys loop

      // FIXME: We do not have mapped object structures implemented due in part
      // to the missing key portion of our pair.
      // objects.push_back( Pair( "", object ) );

      // NOTE: Mapped key -- std::map -- must not be blank, so we fudge it with
      // index. Fudgery here is OK for now, because we do not use this key value
      // value for anything other than nom::Value object dumps (see << operator
      // overload). See also the FIXME note above this note.
      objects[index] = object;

      // EOF JSON object; this will iterate to the next object for us
      value.endl();

      // Move onto the next member key
      ++index;

    } // end if type object
  } // end for value.size() loop

  // TODO: additional validity / err checks?

  // Copy our built nom::Value object to the specified destination object
  dest = objects;

  return true;
}

const std::string JsonSerializer::stringify( const Value& obj ) const
{
  std::stringstream os;

  os << obj;

  return os.str();
}

bool JsonSerializer::serialize_array( const Value& object, JsonCppValue& dest ) const
{
  NOM_STUBBED(NOM);

  return false;
}

bool JsonSerializer::serialize_object( const Value& object, JsonCppValue& dest ) const
{
  NOM_STUBBED(NOM);

  return false;
}

} // namespace nom
