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
#include "nomlib/serializers/JsonConfigFile.hpp"

// Private forward declarations
#include "nomlib/serializers/JsonCppDeserializer.hpp"
#include "nomlib/serializers/JsonCppSerializer.hpp"
#include "nomlib/ptree.hpp" // Value class

namespace nom {

JsonConfigFile::JsonConfigFile( void )
{
  // NOM_LOG_TRACE( NOM );

  if( this->initialize() == false )
  {
    // This shouldn't even be possible yet, unless we are testing a new
    // interface out!
    NOM_LOG_ERR( NOM, "Could not initialize load/save interfaces." );
  }
}

JsonConfigFile::~JsonConfigFile( void )
{
  // NOM_LOG_TRACE( NOM );

  if( this->flush() == false )
  {
    NOM_LOG_ERR( NOM, "Could not save file: " + this->filename() );
  }
}

bool JsonConfigFile::flush( void )
{
  return this->save();
}

IValueDeserializer* JsonConfigFile::deserializer( void ) const
{
  return this->deserializer_.get();
}

IValueSerializer* JsonConfigFile::serializer( void ) const
{
  return this->serializer_.get();
}

bool JsonConfigFile::load( void )
{
  if( this->deserializer() != nullptr )
  {
    Value objects;

    if( this->deserializer()->load( this->filename(), objects ) == false )
    {
      NOM_LOG_ERR( NOM, "Could not deserialize from file: " + this->filename() );
      return false;
    }

    for( auto itr = objects.begin(); itr != objects.end(); ++itr )
    {
      if( itr->object_type() )
      {
        Object object = itr->object();

        for( auto it = object.begin(); it != object.end(); ++it )
        {

          nom::Value::ConstIterator members( it );
          std::string key = members.key();

          if( members->int_type() )
          {
            this->set_property( key, members->get_int() );
          }
          else if( members->uint_type() )
          {
            this->set_property( key, members->get_uint() );
          }
          else if( members->double_type() )
          {
            this->set_property( key, members->get_double() );
          }
          else if( members->string_type() )
          {
            this->set_property( key, members->get_string() );
          }
          else if( members->bool_type() )
          {
            this->set_property( key, members->get_bool() );
          }
          else if( members->array_type() )
          {
            // Not yet implemented
            NOM_LOG_ERR( NOM, "Could not deserialize value: array type supported." );
            return false;
          }
          else if( members->object_type() )
          {
            // Not yet implemented
            NOM_LOG_ERR( NOM, "Could not deserialize value: object type supported." );
            return false;
          }
          else  // Unknown
          {
            NOM_LOG_ERR( NOM, "Could not deserialize unknown value type." );
            return false;
          }

        } // end object iteration
      } // end if object type
    } // end objects iteration
  }

  return true;
}

bool JsonConfigFile::save( void )
{
  if( this->serializer() != nullptr )
  {
    Value output;

    // for( auto itr = this->config_.begin(); itr != this->config_.end(); ++itr )
    // {
    //   if( itr->second.object_type() )
    //   {
    //     Object object = itr->second.object();

    //     for( auto it = object.begin(); it != object.end(); ++it )
    //     {
    //       nom::Value::ConstIterator members( it );
    //       std::string key = members.key();

    //       if( members->int_type() )
    //       {
    //         // this->set_property( key, members->get_int() );
    //         cfg = members->get_int();
    //       }
    //       if( members->string_type() )
    //       {
    //         // this->set_property( key, members->get_string() );
    //         cfg = members->get_string();
    //       }
    //     } // end object iteration
    //   } // end if object type
    // } // end objects iteration

    IConfigFile::value_type cfg = this->get();
    for( auto itr = cfg.begin(); itr != cfg.end(); ++itr )
    {
      std::string key = itr->first;
      Value member = itr->second;

      if( member.int_type() )
      {
        output["root"][key] = member.get_int();
      }
      else if( member.uint_type() )
      {
        output["root"][key] = member.get_uint();
      }
      else if( member.double_type() )
      {
        output["root"][key] = member.get_double();
      }
      else if( member.string_type() )
      {
        output["root"][key] = member.get_string();
      }
      else if( member.bool_type() )
      {
        output["root"][key] = member.get_bool();
      }
      else if( member.array_type() )
      {
        // Not yet implemented
        NOM_LOG_ERR( NOM, "Could not serialize value: array type supported." );
        return false;
      }
      else if( member.object_type() )
      {
        // Not yet implemented
        NOM_LOG_ERR( NOM, "Could not serialize value: object type supported." );
        return false;
      }
      else  // Unknown
      {
        NOM_LOG_ERR( NOM, "Could not serialize unknown value type." );
        return false;
      }
    }

    // Prevent file output if the Value output object's size is less than one.
    // This prevents a bug from happening in the first loading test in
    // JsonConfigFileTest.
    if( output.size() > 0 )
    {
      if( this->serializer()->save( output, this->filename() ) == false )
      {
        NOM_LOG_ERR( NOM, "Could not serialize to file: " + this->filename() );
        return false;
      }
    }
  }

  return true;
}

// Private scope

bool JsonConfigFile::initialize( void )
{
  this->set_deserializer( new JsonCppDeserializer() );
  this->set_serializer( new JsonCppSerializer() );

  return true;
}

void JsonConfigFile::set_deserializer( IValueDeserializer* fp )
{
  this->deserializer_.reset( fp );
}

void JsonConfigFile::set_serializer( IValueSerializer* fp )
{
  this->serializer_.reset( fp );
}

} // namespace nom
