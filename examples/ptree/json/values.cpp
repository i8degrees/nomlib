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

/// \brief nom::Value usage examples

#include <nomlib/json.hpp>
#include <nomlib/xml.hpp>
#include <nomlib/system.hpp>

const std::string APP_NAME = "nom::Value";

/// Relative file path name of the resources directory
const std::string APP_RESOURCES_DIR = "Resources";

/// Relative file path name to our resources for this unit test
const nom::Path p;
const std::string RESOURCE_SANITY = APP_RESOURCES_DIR + p.native() + "sanity.json";
const std::string RESOURCE_SANITY2 = APP_RESOURCES_DIR + p.native() + "sanity2.json";

const std::string RESOURCE_XML = APP_RESOURCES_DIR + p.native() + "xml" + p.native() + "test_one.xml";
const std::string RESOURCE_JSON_BMFONT_GAMEOVER = APP_RESOURCES_DIR + p.native() + "xml" + p.native() + "gameover.json";
const std::string RESOURCE_XML_BMFONT_GAMEOVER = APP_RESOURCES_DIR + p.native() + "xml" + p.native() + "gameover.xml";

using namespace nom; // TODO: Remove

#define NOM_UNIT_TEST(num) \
  ( "Unit Test #num" )

// #define NOM_UNIT_TEST_ONE
// #define NOM_UNIT_TEST_TWO
// #define NOM_UNIT_TEST_FOUR
// #define NOM_UNSERIALIZER_UNIT_TEST_ONE
// #define NOM_SERIALIZER_UNIT_TEST_ONE
// #define NOM_JSONCPP_VALUE_TEST_ONE

/// \brief Enable expected results output during each unit test.
///
/// \note See Makefile for what to pass to enable this feature during compile
/// time.
//#define NOM_DEBUG_TESTS

std::vector<nom::Value> values =  {
                                    nom::Value(),                 // Null
                                    nom::Value( -1 ),             // SignedInteger
                                    nom::Value( 666 ),            // UnsignedInteger
                                    nom::Value( 1.8 ),            // RealNumber
                                    nom::Value( "StringValue!" ), // String
                                    nom::Value( true ),           // Boolean
                                    nom::Value( false ),          // Boolean
                                  };

#if defined( NOM_UNIT_TEST_ONE )

/// \brief Unit test for nom::Array
sint do_test_one( void )
{
  nom::Array list;

  for( auto itr = values.begin(); itr != values.end(); ++itr )
  {
    list.push_back( *itr );
  }

  for( nom::ValueIterator itr = list.begin(); itr != list.end(); ++itr )
  {
    NOM_DUMP( itr->type_name() );
    NOM_DUMP( itr->stringify() );
  }
  NOM_DASHED_ENDL();

  return NOM_EXIT_SUCCESS;
}
#endif

sint do_unserializer_test_two( void )
{
  JsonCppSerializer reader;
  JsonCppSerializer writer;
  nom::Value values; // I/O buffer

  // Initialize nom::Value from JSON file as input
  reader.unserialize( RESOURCE_SANITY, values );

  NOM_DUMP( values );

  // NOM_DUMP( writer.stringify( values ) );

  // Serialize values from our input file above; our input should match the
  // resulting serialized data.
  //
  // Note: output file is saved under working directory of where we execute
  // from.
  writer.serialize( values, "output.json" );

  return NOM_EXIT_SUCCESS;
}

sint do_unserializer_test_three( void )
{
  JsonCppSerializer reader;
  JsonCppSerializer writer;
  nom::Value values; // I/O buffer

  // Initialize nom::Value from JSON file as input
  reader.unserialize( RESOURCE_SANITY2, values );

  NOM_DUMP( values );

  // Serialize values from our input file above; our input should match the
  // resulting serialized data.
  //
  // Note: output file is saved under working directory of where we execute
  // from.
  writer.serialize( values, "output2.json" );

  return NOM_EXIT_SUCCESS;
}

sint do_leaks_test_one( void )
{
  std::stringstream os;
  nom::Timer timer;
  ISerializer* fp = new JsonCppSerializer();
  nom::Value output;

  // os << fp->stringify( RESOURCE_SANITY, output );

  timer.start();

  uint32 dt = 0;
  while( timer.ticks() < 10000 ) // 10s
  {
    if( fp->unserialize( RESOURCE_SANITY, output ) == false )
    {
      return NOM_EXIT_FAILURE;
    }

    // Our values object will automatically be cleared every time we serialize

    dt += 1;
  }

  // dt = ~22,000/10s -- ~2,200/1s
  // dt = ~9,000/4s -- ~2,250/1s

  timer.stop();
  NOM_DUMP(dt);

  return NOM_EXIT_SUCCESS;
}

sint do_serialization_test_two( void )
{
  nom::JsonCppSerializer writer;

  nom::Value val("StringValue");
  NOM_DUMP( val );

  std::string ret = writer.stringify( val );
  NOM_DUMP( ret );

  if( val.stringify() != "StringValue" ) return 1;
  if( ret != "StringValue" ) return 2;

  return NOM_EXIT_SUCCESS;
}

#if defined( NOM_JSONCPP_VALUE_TEST_ONE )
/// \brief nom::JsonCppValue sanity tests.
sint do_jsoncppvalue_test_one( void )
{
  // [ "value" ]
  JsonCppValue val1;
  val1.insert( 0, "value" );
  NOM_DUMP( val1 );
  // FIXME:
  // if( val1.stringify() != "[ \"value\" ]" ) return 1;

  // { "key" : "value" }
  JsonCppValue val2;
  // FIXME:
  // val2.insert( "key", "value" );
  // NOM_DUMP( val2 );

  // FIXME:
  // if( val1.stringify() != "null" ) return 2;

  // [ { "key" : "value" } ]
  JsonCppValue val3;
  val3.insert( "key", "value" );
  NOM_DUMP( val3 );
  // FIXME:
  // if( val1.stringify() != "[ { \"key\" : \"value\" } ]" ) return 3;

  return NOM_EXIT_SUCCESS;
}
#endif

/// \brief nom::Value sanity tests.
sint do_value_test_one( void )
{
  Json::Value sheet, dims, meta, size;

  dims["dims"][0] = 1;
  dims["dims"][1] = 1;
  dims["dims"][2] = 64;
  dims["dims"][3] = 64;
  sheet["cursors.png"][0] = dims;

  dims["dims"][0] = 65;
  dims["dims"][1] = 1;
  dims["dims"][2] = 64;
  dims["dims"][3] = 64;
  sheet["cursors.png"][1] = dims;

  size[0] = 320;
  size[1] = 240;
  meta["modified"] = nom::time();
  meta["size"] = size;

  sheet["meta"] = meta;

  NOM_DUMP( sheet );
  exit(0);

  JsonCppSerializer json; // stringify

  // Value val;
  // Object sheet, dims, meta, size, objects;

  // IntRect rect1( 1, 1, 64, 64 );
  // IntRect rect2( 65, 1, 64, 64 );

  // dims["dims"] = rect1.serialize();
  // sheet["ID_0"] = dims;

  // objects["cursors.png"] = sheet;
  // val = objects;

  // dims["dims"] = rect2.serialize();
  // sheet["ID_1"] = dims;

  // meta["last_modified"] = nom::time();
  // meta["version"] = "0.3.0";
  // meta["scale"] = 1u;

  // Array arr;
  // arr.push_back( 130 );
  // arr.push_back( 16 );
  // meta["size"] = arr;

  // sheet["meta"] = meta;

  // objects["cursors.png"] = sheet;
  // val = objects;

  // NOM_DUMP( val );
  // NOM_DUMP( json.stringify( val ) );

  return NOM_EXIT_SUCCESS;
}

sint do_xml_test_one( void )
{
  nom::Value result;

  ISerializer* serializer = new RapidXmlSerializer();

  if( serializer->unserialize( RESOURCE_XML, result ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  #if defined( NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES )
    NOM_DUMP( result );
  #endif

  if( serializer->serialize( result, "output1.xml" ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  #if defined( NOM_DEBUG_RAPIDXML_SERIALIZER_VALUES )
    NOM_DUMP( result );
  #endif

  return NOM_EXIT_SUCCESS;
}

/// \brief XML to JSON unit tests
sint do_xml_test_two( void )
{
  nom::Value result;

  ISerializer* serializer = new RapidXmlSerializer();
  ISerializer* json_out = new JsonCppSerializer();

  if( serializer->unserialize( RESOURCE_XML, result ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  NOM_DUMP(serializer->stringify( result ));

  #if defined( NOM_DEBUG_RAPIDXML_UNSERIALIZER_VALUES )
    NOM_DUMP( result );
  #endif

  NOM_DUMP( json_out->stringify( result ) );

  return NOM_EXIT_SUCCESS;
}

/// \brief JSON to XML unit tests
///
/// \FIXME Errors & crashed caused by xml->unserialize method call.
sint do_xml_test_three( void )
{
  nom::Value result;

  ISerializer* json = new JsonCppSerializer();
  ISerializer* xml = new RapidXmlSerializer();

  if( json->unserialize( RESOURCE_SANITY, result ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  NOM_DUMP( xml->stringify( result ) );

  if( json->unserialize( RESOURCE_SANITY2, result ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  NOM_DUMP( xml->stringify( result ) );

  // This unit test -- XML transformation of RESOURCE_SANITY -- should *FAIL*
  if( xml->unserialize( RESOURCE_SANITY, result ) == true )
  {
    return NOM_EXIT_FAILURE;
  }

  // NOM_DUMP( result );

  NOM_DUMP( xml->stringify( result ) );

  return NOM_EXIT_SUCCESS;
}

/// \brief Additional JSON & XML conversions for experimentation.
sint do_xml_test_four( void )
{
  nom::Value result;

  ISerializer* json = new JsonCppSerializer();
  ISerializer* xml = new RapidXmlSerializer();

  if( json->unserialize( RESOURCE_JSON_BMFONT_GAMEOVER, result ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  // NOM_DUMP( result );
  NOM_DUMP( json->stringify( result ) );

  if( xml->serialize( result, "gameover.xml" ) == false )
  {
    return NOM_EXIT_FAILURE;
  }

  // NOM_DUMP( result );

  NOM_DUMP( xml->stringify( result ) );

  return NOM_EXIT_SUCCESS;
}

sint do_value_refactoring_test_one( void )
{
// Object obj, obj2;
// obj[0] = 1;
// obj[1] = 2;
// obj[2] = 3;
// obj[3] = 4;

// obj2["testme"] = obj;

  // NOM_DUMP(obj2);
// Value val;
// val.push_back( -1 );
// val.push_back( obj );
  // val.push_back( obj2 );
// NOM_DUMP(val);

  Value value;

  Value obj( Value::ValueType::ObjectValues );
  obj["ttt"] = 7;

  value["testme"] = "boobies";
  value["testme2"] = 5u;
  value["testme3"] = "boobies";
  value["testme4"] = -666;

  // Error; key already exists! (should never be overwritten unless explicitly
  // removed by end-user, such as with nom::Value::erase).
  value["testme4"] = -666;

  NOM_DUMP(value);
  NOM_DUMP(value["testme4"].get_int());
  NOM_DUMP(value["testme"].get_int());

  // Array node with the key name of "arr".
  value["arr"].push_back( 8 );
  value["arr"].push_back( 2 );
  value["arr"].push_back( 3 );
  value["arr"].push_back( 0 );
  NOM_DUMP(value);

  // Creates an empty object node unless obj is non-null.
  value["obj"] = obj;

  // Top-level Array node for "obj" object node.
  value["objA"].push_back( obj );

  value["o"] = Value();
  value["o"] = true;
NOM_DUMP(value);

// FIXME: nom::Value begins to have freeing pointer memory issues here when
// we try deleting the Value::value_.object_ class member.

// Unnamed JSON objects (an array is the top-level node)
nom::ISerializer* fp = new JsonCppSerializer();
Value testme( Value::ValueType::ArrayValues );
testme["root"].push_back( value );
NOM_DUMP( fp->stringify( testme ) );

// Named ("mapped") JSON objects (an object is the top-level node)
Value testme2( Value::ValueType::ObjectValues );
testme2["root"] = value;
NOM_DUMP( fp->stringify( testme2 ) );

// exit(0);

  value["o"] = 2u;
NOM_DUMP(value);

  Value arr( nom::Value::ValueType::ArrayValues );
  value["o"].push_back(arr);

  // This should log an err -- multi-depth array nodes are NOT supported.
  value["o"].push_back(arr);

NOM_DUMP(value);

// exit(0);

  Value arr2( nom::Value::ValueType::ArrayValues );
  // arr2[0] = -64;
  // arr2[1] = 32;
  // arr2[2] = -666;
  // arr2[3] = 666;
  arr2[0].push_back( -64 );
  arr2[1].push_back( 32 );
  arr2[2].push_back( -666 );
  arr2[3].push_back( 666 );

// FIXME: Value is unassigned when trying to use get_int()
NOM_DUMP(arr2[0].get_int() );
NOM_DUMP(arr2[3].get_int() );
NOM_DUMP(arr2);

// exit(0);

  nom::Value::Members members = value.member_names();

  NOM_DUMP( members.size() );

  for( auto itr = members.begin(); itr != members.end(); ++itr )
  {
    NOM_DUMP( *itr );
  }

  // FIXME: This should display the first object of the "cards" array --
  // see ttcards.git/Resources/cards.json.
  nom::Value val = value["cards"][1];
  NOM_DUMP(val);

// exit(0);
  return NOM_EXIT_SUCCESS;
}

sint main( int argc, char* argv[] )
{
  // Return exit code
  sint ret = NOM_EXIT_FAILURE;

  if ( nom::init ( argc, argv ) == false )
  {
    NOM_LOG_ERR ( APP_NAME, "Could not initialize nomlib." );
    exit ( NOM_EXIT_FAILURE );
  }

  // if ( do_test_one() != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(1), "Failed unit test 1" );
  //   return NOM_EXIT_FAILURE;
  // }

  // ret = do_jsoncppvalue_test_one();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Stringify tests
  // ret = do_serialization_test_two();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONUnserializer unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // ret = do_value_test_one();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Unserialize examples/json/Resources/sanity.json & serialize it back to
  // output.json
  // ret = do_unserializer_test_two();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Unserialize examples/json/Resources/sanity2.json & serialize it back to
  // output2.json
  // ret = do_unserializer_test_three();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  ret = do_xml_test_one();
  if( ret != NOM_EXIT_SUCCESS )
  {
    nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
    return NOM_EXIT_FAILURE;
  }

  // ret = do_xml_test_two();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // ret = do_xml_test_three();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // ret = do_xml_test_four();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Memory leaks test
  // if( do_leaks_test_one() != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(1), "Failed performance unit test 1." );
  //   return NOM_EXIT_FAILURE;
  // }

  nom::quit();

  return NOM_EXIT_SUCCESS;
}
