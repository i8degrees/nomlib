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
  //nom::JSON::FileWriter writer;
  Json::Value root ( Json::arrayValue );
  Json::Value node1 ( Json::objectValue );
  Json::Value node2 ( Json::objectValue );
  Json::Value node3 /*( Json::arrayValue )*/;

  // Fatal error; if we are not able to complete this step, it means that
  // we probably cannot rely on our resource paths!
  if ( nom::nomlib_init ( argc, argv ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }

  node1[0]["HelloWorld"] = "I am a string!";
  node1[0]["SignedInteger"] = -42;
  node1[0]["UnsignedInteger"] = 42;

  node2["Float"] = 42.56;
/*
  node2.append ( node1 );
  Json::Value testme;
  //testme["Float"] = 42.56;
  //node2.append ( testme );
*/

  std::vector<std::string> StringArray;
  StringArray.push_back ( "StringArray index 0" );
  StringArray.push_back ( "StringArray index 1" );
  StringArray.push_back ( "StringArray index 2" );
  StringArray.push_back ( "StringArray index 3" );
  for ( auto member = StringArray.begin(); member != StringArray.end(); ++member )
  {
    node3["StringArray"].append ( * member );
  }

  root.append ( node1 );
  root.append ( node2 );
  root.append ( node3 );
/*
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
*/

  std::stringstream os;
  os << root;
  std::cout << os.str();

  std::cout << "\n";
  for ( nom::uint idx = 0; idx < root.size(); ++idx )
  {
    const Json::Value dump = root[idx]["StringArray"];

    for ( nom::uint idx = 0; idx < dump.size(); ++idx )
    {
      std::cout << dump[idx].asString() << std::endl;
    }
  }

  std::cout << "\n";
  for ( nom::uint idx = 0; idx < root.size(); ++idx )
  {
    const Json::Value dump = root[idx]["Float"];
    std::cout << root[idx]["Float"] << "\n";
    /*
    for ( nom::uint idx = 0; idx < dump.size(); ++idx )
    {
      std::cout << dump[idx] << std::endl;
    }
    */
  }

  //nom::JSON::Object testme;
  //nom::JSON::Value val;
  //testme.push_back ( nom::JSON::Value("Hello",42));
  //testme.push_back ( nom::JSON::Value("World",69));
  //val.push_back ( "b00bs", nom::JSON::Value("b00bs", 55));

  //std::cout << testme.get_int("Hello") << "\n";
  //std::cout << testme.get_int("World") << "\n";
  //std::cout << val.get_int("b00bs") << "\n";

  //NOM_DUMP_VAR(testme.size());
  //for ( nom::uint idx = 0; idx < testme.size(); ++idx )
  //{
/*
    std::cout
    << testme[idx].get_int("Hello")
    << testme[idx].get_int("World")
    << "\n";
*/
  //}

/*
  Json::Value::Members members;
  NOM_DUMP_VAR(root.size());
  std::vector<std::string> keys;
  for ( nom::uint idx = 0; idx < root.size(); ++idx )
  {
    members = root[idx].getMemberNames();
    for ( nom::uint idx = 0; idx < members.size(); ++idx )
    {
      std::cout << members[idx] << std::endl;
      keys.push_back( members[idx] );
      //if ( root[members[idx]].type() == Json::stringValue )
      {
        std::cout << "b00bs\n";
        //std::cout << root[members[idx]].asString() << "\n";
      }
      //else if ( root[members[idx]].type() == Json::intValue )
      {
        std::cout << "b00bs\n";
        //std::cout << root[members[idx]].asInt() << "\n";
      }
    }
  }
*/

/*
  std::cout << "\n";
  for ( nom::uint i = 0; i < keys.size(); ++i )
  {
    for ( nom::uint idx = 0; idx < root.size(); ++idx )
    {
      std::cout << root[idx][keys[i]].asString();
      std::cout << root[idx][keys[i]].asInt();
    }
  }
*/

/*
  if ( writer.save ( OUTPUT_JSON_FILENAME, root ) == false )
  {
    nom::DialogMessageBox ( APP_NAME, "Could not save output JSON file: " + OUTPUT_JSON_FILENAME );
    return NOM_EXIT_FAILURE;
  }

  nom::DialogMessageBox ( APP_NAME, "Success! The resulting JSON output has been saved at: " + OUTPUT_JSON_FILENAME );
*/
  return NOM_EXIT_SUCCESS;
}
