#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include <nomlib/xml.hpp>
#include <nomlib/ptree.hpp> // Property Tree (nom::Value)

#include "common.hpp"

namespace nom {

/// \brief This unit test covers the de-serialization of XML object nodes to
/// Value objects nodes.
class RapidXmlDeserializerTest: public ::testing::Test
{
  public:
    RapidXmlDeserializerTest( void ) :
      fp{ new RapidXmlSerializer() }
    {
      // ...
    }

    ~RapidXmlDeserializerTest( void )
    {
      delete this->fp;
      this->fp = nullptr;
    }

    virtual void SetUp( void )
    {
      o.clear();
    }

    virtual void TearDown( void )
    {
      // ...
    }

  protected:
    /// \brief Unit test-wide output variable for use across individual tests.
    ///
    /// \remarks The container is cleared every time a new test is set up.
    Value o;
    nom::ISerializer* fp;

    Value expected_in( const std::string& in )
    {
      return( fp->unserialize( in ) );
    }

    std::string pp_out( const Value& in )
    {
      return fp->serialize( in, nom::ISerializer::Features::HumanReadable );
    }
};

// FIXME: Output should only be "", I *think*.
TEST_F( RapidXmlDeserializerTest, DeserializeNullValue )
{
  o = expected_in( "" );

  EXPECT_EQ( Value::null, o );
}

// NOTE: I *think* JsonCpp has a "strict" option that we could toggle off if it
// were necessary to be able to create key value pairs that aren't JSON-compliant,
// AKA not nested under either an array or object node.
//
// FIXME: This *might* be an err in our interface!
TEST_F( RapidXmlDeserializerTest, DeserializeInvalidInput )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o[0].null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_NullValue )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_IntegerValue )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_UsignedIntegerValue )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_RealNumberValue )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_StringValue )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_BooleanValue )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_ArrayValues )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeArrayValues_SimpleObjectValues )
{
  o = expected_in( "" );

  EXPECT_EQ( true, o.null_type() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeObjectValues_NullValue )
{
  o = expected_in( "<obj></null></obj>" );

  EXPECT_EQ( 0, o.size() );
  EXPECT_EQ( false, o.object_type() );

  EXPECT_EQ( 0, o["obj"].size() );
  EXPECT_EQ( false, o["obj"].object_type() );
  EXPECT_EQ( true, o["obj"].null_type() );

  // CheckObjectValues_NullValue( o );
}

TEST_F( RapidXmlDeserializerTest, DeserializeObjectValues_IntegerValue )
{
  o = expected_in( "<obj><key>-1</key></obj>" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( "-1", o["obj"]["key"].stringify() );

  // CheckObjectValues_IntegerValue( o );
}

TEST_F( RapidXmlDeserializerTest, DeserializeObjectValues_UsignedIntegerValue )
{
  o = expected_in( "<obj><key>1</key></obj>" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( "1", o["obj"]["key"].stringify() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeObjectValues_RealNumberValue )
{
  o = expected_in( "<obj><key1>8.245</key1><key2>8.24</key2></obj>" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 2, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );
  EXPECT_EQ( "8.245", o["obj"]["key1"].stringify() );
  EXPECT_EQ( "8.24", o["obj"]["key2"].stringify() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeObjectValues_StringValue )
{
  o = expected_in( "<obj><key1>Hello</key1><key2>...there!</key2></obj>" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 2, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );

  EXPECT_EQ( "Hello", o["obj"]["key1"].get_string() );
  EXPECT_EQ( "...there!", o["obj"]["key2"].get_string() );
}

TEST_F( RapidXmlDeserializerTest, DeserializeObjectValues_BooleanValue )
{
  o = expected_in( "<obj><key1>true</key1><key2>false</key2></obj>" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 2, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );

  EXPECT_EQ( "true", o["obj"]["key1"].stringify() );
  EXPECT_EQ( "false", o["obj"]["key2"].stringify() );
}

// TODO
TEST_F( RapidXmlDeserializerTest, DISABLED_DeserializeObjectValues_ArrayValues )
{
  o = expected_in( "<obj><arr></arr></obj>" );

  EXPECT_EQ( 1, o.size() );
  EXPECT_EQ( true, o.object_type() );

  EXPECT_EQ( 1, o["obj"].size() );
  EXPECT_EQ( true, o["obj"].object_type() );

  EXPECT_EQ( 5, o["obj"]["arr"].size() );
  EXPECT_EQ( true, o["obj"]["arr"].array_type() );
  EXPECT_EQ( true, o["obj"]["arr"][0].null_type() );
  EXPECT_EQ( "Yeah, buddy!", o["obj"]["arr"][1].get_string() );
  EXPECT_EQ( "-8", o["obj"]["arr"][2].stringify() );
  EXPECT_EQ( "10", o["obj"]["arr"][3].stringify() );
  EXPECT_EQ( "false", o["obj"]["arr"][4].stringify() );
}

// TODO
TEST_F( RapidXmlDeserializerTest, DISABLED_DeserializeObjectValuesSanity2 )
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
