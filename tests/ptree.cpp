#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/json.hpp>
#include <nomlib/xml.hpp>
#include <nomlib/system.hpp>

namespace nom {

// TODO: Rename tests (see array test for an example)

TEST( values, null_type )
{
  Value value;

  EXPECT_EQ( Value::ValueType::Null, value.type() );
  EXPECT_EQ( 0, value.size() );
}

TEST( values, static_null_type )
{
  Value value;

  EXPECT_EQ( Value::ValueType::Null, value.type() );
  EXPECT_EQ( 0, value.size() );
  EXPECT_EQ( Value::null, value );
}

TEST( values, int_type )
{
  Value value( -1 );
  EXPECT_EQ( Value::ValueType::SignedInteger, value.type() );
  EXPECT_EQ( 1, value.size() );
  EXPECT_EQ( -1, value.get_int() );
}

TEST( values, uint_type )
{
  Value value( 1u );
  EXPECT_EQ( Value::ValueType::UnsignedInteger, value.type() );
  EXPECT_EQ( 1, value.size() );
  EXPECT_EQ( 1, value.get_uint() );
}

TEST( values, double_type )
{
  Value value( 1.840 );
  EXPECT_EQ( Value::ValueType::RealNumber, value.type() );
  EXPECT_EQ( 1, value.size() );
  EXPECT_EQ( 1.840, value.get_double() );
}

TEST( values, string_type )
{
  Value value( "Hello, world!" );

  EXPECT_EQ( Value::ValueType::String, value.type() );
  EXPECT_EQ( "Hello, world!", value.get_string() );
  EXPECT_EQ( 1, value.size() );
}

TEST( values, boolean_type )
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

// TODO: values, array_type

TEST( values, object_type ) // Redo?
{
  Value object;

  object["obj"];

  EXPECT_EQ( Value::ValueType::ObjectValues, object.type() );
  EXPECT_EQ( 1, object.size() );
}

TEST( values_interface, int_type )
{
  Value value;

  value["obj"] = -1;
  EXPECT_EQ( Value::ValueType::SignedInteger, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( -1, value["obj"].get_int() );
}

TEST( values_interface, uint_type )
{
  Value value;

  value["obj"] = 1u;
  EXPECT_EQ( Value::ValueType::UnsignedInteger, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( 1, value["obj"].get_uint() );
}

TEST( values_interface, double_type )
{
  Value value;

  value["obj"] = 1.840;
  EXPECT_EQ( Value::ValueType::RealNumber, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( 1.840, value["obj"].get_double() );
}

TEST( values_interface, string_type )
{
  Value value;

  value["obj"] = "Hello, world!";
  EXPECT_EQ( Value::ValueType::String, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( "Hello, world!", value["obj"].get_string() );
}

TEST( values_interface, cstring_type )
{
  std::string expected = "Hello, world!";
  Value value;

  value["obj"] = "Hello, world!";
  EXPECT_EQ( Value::ValueType::String, value["obj"].type() );
  EXPECT_EQ( 1, value["obj"].size() );
  EXPECT_EQ( expected, value["obj"].get_cstring() );
}

TEST( values_interface, array_index_operator )
{
  Value arr;
  arr[0] = 6;
  arr[1] = 10;
  arr[2] = 2;
  arr[3] = 8;

  EXPECT_EQ( Value::ValueType::ArrayValues, arr.type() );
  EXPECT_EQ( 4, arr.size() );

  EXPECT_EQ( 6, arr[0].get_int() );
  EXPECT_EQ( 10, arr[1].get_int() );
  EXPECT_EQ( 2, arr[2].get_int() );
  EXPECT_EQ( 8, arr[3].get_int() );
}

TEST( values_interface, array_push_back )
{
  Value arr;
  arr.push_back( 6 );
  arr.push_back( 10 );
  arr.push_back( 2 );
  arr.push_back( 8 );

  EXPECT_EQ( Value::ValueType::ArrayValues, arr.type() );
  EXPECT_EQ( 4, arr.size() );

  EXPECT_EQ( 6, arr[0].get_int() );
  EXPECT_EQ( 10, arr[1].get_int() );
  EXPECT_EQ( 2, arr[2].get_int() );
  EXPECT_EQ( 8, arr[3].get_int() );
}

TEST( values_interface, single_object )
{
  Value obj, objects;

  obj["key1"] = "Hello, world!";
  obj["key2"] = -6;
  obj["key3"] = Value::null;
  obj["key4"] = true;

  objects["object1"] = obj;

  EXPECT_EQ( Value::ValueType::ObjectValues, obj.type() );
  EXPECT_EQ( 4, obj.size() );

  EXPECT_EQ( Value::ValueType::ObjectValues, objects["object1"].type() );
  EXPECT_EQ( 4, objects["object1"].size() );

  EXPECT_EQ( "Hello, world!", obj["key1"].get_string() );
  EXPECT_EQ( -6, obj["key2"].get_int() );
  EXPECT_EQ( Value::null, obj["key3"].type() );
  EXPECT_EQ( true, obj["key4"].get_bool() );
}

// TODO: object_with_array

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}

} // namespace nom
