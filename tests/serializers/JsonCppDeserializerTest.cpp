#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/serializers.hpp>
#include <nomlib/ptree.hpp> // Property Tree (nom::Value)

#include "serializers/common.hpp"

namespace nom {

/// \brief This unit test covers the de-serialization of JSON objects to the
/// equivalent nom::Value node type.
class JsonCppDeserializerTest: public ::testing::Test
{
  public:
    /// \brief Default constructor; initialize our input/ouput interfaces to a
    /// sane state, suited for automated testing.
    ///
    /// \note Nearly every test fails without compact (no formatting) output.
    JsonCppDeserializerTest( void ) :
      fp{ new JsonCppDeserializer() },
      fp_out{ new JsonCppSerializer( nom::SerializerOptions::Compact ) },
      fp_outs( new JsonCppSerializer( nom::SerializerOptions::HumanFriendly ) )
    {
      // ...
    }

    ~JsonCppDeserializerTest( void )
    {
      delete this->fp;
      this->fp = nullptr;
    }

  protected:
    nom::IValueDeserializer* fp;
    nom::IValueSerializer* fp_out;
    nom::IValueSerializer* fp_outs;

    Value expected_in( const std::string& in )
    {
      return( fp->deserialize( in ) );
    }

    std::string pp_out( const Value& in )
    {
      return fp_out->serialize( in );
    }

    std::string pp_outs( const Value& in )
    {
      return fp_outs->serialize( in );
    }
};

// FIXME: Output should only be "", I *think*.
TEST_F( JsonCppDeserializerTest, DeserializeNullValue )
{
  Value o = expected_in( "null\n" );

  EXPECT_EQ( Value::null, o );
}

// NOTE: I *think* JsonCpp has a "strict" option that we could toggle off if it
// were necessary to be able to create key value pairs that aren't JSON-compliant,
// AKA not nested under either an array or object node.
//
// FIXME: This *might* be an err in our interface!
TEST_F( JsonCppDeserializerTest, DeserializeInvalidInput )
{
  Value o = expected_in( "\n" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_NullValue )
{
  Value o = expected_in( "[{\"key\":null}]\n" );

  ASSERT_TRUE( o.array_type() );
  ASSERT_TRUE( o[0].object_type() );
  ASSERT_TRUE( o[0]["key"].null_type() );

  // FIXME: I don't think this should be ObjectValues (the operation above might
  // have something to do with it!).
  EXPECT_EQ( Value::ValueType::ArrayValues, o.type() );
  EXPECT_EQ( Value::ValueType::ObjectValues, o[0].type() );
  EXPECT_EQ( Value::null, o[0]["key"].type() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_IntegerValue )
{
  Value o = expected_in( "[{\"key\":-1}]\n" );

  ASSERT_TRUE( o.array_type() );
  ASSERT_TRUE( o[0].object_type() );
  ASSERT_TRUE( o[0]["key"].int_type() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_UsignedIntegerValue )
{
  Value o = expected_in( "[{\"key\":1}]\n" );

  EXPECT_EQ( Value::ValueType::ArrayValues, o.type() );
  EXPECT_EQ( Value::ValueType::ObjectValues, o[0].type() );
  EXPECT_EQ( Value::ValueType::SignedInteger, o[0]["key"].type() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_RealNumberValue )
{
  Value o = expected_in( "[{\"key1\":8.244999999999999}]\n" );

  EXPECT_EQ( Value::ValueType::ArrayValues, o.type() );
  EXPECT_EQ( Value::ValueType::ObjectValues, o[0].type() );
  EXPECT_EQ( Value::ValueType::RealNumber, o[0]["key1"].type() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_StringValue )
{
  Value o = expected_in( "[{\"key1\":\"Hello\"}]\n" );

  EXPECT_EQ( true, o.array_type() );
  EXPECT_EQ( true, o[0].object_type() );
  EXPECT_EQ( Value::ValueType::String, o[0]["key1"].type() );
  EXPECT_EQ( "Hello", o[0]["key1"].get_string() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_BooleanValue )
{
  Value o = expected_in( "[{\"key1\":true}]\n");

  EXPECT_EQ( true, o.array_type() );
  EXPECT_EQ( true, o[0].object_type() );
  EXPECT_EQ( Value::ValueType::Boolean, o[0]["key1"].type() );
  EXPECT_EQ( true, o[0]["key1"].get_bool() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_ArrayValues )
{
  Value o = expected_in( "[{\"arr\":[null,\"Yeah, buddy!\",-8,10,false]}]\n" );

  EXPECT_EQ( true, o.array_type() );
  EXPECT_EQ( true, o[0].object_type() );
  EXPECT_EQ( true, o[0]["arr"].array_type() );
  EXPECT_EQ( true, o[0]["arr"][0].null_type() );
  EXPECT_EQ( "Yeah, buddy!", o[0]["arr"][1].get_string() );
  EXPECT_EQ( -8, o[0]["arr"][2].get_int() );

  // FIXME:
  // EXPECT_EQ( 10, o[0]["arr"][3].get_uint() );

  EXPECT_EQ( 10, o[0]["arr"][3].get_int() );
  EXPECT_EQ( false, o[0]["arr"][4].get_bool() );
}

TEST_F( JsonCppDeserializerTest, DeserializeArrayValues_SimpleObjectValues )
{
  Value o = expected_in( "[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false}]\n" );

  EXPECT_EQ( true, o.array_type() );
  EXPECT_EQ( true, o[0].object_type() );
  EXPECT_EQ( 5, o[0]["id"].get_int() );
  EXPECT_EQ( "Diablos", o[0]["name"].get_string() );
  EXPECT_EQ( 4, o[0]["level"].get_int() );
  EXPECT_EQ( false, o[0]["owner"].get_bool() );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_NullValue )
{
  Value o = expected_in( "{\"obj\":{\"key\":null}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( true, o["obj"]["key"].null_type() );

  // CheckObjectValues_NullValue( o );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_IntegerValue )
{
  Value o = expected_in( "{\"obj\":{\"key\":-1}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( -1, o["obj"]["key"].get_int() );

  // CheckObjectValues_IntegerValue( o );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_UsignedIntegerValue )
{
  Value o = expected_in( "{\"obj\":{\"key\":1}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( 1, o["obj"]["key"].get_int() );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_RealNumberValue )
{
  Value o = expected_in( "{\"obj\":{\"key1\":8.244999999999999,\"key2\":8.239999771118164}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 2, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( 8.245, o["obj"]["key1"].get_double() );
  EXPECT_EQ( 8.24f, o["obj"]["key2"].get_float() );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_StringValue )
{
  Value o = expected_in( "{\"obj\":{\"key1\":\"Hello\",\"key2\":\"...there!\"}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 2, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );

  EXPECT_EQ( "Hello", o["obj"]["key1"].get_string() );
  EXPECT_EQ( "...there!", o["obj"]["key2"].get_string() );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_BooleanValue )
{
  Value o = expected_in( "{\"obj\":{\"key1\":true,\"key2\":false}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 2, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );

  EXPECT_EQ( true, o["obj"]["key1"].get_bool() );
  EXPECT_EQ( false, o["obj"]["key2"].get_bool() );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValues_ArrayValues )
{
  Value o = expected_in( "{\"obj\":{\"arr\":[null,\"Yeah, buddy!\",-8,10,false]}}\n" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );

  EXPECT_EQ( 5, o["obj"]["arr"].size() );
  EXPECT_EQ( true, o["obj"]["arr"].array_type() );
  EXPECT_EQ( true, o["obj"]["arr"][0].null_type() );
  EXPECT_EQ( "Yeah, buddy!", o["obj"]["arr"][1].get_string() );
  EXPECT_EQ( -8, o["obj"]["arr"][2].get_int() );
  EXPECT_EQ( 10, o["obj"]["arr"][3].get_int() );
  EXPECT_EQ( false, o["obj"]["arr"][4].get_bool() );
}

TEST_F( JsonCppDeserializerTest, DeserializeObjectValuesSanity2 )
{
  Value in = expected_in( "{\"object1\":{\"boolean1\":false,\"null\":null,\"response1\":\"Yeah buddy!\"},\"object2\":{\"array2\":[0,2,3,\"hax\"],\"boolean2\":true,\"deep_object\":{\"h\":{\"boolean\":false,\"deep_deep_deep_object\":{\"deep_deep_deep_array\":[\"string_1\",\"string_2\"],\"h\":240,\"w\":420},\"key\":\"pair\",\"null\":null},\"w\":-4},\"response2\":\"Light weight!\"},\"object3\":[{\"cmd\":[\"hax\",\"gibson\"],\"deep_object\":{\"h\":{\"boolean\":false,\"deep_deep_deep_object\":{\"deep_deep_deep_array\":[\"string_1\",\"string_2\"],\"h\":240,\"w\":420},\"key\":\"pair\",\"null\":null},\"w\":-4}}],\"object5\":[\"hello, there!\",-1,false]}\n" );

  Value out = sanity2_out();

  ASSERT_TRUE( in == out );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  return RUN_ALL_TESTS();
}
