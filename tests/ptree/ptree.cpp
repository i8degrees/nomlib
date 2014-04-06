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
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/ptree.hpp> // Property Tree (nom::Value)

#include "common.hpp"

namespace nom {

class PropertyTreeTest: public ::testing::Test
{
  protected:
    // ...
};

TEST_F( PropertyTreeTest, ConstructNullValue )
{
  Value value;

  EXPECT_EQ( Value::ValueType::Null, value.type() );
  EXPECT_EQ( 0, value.size() );
}

TEST_F( PropertyTreeTest, StaticNullValue )
{
  Value value;

  EXPECT_EQ( Value::ValueType::Null, value.type() );
  EXPECT_EQ( 0, value.size() );
  EXPECT_EQ( Value::null, value );
}

TEST_F( PropertyTreeTest, ConstructIntValue )
{
  Value value( -1 );
  EXPECT_EQ( Value::ValueType::SignedInteger, value.type() );
  EXPECT_EQ( 1, value.size() );
  EXPECT_EQ( -1, value.get_int() );
}

TEST_F( PropertyTreeTest, ConstructUnsignedIntValue )
{
  Value value( 1u );
  EXPECT_EQ( Value::ValueType::UnsignedInteger, value.type() );
  EXPECT_EQ( 1, value.size() );
  EXPECT_EQ( 1, value.get_uint() );
}

TEST_F( PropertyTreeTest, ConstructRealNumberValue )
{
  Value value( 1.840 );
  EXPECT_EQ( Value::ValueType::RealNumber, value.type() );
  EXPECT_EQ( 1, value.size() );
  EXPECT_EQ( 1.840, value.get_double() );
}

TEST_F( PropertyTreeTest, ConstructStringValue )
{
  Value value( "Hello, world!" );

  EXPECT_EQ( Value::ValueType::String, value.type() );
  EXPECT_EQ( "Hello, world!", value.get_string() );
  EXPECT_EQ( 1, value.size() );
}

TEST_F( PropertyTreeTest, ConstructBooleanValue )
{
  Value value( true );

  EXPECT_EQ( Value::ValueType::Boolean, value.type() );
  EXPECT_EQ( true, value.get_bool() );
  EXPECT_EQ( 1, value.size() );

  value = Value( false );

  EXPECT_EQ( Value::ValueType::Boolean, value.type() );
  EXPECT_EQ( false, value.get_bool() );
  EXPECT_EQ( 1, value.size() );
}

TEST_F( PropertyTreeTest, ConstructObjectValues ) // Redo?
{
  Value object;

  object["obj"];

  EXPECT_EQ( Value::ValueType::ObjectValues, object.type() );
  EXPECT_EQ( 1, object.size() );
}

TEST_F( PropertyTreeTest, OperatorConstructIntValue )
{
  Value value;

  value["obj"] = -1;
  EXPECT_EQ( Value::ValueType::SignedInteger, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( -1, value["obj"].get_int() );
}

TEST_F( PropertyTreeTest, OperatorConstructUnsignedIntValue )
{
  Value value;

  value["obj"] = 1u;
  EXPECT_EQ( Value::ValueType::UnsignedInteger, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( 1, value["obj"].get_uint() );
}

TEST_F( PropertyTreeTest, OperatorConstructRealNumberValue )
{
  Value value;

  value["obj"] = 1.840;
  EXPECT_EQ( Value::ValueType::RealNumber, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( 1.840, value["obj"].get_double() );
}

TEST_F( PropertyTreeTest, OperatorConstructStringValue )
{
  Value value;

  value["obj"] = "Hello, world!";
  EXPECT_EQ( Value::ValueType::String, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( "Hello, world!", value["obj"].get_string() );
}

TEST_F( PropertyTreeTest, OperatorConstructCStringValue )
{
  Value value;

  value["obj"] = "Hello, world!";
  EXPECT_EQ( Value::ValueType::String, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_STREQ( "Hello, world!", value["obj"].get_cstring() );
}

TEST_F( PropertyTreeTest, OperatorConstructArrayIndex )
{
  Value arr;
  arr[0] = Value();
  arr[1] = -6;
  arr[2] = 10u;
  arr[3] = 8.220;
  arr[4] = "2";
  arr[5] = false;

  EXPECT_EQ( Value::ValueType::ArrayValues, arr.type() );
  EXPECT_EQ( 6, arr.size() );

  EXPECT_EQ( Value::null, arr[0] );
  EXPECT_EQ( -6, arr[1].get_int() );
  EXPECT_EQ( 10u, arr[2].get_uint() );
  EXPECT_EQ( 8.220, arr[3].get_double() );
  EXPECT_EQ( "2", arr[4].get_string() );
  EXPECT_EQ( false, arr[5].get_bool() );
}

TEST_F( PropertyTreeTest, ArrayValuesPushBack )
{
  Value arr;
  arr.push_back( Value::null );
  arr.push_back( -6 );
  arr.push_back( 10u );
  arr.push_back( 8.220 );
  arr.push_back( "2" );
  arr.push_back( true );

  EXPECT_EQ( Value::ValueType::ArrayValues, arr.type() );
  EXPECT_EQ( 6, arr.size() );

  EXPECT_EQ( Value::null, arr[0] );
  EXPECT_EQ( -6, arr[1].get_int() );
  EXPECT_EQ( 10u, arr[2].get_uint() );
  EXPECT_EQ( 8.220, arr[3].get_double() );
  EXPECT_EQ( "2", arr[4].get_string() );
  EXPECT_EQ( true, arr[5].get_bool() );
}

TEST_F( PropertyTreeTest, ConstructArrayValuesWithinArrayNode )
{
  Value arr, arr2;
  arr[0] = "Hello!";
  arr[1] = -6;

  arr2[0] = true;
  arr2[1] = 6u;

  arr.push_back( arr2 );

  EXPECT_EQ( Value::ValueType::ArrayValues, arr.type() );
  EXPECT_EQ( 3, arr.size() );
  EXPECT_EQ( "Hello!", arr[0].get_string() );
  EXPECT_EQ( -6, arr[1].get_int() );

  EXPECT_EQ( Value::ValueType::ArrayValues, arr2.type() );
  EXPECT_EQ( 2, arr2.size() );
  EXPECT_EQ( true, arr2[0].get_bool() );
  EXPECT_EQ( 6u, arr2[1].get_uint() );
}

TEST_F( PropertyTreeTest, DuplicateArrayIndex )
{
  Value o;

  o["a"].push_back( 8 );
  o["a"].push_back( 2 );
  o["a"].push_back( 3 );
  o["a"].push_back( 0 );

  // Overwrites the element at index
  o["a"][0] = -8;
  o["a"][1] = 2;
  o["a"][2] = "3";
  o["a"][3] = -666.08;
  o["a"][4] = 666.08;

  EXPECT_EQ( -8, o["a"][0].get_int() );
  EXPECT_EQ( 2, o["a"][1].get_int() );
  EXPECT_EQ( "3", o["a"][2].get_string() );
  EXPECT_EQ( -666.08, o["a"][3].get_double() );
  EXPECT_EQ( 666.08, o["a"][4].get_double() );
}

TEST_F( PropertyTreeTest, OperatorConstructObjectValues )
{
  Value obj, object, objects;

  obj["key0"] = Value();
  obj["key1"] = -6;
  obj["key2"] = 66u;
  obj["key3"] = 6.806;
  obj["key4"] = "Hello, world!";
  obj["key5"] = true;

  // Two (2) array values
  obj["key6"][0] = 11u;
  obj["key6"][1] = "Hello, world!";

  // Object alias
  object["object1"] = obj;
  object["o"] = obj["key6"];
  objects["root"] = object;

  EXPECT_EQ( Value::ValueType::ObjectValues, obj.type() );
  EXPECT_EQ( 7, obj.size() );

  EXPECT_EQ( Value::null, obj["key0"] );
  EXPECT_EQ( -6, obj["key1"].get_int() );
  EXPECT_EQ( 66u, obj["key2"].get_uint() );
  EXPECT_EQ( 6.806, obj["key3"].get_double() );
  EXPECT_EQ( "Hello, world!", obj["key4"].get_string() );
  EXPECT_EQ( true, obj["key5"].get_bool() );

  EXPECT_EQ( Value::ValueType::ArrayValues, obj["key6"].type() );
  EXPECT_EQ( 2, obj["key6"].size() );
  EXPECT_EQ( 11u, obj["key6"][0].get_uint() );
  EXPECT_EQ( "Hello, world!", obj["key6"][1].get_string() );

  EXPECT_EQ( Value::ValueType::ObjectValues, object["object1"].type() );
  EXPECT_EQ( 7, object["object1"].size() );

  EXPECT_EQ( Value::ValueType::ArrayValues, object["o"].type() );
  EXPECT_EQ( 2, object["o"].size() );

  EXPECT_EQ( Value::ValueType::ObjectValues, objects["root"].type() );
  EXPECT_EQ( 2, objects["root"].size() );
}

TEST_F( PropertyTreeTest, ObjectValuesKeyNames )
{
  Value obj;
  Value::Members members;

  obj["key0"] = Value();
  obj["key1"] = -6;
  obj["key2"] = 66u;
  obj["key3"] = 6.806;
  obj["key4"] = "Hello, world!";
  obj["key5"] = true;

  // Two (2) array values
  obj["key6"][0] = 11u;
  obj["key6"][1] = "Hello, world!";

  members = obj.member_names();

  EXPECT_EQ( 7, members.size() );
  EXPECT_EQ( "key0", members[0] );
  EXPECT_EQ( "key1", members[1] );
  EXPECT_EQ( "key2", members[2] );
  EXPECT_EQ( "key3", members[3] );
  EXPECT_EQ( "key4", members[4] );
  EXPECT_EQ( "key5", members[5] );
  EXPECT_EQ( "key6", members[6] );
}

TEST_F( PropertyTreeTest, ValueFindEraseInterface )
{
  Value obj;

  obj["key0"] = Value();
  obj["key1"] = -6;
  obj["key2"] = 66u;
  obj["key3"] = 6.806;
  obj["key4"] = "Hello, world!";
  obj["key5"] = true;

  // Two (2) array values
  obj["key6"][0] = 11u;
  obj["key6"][1] = "Hello, world!";

  EXPECT_NE( Value::null, obj.find( "key2" ) ) << obj;
  EXPECT_NE( Value::null, obj.erase( "key1" ) );
  EXPECT_EQ( Value::null, obj.erase( "key1" ) );
}

TEST_F( PropertyTreeTest, OperatorConstructDuplicateMemberKeys )
{
  Value o;

  o["testme"] = 1;

  // Overwritten
  o["testme"] = "Yeah, buddy!";

  EXPECT_EQ( "Yeah, buddy!", o["testme"].get_string() );
}

TEST_F( PropertyTreeTest, OperatorConstructDeepDepth )
{
  Value o;
  o[0]["cards"]["ID_0"][0] = Value();
  o[0]["cards"]["ID_0"][1] = "Hello!";

  EXPECT_EQ( Value::null, o[0]["cards"]["ID_0"][0] ) << o;
  EXPECT_EQ( "Hello!", o[0]["cards"]["ID_0"][1].get_string() ) << o;
}

} // namespace nom

int main( int argc, char **argv )
{
  ::testing::InitGoogleTest(&argc, argv);

  return RUN_ALL_TESTS();
}
