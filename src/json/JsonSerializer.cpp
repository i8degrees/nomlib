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

          if( itr->null_type() )
          {
            // value.insert( member.key() );
            value[idx][key] = Json::Value();
          }
          else if( itr->int_type() )
          {
            // value.insert( member.key(), itr->get_int() );
            value[idx][key] = Json::Value( itr->get_int() );
          }
          else if( itr->uint_type() )
          {
            // value.insert( member.key(), itr->get_uint() );
            value[idx][key] = Json::Value( itr->get_uint() );
          }
          else if( itr->double_type() )
          {
            // value.insert( member.key(), itr->get_double() );
            value[idx][key] = Json::Value( itr->get_double() );
          }
          else if( itr->bool_type() )
          {
            // value.insert( member.key(), itr->get_bool() );
            value[idx][key] = Json::Value( itr->get_bool() );
          }
          else if( itr->string_type() )
          {
            // value.insert( member.key(), itr->get_cstring() );
            value[idx][key] = Json::Value( itr->get_cstring() );
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
              // values.clear();

              #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
                NOM_DUMP( itr->type_name() );
                NOM_DUMP( member.key() );
                NOM_DUMP( itr->stringify() );
              #endif

              if( itr->null_type() )
              {
                // [ { "array": [ null ] } ]
                // values.push_back( Value() );
                value[idx][key].append (Json::Value() );
              }
              else if( itr->int_type() )
              {
                // [ { "array": [ -1 ] } ]
                // values.push_back( itr->get_int() );
                value[idx][key].append( itr->get_int() );
              }
              else if( itr->uint_type() )
              {
                // [ { "array": [ 1 ] } ]
                // values.push_back( itr->get_uint() );
                value[idx][key].append( itr->get_uint() );
              }
              else if( itr->double_type() )
              {
                // [ { "array": [ 1.2 ] } ]
                // values.push_back( itr->get_double() );
                value[idx][key].append( itr->get_double() );
              }
              else if( itr->string_type() )
              {
                // [ { "array": [ "string" ] } ]
                // values.push_back( itr->get_cstring() );
                value[idx][key].append( itr->get_cstring() );
              }
              else if( itr->bool_type() )
              {
                // [ { "array": [ true ] } ]
                // values.push_back( itr->get_bool() );
                value[idx][key].append( itr->get_bool() );
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
              // value.insert( member.key(), values );
            } // end for array elements loop
          } // end if JSON array
        }
      }

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

        if( itr->null_type() )
        {
          value[root_key][key] = Json::Value();
        }
        else if( itr->int_type() )
        {
          // value.insert( member.key(), itr->get_int() );
          value[root_key][key] = Json::Value( itr->get_int() );
        }
        else if( itr->uint_type() )
        {
          // value.insert( member.key(), itr->get_uint() );
          value[root_key][key] = Json::Value( itr->get_uint() );
        }
        else if( itr->double_type() )
        {
          // value.insert( member.key(), itr->get_double() );
          value[root_key][key] = Json::Value( itr->get_double() );
        }
        else if( itr->bool_type() )
        {
          // value.insert( member.key(), itr->get_bool() );
          value[root_key][key] = Json::Value( itr->get_bool() );
        }
        else if( itr->string_type() )
        {
          // value.insert( member.key(), itr->get_cstring() );
          value[root_key][key] = Json::Value( itr->get_cstring() );
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
            // values.clear();

            #if defined( NOM_DEBUG_JSON_SERIALIZER_VALUES )
              NOM_DUMP( itr->type_name() );
              NOM_DUMP( member.key() );
              NOM_DUMP( itr->stringify() );
            #endif

            if( itr->null_type() )
            {
              // [ { "array": [ null ] } ]
              // values.push_back( Value() );
              value[root_key][key].append (Json::Value() );
            }
            else if( itr->int_type() )
            {
              // [ { "array": [ -1 ] } ]
              // values.push_back( itr->get_int() );
              value[root_key][key].append( itr->get_int() );
            }
            else if( itr->uint_type() )
            {
              // [ { "array": [ 1 ] } ]
              // values.push_back( itr->get_uint() );
              value[root_key][key].append( itr->get_uint() );
            }
            else if( itr->double_type() )
            {
              // [ { "array": [ 1.2 ] } ]
              // values.push_back( itr->get_double() );
              value[root_key][key].append( itr->get_double() );
            }
            else if( itr->string_type() )
            {
              // [ { "array": [ "string" ] } ]
              // values.push_back( itr->get_cstring() );
              value[root_key][key].append( itr->get_cstring() );
            }
            else if( itr->bool_type() )
            {
              // [ { "array": [ true ] } ]
              // values.push_back( itr->get_bool() );
              value[root_key][key].append( itr->get_bool() );
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
            // value.insert( member.key(), values );
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

          if( value[idx][key].isNull() )
          {
            // NOM_DUMP( "null" );
            object[key] = Value();
          }
          else if( value[idx][key].isInt() )
          {
            // NOM_DUMP( value[idx][key].asInt() );
            object[key] = value[idx][key].asInt();
          }
          else if( value[idx][key].isUInt() )
          {
            // NOM_DUMP( value[idx][key].asUInt() );
            object[key] = value[idx][key].asUInt();
          }
          else if( value[idx][key].isDouble() )
          {
            // NOM_DUMP( value[idx][key].asDouble() );
            object[key] = value[idx][key].asDouble();
          }
          else if( value[idx][key].isString() )
          {
            // NOM_DUMP( value[idx][key].asString() );
            object[key] = value[idx][key].asString();
          }
          else if( value[idx][key].isBool() )
          {
            // NOM_DUMP( value[idx][key].asBool() );
            object[key] = value[idx][key].asBool();
          }
          else if( value[idx][key].isArray() )
          {
            Array array;
            for( auto i = 0; i != value[idx][key].size(); ++i )
            {
              // { "cmd" : [ "hax" ] }
              // NOM_DUMP( value[idx][key][i] );

              if( value[idx][key][i].isNull() )
              {
                // NOM_DUMP( "null" );
                array.push_back( Value() );
              }
              else if( value[idx][key][i].isInt() )
              {
                // NOM_DUMP( value[idx][key][i].asInt() );
                array.push_back( value[idx][key][i].asInt() );
              }
              else if( value[idx][key][i].isUInt() )
              {
                // NOM_DUMP( value[idx][key][i].asUInt() );
                array.push_back( value[idx][key][i].asUInt() );
              }
              else if( value[idx][key][i].isDouble() )
              {
                // NOM_DUMP( value[idx][key][i].asDouble() );
                array.push_back( value[idx][key][i].asDouble() );
              }
              else if( value[idx][key][i].isString() )
              {
                // NOM_DUMP( value[idx][key][i].asString() );
                array.push_back( value[idx][key][i].asString() );
              }
              else if( value[idx][key][i].isBool() )
              {
                // NOM_DUMP( value[idx][key][i].asBool() );
                array.push_back( value[idx][key][i].asBool() );
              }
              else if( value[idx][key][i].isObject() )
              {
                // NOM_DUMP( object_members.size() );
                std::vector<std::string> object_members = value[idx][key][i].getMemberNames();

                for( auto pos = 0; pos != object_members.size(); ++pos )
                {
                  std::string key = object_members[pos];

                  // { "map" : [ { "cmd" : [...] } ] }
                  // NOM_DUMP(key);

                  // TODO: Finish "deep-depth" mapped objects.
                  //
                  // See also: sanity.json holds a mapped object with the member
                  // key of "map", of which contains an array that holds one
                  // object, with the member key "cmd", of which is an array.

                  // Should print the member key "cmd".
                  // NOM_DUMP( key );
/* Incomplete prototype
                  if( value[idx][key][i].isArray() )
                  {
                    NOM_DUMP("arr!");
                    NOM_DUMP( value[idx][key].size() );

                    Array arr;
                    for( auto i = 0; i != value[idx][key].size(); ++i )
                    {
                      if( value[idx][key][i].isNull() )
                      {
                        // NOM_DUMP( "null" );
                        arr.push_back( Value() );
                      }
                      else if( value[idx][key][i].isInt() )
                      {
                        // NOM_DUMP( value[idx][key][i].asInt() );
                        arr.push_back( value[idx][key][i].asInt() );
                      }
                      else if( value[idx][key][i].isUInt() )
                      {
                        // NOM_DUMP( value[idx][key][i].asUInt() );
                        arr.push_back( value[idx][key][i].asUInt() );
                      }
                      else if( value[idx][key][i].isDouble() )
                      {
                        // NOM_DUMP( value[idx][key][i].asDouble() );
                        arr.push_back( value[idx][key][i].asDouble() );
                      }
                      else if( value[idx][key][i].isString() )
                      {
                        NOM_DUMP( value[idx][key][i].asString() );
                        arr.push_back( value[idx][key][i].asString() );
                      }
                      else if( value[idx][key][i].isBool() )
                      {
                        // NOM_DUMP( value[idx][key][i].asBool() );
                        arr.push_back( value[idx][key][i].asBool() );
                      }
                      else
                      {
                        // TODO: case handling logic
                        NOM_LOG_ERR( NOM, "Unknown value type?!" );
                      }
                    }
                  }
EOF of incomplete prototype */
                }
              }
            }

            object[key] = array;
          }
        }
      }

      array.push_back( object );
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

          if( value[root_key][key].isInt() )
          {
            // NOM_DUMP( value[root_key][key].asInt() );
            object[key] = value[root_key][key].asInt();
          }
          else if( value[root_key][key].isUInt() )
          {
            // NOM_DUMP( value[root_key][key].asUInt() );
            object[key] = value[root_key][key].asUInt();
          }
          else if( value[root_key][key].isDouble() )
          {
            // NOM_DUMP( value[root_key][key].asDouble() );
            object[key] = value[root_key][key].asDouble();
          }
          else if( value[root_key][key].isString() )
          {
            // NOM_DUMP( value[root_key][key].asString() );
            object[key] = value[root_key][key].asString();
          }
          else if( value[root_key][key].isBool() )
          {
            // NOM_DUMP( value[root_key][key].asBool() );
            object[key] = value[root_key][key].asBool();
          }
          else if( value[root_key][key].isArray() )
          {
            // FIXME: Incomplete implementation -- we only handle integers at
            // the moment.

            Array array;
            for( auto i = 0; i != value[root_key][key].size(); ++i )
            {
              if( value[root_key][key][i].isInt() )
              {
                // NOM_DUMP( value[root_key][key][i].asInt() );
                array.push_back( value[root_key][key][i].asInt() );
              }
              else if( value[root_key][key][i].isUInt() )
              {
                // NOM_DUMP( value[root_key][key][i].isUInt() );
                array.push_back( value[root_key][key][i].isUInt() );
              }
              else if( value[root_key][key][i].isString() )
              {
                // NOM_DUMP( value[root_key][key][i].asString() );
                array.push_back( value[root_key][key][i].asString() );
              }
              else if( value[root_key][key][i].isArray() )
              {
                NOM_DUMP( value[root_key][key][i] );

                // TODO
              }
              else if( value[root_key][key][i].isObject() )
              {
                // TODO
              }
              else
              {
                // TODO
              }
            }

            object[key] = array;
          }
          else if( value[root_key][key].isObject() )
          {
            NOM_DUMP( key );

            // TODO
          }
        }

        objects[root_key] = object;
      }
    }
  }
        // FIXME: We do not have mapped object structures implemented due in part
        // to the missing key portion of our pair.
        // objects.push_back( Pair( "", object ) );

        // NOTE: Mapped key -- std::map -- must not be blank, so we fudge it with
        // index. Fudgery here is OK for now, because we do not use this key value
        // value for anything other than nom::Value object dumps (see << operator
        // overload). See also the FIXME note above this note.
        // objects[index] = object;
        // objects[root_key] = object;

        // EOF JSON object; this will iterate to the next object for us
        // value.endl();

        // Move onto the next member key
        // ++index;
    // } // end if type object
  // } // end for value.size() loop

  // value.dump( value );

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
  NOM_STUBBED(NOM);

  return false;
}

bool JsonSerializer::serialize_object( const Value& object, Json::Value& dest ) const
{
  NOM_STUBBED(NOM);

  return false;
}

} // namespace nom
