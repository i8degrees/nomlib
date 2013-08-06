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
#include "nomlib/json/JSON_FileReader.h"

namespace nom {

JSON_FileReader::JSON_FileReader ( void )
{
NOM_LOG_CLASSINFO;
}

JSON_FileReader::~JSON_FileReader ( void )
{
NOM_LOG_CLASSINFO;

  if ( this->fp.is_open() )
    this->fp.close();
}

//const json_spirit::mValue& find_value ( const json_spirit::mObject& obj, const std::string& name )
//{
  //json_spirit::mObject::const_iterator i = obj.find ( name );

  //assert ( i != obj.end() );
  //assert ( i->first == name );

  //return i->second;
//}

int JSON_FileReader::readInt ( const json_spirit::Value &value )
{
  return value.get_int();
}

std::string JSON_FileReader::readString ( const json_spirit::Value &value )
{
  return value.get_str();
}

JSON_Value JSON_FileReader::getObject ( const json_spirit::Object &obj, const std::string& name )
{
  for ( json_spirit::Object::size_type o = 0; o != obj.size(); o++ )
  {
    const json_spirit::Pair &pair = obj[o];
    const std::string &path = pair.name_;
    const json_spirit::Value &value = pair.value_;

    if ( path == name )
    {
      switch ( value.type() )
      {
        default:
        break;

        case json_spirit::int_type:
          //std::cout << path << ": " << value.get_int() << std::endl;
          return JSON_Value ( path, value );
        break;

        case json_spirit::array_type:
        {
          const json_spirit::Array &arr = value.get_array();
          for ( int i = 0; i < arr.size(); i++ )
          {
            //temp.push_back ( arr[i].get_int() );
            //std::cout << arr[i].get_int();
          }
          //return JSON_Value ( path, temp );
          /*
          if ( i == arr.size() - 1 ) // next to last element
            std::cout << " ";
          else
            std::cout << ", ";
          }
          //std::cout << std::endl << "---" << std::endl;
          */
        }
        break;

        case json_spirit::str_type:
          //std::cout << path << ": " << value.get_str() << std::endl;
          return JSON_Value ( path, value );
        break;
      }
    }
  }
  return JSON_Value ( "ERR", "Undefined" );
}

bool JSON_FileReader::Parse ( std::string filename )
{
  json_spirit::Object root_obj;
  json_spirit::Value value;

  this->fp.open ( filename );

  if ( this->fp.is_open() )
  {
    if ( this->fp.good() )
    {
      //json_spirit::read ( fp, value );

      json_spirit::Array &values = value.get_array();

      for ( json_spirit::Array::size_type i = 0; i != values.size(); i++ )
      {
        root_obj = values[i].get_obj();

        val.push_back ( getObject ( root_obj, "ID" ) );
        val.push_back ( getObject ( root_obj, "Element" ) );
        val.push_back ( getObject ( root_obj, "Name" ) );
        val.push_back ( getObject ( root_obj, "Ranks" ) );
      }
    }
    else
    {
      std::cout << "ERR: " << "Reading file" << std::endl;
      return false;
    }

    this->fp.close();
  }
  else
  {
    std::cout << "ERR: " << "Opening file" << std::endl;
    return false;
  }

  for ( int i = 0; i < val.size(); i++ )
  {
    if ( val[i].getName() == "ID" )
    {
      std::cout << val[i].getValue().get_int() << "\n";
    }
    else if ( val[i].getName() == "Name" )
    {
      std::cout << val[i].getString() << "\n";
    }
  }

  return true;
}


} // namespace nom
