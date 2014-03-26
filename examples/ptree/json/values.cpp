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
#include <nomlib/system.hpp>

const std::string APP_NAME = "nom::Value";

/// Relative file path name of the resources directory
const std::string APP_RESOURCES_DIR = "Resources";

/// Relative file path name to our resources for this unit test
const nom::Path p;
const std::string RESOURCE_SANITY = APP_RESOURCES_DIR + p.native() + "sanity.json";
const std::string RESOURCE_SANITY2 = APP_RESOURCES_DIR + p.native() + "sanity2.json";

using namespace nom; // TODO: Remove

#define NOM_UNIT_TEST(num) \
  ( "Unit Test #num" )

// #define NOM_UNIT_TEST_ONE
// #define NOM_UNIT_TEST_TWO
// #define NOM_UNIT_TEST_FOUR
// #define NOM_UNSERIALIZER_UNIT_TEST_ONE
// #define NOM_SERIALIZER_UNIT_TEST_ONE

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

#if defined( NOM_UNIT_TEST_TWO )
sint do_test_two( void )
{
  nom::JsonCppValue json;

  nom::Array list;

  for( auto itr = values.begin(); itr != values.end(); ++itr )
  {
    list.push_back( *itr );
  }

  nom::Object node;
  // nom::Value container containing an array within an object.
  node.push_back( nom::Pair( "root", list ) );

  // nom::Value container not using an array or object -- not JSON compliant,
  // but valid.
  // node.push_back( nom::Pair( "root", testme3 ) );
  node.push_back( nom::Pair( "string", "Yeah buddy!" ) ); // StringValue
  node.push_back( nom::Pair( "response", "Light weight!" ) ); // StringValue
  node.push_back( nom::Pair( "response2", values[3] ) ); // RealValue

  node.push_back( nom::Pair( "obj2", node ) ); // ObjectValue w/ key
  node.push_back( nom::Pair( "", node ) ); // ObjectValue w/o key

  node.push_back( nom::Pair( "", list ) ); // ObjectValue w/ keyless ArrayValues

  for( nom::ValueIterator itr = node.begin(); itr != node.end(); ++itr )
  {
    nom::ValueIterator member( itr );

    if ( itr->array_type() )
    {
      NOM_DUMP( itr->type_name() );
      NOM_DUMP( member.key() );

      nom::Array array = itr->array();
      for( nom::ValueIterator itr = array.begin(); itr != array.end(); ++itr )
      {
        std::vector<nom::Value> values;
        NOM_DUMP( itr->type_name() );

        if( itr->null_type() )
        {
          values.push_back( Value() );
        }
        else if( itr->int_type() )
        {
          values.push_back( itr->get_int() );
        }
        else if( itr->uint_type() )
        {
          values.push_back( itr->get_uint() );
        }
        else if( itr->double_type() )
        {
          values.push_back( itr->get_double() );
        }
        else if( itr->bool_type() )
        {
          values.push_back( itr->get_bool() );
        }
        else if( itr->string_type() )
        {
          values.push_back( itr->get_string() );
        }
        else
        {
          values.push_back( itr->stringify() );
        }

        NOM_DUMP( itr->stringify() );

        json.insert( member.key(), values );
      }
    }
    else if( itr->object_type() )
    {
      NOM_DUMP( itr->type_name() );
      NOM_DUMP( member.key() );

      nom::Object object = itr->object();

      json.endl(); // Terminate existing object & begin a new one!
      for( nom::ValueIterator itr = object.begin(); itr != object.end(); ++itr )
      {
        nom::ValueIterator member( itr );
        NOM_DUMP( itr->stringify() );

        //&& itr->null_type() == false )
        if( itr->array_type() == false && itr->object_type() == false )
        {
          // json.insert( itr->type_name(), itr->stringify() );
          json.insert( member.key(), itr->stringify() );
        }
      }
    }
    else
    {
      NOM_DUMP( itr->type_name() );
      NOM_DUMP( member.key() );
      NOM_DUMP( itr->stringify() );

      json.insert( member.key(), itr->stringify() );
    }
  }
  NOM_DASHED_ENDL();

// NOM_DUMP(writer.stringify( json ) );

  NOM_DASHED_ENDL();

  return NOM_EXIT_SUCCESS;
}
#endif

sint do_test_three( void )
{
  return NOM_EXIT_SUCCESS;
}

#if defined( NOM_UNIT_TEST_FOUR )
sint do_test_four( void )
{
  nom::JsonCppValue json;

  nom::Array list;
  for( auto itr = values.begin(); itr != values.end(); ++itr )
  {
    list.push_back( *itr );
  }

  nom::Object node;
  // nom::Value container containing an array within an object.
  node.push_back( nom::Pair( "root", list ) );

  // Beginning of a typical JSON compliant multi-object (must be within array).
  nom::Array root;
  root.push_back( node ); // ObjectValue

  // Several nom::Value container objects within an object.
  root.push_back( list ); // ArrayValue

  nom::Value a( root ); // Feed to JSONSerializer

  // Non array, object container (not JSON compliant, but valid with us)
  // nom::Value a( values[4] );

  NOM_DUMP(a.type_name() ); // ArrayValue
  NOM_DUMP(a.size()); // 2

  for( nom::ValueIterator::Iterator it = a.begin(); it != a.end(); ++it )
  {
    // nom::Value container holding an object
    if( it->object_type() )
    {
      NOM_DUMP( it->type_name() );
      nom::Object object = it->object();

      for( nom::ValueIterator::Iterator it = object.begin(); it != object.end(); ++it )
      {
        // Optional key value within object; a named object. Unnamed objects
        // are OK, too.
        // nom::ValueIterator keys( it );
        // if( keys->name() != "" )
        // {
        //   NOM_DUMP( keys->name() );
        // }

        // Do not output invalid values within an object.
        if( ! it->null_type() && ! it->array_type() && ! it->object_type() )
        {
          // nom::Value container holding a non-array, non-object value within
          // an object.
          NOM_DUMP( it->type_name() );
          NOM_DUMP( it->stringify() );

          json.insert( it->type_name(), it->stringify() );
        }

        // nom::Value container holding an object whose holding an array
        if ( it->array_type() )
        {
          nom::Array array = it->array();
          for( nom::ValueIterator it = array.begin(); it != array.end(); ++it )
          {
            NOM_DUMP( it->type_name() );
            NOM_DUMP( it->stringify() );

            json.insert( it->type_name(), it->stringify() );
          }

          // End of object record
          json.endl();
        }
      }
    }

    // nom:Value container holding an array
    // else if ( it->type() == Value::ValueType::ArrayValue  ||
    //           it->type() == Value::ValueType::ObjectValue
    //         )
    else if ( it->array_type() )
    {
      NOM_DUMP( it->type_name() );
      nom::Array array = it->array();
      for( nom::ArrayIterator it = array.begin(); it != array.end(); ++it )
      {
        NOM_DUMP( it->type_name() );
        NOM_DUMP( it->stringify() );

        json.insert( it->type_name(), it->stringify() );
      }
    }
    // nom::Value container without an array or object inside it
    else
    {
      NOM_DUMP( it->type_name() );
      NOM_DUMP( it->stringify() );

      json.insert( it->type_name(), it->stringify() );
    }
  }

// NOM_DUMP_VAR( writer.stringify( json ) ); // nom::Value

  return NOM_EXIT_SUCCESS;
}
#endif

#if defined( NOM_SERIALIZER_UNIT_TEST_ONE )
// FIXME
sint do_serializer_test_one( void )
{
  nom::JsonSerializer writer;

  nom::Array list;

  for( auto itr = values.begin(); itr != values.end(); ++itr )
  {
    list.push_back( *itr );
  }

  nom::Object node;
  // nom::Value container containing an array within an object.
  node.push_back( nom::Pair( "root", list ) );

  // nom::Value container not using an array or object -- not JSON compliant,
  // but valid.
  // node.push_back( nom::Pair( "root", testme3 ) );
  node.push_back( nom::Pair( "string", "Yeah buddy!" ) ); // StringValue
  node.push_back( nom::Pair( "string2", "Yeah buddy!" ) ); // StringValue
  node.push_back( nom::Pair( "response", "Light weight!" ) ); // StringValue
  node.push_back( nom::Pair( "response2", values[3] ) ); // RealValue
  node.push_back( nom::Pair( "array", list ) ); // ObjectValue w/ keyless ArrayValues

  nom::Object obj2;
  obj2.push_back( nom::Pair( "RealValue", values[3] ) );
  // FIXME: obj2.push_back( nom::Pair( "null", values[0] ) );
  obj2.push_back( nom::Pair( "null", values[2] ) );

  nom::Object obj3;
  obj3.push_back( nom::Pair( "testme", values[4] ) );
  obj3.push_back( nom::Pair( "testme2", values[1] ) );

  node.push_back( nom::Pair( "obj2", obj2 ) ); // ObjectValue w/ key
  node.push_back( nom::Pair( "obj3", obj3 ) ); // ObjectValue w/ key

  nom::Value values ( node );

  // Output file is saved under working directory of where we execute from;
  // build/examples/debug
  // FIXME:
  // writer.serialize( "values.json", values );

  return NOM_EXIT_SUCCESS;
}
#endif

sint do_serializer_test_two( void )
{
  return NOM_EXIT_SUCCESS;
}

#if defined( NOM_UNSERIALIZER_UNIT_TEST_ONE )
sint do_unserializer_test_one( void )
{
  JsonSerializer reader;

  nom::Array list;
  for( auto itr = values.begin(); itr != values.end(); ++itr )
  {
    list.push_back( *itr );
  }

  nom::Object node;
  // nom::Value container containing an array within an object.
  node.push_back( nom::Pair( "root", list ) );

  // nom::Value container not using an array or object -- not JSON compliant,
  // but valid.
  // node.push_back( nom::Pair( "root", testme3 ) );
  node.push_back( nom::Pair( "string", "Yeah buddy!" ) ); // StringValue
  node.push_back( nom::Pair( "string2", "Yeah buddy!" ) ); // StringValue
  node.push_back( nom::Pair( "response", "Light weight!" ) ); // StringValue
  node.push_back( nom::Pair( "response2", values[3] ) ); // RealValue
  node.push_back( nom::Pair( "array", list ) ); // ObjectValue w/ keyless ArrayValues

  nom::Object obj2;
  obj2.push_back( nom::Pair( "obj2", values[3] ) );
  node.push_back( nom::Pair( "obj3", obj2 ) ); // ObjectValue w/ key

  nom::Value testme( node ); // ObjectValue
  NOM_DUMP( testme );

  return NOM_EXIT_SUCCESS;
}
#endif

sint do_unserializer_test_two( void )
{
  JsonSerializer reader;
  JsonSerializer writer;
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
  JsonSerializer reader;
  JsonSerializer writer;
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
  JsonSerializer reader;
  nom::Value values;

  nom::Timer timer;
  timer.start();

  uint32 dt = 0;
  while( timer.ticks() < 10000 ) // 10s
  {
    if( reader.unserialize( RESOURCE_SANITY, values ) == false )
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
  nom::JsonSerializer writer;

  nom::Value val("StringValue");
  NOM_DUMP( val );

  std::string ret = writer.stringify( val );
  NOM_DUMP( ret );

  if( val.stringify() != "StringValue" ) return 1;
  if( ret != "StringValue" ) return 2;

  return NOM_EXIT_SUCCESS;
}

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
  // Json::Value o1, o2;
  // o2["obj2"]["pair1"] = "value";
  // o2["obj2"]["pair2"] = "value";
  // o1["obj"] = o2;
  // NOM_DUMP(o1);
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

  // if ( do_test_two() != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(2), "Failed unit test 2." );
  //   return NOM_EXIT_FAILURE;
  // }

  // Not implemented
  // if ( do_test_three() != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(3), "Failed unit test 3." );
  //   return NOM_EXIT_FAILURE;
  // }

  // if ( do_test_four() != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(4), "Failed unit test 4." );
  //   return NOM_EXIT_FAILURE;
  // }

  // ret = do_jsoncppvalue_test_one();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONCPP unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // ret = do_serializer_test_one();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONCPP unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Stringify tests
  // ret = do_serialization_test_two();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONUnserializer unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Dump tree test
  // ret = do_unserializer_test_one();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONCPP unit test " + std::to_string(ret) );
  //   return NOM_EXIT_FAILURE;
  // }

  // Unserialize examples/json/Resources/sanity.json & serialize it back to
  // output.json
  ret = do_unserializer_test_two();
  if( ret != NOM_EXIT_SUCCESS )
  {
    nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONCPP unit test " + std::to_string(ret) );

  ret = do_unserializer_test_three();
  if( ret != NOM_EXIT_SUCCESS )
  {
    nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed unit test " + std::to_string(ret) );
    return NOM_EXIT_FAILURE;
  }

  // Not implemented
  // ret = do_serializer_test_two();
  // if( ret != NOM_EXIT_SUCCESS )
  // {
  //   nom::DialogMessageBox( NOM_UNIT_TEST(ret), "Failed JSONCPP unit test " + std::to_string(ret) );
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
