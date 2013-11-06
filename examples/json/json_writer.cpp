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

// JSON FileWriter usage example

#include <iostream>
#include <string>
#include <cstdlib>
#include <cassert>

#include <nomlib/json.hpp>
#include <nomlib/system.hpp>

const nom::Path p;
const std::string APP_NAME = "examples/json_writer";
const std::string OUTPUT_JSON_FILENAME = "json_writer.json";

int main ( int argc, char* argv[] )
{
  nom::JSON::FileWriter writer;
  Json::Value root;

  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if ( nom::nomlib_init ( argc, argv ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
    exit ( EXIT_FAILURE );
  }

  root[0]["HelloWorld"] = "I am a string!";
  root[0]["SignedInteger"] = -42;
  root[0]["UnsignedInteger"] = 42;
  root[0]["Float"] = 42.56;
  root[0]["BooleanTrue"] = true;
  root[0]["BooleanFalse"] = false;
  root[0]["null"];
  root[0]["StringArray"][0] = "StringArray index 0";
  root[0]["StringArray"][1] = "StringArray index 1";
  root[0]["StringArray"][2] = "StringArray index 2";
  root[0]["StringArray"][3] = "StringArray index 3";

  root[0]["IntegerArray"][0] = 6;
  root[0]["IntegerArray"][1] = 10;
  root[0]["IntegerArray"][2] = 9;
  root[0]["IntegerArray"][3] = 8;

  if ( writer.save ( OUTPUT_JSON_FILENAME, root ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not save output JSON file: " + OUTPUT_JSON_FILENAME );
    return EXIT_FAILURE;
  }

  nom::DialogMessageBox ( APP_NAME, "Success! The resulting JSON output has been saved at: " + OUTPUT_JSON_FILENAME );

  return EXIT_SUCCESS;
}
