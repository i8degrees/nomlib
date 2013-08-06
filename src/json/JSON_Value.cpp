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
#include "nomlib/json/JSON_Value.h"

namespace nom {

JSON_Value::JSON_Value ( void )
{
NOM_LOG_CLASSINFO;

  this->name = "\0";
  this->value = 0;
}

JSON_Value::JSON_Value ( const std::string name_, const json_spirit::Value value_ )
{
NOM_LOG_CLASSINFO;

  this->name = name_;
  this->value = value_;
}

/*
JSON_Value::JSON_Value ( const std::string name_, const JSON_Array value_ )
{
  #ifdef DEBUG_JSON_VALUE_OBJ
    std::cout << "JSON_Value::JSON_Value (): " << "Hello, world!" << std::endl << std::endl;
  #endif

  this->name = name_;
  this->value = value_;
}
*/

JSON_Value::~JSON_Value ( void )
{
NOM_LOG_CLASSINFO;
}

const std::string JSON_Value::getName ( void )
{
  return this->name;
}

const json_spirit::Value JSON_Value::getValue ( void ) const
{
  switch ( this->value.type() )
  {
    default:
    break;

    case json_spirit::int_type:
      //return json_spirit::Value ( this->value );
      return this->value.get_int();
    break;

    case json_spirit::array_type:
      return this->value.get_array();
    break;

    case json_spirit::str_type:
      //return this->value.get_str();
    break;
  }
  return 0;
  //return "Undefined value";
}

const std::string JSON_Value::getString ( void ) const
{
  return this->value.get_str();
}

void JSON_Value::setName ( const std::string name_ )
{
  this->name = name_;
}

void JSON_Value::setValue ( const json_spirit::Value value_ )
{
  this->value = value_;
}


} // namespace nom
