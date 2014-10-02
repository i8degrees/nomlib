#include <iostream>
#include <string>

#include "gtest/gtest.h"

// nom::init functions
#include "nomlib/system/init.hpp"
#include "nomlib/system/dialog_messagebox.hpp"
#include "nomlib/system/SearchPath.hpp"

#include <nomlib/serializers.hpp>
#include <nomlib/ptree.hpp> // Property Tree (nom::Value)

#include "nomlib/tests/serializers/common.hpp"

namespace nom {

/// \brief This unit test covers the serialization of Value objects to JSON
// object nodes that begin as either an array or object.
class JsonCppSerializerTest: public ::testing::Test
{
  public:
    const std::string APP_NAME = "JsonCppSerializerTest";

    /// \brief Default constructor; initialize our input/ouput interfaces to a
    /// sane state, suited for automated testing.
    ///
    /// \note Nearly every test fails without compact (no formatting) output.
    JsonCppSerializerTest() :
      fp{ new JsonCppSerializer( nom::SerializerOptions::Compact ) },
      fp_in{ new JsonCppDeserializer() },
      fp_outs{ new JsonCppSerializer( nom::SerializerOptions::HumanFriendly ) }
    {
      // ...
    }

    virtual ~JsonCppSerializerTest()
    {
      delete this->fp;
      this->fp = nullptr;
    }

    /// \remarks This method is called after construction, at the start of each
    /// unit test.
    virtual void SetUp()
    {
      // nom::init sets the working directory to this executable's directory
      // path; i.e.: build/tests or build/tests/Debug depending on build
      // environment. Resource path roots become absolute directory paths from
      // here on out.
      if( resources.load_file(APP_NAME + ".json") == false )
      {
        FAIL()
        << "Could not determine the root resource path for " << APP_NAME << ".json";
      }
    }

  protected:
    nom::SearchPath resources;

    nom::IValueSerializer* fp;
    nom::IValueDeserializer* fp_in;
    nom::IValueSerializer* fp_outs;

    void expected_out( Value& in, const std::string& out )
    {
      std::string ret;

      ret = fp->serialize( in );

      EXPECT_EQ( out, ret ) << in;
    }

    void not_expected_out( Value& in, const std::string& out )
    {
      std::string ret;

      ret = fp->serialize( in );

      EXPECT_NE( out, ret ) << in;
    }

    std::string pp_out( const Value& in )
    {
      return fp->serialize( in );
    }

    std::string pp_outs( const Value& in )
    {
      return fp_outs->serialize( in );
    }
};

// FIXME: Output should be just a newline, I think.
TEST_F( JsonCppSerializerTest, SerializeNullValue )
{
  Value o;

  expected_out( o, "null\n" );
}

TEST_F( JsonCppSerializerTest, SerializeInvalidIntegerValue )
{
  Value o;

  o["key"] = -1;

  not_expected_out( o, "" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_NullValue )
{
  Value o;

  o[0]["key"] = Value();

  expected_out( o, "[{\"key\":null}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_IntegerValue )
{
  Value o;

  o[0]["key"] = -1;

  expected_out( o, "[{\"key\":-1}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_UsignedIntegerValue )
{
  Value o;
  o[0]["key"] = 1u;

  expected_out( o, "[{\"key\":1}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_RealNumberValue )
{
  Value o;
  o[0]["key1"] = 8.245;
  o[1]["key2"] = 8.24f;

  expected_out( o, "[{\"key1\":8.244999999999999},{\"key2\":8.239999771118164}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_StringValue )
{
  Value o;
  o[0]["key1"] = "Hello";
  o[1]["key2"] = "...there!";

  expected_out( o, "[{\"key1\":\"Hello\"},{\"key2\":\"...there!\"}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_BooleanValue )
{
  Value o;
  o[0]["key1"] = true;
  o[1]["key2"] = false;

  expected_out( o, "[{\"key1\":true},{\"key2\":false}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_ArrayValues )
{
  Value o;
  o[0]["arr"][0] = Value::null;
  o[0]["arr"][1] = "Yeah, buddy!";
  o[0]["arr"][2] = -8;
  o[0]["arr"][3] = 10u;
  o[0]["arr"][4] = false;

  expected_out( o, "[{\"arr\":[null,\"Yeah, buddy!\",-8,10,false]}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_SimpleObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["owner"] = false;

  expected_out( o, "[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_MultipleSimpleObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["owner"] = false;

  o[1]["id"] = 99u;
  o[1]["name"] = "Squall";
  o[1]["level"] = 4u;
  o[1]["owner"] = Value();

  o[2]["id"] = 55u;
  o[2]["name"] = "Zell";
  o[2]["level"] = 2u;
  o[2]["owner"] = true;

  o[3] = Value();

  expected_out( o, "[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false},{\"id\":99,\"level\":4,\"name\":\"Squall\",\"owner\":null},{\"id\":55,\"level\":2,\"name\":\"Zell\",\"owner\":true}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_ArrayValuesInObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["ranks"][0] = 4u;
  o[0]["ranks"][1] = 8u;
  o[0]["ranks"][2] = 5u;
  o[0]["ranks"][3] = 10u;
  o[0]["owner"] = false;

  expected_out( o, "[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_MultipleArrayValuesInObjectValues )
{
  Value o;

  o[0]["id"] = 5u;
  o[0]["name"] = "Diablos";
  o[0]["level"] = 4u;
  o[0]["ranks"][0] = 4u;
  o[0]["ranks"][1] = 8u;
  o[0]["ranks"][2] = 5u;
  o[0]["ranks"][3] = 10u;
  o[0]["owner"] = false;

  o[1]["id"] = 0u;
  o[1]["name"] = "Geezard";
  o[1]["level"] = 1u;
  o[1]["ranks"][0] = 5u;
  o[1]["ranks"][1] = 3u;
  o[1]["ranks"][2] = 1u;
  o[1]["ranks"][3] = 3u;
  o[1]["owner"] = true;

  expected_out( o, "[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]},{\"id\":0,\"level\":1,\"name\":\"Geezard\",\"owner\":true,\"ranks\":[5,3,1,3]}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_ObjectInsideArrayValues )
{
  Value o, obj;

  obj[0]["id"] = 5u;
  obj[0]["name"] = "Diablos";
  obj[0]["level"] = 4u;
  obj[0]["ranks"][0] = 4u;
  obj[0]["ranks"][1] = 8u;
  obj[0]["ranks"][2] = 5u;
  obj[0]["ranks"][3] = 10u;
  obj[0]["owner"] = false;

  o[0]["cards"] = obj;

  obj[1]["id"] = 0u;
  obj[1]["name"] = "Geezard";
  obj[1]["level"] = 1u;
  obj[1]["ranks"][0] = 5u;
  obj[1]["ranks"][1] = 3u;
  obj[1]["ranks"][2] = 1u;
  obj[1]["ranks"][3] = 3u;
  obj[1]["owner"] = true;

  o[1]["cards"] = obj;

  expected_out( o, "[{\"cards\":[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]},{\"id\":0,\"level\":1,\"name\":\"Geezard\",\"owner\":true,\"ranks\":[5,3,1,3]}]},{\"cards\":[{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]},{\"id\":0,\"level\":1,\"name\":\"Geezard\",\"owner\":true,\"ranks\":[5,3,1,3]}]}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_ComplexArrayValues )
{
  Value o;
  Value obj;
  Value arr;
  Value obj2;

  obj["name"] = "Geezard"; // Object node

  // Array node
  obj["ranks"][0] = 1u;
  obj["ranks"][1] = 5u;
  obj["ranks"][2] = 3u;
  obj["ranks"][3] = 1u;
  obj["ranks"][4] = 3u;
  obj["ranks"][5] = true;

  arr["__ranks"].push_back( obj ); // Array node

  obj2["name"] = "Geezard"; // Under an Object node
  obj2["__array"].push_back( arr ); // Array node

  o[0]["cards"] = obj2;

  expected_out( o, "[{\"cards\":{\"__array\":[{\"__ranks\":[{\"name\":\"Geezard\",\"ranks\":[1,5,3,1,3,true]}]}],\"name\":\"Geezard\"}}]\n" );
}

TEST_F( JsonCppSerializerTest, SerializeArrayValues_ComplexObjectValues )
{
  Value o;
  Value obj1;
  Value obj2;
  Value obj3;
  Value obj4;

  obj1["id"] = 0u;
  obj1["name"] = "Geezard";
  o[0]["cards"]["obj1"] = obj1;

  obj2["id"] = 1u;
  obj2["name"] = "Bite Bug";
  o[1]["cards"]["obj2"] = obj2;

  obj3["id"] = 2u;
  obj3["name"] = "MiniMog";
  o[2]["cards"]["obj3"]["obj4"] = obj3;

  obj4["id"] = 3u;
  obj4["name"] = "Chicobo";
  o[3]["cards"]["obj4"] = obj4;

  expected_out( o, "[{\"cards\":{\"obj1\":{\"id\":0,\"name\":\"Geezard\"}}},{\"cards\":{\"obj2\":{\"id\":1,\"name\":\"Bite Bug\"}}},{\"cards\":{\"obj3\":{\"obj4\":{\"id\":2,\"name\":\"MiniMog\"}}}},{\"cards\":{\"obj4\":{\"id\":3,\"name\":\"Chicobo\"}}}]\n" );
}

// ObjectValues

TEST_F( JsonCppSerializerTest, SerializeObjectValues_NullValue )
{
  Value o;

  o["obj"]["key"] = Value();

  expected_out( o, "{\"obj\":{\"key\":null}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_IntegerValue )
{
  Value o;

  o["obj"]["key"] = -1;

  expected_out( o, "{\"obj\":{\"key\":-1}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_UsignedIntegerValue )
{
  Value o;
  o["obj"]["key"] = 1u;

  expected_out( o, "{\"obj\":{\"key\":1}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_RealNumberValue )
{
  Value o;
  o["obj"]["key1"] = 8.245;
  o["obj"]["key2"] = 8.24f;

  expected_out( o, "{\"obj\":{\"key1\":8.244999999999999,\"key2\":8.239999771118164}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_StringValue )
{
  Value o;
  o["obj"]["key1"] = "Hello";
  o["obj"]["key2"] = "...there!";

  expected_out( o, "{\"obj\":{\"key1\":\"Hello\",\"key2\":\"...there!\"}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_BooleanValue )
{
  Value o;
  o["obj"]["key1"] = true;
  o["obj"]["key2"] = false;

  expected_out( o, "{\"obj\":{\"key1\":true,\"key2\":false}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_ArrayValues )
{
  Value o;
  o["obj"]["arr"][0] = Value::null;
  o["obj"]["arr"][1] = "Yeah, buddy!";
  o["obj"]["arr"][2] = -8;
  o["obj"]["arr"][3] = 10u;
  o["obj"]["arr"][4] = false;

  expected_out( o, "{\"obj\":{\"arr\":[null,\"Yeah, buddy!\",-8,10,false]}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_SimpleObjectValues )
{
  Value o;

  o["obj"]["id"] = 5u;
  o["obj"]["name"] = "Diablos";
  o["obj"]["level"] = 4u;
  o["obj"]["owner"] = false;

  expected_out( o, "{\"obj\":{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_MultipleSimpleObjectValues )
{
  Value o;

  o["obj1"]["id"] = 5u;
  o["obj1"]["name"] = "Diablos";
  o["obj1"]["level"] = 4u;
  o["obj1"]["owner"] = false;

  o["obj2"]["id"] = 99u;
  o["obj2"]["name"] = "Squall";
  o["obj2"]["level"] = 4u;
  o["obj2"]["owner"] = Value();

  o["obj3"]["id"] = 55u;
  o["obj3"]["name"] = "Zell";
  o["obj3"]["level"] = 2u;
  o["obj3"]["owner"] = true;

  o["obj4"] = Value();

  expected_out( o, "{\"obj1\":{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false},\"obj2\":{\"id\":99,\"level\":4,\"name\":\"Squall\",\"owner\":null},\"obj3\":{\"id\":55,\"level\":2,\"name\":\"Zell\",\"owner\":true}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_ArrayValuesInObjectValues )
{
  Value o;

  o["obj"]["id"] = 5u;
  o["obj"]["name"] = "Diablos";
  o["obj"]["level"] = 4u;
  o["obj"]["ranks"][0] = 4u;
  o["obj"]["ranks"][1] = 8u;
  o["obj"]["ranks"][2] = 5u;
  o["obj"]["ranks"][3] = 10u;
  o["obj"]["owner"] = false;

  expected_out( o, "{\"obj\":{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_MultipleArrayValuesInObjectValues )
{
  Value o;

  o["obj1"]["id"] = 5u;
  o["obj1"]["name"] = "Diablos";
  o["obj1"]["level"] = 4u;
  o["obj1"]["ranks"][0] = 4u;
  o["obj1"]["ranks"][1] = 8u;
  o["obj1"]["ranks"][2] = 5u;
  o["obj1"]["ranks"][3] = 10u;
  o["obj1"]["owner"] = false;

  o["obj2"]["id"] = 0u;
  o["obj2"]["name"] = "Geezard";
  o["obj2"]["level"] = 1u;
  o["obj2"]["ranks"][0] = 5u;
  o["obj2"]["ranks"][1] = 3u;
  o["obj2"]["ranks"][2] = 1u;
  o["obj2"]["ranks"][3] = 3u;
  o["obj2"]["owner"] = true;

  expected_out( o, "{\"obj1\":{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]},\"obj2\":{\"id\":0,\"level\":1,\"name\":\"Geezard\",\"owner\":true,\"ranks\":[5,3,1,3]}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_ObjectInsideArrayValues )
{
  Value o, obj;

  obj["obj1"]["id"] = 5u;
  obj["obj1"]["name"] = "Diablos";
  obj["obj1"]["level"] = 4u;
  obj["obj1"]["ranks"][0] = 4u;
  obj["obj1"]["ranks"][1] = 8u;
  obj["obj1"]["ranks"][2] = 5u;
  obj["obj1"]["ranks"][3] = 10u;
  obj["obj1"]["owner"] = false;

  o["root"]["cards"].push_back( obj );

  obj["obj2"]["id"] = 0u;
  obj["obj2"]["name"] = "Geezard";
  obj["obj2"]["level"] = 1u;
  obj["obj2"]["ranks"][0] = 5u;
  obj["obj2"]["ranks"][1] = 3u;
  obj["obj2"]["ranks"][2] = 1u;
  obj["obj2"]["ranks"][3] = 3u;
  obj["obj2"]["owner"] = true;

  o["root"]["cards"].push_back( obj );

  expected_out( o, "{\"root\":{\"cards\":[{\"obj1\":{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]},\"obj2\":{\"id\":0,\"level\":1,\"name\":\"Geezard\",\"owner\":true,\"ranks\":[5,3,1,3]}},{\"obj1\":{\"id\":5,\"level\":4,\"name\":\"Diablos\",\"owner\":false,\"ranks\":[4,8,5,10]},\"obj2\":{\"id\":0,\"level\":1,\"name\":\"Geezard\",\"owner\":true,\"ranks\":[5,3,1,3]}}]}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_ComplexArrayValues )
{
  Value o;
  Value obj;
  Value arr;
  Value obj2;

  obj["name"] = "Geezard"; // Object node

  // Array node
  obj["ranks"][0] = 1u;
  obj["ranks"][1] = 5u;
  obj["ranks"][2] = 3u;
  obj["ranks"][3] = 1u;
  obj["ranks"][4] = 3u;
  obj["ranks"][5] = true;

  arr["__ranks"].push_back( obj ); // Array node

  obj2["name"] = "Geezard"; // Under an Object node
  obj2["__array"].push_back( arr ); // Array node

  o["root"]["cards"] = obj2;

  expected_out( o, "{\"root\":{\"cards\":{\"__array\":[{\"__ranks\":[{\"name\":\"Geezard\",\"ranks\":[1,5,3,1,3,true]}]}],\"name\":\"Geezard\"}}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValues_ComplexObjectValues )
{
  Value o;
  Value obj1;
  Value obj2;
  Value obj3;
  Value obj4;

  obj1["id"] = 0u;
  obj1["name"] = "Geezard";
  o["root"]["cards"]["obj1"] = obj1;

  obj2["id"] = 1u;
  obj2["name"] = "Bite Bug";
  o["root"]["cards"]["obj2"] = obj2;

  obj3["id"] = 2u;
  obj3["name"] = "MiniMog";
  o["root"]["cards"]["obj3"]["obj4"] = obj3;

  obj4["id"] = 3u;
  obj4["name"] = "Chicobo";
  o["root"]["cards"]["obj4"] = obj4;

  expected_out( o, "{\"root\":{\"cards\":{\"obj1\":{\"id\":0,\"name\":\"Geezard\"},\"obj2\":{\"id\":1,\"name\":\"Bite Bug\"},\"obj3\":{\"obj4\":{\"id\":2,\"name\":\"MiniMog\"}},\"obj4\":{\"id\":3,\"name\":\"Chicobo\"}}}}\n" );
}

TEST_F( JsonCppSerializerTest, SerializeObjectValuesSanity2 )
{
  Value o;

  o = sanity2_out();

  expected_out( o, "{\"object1\":{\"boolean1\":false,\"null\":null,\"response1\":\"Yeah buddy!\"},\"object2\":{\"array2\":[0,2,3,\"hax\"],\"boolean2\":true,\"deep_object\":{\"h\":{\"boolean\":false,\"deep_deep_deep_object\":{\"deep_deep_deep_array\":[\"string_1\",\"string_2\"],\"h\":240,\"w\":420},\"key\":\"pair\",\"null\":null},\"w\":-4},\"response2\":\"Light weight!\"},\"object3\":[{\"cmd\":[\"hax\",\"gibson\"],\"deep_object\":{\"h\":{\"boolean\":false,\"deep_deep_deep_object\":{\"deep_deep_deep_array\":[\"string_1\",\"string_2\"],\"h\":240,\"w\":420},\"key\":\"pair\",\"null\":null},\"w\":-4}}],\"object5\":[\"hello, there!\",-1,false]}\n" );
}

TEST_F( JsonCppSerializerTest, FileIO )
{
  Value in;
  Value o;

  o = sanity2_out();

  ASSERT_TRUE( fp->save( o, "output_human-readable.json" ) ) << o;

  // Just to be safe!
  o.clear();
  o = sanity2_out();

  ASSERT_TRUE( fp->save( o, "output_compact.json" ) ) << o;

  // Attempt to verify that the object we serialized is, in fact, the same data
  // representation as what we are able to retrieve.
  ASSERT_TRUE( fp_in->load( "output_human-readable.json", in ) ) << in;

  ASSERT_TRUE( in == o ) << in;

  // Just to be safe!
  in.clear();

  ASSERT_TRUE( fp_in->load( "output_compact.json", in ) ) << in;

  ASSERT_TRUE( in == o ) << in;
}

/// \todo Our JSON serializer does not currently handle array objects within
/// array objects, and must be implemented in order for the
/// RESOURCE_JSON_INVENTORY JSON tests to pass.
/// to.
TEST_F( JsonCppSerializerTest, ExtraIO )
{
  Value in;
  Value os;

  // ASSERT_TRUE( fp_in->load( resources.path() + RESOURCE_JSON_INVENTORY, os ) );
  // EXPECT_EQ( 2, os.size() );

  // ASSERT_TRUE( fp_in->load( resources.path() + RESOURCE_JSON_INVENTORY, os ) );
  // EXPECT_EQ( 2, os.size() );

  // FIXME: This will load for us only if we use Json::Reader directly to read.
  // ASSERT_TRUE( fp_in->load( resources.path() + RESOURCE_JSON_GAMEDATA, os ) );
  // EXPECT_EQ( 4, os.size() );
}

} // namespace nom

int main( int argc, char** argv )
{
  ::testing::InitGoogleTest( &argc, argv );

  // Set the current working directory path to the path leading to this
  // executable file; used for unit tests that require file-system I/O.
  if( nom::init( argc, argv ) == false )
  {
    nom::DialogMessageBox( "Critical Error", "Could not initialize nomlib.", nom::MessageBoxType::NOM_DIALOG_ERROR );
    return NOM_EXIT_FAILURE;
  }
  atexit( nom::quit );

  return RUN_ALL_TESTS();
}
