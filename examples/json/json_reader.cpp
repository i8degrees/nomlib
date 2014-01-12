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

// JSON FileReader usage example

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/json.hpp>
#include <nomlib/system.hpp>

const std::string APP_NAME = "examples/json_reader";

const nom::Path p;
const std::string APP_RESOURCES_DIR = "Resources";
const std::string RESOURCE_JSON_EXAMPLE1_GOOD = APP_RESOURCES_DIR + p.native() + "ex1_good.json";
const std::string RESOURCE_JSON_EXAMPLE1_BAD = APP_RESOURCES_DIR + p.native() + "ex1_bad.json";

int main ( int argc, char* argv[] )
{
  nom::JSON::FileReader parser;
  Json::Value root;

  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if ( nom::init ( argc, argv ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }
  atexit(nom::quit);

  if ( parser.load ( RESOURCE_JSON_EXAMPLE1_GOOD, root ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not parse input resource file: " + RESOURCE_JSON_EXAMPLE1_GOOD );
    return NOM_EXIT_FAILURE;
  }

  if ( root.isObject() == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Invalid JSON: does not begin as an object declaration." );
    return NOM_EXIT_FAILURE;
  }

  if ( root["User"]["UserName"].isNull() == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Root node 'User' has a UserName of '" + root["User"]["UserName"].asString() + "'", nom::NOM_DIALOG_INFORMATION );
  }
  else
  {
    nom::DialogMessageBox ( APP_NAME, "Could not parse root node 'User' parameter 'UserName' with input file: " + RESOURCE_JSON_EXAMPLE1_GOOD );
    return NOM_EXIT_FAILURE;
  }

  if ( root["User2"]["UserName"].isNull() == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Root node 'User2' has a UserName of '" + root["User2"]["UserName"].asString() + "'", nom::NOM_DIALOG_INFORMATION );
  }
  else
  {
    nom::DialogMessageBox ( APP_NAME, "Could not parse root node 'User2' parameter 'UserName' with input file: " + RESOURCE_JSON_EXAMPLE1_GOOD );
    return NOM_EXIT_FAILURE;
  }

  return NOM_EXIT_SUCCESS;
}
